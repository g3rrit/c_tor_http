#include <stdio.h>
#include <stdlib.h>

#include "http.h"
#include "util.h"

int callback(char *data, int size);

int main()
{
    http_get("reddit.com", "/", &callback);

    return 0;
}


int callback(char *data, int size)
{
    char buffer[size];
    memcpy(buffer, data, size);
    buffer[size] = 0;
    printf("received %i bytes:\n----------\n%s\n----------\n",size, buffer);
    return 1;
}
