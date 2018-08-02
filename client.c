#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int sockfd, n, counter = 0;
    char buffer[1024 + 1];
    struct sockaddr_in servaddr;
    //commond args
    if (argc != 2){
        perror("commond args are not enough");
    }
    //create connect socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("create socket error");
    }
    //init server addr
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(6444);
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) < 0){
        perror("init server addr error");
    }
    //connect
    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
        perror("connect error");
    }
    //read
    while((n =  read(sockfd, buffer, 1024)) > 0){
        counter++;
        //null terminate
        buffer[n] = 0;
        if (fputs(buffer, stdout) == EOF){
            perror("fputs error");
        }
    }
    if (n < 0){
        perror("read error");
    }
    printf("counter = %d\n", counter);
    exit(0);
}
