// util.h
//
// Common socket utilities.

#ifndef SOCKET_UTIL_H
#define SOCKET_UTIL_H

#define SERVER_PORT    "8080"
#define SERVER_ADDR    "127.0.0.1"
#define SERVER_BACKLOG 5

/**
 * @brief Create a client socket.
 * @param address The string representation of the server address
 * @param port The string representation of the server port
 * @return Connected client socket on success, -1 on failure
 */
int client_socket(const char* address, const char* port);

/**
 * @brief Create a server socket.
 * @param port The string representation of the local server port
 * @return Bound server socket on success, -1 on failure
 */
int server_socket(const char* port);

#endif // SOCKET_UTIL_H