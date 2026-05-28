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

class CanTransceiver{
    // socket to connect to can interface
    int socket_fd;
    //struct of ifreq to bind socket
    struct ifreq ifr;
    //can socket struct
    struct sockaddr_can can_addr;
    //ctor to establish connection
    CanTransceiver(std::string_view ifname);
    //deletion of copy contructor
    CanTransceiver(const CanTransceiver&)=delete;
    //deletion of assignment operator
    CanTransceiver operator=(const CanTransceiver&)=delete;
    public:
    
    //static method to get instance of CanReceiver as singleton
    static CanTransceiver& getInstance(std::string_view ifname=IFNAME);
    [[nodiscard]] int GetCanMessage(can_frame * frame);
    int SendCanMessage(can_frame * frame);
    ~CanTransceiver();
};