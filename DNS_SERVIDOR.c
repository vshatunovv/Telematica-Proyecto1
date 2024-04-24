#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h> // Para getaddrinfo
#include <sys/types.h>
#include <sys/socket.h>

#define MAXLINE 1024

void manejar_peticion(char *query, char *client_ip, char *log_file) {
    struct addrinfo hints, *res, *p;
    int status;
    char ipstr[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // AF_INET para IPv4 o AF_INET6 para IPv6
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(query, NULL, &hints, &res)) != 0) {
        fprintf(stderr, "Error al resolver el nombre de dominio: %s\n", gai_strerror(status));
        return;
    }

    for(p = res; p != NULL; p = p->ai_next) {
        void *addr;
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
        addr = &(ipv4->sin_addr);

        // Convertir la dirección IP a una cadena y guardarla en ipstr
        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
    }

    freeaddrinfo(res); // liberar la lista enlazada

    // Obtener la fecha y hora actual
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);
    
    // Registrar la transacción en el archivo de log
    FILE *log = fopen(log_file, "a");
    if (log != NULL) {
        fprintf(log, "%s %s %s %s\n", timestamp, client_ip, query, ipstr);
        fclose(log);
    } else {
        printf("Error al abrir el archivo de log\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Uso: %s <ip> <port> </path/zones/midominio.com> </path/log.log>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *ip = argv[1];
    int port = atoi(argv[2]);
    char *zone_file = argv[3];
    char *log_file = argv[4];

    // Configurar la dirección y el puerto del servidor DNS
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(ip);
    servaddr.sin_port = htons(port);

    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }

    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Error al enlazar el socket");
        exit(EXIT_FAILURE);
    }

    printf("Iniciando servidor DNS en %s:%d\n", ip, port);

    while (1) {
        printf("Esperando mensajes de los clientes...\n");

        char buffer[MAXLINE];
        struct sockaddr_in cliaddr;
        socklen_t len = sizeof(cliaddr);
        int n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
        buffer[n] = '\0';

        printf("Mensaje recibido del cliente: %s\n", buffer);

        // Procesar la petición del cliente
        manejar_peticion(buffer, inet_ntoa(cliaddr.sin_addr), log_file);
        
        // Enviar la respuesta al cliente
        // Aquí necesitas determinar qué enviar como respuesta. ¿Quizás un mensaje de confirmación?
        //sendto(sockfd, (const char *)response_ip, strlen(response_ip), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
    }

    close(sockfd);

    return 0;
}
