#include "http.h"

#include "util.h"

#include "socks.h"

#include "openssl/ssl.h"

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

    close(sock);

    return recv_bytes;
}

int http_get_from_file(char *f_url, int (*callback)(char *data, int size))
{
    FILE *file = fopen(f_url, "r");

    fseek(file, 0L, SEEK_END);
    int request_size = ftell(file);
    rewind(file);

    char request[request_size + 3];
    memset(request, 0, request_size + 3);

    request[request_size] = '\r';
    request[request_size+1] = '\n';

    printf("lol\n");

    fread(request, 1, request_size, file);

    fclose(file);

    char host[256];
    memset(host, 0, 256);

    char *needle = strstr(request, "Host:");
    needle += 6;

    for(int i = 0; needle[i] != 13; i++)
        host[i] = needle[i];

    printf("Host: %s\n", host);

    printf("request:\n----------\n%s\n----------\n", request);

    int sock = tor_connect(host, "80");

    if(sock <= 0)
    {
        printf("error: %s\n", socks_get_error(sock));
        return 0;
    }

    send_all(sock, request, request_size + 3);

    int recv_bytes = recv_to(sock, callback);

    close(sock);

    return recv_bytes;
}

int https_get(char *host, char *url, int (*callback)(char *data, int size))
{
    int request_size = sizeof(host) + sizeof(url) + 50;

    char request[request_size];
    memset(request, 0 , request_size);

    sprintf(request, "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", url, host);

    printf("request:\n----------\n%s\n----------\n", request);

    int sock = tor_connect(host, "443");

    if(sock <= 0)
    {
        printf("error: %s\n", socks_get_error(sock));
        return 0;
    }

    //init ssl
    SSL_load_error_strings();
    SSL_library_init();
    SSL_CTX *ctx = SSL_CTX_new(SSLv23_client_method());

    SSL *ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sock);

    int recv_bytes = 0;

    int err = SSL_connect(ssl);
    if(err != 1)
    {
        printf("error setting up ssl connection\n");
        goto close;
    }

    ssl_send_all(ssl, request, request_size);

    recv_bytes = ssl_recv_to(ssl, callback);

close:
    //close ssl
    SSL_free(ssl);
    close(sock);

    return recv_bytes;
}

int https_get_from_file(char *f_url, int (*callback)(char *data, int size))
{
    FILE *file = fopen(f_url, "r");

    fseek(file, 0L, SEEK_END);
    int request_size = ftell(file);
    rewind(file);

    char request[request_size + 3];
    memset(request, 0, request_size + 3);

    request[request_size] = '\r';
    request[request_size+1] = '\n';

    fread(request, 1, request_size, file);

    fclose(file);

    char host[256];
    memset(host, 0, 256);

    char *needle = strstr(request, "Host:");
    needle += 6;

    for(int i = 0; needle[i] != 13; i++)
        host[i] = needle[i];

    printf("Host: %s\n", host);

    printf("request:\n----------\n%s\n----------\n", request);

    int sock = tor_connect(host, "443");

    if(sock <= 0)
    {
        printf("error: %s\n", socks_get_error(sock));
        return 0;
    }

    //init ssl
    SSL_load_error_strings();
    SSL_library_init();
    SSL_CTX *ctx = SSL_CTX_new(SSLv23_client_method());

    SSL *ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sock);

    int recv_bytes = 0;

    int err = SSL_connect(ssl);
    if(err != 1)
    {
        printf("error setting up ssl connection\n");
        goto close;
    }

    ssl_send_all(ssl, request, request_size + 3);

    recv_bytes = ssl_recv_to(ssl, callback);

close:
    //close ssl
    SSL_free(ssl);
    close(sock);

    return recv_bytes;
}
