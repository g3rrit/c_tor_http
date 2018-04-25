#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "openssl/ssl.h"

int recv_all(int sock, char *data, int size);

int recv_to(int sock, int (*callback)(char *data, int size));

int send_all(int sock, char *data, int size);

//ssl
int ssl_send_all(SSL *ssl, char *data, int size);

int ssl_recv_to(SSL *ssl, int(*callback)(char *data, int size));

#endif
