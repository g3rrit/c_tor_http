#include "util.h"

int recv_all(int sock, char *data, int size)
{
    if(sock < 0 || !data)
        return 0;

    int bytes = 0;
    int total = 0;
    while(total < size)
    {
        bytes = recv(sock, data + total, size - total, 0);
        if(bytes < 0)
            break;
        total += bytes;
    }
    return total;
}

int recv_to(int sock, int (*callback)(char *data, int size))
{
    if(sock < 0)
        return 0;

#define MAX_RECV 1024

    char buffer[MAX_RECV];
    int bytes = 0;
    int total = 0;
    while(1)
    {
        bytes = recv(sock, buffer, MAX_RECV, 0); 
        if(bytes <= 0)
            break;
        total += bytes;
        if(!callback(buffer, bytes))
            break;
    }
    return total;
}

int send_all(int sock, char *data, int size)
{
    if(sock < 0 || !data)
        return 0;

    int bytes = 0;
    int total = 0;
    while(total < size)
    {
        bytes = send(sock, data + total, size - total, 0); 
        if(bytes < 0)
            break;
        total += bytes;
    }
    return total;
}

int ssl_send_all(SSL *ssl, char *data, int size)
{
    int bytes = 0;
    int total = 0;
    while(total < size)
    {
        bytes = SSL_write(ssl, data + total, size - total); 
        if(bytes < 0)
            break;
        total += bytes;
    }
    return total;
}

int ssl_recv_to(SSL *ssl, int(*callback)(char *data, int size))
{
#define MAX_RECV 1024

    char buffer[MAX_RECV];
    int bytes = 0;
    int total = 0;
    while(1)
    {
        bytes = SSL_read(ssl, buffer, MAX_RECV); 
        if(bytes <= 0)
            break;
        total += bytes;
        if(!callback(buffer, bytes))
            break;
    }
    return total;

}


