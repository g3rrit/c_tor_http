#include <stdio.h>
#include <stdlib.h>

#include "tcp.h"

#include "socks.h"

#include "util.h"

int main()
{
    int error = tor_connect("www.google.de", "80");
    if(error <= 0)
    {
        printf("error connecting to google\n");
        printf("error: %s\n", socks_get_error(error));
    }

    char request[] = "GET / HTTP/1.1\r\nHost: www.google.de\r\n\r\n";



    return 0;
}
