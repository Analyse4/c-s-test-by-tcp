//
//client.c
//cstest
//
//Created by logic_04 on 2018/8/1.
//Copyright @ 2018 logic_04. All right reserved.
//

#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include "protocol.h"

int Test1(int sockfd){
    int n = 0;
    //fill req
    PersonInfoReq personinforeq;
    personinforeq.header.id = 1;
    personinforeq.age = 10;
    personinforeq.atk = 20;
    //init msg ack
    PersonInfoAck personinfoack;
    //write to server
    if((n = write(sockfd, &personinforeq, sizeof(personinforeq))) < sizeof(personinforeq)){
        perror("write error");
    }
    //read from server

    sleep(1);

    while((n = read(sockfd, &personinfoack, sizeof(personinfoack))) > 0){
        printf("messageid: %d\n", personinfoack.header.id);
        printf("person_age: %d\n", personinfoack.age);
        printf("person_atk; %d\n", personinfoack.atk);
    }
    if (n < 0){
        perror("read error");
    }


    return 0;
}


int main(int argc, char **argv)
{
    int sockfd, counter = 0;
    char buffer[1024 + 1];
    struct sockaddr_in servaddr;
    PersonInfoReq personinforeq;
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
    //test1
    Test1(sockfd);


    //fill req
    //personinforeq.header.id = 1;
    //personinforeq.age = 10;
    //personinforeq.atk = 20;
    ////write to server
    //if((n = write(sockfd, &personinforeq, sizeof(personinforeq))) < sizeof(personinforeq)){
    //    perror("write error");
    //}
    ////read
   // while((n =  read(sockfd, buffer, 1024)) > 0){
   //     counter++;
   //     //null terminate
   //     buffer[n] = 0;
   //     if (fputs(buffer, stdout) == EOF){
   //         perror("fputs error");
   //     }
   // }
   // if (n < 0){
   //     perror("read error");
   // }
   // printf("counter = %d\n", counter);
    exit(0);
}
