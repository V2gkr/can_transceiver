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

union chartofloatconverter{
    u_int8_t chars[4];
    float floatval;
};

float CharToFloat(u_int8_t *arr);

int main(void){
    CanReceiver& can_receiver=CanReceiver::getInstance();
    int nbytes;
    while(1){
        std::cout<<"\033[H\033[2J";
        nbytes=can_receiver.GetCanMessage(&canframe,sizeof(struct can_frame));
        if(nbytes){
            nbytes=0;
            std::cout<<"\nmessage id: 0x"<<std::hex<<canframe.can_id<<"\n";
            std::cout<<"message data:";
            std::cout<<CharToFloat(canframe.data);
            // for(u_int8_t i=0;i<canframe.len;i++)
            //     std::cout<<(int)canframe.data[i]<<" ";
            std::cout<<std::endl;
        }
        // std::cout<<"\033[2J\033[H";
    }
    return 0;
}

float CharToFloat(u_int8_t arr[]){
    chartofloatconverter conv;
    for(u_int8_t i=0;i<4;i++)
        conv.chars[i]=arr[i];
    return conv.floatval;
}