#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// GET YEAR FUNCTION I CREATED for get the year
// return year 1972
int
sys_getyear(void)
{
  return 1975;
}

// GET YEAR FUNCTION I CREATED for get size
// return year 1972
int
sys_getmysize(void)
{
  return proc->sz;
}


// ----------------------------------------
// GET YEAR FUNCTION I CREATED for get size
// return year 1972
int
sys_getkernalstartaddr(void)
{
  return KERNBASE;
}

// GET YEAR FUNCTION I CREATED for get size
// return year 1972
int
sys_getkernalendaddr(void)
{
  return KERNBASE+PHYSTOP;
}

// GET YEAR FUNCTION I CREATED for get size
// return year 1972
int
sys_getkernalvariaddr(void)
{
  int a;
  int b = (int) &a;
  return b;
}

// GET YEAR FUNCTION I CREATED for get size
// return year 1972
int
sys_getsystemcalladdr(void)
{
  return (int) &sys_fork;
}

// set priority
int
sys_setpriority(void)
{
  int priority;
  if(argint(0, &priority) < 0)
    return -1;
  proc->priority = priority;
  return (int) proc->priority;
}

//sample
int
sys_mycall(void)
{
    int size;
    char *buf;
    char *s;

    if (argint(0, &size) <0){
        return -1;
    }

    if (argptr(1, &buf,size) <0){
        return -1;
    }

    s = buf;

    while(buf + size > s){
        *(int *)s = 1;
        s+=4;
        *(int *)s = 2;
        s+=4;
    }

    return 3;
}

//get p table function to this scope
extern struct proc * proc_table_start(void);
int
sys_getptable(void)
{
    int size;
    char *buf;
    char *s;
   
    if (argint(0, &size) <0){
        return -1;
    }

    if (argptr(1, &buf,size) <0){
        return -1;
    }

    s = buf;
    
    struct proc *p = '\0';
    p = proc_table_start();

    int process_count = 0;
    while(buf + size > s && p->state != UNUSED){
	*(int *)s = p->pid;
  	s += 4;
	*(int *)s = p->state;
  	s += 4;
	memmove(s,p->name,16);
	s += 16;
  	*(int *)s = p->parent->pid;
  	s += 4;
	*(int *)s = p->priority;
  	s += 4;
	*(int *)s = p->sz;
  	s += 4;
	*(int *)s = p->killed;
  	s += 4;
	// increment to the next address
	p++;
	process_count++;
    }

    return process_count;
}

