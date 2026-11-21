#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"

struct semaphore {
  	struct spinlock lock;
  	int value;
  	int used;
};

struct semaphore sems[2];

void
seminit(void)
{
	int i;
  	for(i = 0; i < 2; i++){
		initlock(&sems[i].lock, "sem");
    		sems[i].used = 0;
    		sems[i].value = 0;
  	}
}

//  semaphore with an initial value. returns its id, or -1.
int
semalloc(int value)
{
  	int i;
  	if(value < 0)
    		return -1;
  	for(i = 0; i < 2; i++){
    		acquire(&sems[i].lock);
    		if(!sems[i].used){
      			sems[i].used = 1;
      			sems[i].value = value;
      			release(&sems[i].lock);
      			return i;
    		}
    	release(&sems[i].lock);
  	}
  	return -1;
}


int
semwait(int id)
{
  	struct semaphore *s;
  	if(id < 0 || id >= 2)
    		return -1;
  	s = &sems[id];
  	acquire(&s->lock);
  	while(s->used && s->value <= 0){
    		if(myproc()->killed){
      			release(&s->lock);
      			return -1;
    		}
    		sleep(s, &s->lock);
  	}
  	if(!s->used){
    		release(&s->lock);
    		return -1;
  	}
  	s->value--;
  	release(&s->lock);
  	return 0;
}

// increment and wake processes
int
semsignal(int id)
{
  	struct semaphore *s;
  	if(id < 0 || id >= 2)
    		return -1;
  	s = &sems[id];
  	acquire(&s->lock);
  	if(!s->used){
    		release(&s->lock);
    		return -1;
  	}
  	s->value++;
  	wakeup(s);
  	release(&s->lock);
  	return 0;
}

int
semfree(int id)
{
  	if(id < 0 || id >= 2)
    		return -1;
  	acquire(&sems[id].lock);
  	sems[id].used = 0;
  	wakeup(&sems[id]);
  	release(&sems[id].lock);
  	return 0;
}
