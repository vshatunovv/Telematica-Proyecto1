import socket
import sys
import datetime

def registrar_log(log_file, query, response_ip):
    with open(log_file, "a") as log:
        timestamp = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        log.write(f"{timestamp} {query} {response_ip}\n")

def parse_ip_address(response_bytes):
    # La respuesta_bytes puede contener múltiples direcciones IP
    # Pero para este caso, supondremos que solo hay una dirección IP
    ip_parts = [str(byte) for byte in response_bytes]
    return '.'.join(ip_parts)

def main():
    if len(sys.argv) != 2:
        print("Uso: python3 DNSCLIENTE.py </path/log.log>")
        sys.exit(1)

    log_file = sys.argv[1]

    # Crear un socket UDP
    sockfd = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    # Configurar la dirección del servidor DNS
    server_address = ('127.0.0.1', 53)  # Cambia la dirección IP según corresponda

    query = input("Ingrese el nombre de dominio a consultar: ")

    try:
        # Enviar la consulta DNS al servidor
        sockfd.sendto(query.encode(), server_address)

        # Recibir la respuesta del servidor
        response, server = sockfd.recvfrom(1024)

        print("Respuesta del servidor DNS:", response)
        
        # Convertir los bytes de la respuesta a una dirección IP legible
        response_ip = parse_ip_address(response)
        
        # Registrar la transacción en el archivo de log del cliente
        registrar_log(log_file, query, response_ip)
    except Exception as e:
        print("Error:", e)
    finally:
        # Cerrar el socket
        sockfd.close()

if __name__ == "__main__":
    main()

