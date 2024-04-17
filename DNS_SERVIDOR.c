#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUF_SIZE 1024
#define PORT 53
#define MAX_THREADS 10

// Estructura para pasar datos al hilo
struct thread_data {
    int sockfd;
    struct sockaddr_in client_addr;
};

// Función para manejar una solicitud DNS
void *handle_dns_request(void *arg) {
    struct thread_data *data = (struct thread_data *)arg;
    char buffer[BUF_SIZE];

    // Recibir paquete DNS del cliente
    socklen_t client_addr_len = sizeof(data->client_addr);
    int bytes_received = recvfrom(data->sockfd, buffer, BUF_SIZE, 0,
                                  (struct sockaddr *)&data->client_addr, &client_addr_len);
    if (bytes_received == -1) {
        perror("recvfrom");
        close(data->sockfd);
        free(data);
        pthread_exit(NULL);
    }

    // Aquí deberías analizar la consulta DNS y generar una respuesta adecuada

    // Enviar respuesta DNS al cliente
    int bytes_sent = sendto(data->sockfd, buffer, bytes_received, 0,
                            (struct sockaddr *)&data->client_addr, client_addr_len);
    if (bytes_sent == -1) {
        perror("sendto");
        close(data->sockfd);
        free(data);
        pthread_exit(NULL);
    }

    // Cerrar el socket y liberar memoria
    close(data->sockfd);
    free(data);
    pthread_exit(NULL);
}

int main() {
    int sockfd;

    // Crear socket UDP
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Configurar dirección y puerto del servidor
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    // Vincular el socket a la dirección y puerto
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    printf("Servidor DNS iniciado. Esperando solicitudes...\n");

    // Escuchar peticiones DNS entrantes
    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);

        // Aceptar la conexión entrante
        int client_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_sockfd == -1) {
            perror("accept");
            continue;
        }

        // Crear hilo para manejar la solicitud DNS
        pthread_t thread;
        struct thread_data *data = (struct thread_data *)malloc(sizeof(struct thread_data));
        if (data == NULL) {
            perror("malloc");
            close(client_sockfd);
            continue;
        }
        data->sockfd = client_sockfd;
        memcpy(&data->client_addr, &client_addr, sizeof(client_addr));
        if (pthread_create(&thread, NULL, handle_dns_request, (void *)data) != 0) {
            perror("pthread_create");
            close(client_sockfd);
            free(data);
            continue;
        }
        pthread_detach(thread); // Liberar recursos del hilo automáticamente
    }

    // Cerrar el socket principal
    close(sockfd);

    return 0;
}
