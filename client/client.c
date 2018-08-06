//
//client.c
//cstest
//
//Created by logic_04 on 2018/8/1.
//Copyright @ 2018 logic_04. All right reserved.
//

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include "protocol.h"
#include <thread>
#include <iostream>

void Write(int sockfd){
    int n = 0;
    //fill req
    PersonInfoReq personinforeq;
    personinforeq.header.id = 1;
    personinforeq.age = 10;
    personinforeq.atk = 20;
    //write to server
    if((n = write(sockfd, &personinforeq, sizeof(personinforeq))) < sizeof(personinforeq)){
        perror("write error");
    }
}

void Read(int sockfd){
    int n = 0;
    //init msg ack
    PersonInfoAck personinfoack;
    
//    sleep(10);
//
    //read from server
    while((n = read(sockfd, &personinfoack, sizeof(personinfoack))) > 0){
        printf("messageid: %d\n", personinfoack.header.id);
        printf("person_age: %d\n", personinfoack.age);
        printf("person_atk; %d\n", personinfoack.atk);
    }
    if (n < 0){
        perror("read error");
    }
}

int Test2(int sockfd){
    std::thread threads[2];
    //start threads
    std::cout << "Spawning 2 threads for writing and reading..." << std::endl;
    threads[0] = std::thread(Write, sockfd);
    threads[1] = std::thread(Read, sockfd);
    std::cout << "Done spawning threads!Now wait for them to join" << std::endl;
    //join
    for (auto& t: threads) {
        t.join();
    }
    std::cout << "All thread join\n" << std::endl;
    return 0;
}

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
    memset(&servaddr, 0, sizeof(servaddr));
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
    //Test1(sockfd);
    
    //test2
    Test2(sockfd);
    
    
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
    _exit(0);
}
