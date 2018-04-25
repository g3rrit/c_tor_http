#ifndef HTTP_H
#define HTTP_H

int http_get(char *host, char *url, int (*callback)(char *data, int size));

int https_get(char *host, char *url, int (*callback)(char *data, int size));

#endif
