#include <iostream>
#include <thread>
#include <vector>
#include <unistd.h>
#include <arpa/inet.h>
#include <mutex>

void handleClient(int client_fd) {
  char buffer[1024];
  ssize_t bytes_received;

  // Echo data back to the client
  while ((bytes_received = read(client_fd, buffer, sizeof(buffer))) > 0) {
    write(client_fd, buffer, bytes_received);
  }

  close(client_fd);
}

int main () {
  int server_fd;
  struct sockaddr_in server_addr; // socketaddr_in is the data type used to store address of socket
  const int port = 8080;

  // Create a socket
    // AF_INET specifies the IPv4 protocol family
    // SOCK_STREAM defines the TCP type socket
  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    std::cerr << "Failed to create socket." << std::endl;
    return 1;
  }

  // Define server address
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port); // htons convert unsigned int from machine byte order to netword byte order
  server_addr.sin_addr.s_addr = INADDR_ANY; // INADDR_ANY make socket listen to all the available IPs

  // Bind socket to the address and port
  if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    std::cerr << "Bind failed.";
    return 1;
  }

  // Listen for connections
  if (listen(server_fd, 5) < 0) {
    std::cerr << "Listen failed." << std::endl;
    return 1;
  }

  std::cout << "Server is listening on port " << port << std::endl;

  // Accept and handle clients in seperate threads
  while (true) {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);

    if (client_fd < 0) {
      std::cerr << "Failed to accept connections" << std::endl;
      continue;
    }

    std::cout << "Client connected." << std::endl;
    
    // Create a new thread to handle the client
    std::thread(handleClient, client_fd).detach();
  }

  // Close server socket
  close(server_fd);

  return 0;
}
