# Implementación de un Servicio Ligero de Resolución de Nombres DNS

## Introducción

En este proyecto se desarrolló un servicio de resolución de nombres DNS, compuesto por un cliente y un servidor, implementados en Python y C respectivamente. El cliente realiza consultas DNS al servidor y registra las transacciones en un archivo de registro. El servidor recibe las consultas, genera respuestas de ejemplo y registra las transacciones en un archivo de registro.

## Desarrollo

### Requisitos del Proyecto

El proyecto se desarrolló cumpliendo con los siguientes requisitos:

- Implementación de un cliente DNS en Python.
- Implementación de un servidor DNS en C.
- Registro de transacciones en archivos de log tanto para el cliente como para el servidor.
- Comunicación entre el cliente y el servidor a través del protocolo DNS sobre UDP.
- Utilización de sockets para la comunicación entre cliente y servidor.

### Estructura del Proyecto

El proyecto se estructura en dos partes principales:

- Cliente DNS (DNSCLIENTE.py): Escrito en Python, este script permite al usuario realizar consultas DNS al servidor y registra las transacciones en un archivo de log.
- Servidor DNS (dns_servidor.c): Escrito en C, este programa implementa un servidor DNS ligero que recibe consultas de clientes, genera respuestas de ejemplo y registra las transacciones en un archivo de log.

### Ejecución del Proyecto

Para ejecutar el cliente, se debe ejecutar el siguiente comando:

python3 DNSCLIENTE.py logdns.txt


Para ejecutar el servidor, se debe compilar el archivo `dns_servidor.c` y luego ejecutarlo con los parámetros adecuados:

gcc -o dns_servidor dns_servidor.c
./dns_servidor <ip> <port> </path/zones/midominio.com> </path/log.log>


## Aspectos Logrados y No Logrados

### Logrados

- Implementación funcional de un cliente y un servidor DNS.
- Registro de transacciones en archivos de log.
- Comunicación exitosa entre el cliente y el servidor a través del protocolo DNS sobre UDP.

### No Logrados

- Implementación de la resolución de nombres DNS de manera completa y precisa. En este proyecto, el servidor devuelve algunos parametros de ejemplo para asegurar la funcionalidad y la conexion del cliente con el servidor 

## Conclusiones

El desarrollo de este proyecto permitió comprender los fundamentos del protocolo DNS y mejorar las habilidades en el manejo de sockets en Python y C. Aunque el proyecto satisface los requisitos básicos, hay margen para mejoras en la precisión y funcionalidades del servidor DNS.

## Referencias

[1] Documentación oficial de Python: https://docs.python.org/3/
[2] Manuales de Linux: https://man7.org/linux/man-pages/
[3] RFC 1035: https://tools.ietf.org/html/rfc1035
[4] https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
