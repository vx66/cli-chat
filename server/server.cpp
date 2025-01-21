#include <iostream>
#include <winsock2.h> // Librería para el uso de sockets en Windows

#pragma comment(lib, "ws2_32.lib") // Enlazar la librería Winsock automáticamente

int main() {
    WSADATA wsaData; // Estructura que contiene información sobre la implementación de Windows Sockets
    SOCKET serverSocket, clientSocket; // Sockets: uno para el servidor, otro para el cliente
    SOCKADDR_IN serverAddr, clientAddr; // Estructuras para las direcciones del servidor y cliente

    // Inicializar Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }

    // Crear el socket del servidor
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Error al crear el socket\n";
        WSACleanup(); // Limpia recursos de Winsock
        return 1;
    }   

    // Configurar la dirección del servidor
    serverAddr.sin_family = AF_INET; // Usar IPv4
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Aceptar conexiones desde cualquier dirección
    serverAddr.sin_port = htons(8080); // Puerto del servidor (convertido a formato de red)

    // Asociar el socket del servidor con la dirección
    if(bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Error al asociar el socket con la dirección\n";
        closesocket(serverSocket); // Cerrar el socket del servidor
        WSACleanup(); // Limpia recursos de Winsock
        return 1;
    }

     // 5. Poner el socket en modo de escucha
    if (listen(serverSocket, 1) == SOCKET_ERROR) {
        std::cerr << "Error al escuchar conexiones\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    std::cout << "Servidor escuchando en el puerto 8080...\n";

    // 6. Aceptar una conexión entrante
    int clientAddrSize = sizeof(clientAddr);
    clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Error al aceptar conexión\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    std::cout << "Cliente conectado\n";

    // 7. Comunicación con el cliente
    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer)); // Limpiar el buffer para evitar datos residuales
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0); // Recibir datos
        if (bytesReceived <= 0) { // Si el cliente cierra la conexión
            std::cout << "Cliente desconectado\n";
            break;
        }
        std::cout << "Cliente: " << buffer << "\n";

        // Leer mensaje desde el servidor y enviarlo al cliente
        std::cout << "Tú: ";
        std::cin.getline(buffer, sizeof(buffer));
        send(clientSocket, buffer, strlen(buffer), 0);
    }

    // 8. Cerrar los sockets y limpiar Winsock
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
