#include "can_transceiver.h"
#include <cerrno>
#include <system_error>

CanTransceiver& CanTransceiver::getInstance(std::string_view ifname){
    static CanTransceiver instance(ifname);
    return instance;
}

CanTransceiver::CanTransceiver(std::string_view ifname){
    socket_fd=socket(PF_CAN,SOCK_RAW,CAN_RAW);
    strncpy(ifr.ifr_ifrn.ifrn_name,ifname.data(),ifname.size());
    ioctl(socket_fd,SIOCGIFINDEX,&ifr);
    can_addr.can_family=AF_CAN;
    can_addr.can_ifindex=ifr.ifr_ifru.ifru_ivalue;
    bind(socket_fd,(const sockaddr *)&can_addr,sizeof(can_addr));
    struct timeval tv;
    tv.tv_sec=1;
    tv.tv_usec=0;
    setsockopt(socket_fd,SOL_SOCKET,SO_RCVTIMEO,(const char*)&tv,sizeof(tv));
}

CanTransceiver::~CanTransceiver(){
    close(socket_fd);
}

int CanTransceiver::GetCanMessage(struct can_frame * frame){
    int nbytes=read(socket_fd,frame,sizeof(can_frame));
    //if error maybe signalize somehow to user , maybe add timeout for blocking read?
    if(nbytes<0){
        if(errno==EAGAIN || errno==EWOULDBLOCK){
            return 0;
        }
        if(errno==EINTR){
            return 0;
        }
        throw std::system_error(errno,std::generic_category(),"error in can frame read");
    }
    return nbytes;
}

int CanTransceiver::SendCanMessage(can_frame * frame){
    int nbytes=write(socket_fd,frame,sizeof(can_frame));
    if(nbytes==-1){
        throw std::system_error(errno,std::generic_category(),"error in can frame write");
    }
    return nbytes;
}
