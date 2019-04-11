/*
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 *  ======== empty.c ========
 */
/* Board Header file */
#include "Board.h"


/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <xdc/runtime/Error.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>
#include <signal.h>
#include <time.h>
#include <types.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <stdbool.h>
#include <ti/sysbios/knl/semaphore.h>
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include <ti/sysbios/knl/Clock.h>
#include "inc/hw_types.h"
#include "inc/hw_i2c.h"
#include <pthread.h>
#include "FreeRTOS.h"
#include "queue.h"
#include <unistd.h>
#include <stdarg.h>
//Macros
#define slave_addr (0x48)
#define LOGGER_LED 0
#define LOGGER_TEMP 1
#define tempreg_addr   (0x00)
#define TASKSTACKSIZE_LED   1024
#define TASKSTACKSIZE_LOG   1024
#define TASKSTACKSIZE_TEMP  1024
Task_Struct taskled_struct;
Task_Struct tasktemp_struct;
Task_Struct tasklog_struct;
Char temp_stack[TASKSTACKSIZE_TEMP];
Char led_stack[TASKSTACKSIZE_LED];
Char log_stack[TASKSTACKSIZE_LOG];
char* data;
char* logptr;
int i,flag,n;
time_t currtime;
struct tm *tm_date;
UART_Handle uart;
UART_Params uartParams;
QueueHandle_t queue_temp,queue_led;
//semaphore
Semaphore_Struct uartsemwrite_struct,uartsemread_struct;
Semaphore_Handle uartsemwrite,uartsemread;
Semaphore_Params uartsemwrite_params,uartsemread_params;

typedef struct
{
    int temp;
    int time1;
}temp_struct;

typedef struct
{
    int time1;
    int led_toggle_count;
}ledtoggle_struct;

uart_init()
{
   /* Create a UART with data processing off. */
   UART_Params_init(&uartParams);
   uartParams.writeDataMode = UART_DATA_BINARY;
   uartParams.readDataMode = UART_DATA_BINARY;
   uartParams.readReturnMode = UART_RETURN_FULL;
   uartParams.readEcho = UART_ECHO_OFF;
   uartParams.baudRate = 9600;
   uart = UART_open(Board_UART0, &uartParams);
   if (uart == NULL)
   {
       System_abort("Error opening the UART");
   }
}

void InitI2C0(void)
{
    //enable I2C module 0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);

    //reset module
    SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);

    //enable GPIO peripheral that contains I2C 0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    // Configure the pin muxing for I2C0 functions on port B2 and B3.
    GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);

    // Select the I2C function for these pins.
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);

    // Enable and initialize the I2C0 master module.  Use the system clock for
    // the I2C0 module.  The last parameter sets the I2C data transfer rate.
    // If false the data rate is set to 100kbps and if true the data rate will
    // be set to 400kbps.
    I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false);

    //clear I2C FIFOs
    HWREG(I2C0_BASE + I2C_O_FIFOCTL) = 80008000;
}

Void ledFxn(UArg arg0, UArg arg1)
{
    uint32_t timerval= Clock_getTicks();
    int timer_counter=0;
    ledtoggle_struct data_led;
    while (flag)
    {
        Task_sleep((unsigned int)arg0);
        GPIO_toggle(Board_LED1);
        GPIO_toggle(Board_LED0);
        Semaphore_pend(uartsemwrite, BIOS_WAIT_FOREVER);
        data_led.time1 = Clock_getTicks();
        data_led.led_toggle_count = ++timer_counter;
        xQueueSend(queue_led,(void*)&data_led,(TickType_t)10);
        n=LOGGER_LED;
        Semaphore_post(uartsemread);
    }
}


