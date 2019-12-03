#include <stdio.h>
#include <sys/time.h>
#include <time.h>

/*
	this code prints current local time in a like a boss format.
*/

int main() {
	// defining tv varibale for time information from syscall
    struct timeval tv;
    // call gettimeofday syscall to get time information in tv variable
    gettimeofday(&tv, 0);
    // defining timeinfo varibale for time info in manner format
    struct tm * timeinfo;
    time_t rawtime = tv.tv_sec;
    // call localtime function for converting time info in user freindly format
    timeinfo = localtime (&rawtime);
    printf ("current local time and date: %s", asctime(timeinfo));
	return 0;
}
