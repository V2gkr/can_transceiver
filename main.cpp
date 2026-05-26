#include <iostream>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <unistd.h>
#include <string.h>
#include <cstring>
#include "can_receiver.h"
#include <csignal>
// #define IFNAME "can0"

// struct ifreq ifr;
// struct sockaddr_can can_addr;

#define TEMP_FILTER         0x102
#define VBUS_FILTER         0x103
#define TELEMETRY_FILTER    0x101
#define ALARM_FILTER        0x104

struct can_frame canframe;
CanReceiver& can_receiver=CanReceiver::getInstance();

union AlarmConverter{
    u_int8_t chars[2];
    u_int16_t halfword;
};

union chartofloatconverter{
    u_int8_t chars[4];
    float floatval;
};

float CharToFloat(u_int8_t *arr);

void ParseCanFrame(can_frame * frame);

u_int16_t ParseAlarm(u_int8_t arr[]);

void signal_handler(int sig){
    exit(sig);
}

int main(void){
    //can_receiver=CanReceiver::getInstance();
    int nbytes;
    signal(SIGINT,signal_handler);
    signal(SIGTERM,signal_handler);
    while(1){
        
        nbytes=can_receiver.GetCanMessage(&canframe,sizeof(struct can_frame));
        if(nbytes){
            nbytes=0;
            std::cout<<"\033[H\033[2J";
            ParseCanFrame(&canframe);
        }
    }
    return 0;
}

float CharToFloat(u_int8_t arr[]){
    chartofloatconverter conv;
    std::memcpy(conv.chars, arr, 4);
    return conv.floatval;
}

u_int16_t ParseAlarm(u_int8_t arr[]){
    AlarmConverter conv;
    std::memcpy(conv.chars, arr, 2);
    return conv.halfword;
}

void ParseCanFrame(can_frame * frame){
    switch(frame->can_id){
        case TELEMETRY_FILTER:{
        float speed=CharToFloat(frame->data+4);
        float current=CharToFloat(frame->data);
        std::cout<<"\nspeed: "<<speed<<"\n";
        std::cout<<"\ncurrent: "<<current<<std::endl;
        }
        break;
        case VBUS_FILTER:{
        float vbus=CharToFloat(frame->data);
        std::cout<<"\nvbus: "<<vbus<<std::endl;
        }
        break;
        case TEMP_FILTER:{
        float temp=CharToFloat(frame->data);
        std::cout<<"\nTemperature: "<<temp<<std::endl;
        }
        break;
        case ALARM_FILTER:{
        u_int16_t esc_alarm=ParseAlarm(frame->data);
        u_int16_t Motor_alarm=ParseAlarm(frame->data+2);
        }
        break;
        default:
        std::cout<<"\nno suitable filter"<<std::endl;
        break;
    }
}