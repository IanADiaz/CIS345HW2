#include "types.h"
#include "stat.h"
#include "user.h"

// bounded access. semaphore starts at 2, so only two child processes can be inside critical section  at once.
void
test_bounded(void)
{
  	int s = semalloc(2);
  	int pm = semalloc(1);
  	int i;
  	for(i = 0; i < 5; i++){
    		if(fork() == 0){
      			semwait(pm); printf(1, "[child %d] trying to enter\n", i); semsignal(pm);
      			semwait(s);
      			semwait(pm); printf(1, "[child %d] IN\n", i); semsignal(pm);
      			sleep(50);
      			semwait(pm); printf(1, "[child %d] OUT\n", i); semsignal(pm);
      			semsignal(s);
      			exit();
    		}
  	}
  	for(i = 0; i < 5; i++)
    		wait();
  	semfree(s);
  	semfree(pm);
}

int
main(void)
{
  	printf(1, "Bounded access test (N=2)\n");
  	test_bounded();
  	printf(1, "Test done\n");
  	exit();
}
