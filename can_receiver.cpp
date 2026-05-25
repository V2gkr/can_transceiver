#include "can_receiver.h"
#include <cerrno>
#include <system_error>

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
    struct timeval tv;
    tv.tv_sec=1;
    tv.tv_usec=0;
    setsockopt(socket_fd,SOL_SOCKET,SO_RCVTIMEO,(const char*)&tv,sizeof(tv));
}

CanReceiver::~CanReceiver(){
    close(socket_fd);
}

int CanReceiver::GetCanMessage(struct can_frame * frame,u_int8_t size){
    int nbytes=read(socket_fd,frame,size);
    //if error maybe signalize somehow to user , maybe add timeout for blocking read?
    if(nbytes<0){
        if(errno=EAGAIN || errno==EWOULDBLOCK){
            return 0;
        }
        if(errno==EINTR){
            return 0;
        }
        throw std::system_error(errno,std::generic_category(),"error in can frame read");
    }
    return nbytes;
}
