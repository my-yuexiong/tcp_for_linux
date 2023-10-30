#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
    //1. create a socket fot listening
    int fd = socket(AF_INET,SOCK_STREAM,0);

    //check whether the socket is successfully created
    if(fd == -1)
    {
        std::cout<<"creation failure"<<std::endl;
        return -1;
    }

    //2. bind the local ip address and port
    struct sockaddr_in saddr;//define a struct
    //initializes three members
    saddr.sin_family = AF_INET;//ipv4
    saddr.sin_port = htons(22222);
    saddr.sin_addr.s_addr = INADDR_ANY;//listen to all interfaces

    int ret = bind(fd,(struct sockaddr*)&saddr,sizeof(saddr));

    //check whether the binding is successful
    if(ret == -1)
    {
        std::cout<<"binding failure"<<std::endl;
        return -1;
    }

    //3. set listening
    ret = listen(fd,5);
    //check whether the listening is successful
    if(ret == -1)
    {
        std::cout<<"listening failure"<<std::endl;
        return -1;
    }
    //4. block and wait for the client to connect
    struct sockaddr_in caddr;
    socklen_t addrlen = sizeof(caddr);
    int cfd = accept(fd,(sockaddr *)&caddr,&addrlen);

    //check whether the connection is successful
    if(cfd == -1)
    {
        std::cout<<"connection failure"<<std::endl;
        return -1;
    }
    //if the connection is successfully estanlishen, the ip and port of the client are displayed
    char ip[32];
    std::cout<<inet_ntop(AF_INET,&caddr.sin_addr.s_addr,ip,sizeof(ip))<<std::endl;
    std::cout<<ntohs(caddr.sin_port)<<std::endl;

    //5. communicate
    while(1)
    {
        //accepting of date
        char buff[1024];
        int len = recv(cfd,buff,sizeof(buff),0);

        //check whether the date is accepted successfully
        if(len > 0)
        {
            printf("client say:%s\n",buff);
            send(cfd,buff,len,0);
        }
        else if(len == 0)
        {
            printf("the client is disconnected\n");
            break;
        }
        else
        {
            std::cout<<"connection failure"<<std::endl;
            break;
        }
    }

    //6. close the socket
    close(fd);
    close(cfd);
    return 0;
}