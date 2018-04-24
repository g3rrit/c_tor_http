#ifndef SOCKS_H
#define SOCKS_H

//  used some code from 2016 cacuhuatl < cacuhuatl at autistici dot org >

//returns socket on success
//  on failure returns 0
int socks_connect(char *dest_ip, char *dest_port, char *ip, char *port, char *usrname, char *password);

//returns socket on success
//  on failure returns 0
int tor_connect(char *ip, char *port);

char *socks_get_error(int error);

#endif
