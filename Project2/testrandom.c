#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#define BUFFER_LENGTH 20
static char receive[BUFFER_LENGTH] = {'\0'}; 

int main(){
	int ret, fd;
	fd = open("/dev/RANDOMDevice",O_RDWR);
	if(fd < 0){
		perror("Can't open Device !\n");
		return errno;
	}

	printf("\nReading from Device \n");
	ret = read(fd,receive,BUFFER_LENGTH);
	if(ret < 0){
		printf("\nFailed !!!\n");
		return errno;
	}
	else{
		printf("Random Number is %s \n", receive);
	}
	return 0;
}
