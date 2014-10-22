#ifndef IRC_SOCK_H
#define IRC_SOCK_H

int get_socket(const char* host, const char* port);
int sck_send(int socket, const char* data, size_t size);
int sck_sendf(int socket, const char* fmt, ...);
int sck_recv(int socket, char* buffer, size_t size);

#endif /* IRC_SOCK_H */
