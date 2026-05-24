#include <iostream>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <unistd.h>
#include <string.h>
#define IFNAME "can0"

struct ifreq ifr;
struct sockaddr_can can_addr;
struct can_frame canframe;

int main(void){
    int sock_fd=socket(PF_CAN,SOCK_RAW,CAN_RAW);
    memcpy(ifr.ifr_ifrn.ifrn_name,IFNAME,4);
    ioctl(sock_fd,SIOCGIFINDEX,&ifr);

    can_addr.can_family=AF_CAN;
    can_addr.can_ifindex=ifr.ifr_ifru.ifru_ivalue;

    bind(sock_fd,(const sockaddr *)&can_addr,sizeof(can_addr));
    int nbytes;
    while(1){
        std::cout<<"\033[2J\033[H";
        nbytes=read(sock_fd,&canframe,sizeof(can_frame));
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