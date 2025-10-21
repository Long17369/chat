#include "./network.h"
using namespace std;
//tcp客户端
tcpclient::tcpclient():m_socketfd(-1){}
bool tcpclient::connect(const string &ip,const unsigned &port){
    if(m_socketfd!=-1) return false;
    if((m_socketfd=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP))==-1) return false;
    m_ip=ip;
    m_port=port;
    struct sockaddr_in servaddr;
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(m_port);
    struct hostent *h;
    if((h=gethostbyname(m_ip.c_str()))==nullptr){
        ::close(m_socketfd);
        m_socketfd=-1;
        return false;
    }
    memcpy(&servaddr.sin_addr,h->h_addr,h->h_length);
    if(::connect(m_socketfd,(struct sockaddr *)&servaddr,sizeof(servaddr))==-1){
        ::close(m_socketfd);
        m_socketfd=-1;
        return false;
    }
    return true;
}

bool tcpclient::send(const void *buffer,size_t len){
    if(m_socketfd==-1) return false;
    if(::send(m_socketfd,buffer,len,0)<=0) return false;
    return true;
}

bool tcpclient::recv(void *buffer,const size_t &maxlen,size_t &readlen){
    memset(buffer,0,maxlen);
    readlen=::recv(m_socketfd,buffer,maxlen,0);
    if(readlen<=0){
        return false;
    }
    return true;
}

bool tcpclient::close(){
    if(m_socketfd==-1) return false;
    ::close(m_socketfd);
    m_socketfd=-1;
    return true;
}
tcpclient::~tcpclient(){close();}
//tcp服务端
tcpservice::tcpservice():m_listenfd(-1),m_clientfd(-1){}
bool tcpservice::iniserver(const unsigned short &port){
    if(m_listenfd!=-1||m_clientfd!=-1){
        return false;
    }
    if((m_listenfd=socket(AF_INET,SOCK_STREAM,0))==-1) return false;
    m_port=port;
    struct sockaddr_in servaddr;
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(m_port);
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    if(bind(m_listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr))==-1){
        close(m_listenfd);
        m_listenfd=-1;
        return false;
    }
    if(listen(m_listenfd,5)==-1){
        close(m_listenfd);
        m_listenfd=-1;
        return false;
    }
    return true;
}
bool tcpservice::accept(){
    if (m_listenfd==-1) return false;
    struct sockaddr_in caddr;
    socklen_t addrlen=sizeof(caddr);
    m_clientfd=::accept(m_listenfd,(struct sockaddr *)&caddr,&addrlen);
    if(m_clientfd==-1){
        return false;
    }
    m_clientip=inet_ntoa(caddr.sin_addr);
    return true;
}
const string & tcpservice::clientip() const{
    return m_clientip;
}
bool tcpservice::send(const void *buffer,size_t len){
    if(m_clientfd==-1) return false;
    if((::send(m_clientfd,buffer,len,0))<=0) return false;
    return true;
}
bool tcpservice::recv(void *buffer,const size_t &maxlen,size_t &readlen){
    memset(buffer,0,maxlen);
    readlen=::recv(m_clientfd,buffer,maxlen,0);
    if(readlen<0){
       return false;
    }
    return true;
}
bool tcpservice::closelisten(){
    if(m_listenfd==-1) return false;
    ::close(m_listenfd);
    m_listenfd=-1;
    return true;
}
bool tcpservice::closeclient(){
    if(m_clientfd==-1) return false;
    ::close(m_clientfd);
    m_clientfd=-1;
    return true;
}
tcpservice::~tcpservice(){closelisten();closeclient();}
