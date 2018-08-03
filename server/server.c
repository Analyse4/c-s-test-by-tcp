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


int OnMsg(const char * buffer)
{
    PersonInfoReq personinforeq;
    Header * pheader = (Header *)malloc(sizeof(Header));
    memcpy(pheader, buffer, sizeof(Header));
    printf("message id is %d\n", pheader->id);
    switch(pheader->id){
        case 1:
            memcpy(&personinforeq, buffer, sizeof(PersonInfoReq));
            printf("messageid: %d\n", personinforeq.header.id);
            printf("person_age: %d\n", personinforeq.age);
            printf("person_atk: %d\n", personinforeq.atk);

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
    time_t ticks;
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
        //read from client
        while((n = read(connectfd, buffer, 19)) > 0){
            buffer[n] = 0;
            if (fputs(buffer, stdout) == EOF){
                perror("fputs error");
            }
        }
        if(n < 0){
            perror("read error");
        }
        printf("receive %d bytes\n", strlen(buffer));
        memcpy(&personinforeq, buffer, sizeof(PersonInfoReq));

        printf("messageid: %d\n", personinforeq.header.id);
        printf("person_age: %d\n", personinforeq.age);
        printf("person_atk: %d\n", personinforeq.atk);
        //消息处理
        OnMsg(buffer);
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
