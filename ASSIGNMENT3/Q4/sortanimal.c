#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/sort.h>
#include <linux/slab.h>
#include <linux/sort.h>
#define MAX (50)

char* animal_array[MAX]={"dog","cat","bear","tiger","cheetah","leopard","lion","fox","wolf","cow","bison","ox","zebra","horse","donkey","yak","snake","monkey","giraffe","stag","deer","hyena","bat","zebra","fox","bison","cow","cow","cheetah","cheetah","giraffe","wolf","lion","fish","crocodile","rhino","monkey","cat","dog","monkey","donkey","yak","panda","penguin","tiger","lion","hyena","ox","bison","rat"};

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SANIKA DONGRE");
MODULE_DESCRIPTION("Creating a sorting module");
MODULE_VERSION("0.1");

struct node_t
{
	char* animal_type;
	int count;
	struct list_head list;
}listnodeanimals, listnodeanimals_filter;

char* filteranimal = "none";
int filtercount = 0;

module_param(filteranimal, charp, 0644);
module_param(filtercount, int, 0644);
MODULE_PARM_DESC(filteranimal,"The animal names will be considereds");
MODULE_PARM_DESC(filtercount,"The animals who are greater than the specified value will be considered");

struct node_t *nodeheadptr, *nodeheadptr1;
struct list_head *headptr;
char* nodeprev[1];
int counter=1,nodecounter=0,filterednodecounter = 0;

static int linkedlist_create(void)
{
	char* temp;
	int i=0,j=0,found=0;
	for(j=0;j<MAX;j++)
	{
		for(i=j;i<MAX;i++)
		{
			if(strcmp(*(animal_array+j),*(animal_array+i))>0)
			{
				temp=*(animal_array+i);
            			*(animal_array+i)=*(animal_array+j);
            			*(animal_array+j)=temp;
			}
		}
	}
	INIT_LIST_HEAD(&listnodeanimals.list);
	INIT_LIST_HEAD(&listnodeanimals_filter.list);
	for(i=0; i<MAX; i++)
	{
		found=0;
		list_for_each_entry(nodeheadptr, &listnodeanimals.list,list)
		{	
			if(strcmp(animal_array[i],nodeheadptr->animal_type) == 0)
			{
				found=1;
				nodeheadptr->count += 1;
				break;			
			}
		}
		if(found==0)
		{
			nodeheadptr = (struct node_t*)kmalloc(sizeof(listnodeanimals),GFP_KERNEL);
			nodeheadptr->animal_type = animal_array[i];
			nodeheadptr->count = 1;
			list_add_tail(&(nodeheadptr->list),&(listnodeanimals.list));
			nodecounter ++;
		}
	}
	list_for_each_entry(nodeheadptr, &listnodeanimals.list,list)
	{
		printk("The animal type is %s and the count is %d",nodeheadptr->animal_type,nodeheadptr->count);
		if(strcmp(filteranimal,"none")== 0)
		{
			if(nodeheadptr->count > filtercount)
			{
				nodeheadptr1 = (struct node_t *)kmalloc(sizeof(listnodeanimals),GFP_KERNEL);
				nodeheadptr1->animal_type = nodeheadptr->animal_type;
				nodeheadptr1->count = nodeheadptr->count;
				list_add_tail(&(nodeheadptr1->list),&(listnodeanimals_filter.list));
				filterednodecounter++;
			}
		}
		else if((strcmp(nodeheadptr->animal_type,filteranimal)== 0) && (nodeheadptr->count > filtercount))
		{
			nodeheadptr1 = (struct node_t *)kmalloc(sizeof(listnodeanimals),GFP_KERNEL);
			nodeheadptr1->animal_type = nodeheadptr->animal_type;
			nodeheadptr1->count = nodeheadptr->count;
			list_add_tail(&(nodeheadptr1->list),&(listnodeanimals_filter.list));
			filterednodecounter++;
		}
	}
	printk("The number of nodes are %d",nodecounter);
	printk("Amount of memory dynamically allocated is %d bytes",nodecounter*sizeof(listnodeanimals));
	printk("The filter criteria is animal type is %s and the count is %d",filteranimal,filtercount);
	list_for_each_entry(nodeheadptr1, &listnodeanimals_filter.list,list)
	{
		printk("animal_type is %s and count is %d",nodeheadptr1->animal_type,nodeheadptr1->count);
	}
	printk("The filtered animals are %d",filterednodecounter);
	printk("Amount of memory dynamically allocated  for filtered animals is %d bytes",filterednodecounter*sizeof(listnodeanimals));
	printk("\n");
	return 0;
}

static void linkedlist_exit(void)
{
	list_for_each(headptr, &listnodeanimals.list)
	{
		kfree(list_entry(headptr, struct node_t, list));
	}
	printk("first list is free");
	printk("The memory freed is %d bytes",nodecounter*sizeof(listnodeanimals));
	list_for_each(headptr, &listnodeanimals_filter.list)
	{
		kfree(list_entry(headptr, struct node_t, list));
	}
	printk("The second linked list is free");
	printk("The memory freed is %d bytes",filterednodecounter*sizeof(listnodeanimals));
}

module_init(linkedlist_create);
module_exit(linkedlist_exit);
