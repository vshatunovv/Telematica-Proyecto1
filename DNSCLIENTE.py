import socket
import sys
import datetime

# Función para construir la consulta DNS
def construir_consulta(dominio, tipo_registro):
    # ID de la consulta
    identificador = 1234

    # Campos de la cabecera de la consulta DNS
    cabecera = bytearray(12)
    cabecera[0] = (identificador >> 8) & 0xFF  # ID - byte alto
    cabecera[1] = identificador & 0xFF  # ID - byte bajo
    cabecera[2] = 1 << 0  # QR (1 bit): 0 para consulta, 1 para respuesta
    cabecera[2] |= 0 << 1  # OPCODE (4 bits): 0 para consulta estándar
    cabecera[2] |= 0 << 4  # RD (1 bit): 0 para consulta recursiva no deseada
    cabecera[4] = 1  # QDCOUNT: número de consultas en la sección de pregunta
    cabecera[5] = 0  # ANCOUNT: número de registros en la sección de respuesta
    cabecera[6] = 0  # NSCOUNT: número de registros en la sección de autoridad
    cabecera[7] = 0  # ARCOUNT: número de registros en la sección adicional

    # Campos de la sección de pregunta
    consulta = bytearray()
    partes_dominio = dominio.split('.')
    for parte in partes_dominio:
        longitud = len(parte)
        consulta.append(longitud)
        for caracter in parte:
            consulta.append(ord(caracter))
    consulta.append(0)  # Terminador de cadena
    consulta += bytearray([0, 0, 1, 0, 1])  # QTYPE (2 bytes): Tipo de registro (A)
    consulta += bytearray([0, 1])  # QCLASS (2 bytes): Clase de consulta (IN)

    # Combinar la cabecera y la consulta para formar la consulta DNS completa
    consulta_dns = cabecera + consulta

    return consulta_dns

# Función para enviar la consulta DNS y recibir la respuesta
def enviar_consulta(direccion_ip_servidor, puerto, consulta, tipo_registro, dominio, log_file):
    try:
        # Crear un socket UDP
        cliente_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        
        # Enviar la consulta al servidor DNS
        cliente_socket.sendto(consulta, (direccion_ip_servidor, puerto))

        # Escribir en el archivo de log
        escribir_log(log_file, f"Consulta enviada a {direccion_ip_servidor}:{puerto} - Tipo: {tipo_registro} - Dominio: {dominio}", tipo_registro, dominio)
        
        # Recibir la respuesta del servidor
        respuesta, _ = cliente_socket.recvfrom(1024)

        # Obtener la dirección IP de la respuesta
        direccion_ip_respuesta = ".".join(map(str, respuesta[-4:]))

        # Escribir en el archivo de log
        escribir_log(log_file, f"Respuesta recibida: {direccion_ip_respuesta}", tipo_registro, direccion_ip_respuesta)
        
        return respuesta
    except Exception as e:
        # Manejar errores y escribir en el archivo de log
        escribir_log(log_file, f"Error al enviar consulta DNS: {e}", tipo_registro, "")
        return None
    finally:
        # Cerrar el socket
        cliente_socket.close()




# Función para escribir en el archivo de log
# Función para escribir en el archivo de log
# Función para escribir en el archivo de log
def escribir_log(log_file, mensaje, query="", response_ip=""):
    with open(log_file, 'a') as archivo:
        # Obtener la fecha y hora actual
        now = datetime.datetime.now()
        fecha_hora_actual = now.strftime("%Y-%m-%d %H:%M:%S")
        
        # Escribir el mensaje en el archivo de log
        archivo.write(f"{fecha_hora_actual} {mensaje} {query} {response_ip}\n")


if __name__ == "__main__":
    # Verificar los argumentos de línea de comandos
    if len(sys.argv) != 5:
        print("Uso: python3 dnscliente.py <ip_servidor> <tipo_registro> <nombre_dominio> logdns.txt")
        sys.exit(1)

    # Obtener los argumentos de línea de comandos
    direccion_ip_servidor = sys.argv[1]
    tipo_registro = sys.argv[2]
    dominio = sys.argv[3]
    log_file = sys.argv[4]

    print("Ejecutando consulta DNS...")
    # Construir la consulta DNS
    consulta = construir_consulta(dominio, tipo_registro)

    # Enviar la consulta al servidor DNS y recibir la respuesta
    # Enviar la consulta al servidor DNS y recibir la respuesta
    respuesta = enviar_consulta(direccion_ip_servidor, 53, consulta, tipo_registro, dominio, log_file)

    print("Consulta DNS completada.")


    if respuesta:
        print("Respuesta del servidor DNS:", respuesta)
    else:
        print("No se recibió ninguna respuesta del servidor DNS.")
