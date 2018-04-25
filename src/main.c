#include <stdio.h>
#include <stdlib.h>

#include "http.h"
#include "util.h"

FILE *outfile = 0;
int total_bytes = 0;

int callback(char *data, int size);

int main(int argc, char **argv)
{
#define print_usage() printf("usage: program http(s)://www.website.com/resource.html outputfile.txt\n")

    if(argc != 3)
    {
        print_usage();
        return 0;
    }

    int i;
    for(i = 0;argv[1][i] != '/';i++);
    i += 2;
    int n;
    for(n = 0;argv[1][i + n] != '/';n++);
    int c;

    char host[n];
    char url[strlen(argv[1]) - n - i];

    strncpy(host, argv[1] + i, sizeof(host));
    strncpy(url, argv[1] + n + i, sizeof(url));

    printf("host: %s\n", host);
    printf("url:  %s\n", url);

    outfile = fopen(argv[2], "wb");

    if(argv[1][4] == 's')
    {
        printf("https request\n");
        https_get(host, url, &callback);
    }
    else
    {
        printf("http request\n");
        http_get(host, url, &callback);
    }

    fclose(outfile);

    printf("\n");

    return 0;
}


int callback(char *data, int size)
{
    total_bytes += size;
    printf("\r                                         ");
    fflush(stdout);
    printf("\r%i total| %i received",total_bytes, size);
    fflush(stdout);
    fwrite(data, size, 1, outfile);
    return 1;
}
