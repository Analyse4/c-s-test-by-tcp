//
//  server.c
//  cstest
//
//  Created by logic_04 on 2018/8/1.
//  Copyright © 2018年 logic_04. All rights reserved.
//

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "protocol.h"


int PersonInfoReqHandle(const char * buffer, int connectfd)
{
    int nbytes = 0;
    PersonInfoReq personinforeq;
    PersonInfoAck personinfoack;
    memcpy(&personinforeq, buffer, sizeof(PersonInfoReq));
    // print msg
    printf("messageid: %d\n", personinforeq.header.id);
    printf("person_age: %d\n", personinforeq.age);
    printf("person_atk: %d\n", personinforeq.atk);
    //edit msg ack
    personinfoack.header.id = personinforeq.header.id;
    personinfoack.age = personinforeq.age;
    personinfoack.atk = personinforeq.atk + 100;
    //write to connectfd
    nbytes = sizeof(PersonInfoAck);
    if ((write(connectfd, &personinfoack, nbytes) < nbytes)) {
        perror("write error");
    }
	
    return 0;
}



int OnMsg(const char * buffer, int connectfd)
{
    printf("in on msg\n");
    Header * pheader = (Header *)malloc(sizeof(Header));
    memcpy(pheader, buffer, sizeof(Header));
    printf("message id is %d\n", pheader->id);
    switch(pheader->id){
        case Msg_PersonInfoReq:
            printf("in msg_personinforeq\n");
            PersonInfoReqHandle(buffer, connectfd);
            break;
        case 2:
            //TODO:
            break;
        default:
            printf("The message is undefined!\n");
    }
}



int main(int argc, const char * argv[]) {
    int n, nbytes;
    int listenfd, connectfd;
    struct sockaddr_in servaddr;
    PersonInfoReq personinforeq;
    char buffer[20];
    memset(buffer , 0, 20);
    //create listen socket
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket error");
    }
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(6444);
    //bind
    if (bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr))) {
        perror("bind error");
    }
    //listen
    if ((n = listen(listenfd, 1024)) < 0) {
        perror("listen error");
    }
    //accept
    for (; ; ) {
        if ((connectfd = accept(listenfd, NULL, 0)) < 0){
            perror("accept error");
        }
        int count = 0;
        //read from client
        //这样写有bug，一旦客户端又写又读将会阻塞在read函数里，
        //从打印信息可以看出read所在的循环循环到第二次卡住，
        //服务端内核空间确实有客户端写来的数据
        //服务端的read循环第一次把服务端内核空间的数据拷贝到用户空间满足循环条件从而进入第二次循环判断
        //之后阻塞在第二次read里
        
//        while((n = read(connectfd, buffer, 19)) > 0){
//            count++;
//            buffer[n] = 0;
//            if (fputs(buffer, stdout) == EOF){
//                perror("fputs error");
//            }
//            printf("count: %d\n", count);
//        }
        
        
//        //recv from client
        n = recv(connectfd, buffer, 19, 0);
        buffer[n] = 0;

        if(n < 0){
            perror("read error");
        }
        if (fputs(buffer, stdout) == EOF){
            perror("fputs error");
        }
        
        
        printf("receive %d bytes\n", strlen(buffer));
        //memcpy(&personinforeq, buffer, sizeof(PersonInfoReq));

        //printf("messageid: %d\n", personinforeq.header.id);
        //printf("person_age: %d\n", personinforeq.age);
        //printf("person_atk: %d\n", personinforeq.atk);
        //消息处理
        OnMsg(buffer, connectfd);
        //specific logic
        //ticks = time(NULL);
        //snprintf(buffer, sizeof(buffer), "%.24s\r\n", ctime(&ticks));
        ////write to connectfd
        //nbytes = strlen(buffer);
        //if ((write(connectfd, buffer, nbytes) < nbytes)) {
        //    perror("write error");
        //}
        //close connectfd
        close(connectfd);
    }
    return 0;
}
