#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
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
  return myproc()->pid;
}

int
sys_getppid(void)
{
	return myproc()->parent->pid;
}

// yield the cpu to the next process
void
sys_yield(void)
{
	struct proc * p = myproc();
	// check variable prevent gaming sheduling
	static int check = 0;
	int stride_share = 100 - mlfq_share;
	check += stride_share;
	if (check > 100) {
		p->ticks++;
		totalticks++;
		check -= 100;
	}
	yield();
}

// get the level of current process ready queue of MLFQ.
// Returns one of the level of MLFQ (0/1/2)
int
sys_getlev(void)
{
	return myproc()->level;
}

// inquires to obtain cpu share(%)
int 
sys_set_cpu_share(void)
{
	int share;
	argint(0, &share);
	return set_cpu_share(share);
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
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
    if(myproc()->killed){
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
