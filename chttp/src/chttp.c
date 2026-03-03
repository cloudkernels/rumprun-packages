#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 80
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    char buffer[BUFFER_SIZE];

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Bind to port
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_fd, 10) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Listening on http://localhost:%d\n", PORT);

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr*)&addr, &addr_len);
        if (client_fd < 0) {
            perror("accept");
            continue;
        }

        // Read request (discard it)
        read(client_fd, buffer, BUFFER_SIZE);

        // Send minimal 200 OK response (no body)
        const char *response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Length: 0\r\n"
            "Connection: close\r\n"
            "\r\n";

        write(client_fd, response, strlen(response));

        close(client_fd);
    }

    return 0;
}
