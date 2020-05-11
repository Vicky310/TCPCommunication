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

void servicePlayers(int *sd, char **playerNames){

	char *messages[] = {"Game over: you won the game",
			   "Game over: you lost the game",
			   "You can now play"};	
	int totalScore[2];
	int player1score, player2score;
	totalScore[0]=0;
	char message[255];
	int n;
	totalScore[1]=0;
	printf("\n Starting the game \n");
	while(1) {
		if(totalScore[0]>=100 || totalScore[1]>=100)
		{
			if(totalScore[0]>=100)
			{
				write(sd[0],messages[0], strlen(messages[0])+1);
				write(sd[1],messages[1], strlen(messages[1])+1);
				printf("\n%s Won the game with %d points\n",playerNames[0], totalScore[0]);				
			}
			else {
				write(sd[1],messages[0], strlen(messages[0])+1);
                write(sd[0],messages[1], strlen(messages[1])+1);
				printf("\n%s Won the game with %d points\n",playerNames[1], totalScore[1]);	
			}
			close(sd[0]);
			close(sd[1]);
			return;
		}
		write(sd[0],messages[2], (strlen(messages[2])+1));
		if(n=read(sd[0], message, 255)){
			message[n]='\0';
			sscanf(message,"%d",&player1score);
			totalScore[0]+=player1score;
			printf("%s score \n%d\n",playerNames[0], totalScore[0]);

		}		
		sleep(2);
		write(sd[1],messages[2], (strlen(messages[2])+1));
                if(n=read(sd[1], message, 255)){
                        message[n]='\0';
			sscanf(message,"%d",&player2score);
			totalScore[1]+=player2score;
                        printf("%s score \n%d\n",playerNames[1], totalScore[1]);

                }
	
	}

}


int main(int argc, char *argv[]){
int sd, player, portNumber;
int count = 0;
int players[2];
char *playerNames[2] = {"TOTO","TITI"};
socklen_t len;
struct sockaddr_in servAdd;
if(argc != 2){
printf("Call model: %s <Port #>\n", argv[0]);
exit(0);
}
if ((sd=socket(AF_INET,SOCK_STREAM,0))<0){
fprintf(stderr, "Cannot create socket\n");
exit(1);
}
servAdd.sin_family = AF_INET;
servAdd.sin_addr.s_addr = htonl(INADDR_ANY);
sscanf(argv[1], "%d", &portNumber);
servAdd.sin_port = htons((uint16_t)portNumber);
bind(sd,(struct sockaddr*)&servAdd,sizeof(servAdd));
listen(sd, 5);
while(1){
if(count <3) {
player=accept(sd,(struct sockaddr*)NULL,NULL);
if(player) {
	players[count] = player;
	count++;
}

printf("\nGot player %d\n",count);	
} else {
	printf("Cannot accept more players!");
}
if(count == 2) {
	if(!fork()) {
		servicePlayers(players, playerNames);
		exit(0);
	}
	count = 0;
	close(players[0]);
	close(players[1]);

} else if(count == 1) {
	printf("\n Waiting for one more player\n");
}

}
}

