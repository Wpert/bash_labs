#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <netinet/in.h>

// to open / write / read file
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string.h> // strcat

int main() {
        int serv_sock = socket(AF_INET, SOCK_STREAM, 0);

        struct sockaddr_in serv_addr;
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = 0;
        serv_addr.sin_port = htons(2281);
        bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
        listen(serv_sock, 10);

        printf("Server has started\n");

        struct stat st;
        stat("index.html", &st);
        int file_size = st.st_size;
        FILE* html_page = fopen("index.html", "r");
        
        perror("init");
        
        char* buffer = (char *)malloc(file_size * sizeof(char));
        fread(buffer, file_size, 1, html_page);
        fclose(html_page);

        char* resp;
        if(0 > asprintf(&resp, "HTTP/1.1 200 OK\r\nServer: vpert (v0.1)\r\nContent-Length: %d\r\nConnection: close\r\n\r\n", file_size)) return -1;
        strcat(resp, buffer);
        
        perror("str");
        int user_count = 0;

        while(1) {
                // блокируем программу до появления след соединения
                int sock = accept(serv_sock, 0, 0);
                
                if (fork() == 0) {
                        printf("Client tried to connect %d\n", user_count);
                        
                        // write(sock, resp, strlen(resp));
                        send(sock, resp, strlen(resp), 0);
                        
                        printf("Client recieved %ld bytes\n", strlen(resp));
                        perror("response");
                        // free(resp); // не надо тк работает copy on write
                        // то есть пока не буду менять строку, то указатель будет
                        // на той же области памяти, что и родительский процесс
                        // а значит free() вызовем только под конец работы нашего сервера
                        close(sock);
                        return 0;
                }
                perror("handler");
                ++user_count;
                close(sock);
        }

        free(resp);
        close(serv_sock);
        return 0;
}
