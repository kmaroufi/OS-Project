#include<stdio.h>
#include<sys/time.h>
#include<unistd.h>
#define time_interval 1000  //interval for checking cpu usage

/*
	checks cpu usage in a certain time interval 
	and prints the cpu utilization in that interval as a percentage
*/
int main(){
	struct timeval t0; // 1st_time_taken
   	struct timeval t1; // 2nd_time_taken
   	char *address = "/proc/stat";  //address of cpu status file
   	FILE *statFile = fopen(address, "r"); 
	float elapsed;
	int user_usage1, nice_usage1 ,kernel_usage1, idle1, io_wait1, irq1, soft_irq1,user_usage2, nice_usage2 ,kernel_usage2, idle2, io_wait2, irq2, soft_irq2; 
	fscanf(statFile,"cpu  %d %d %d %d %d %d %d",&user_usage1, &nice_usage1 ,&kernel_usage1, &idle1, &io_wait1, &irq1, &soft_irq1); //gets the first line of cpu status file
	fclose(statFile);
	statFile = NULL;
	gettimeofday(&t0, 0);
	sleep(1);
   	while(1){   //waiting for the end of time interval
	   	elapsed = (t1.tv_sec - t0.tv_sec) / 1000.0f + (t1.tv_usec - t0.tv_usec) * 1000.0f;
   		if(elapsed >= time_interval){	
		   	statFile = fopen(address, "r");
			gettimeofday(&t1, 0);
   			fscanf(statFile,"cpu  %d %d %d %d %d %d %d ",&user_usage2, &nice_usage2 ,&kernel_usage2, &idle2, &io_wait2, &irq2, &soft_irq2); //
			break;
		}
	}
	fclose(statFile);
	int real_usage = 
   		(user_usage2 - user_usage1) + 
		   (nice_usage2 - nice_usage1) + 
		   (kernel_usage2 -kernel_usage1)+ 
		   (irq2 - irq1) + 
		   (soft_irq2 - soft_irq1); //real usage time
//	printf("usages : %d %d %d %d %d %d %d",user_usage1, nice_usage1 ,kernel_usage1, idle1, io_wait1, irq1, soft_irq1);	   
//	printf("usages : %d %d %d %d %d %d %d",user_usage2, nice_usage2 ,kernel_usage2, idle2, io_wait2, irq2, soft_irq2);
//	printf("delta_usages: %d %d %d %d %d %d %d",	(user_usage2 - user_usage1) , 
//		   (nice_usage2 - nice_usage1) , 
//		   (kernel_usage2 -kernel_usage1), 
//		   (irq2 - irq1) ,
//		   (soft_irq2 - soft_irq1),
//		   (idle2 - idle1) , 
//		   (io_wait2 - io_wait1));
	int idle_usage =
		(idle2 - idle1) + 
		   (io_wait2 - io_wait1) ;     //idle usage times
   	float utilization = (float)real_usage / ((float)real_usage+(float)idle_usage); // utilization of cpu
   	printf("cpu utilization : %.2f%%\n",utilization*100.0);
	return 0;
}
