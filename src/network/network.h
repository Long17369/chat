#ifndef NETWORK
#define NETWORK
#include<iostream>
#include<unistd.h>
#include<cstring>
#include <string>


#ifdef __linux__
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif


class tcpclient {
    private:
    int m_socketfd;
    string m_ip;
    unsigned short m_port;
    public:
    tcpclient();
    bool connect(const string& ip, const unsigned& port);

    bool send(const void* buffer, size_t len);

    bool recv(void* buffer, const size_t& maxlen, size_t& readlen);

    bool close();

    ~tcpclient();

};

class tcpservice {
    private:
    int m_listenfd;
    int m_clientfd;
    string m_clientip;
    unsigned short m_port;
    public:
    tcpservice();
    bool iniserver(const unsigned short& port);

    bool accept();
    const string& clientip() const;
    bool send(const void* buffer, size_t len);
    bool recv(void* buffer, const size_t& maxlen, size_t& readlen);
    bool closelisten();
    bool closeclient();
    ~tcpservice();
};
#endif
