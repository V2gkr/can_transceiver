#include <iostream>
#include <cstring>
#include "can_transceiver.h"
#include <csignal>
#include <thread>
// #define IFNAME "can0"

// struct ifreq ifr;
// struct sockaddr_can can_addr;

#define TEMP_FILTER         0x102
#define VBUS_FILTER         0x103
#define TELEMETRY_FILTER    0x101
#define ALARM_FILTER        0x104

#define ESC_ONOFF_FILTER    0x201
#define ESC_SET_SPEED       0x202

#define IFNAME "can0"

CanTransceiver& can_trx=CanTransceiver::getInstance(IFNAME);

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

void ReceiveThread(){
    struct can_frame canframe_rx;
    while(1){
        if(int nbytes=can_trx.GetCanMessage(&canframe_rx)){
            nbytes=0;
            std::cout<<"\033[H\033[2J";
            ParseCanFrame(&canframe_rx);
        }
    }
}

void TransmitThread(){
    struct can_frame canframe_tx;
    canframe_tx.can_dlc=1;
    canframe_tx.can_id=ESC_ONOFF_FILTER;
    canframe_tx.data[0]=0x01;
    can_trx.SendCanMessage(&canframe_tx);
    while(1){
        //writes should be coming outside , or some time-controlled transmission
    }
}

int main(void){
    
    pthread_t rx_tid,tx_tid;
    
    signal(SIGINT,signal_handler);
    signal(SIGTERM,signal_handler);
    std::thread rx_thread(ReceiveThread);
    std::thread tx_thread(TransmitThread);

    rx_thread.join();
    tx_thread.join();

    return EXIT_SUCCESS;
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