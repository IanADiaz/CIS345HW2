#include "types.h"
#include "defs.h"
#include "param.h"
#include "x86.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"
#include "qlock.h"

void
initqlock(struct qlock *q, char *name)
{
	initlock(&q->lk, "qlock");
	q->name = name;
	q->locked = 0;
}

void
acquireq(struct qlock *q)
{
	acquire(&q->lk);
	while (q->locked) {
		sleep(q, &q->lk);
	}
	q->locked = 1;
	release(&q->lk);
}

void
releaseq(struct qlock *q)
{
	acquire(&q->lk);
	q->locked = 0;
	wakeup(q);
	release(&q->lk);
}