Void tempFxn(UArg arg0, UArg arg1)
{
    temp_struct data_temp;
    uint8_t data_msb=0,data_lsb=0;
    while (flag)
    {
        Task_sleep((unsigned int)arg0);
        I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, false);

        //specify register to be read
        I2CMasterDataPut(I2C0_BASE, tempreg_addr);

        //send control byte and register address byte to slave device
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);

        //wait for MCU to finish transaction
        while(I2CMasterBusy(I2C0_BASE));

        //specify that we are going to read from slave device
        I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, true);

        //send control byte and read from the register we
        //specified
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);

        //wait for MCU to finish transaction
        while(I2CMasterBusy(I2C0_BASE));

        //return data pulled from the specified register
        data_msb=I2CMasterDataGet(I2C0_BASE);

        //send control byte and read from the register we
       //specified
       I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
       //wait for MCU to finish transaction
       while(I2CMasterBusy(I2C0_BASE));

       data_lsb=I2CMasterDataGet(I2C0_BASE);
       data_temp.temp = (((data_msb << 8) | data_lsb) >> 4)/16.0;
       data_temp.time1 =  Clock_getTicks();
       Semaphore_pend(uartsemwrite, BIOS_WAIT_FOREVER);
       n=LOGGER_TEMP;
       xQueueSend(queue_temp,(void*)&data_temp,10);
       Semaphore_post(uartsemread);
    }
}

Void logFxn(UArg arg0, UArg arg1)
{
    ledtoggle_struct ledop;
    temp_struct tempop;
    while (flag)
    {
        Task_sleep((unsigned int)arg0);
        Semaphore_pend(uartsemread, BIOS_WAIT_FOREVER);
        if(n==LOGGER_LED)
        {
            xQueueReceive(queue_led,&(ledop),(TickType_t)10);
            sprintf(data,"Sanika Dongre timestamp:%d msec\t LED TASK TOGGLE TOTAL is %d\n\r",ledop.time1,ledop.led_toggle_count);
        }
        else if(n==LOGGER_TEMP)
        {
            xQueueReceive(queue_temp,&(tempop),(TickType_t)10);
            sprintf(data,"Sanika Dongre timestamp:%d msec\t TEMP TASK Temp: %d C, %dF, %dK\n\r",tempop.time1, tempop.temp, ((tempop.temp*9)/5),(tempop.temp)+273);
        }
        UART_write(uart,data,strlen(data));
        Semaphore_post(uartsemwrite);
    }
    free(data);
}

 /*  ======== main ========
 */
int main(void)
{
    Task_Params task_led,task_log,task_temp;
    /* Call board init functions */
    Board_initGeneral();
    Board_initGPIO();
    Board_initI2C();
    Board_initUART();
    InitI2C0();
    queue_temp = xQueueCreate(10, sizeof(temp_struct) );
    queue_led = xQueueCreate(10, sizeof(ledtoggle_struct) );
    flag = 1;
    data = (char*)malloc(400);

    //Semaphore Initializations
    Semaphore_Params_init(&uartsemwrite_params);
    Semaphore_construct(&uartsemwrite_struct, 1, &uartsemwrite_params);
    uartsemwrite = Semaphore_handle(&uartsemwrite_struct);
    Semaphore_Params_init(&uartsemread_params);
    Semaphore_construct(&uartsemread_struct, 0, &uartsemread_params);
    uartsemread = Semaphore_handle(&uartsemread_struct);

   //temp task thread
    Task_Params_init(&task_temp);
    task_temp.arg0 = 1000;
    task_temp.stackSize = TASKSTACKSIZE_TEMP;
    task_temp.stack = &temp_stack;
    Task_construct(&tasktemp_struct, (Task_FuncPtr)tempFxn, &task_temp, NULL);

    //led task thread
    Task_Params_init(&task_led);
    task_led.arg0 = 100;
    task_led.stackSize = TASKSTACKSIZE_LED;
    task_led.stack = &led_stack;
    Task_construct(&taskled_struct, (Task_FuncPtr)ledFxn, &task_led, NULL);

    //logger task thread
    Task_Params_init(&task_log);
    task_log.arg0 = 20;
    task_log.stackSize = TASKSTACKSIZE_LOG;
    task_log.stack = &log_stack;
    Task_construct(&tasklog_struct, (Task_FuncPtr)logFxn, &task_log, NULL);
    uart_init();
    /* Start BIOS */
    BIOS_start();
    return (0);
}
