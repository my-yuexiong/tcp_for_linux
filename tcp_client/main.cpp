#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>

int main()
{
    //1. create socket for communication
    int fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd == -1)
    {
        std::cout<<"creatation failue"<<std::endl;
        return -1;
    }

    //2. connecting ip address and port of the server
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(22222);
    inet_pton(AF_INET,"192.168.176.193",&saddr.sin_addr.s_addr);
    int ret = connect(fd,(struct sockaddr*)&saddr,sizeof(saddr));
    if(ret == -1)
    {
        std::cout<<"connection failue"<<std::endl;
        return -1;
    }

    //3. communicate
    int num = 0;
    while(1)
    {
        //send date
        char buff[1024];
        sprintf(buff,"hello server%d\n",num++);
        send(fd,buff,strlen(buff)+1,0);

        //recieve date
        memset(buff,0,sizeof(buff));
        int len = recv(fd,buff,sizeof(buff),0);
        if(len > 0)
        {
            printf("server say:%s\n",buff);
        }
        else if(len == 0)
        {
            printf("the server is disconnected\n");
            break;
        }
        else
        {
            std::cout<<"-1"<<std::endl;
            break;
        }
        sleep(1);
    }

    //4. close the socket
    close(fd);
    return 0;
}