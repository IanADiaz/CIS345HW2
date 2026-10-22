#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
	int i, j;
	for (i = 0; i < 4; i++){
		if(fork() == 0){
			for(j = 0; j < 3; j++)
				qtest();
			exit();
		}
	}
	for(i = 0; i < 4; i++)
		wait();
	exit();
}
