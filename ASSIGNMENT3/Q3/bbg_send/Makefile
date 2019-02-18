
obj-m += sanika_timer.o

bbg_out:
	make ARCH=arm CROSS_COMPILE=~/buildroot/output/host/bin/arm-buildroot-linux-uclibcgnueabihf- -C ~/buildroot/output/build/linux-4796173fc58688055a99a1cef19a839174067220 M=$(shell pwd) modules

cleanbbg: 
	make -C ~/buildroot/output/build/linux-4796173fc58688055a99a1cef19a839174067220 M=$(shell pwd) clean
