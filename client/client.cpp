#include <iostream>
#include <winsock2.h>  // Librería para manejar sockets en Windows

#pragma comment(lib, "ws2_32.lib")  // Enlazar la librería Winsock automáticamente

int main() {
    WSADATA wsaData;                // Estructura para inicializar Winsock
    SOCKET clientSocket;            // Socket del cliente
    sockaddr_in serverAddr;         // Estructura para la dirección del servidor

    // 1. Inicializar Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Error al inicializar Winsock\n";
        return 1;
    }

    // 2. Crear un socket para el cliente
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Error al crear el socket\n";
        WSACleanup(); // Limpia recursos de Winsock
        return 1;
    }

    // 3. Configurar la dirección del servidor
    serverAddr.sin_family = AF_INET;              // Usar IPv4
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Dirección IP del servidor
    serverAddr.sin_port = htons(8080);            // Puerto del servidor (convertido a formato de red)

    // 4. Conectarse al servidor
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Error al conectar con el servidor\n";
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    std::cout << "Conectado al servidor\n";

    // 5. Comunicación con el servidor
    char buffer[1024];
    while (true) {
        // Leer mensaje desde el cliente y enviarlo al servidor
        std::cout << "Tú: ";
        std::cin.getline(buffer, sizeof(buffer));
        send(clientSocket, buffer, strlen(buffer), 0);

        // Recibir mensaje del servidor
        memset(buffer, 0, sizeof(buffer)); // Limpiar el buffer
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) { // Si el servidor cierra la conexión
            std::cout << "Servidor desconectado\n";
            break;
        }
        std::cout << "Servidor: " << buffer << "\n";
    }

    // 6. Cerrar el socket y limpiar Winsock
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}