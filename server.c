//
//  main.c
//  cstest2
//
//  Created by Ozil on 2018/8/1.
//  Copyright © 2018年 Ozil. All rights reserved.
//

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

int main(int argc, const char * argv[]) {
    int n, nbytes;
    int listenfd, connectfd;
    struct sockaddr_in servaddr;
    char buffer[20];
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
        //specific logic
        ticks = time(NULL);
        snprintf(buffer, sizeof(buffer), "%.24s\r\n", ctime(&ticks));
        //write to connectfd
        nbytes = strlen(buffer);
        if ((write(connectfd, buffer, nbytes) < nbytes)) {
            perror("write error");
        }
        //close connectfd
        close(connectfd);
    }
    return 0;
}