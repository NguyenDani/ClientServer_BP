#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

int main () {
  int client_fd;
  struct sockaddr_in server_addr;
  const int port = 8080;
  const char *server_ip = "127.0.0.1"; // Localhost for testing

  // Create a socket
  client_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (client_fd < 0) {
      std::cerr << "Failed to create socket." << std::endl;
      return 1;
  }

  // Define server address
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

  // Connect to the server
  if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
      std::cerr << "Failed to connect to server." << std::endl;
      return 1;
  }

  std::cout << "Connected to server." << std::endl;

  // Send a message to the server
  const char *message = "Hello, Server!";
  write(client_fd, message, std::strlen(message));

  // Read the echoed message from the server
  char buffer[1024];
  ssize_t bytes_received = read(client_fd, buffer, sizeof(buffer));
  if (bytes_received > 0) {
      std::cout << "Received from server: " << std::string(buffer, bytes_received) << std::endl;
  }

  // Close the client socket
  close(client_fd);

  return 0;
}
