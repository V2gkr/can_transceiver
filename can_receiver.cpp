#include "can_receiver.h"

CanReceiver& CanReceiver::getInstance(){
    static CanReceiver instance;
    return instance;
}

CanReceiver::CanReceiver(){
    socket_fd=socket(PF_CAN,SOCK_RAW,CAN_RAW);
    memcpy(ifr.ifr_ifrn.ifrn_name,IFNAME,4);
    //strncpy(ifr.ifr_ifrn.ifrn_name,IFNAME,4);
    ioctl(socket_fd,SIOCGIFINDEX,&ifr);
    can_addr.can_family=AF_CAN;
    can_addr.can_ifindex=ifr.ifr_ifru.ifru_ivalue;
    bind(socket_fd,(const sockaddr *)&can_addr,sizeof(can_addr));
    std::cout<<"wazzup ctor"<<std::endl;
}

CanReceiver::~CanReceiver(){
    close(socket_fd);
}

int CanReceiver::GetCanMessage(struct can_frame * frame,u_int8_t size){
    std::cout<<"wazzup";
    nbytes=read(socket_fd,&frame,size);
    //if error maybe signalize somehow to user , maybe add timeout for blocking read?
    return nbytes;
}
