#pragma once
#include <iostream>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <unistd.h>
#include <string.h>
#define IFNAME "can0"

class CanReceiver{
    // socket to connect to can interface
    int socket_fd;
    //struct of ifreq to bind socket
    struct ifreq ifr;
    //can socket struct
    struct sockaddr_can can_addr;
    //ctor to establish connection
    CanReceiver();
    //deletion of copy contructor
    CanReceiver(const CanReceiver&)=delete;
    //deletion of assignment operator
    CanReceiver operator=(const CanReceiver&)=delete;
    public:
    //static method to get instance of CanReceiver as singleton
    static CanReceiver& getInstance();
    int GetCanMessage(struct can_frame * frame,u_int8_t size);
    ~CanReceiver();
};