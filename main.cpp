#include <iostream>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <unistd.h>
#include <string.h>
#include "can_receiver.h"
// #define IFNAME "can0"

// struct ifreq ifr;
// struct sockaddr_can can_addr;
struct can_frame canframe;

int main(void){
    CanReceiver& can_receiver=CanReceiver::getInstance();
    int nbytes;
    while(1){
        //std::cout<<"\033[2J\033[H";
        nbytes=can_receiver.GetCanMessage(&canframe,sizeof(struct can_frame));
        if(nbytes){
            nbytes=0;
            std::cout<<"message id:"<<canframe.can_id<<std::endl;
            std::cout<<"message data:";
            for(u_int8_t i=0;i<canframe.len;i++)
                std::cout<<(int)canframe.data[i]<<" ";
            std::cout<<std::endl;
        }
    }
    return 0;
}