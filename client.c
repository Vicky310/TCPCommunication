// Arvind Narayan Srinivasan - 105187866
// Vigneshwaran Balasubramani - 105221382
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<netinet/in.h>
#include<fcntl.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<time.h>

int main(int argc, char *argv[]){
char message[255];
char score[255];
int server, portNumber;
socklen_t len;
struct sockaddr_in servAdd;
if(argc != 3){
printf("Call model:%s <IP> <Port#>\n",argv[0]);
exit(0);
}
if((server = socket(AF_INET, SOCK_STREAM, 0))<0){
fprintf(stderr, "Cannot create socket\n");
exit(1);
}
servAdd.sin_family = AF_INET;
sscanf(argv[2], "%d", &portNumber);
servAdd.sin_port = htons((uint16_t)portNumber);
if(inet_pton(AF_INET,argv[1],&servAdd.sin_addr)<0){
fprintf(stderr, " inet_pton() has failed\n");
exit(2);
}
if(connect(server,(struct sockaddr *)&servAdd,
sizeof(servAdd))<0){
fprintf(stderr, "connect() has failed, exiting\n");
exit(3);
}
while(1){
if(read(server, message, 255)<0){
fprintf(stderr, "read() error\n");
exit(3);
}
int dice = 0;
long int ss=0;
if(!strcmp(message,"Game over: you won the game")) {
	printf("I won the game\n");
	close(server);
	exit(0);
} else if (!strcmp(message,"Game over: you lost the game")){
	printf("I lost the game\n");
	close(server);
	exit(0);
} else if(!strcmp(message,"You can now play")) {
	printf("Playing my dice\n");
	dice =(int) time(&ss)%10 + 1;
	printf("My Score is \n%d\n", dice);
	sleep(3);
	sprintf(score,"%d", dice);
	write(server, score, strlen(score)+1);
} else {
	fprintf(stderr, "Not a valid command \n%s",message);
	close(server);
	exit(1);
}

}
}
