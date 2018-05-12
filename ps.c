#include "types.h"
#include "stat.h"
#include "user.h"

#define NUM_PROCESSES 10

enum procstate { UNUSED, EMBRYO, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };

struct pstat {
    int pid;
    enum procstate state;
    char name[16]; 
    int parent_pid;
    int priority;
    int memory;
    int killed;
};

int main(void) {
    static char *states[] = {
	  [UNUSED]    "unused",
	  [EMBRYO]    "embryo",
	  [SLEEPING]  "sleep ",
	  [RUNNABLE]  "runble",
	  [RUNNING]   "run   ",
	  [ZOMBIE]    "zombie"
    };

    struct pstat process_array[NUM_PROCESSES];
    struct pstat *p;
    int process_count = getptable( NUM_PROCESSES*sizeof(struct pstat), &process_array);
    if(process_count > -1){
	p = &process_array[0];
	int i;
	printf(1,"pid\tstate\tpriority\tkilled\tname\tmemory\tparent pid\t \n");
	for(i=0; i<process_count; i++){
		printf(1, "%d\t", p->pid);
		printf(1, "%s\t", states[p->state]);
		printf(1, "%d\t\t",p->priority);
		printf(1, "%d\t",p->killed);
		printf(1, "%s\t", p->name);
		printf(1, "%d\t", p->memory);
		printf(1, "%d\t",p->parent_pid);		
		printf(1, "\n");
		p++;
	}

    }else{
	printf(1,"Failed to execute the command \n");
    }
    
    exit();
}
