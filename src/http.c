#include "http.h"

#include "util.h"

#include "socks.h"

int http_get(char *host, char *url, int (*callback)(char *data, int size))
{
    int request_size = sizeof(host) + sizeof(url) + 50;

    char request[request_size];
    memset(request, 0 , request_size);

    sprintf(request, "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", url, host);

    printf("request:\n----------\n%s\n----------\n", request);

    int sock = tor_connect(host, "80");

    if(sock <= 0)
    {
        printf("error: %s\n", socks_get_error(sock));
        return 0;
    }

    send_all(sock, request, request_size);

    int recv_bytes = recv_to(sock, callback);

    return recv_bytes;
}
