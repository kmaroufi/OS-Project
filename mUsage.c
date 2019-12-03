#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/kthread.h>
#include <linux/delay.h>


MODULE_LICENSE("OSOS");       // set licence of module
MODULE_AUTHOR("Hn.Bktsh");    // author of modules
MODULE_DESCRIPTION("Constantly shows Memory usage by proccesses");


int prev_mem[30000]; // last status of memory usages
bool goOn = true; // set to false upon cleanup

int checker(void *data)   // thread that runs upon initialization
{
	struct task_struct* task;
	while(goOn)
	{  
		for_each_process(task)             // checking all proccesses
		{
			if (task->mm != NULL && prev_mem[task->pid] != task->mm->total_vm) // if the amount of memory allocated by the process has changed
			{ 
					if (prev_mem[task->pid] < task->mm->total_vm) //allocation
						printk(KERN_INFO " Allocation   : process %d += %d pages", task->pid, task->mm->total_vm - prev_mem[task->pid]);
					else if (prev_mem[task->pid] > task->mm->total_vm) // deallocation
						printk(KERN_INFO " Deallocation : process %d -= %d pages", task->pid, prev_mem[task->pid] - task->mm->total_vm);
					prev_mem[task->pid] = task->mm->total_vm; // update stats
			}
		}
		msleep(1);
	}
	return 0;
}

struct task_struct *thread;

int start(void) {
	printk(KERN_INFO "***** Module initialized *****");
	thread = kthread_create(&checker,NULL,"Thread");
        thread = kthread_run(&checker,NULL,"Thread");
 	return 0;
}

void remove(void){
	goOn = false;
	printk(KERN_INFO "***** Module aborted *****");
	kthread_stop(thread);
}

module_init(start);
module_exit(remove);
