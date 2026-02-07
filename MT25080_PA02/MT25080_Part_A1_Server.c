//This file is created using chatGPT
#include "MT25080_Part_A_Common.h"

// Thread function
void *handle_client(void *arg) {
  int client_fd = *(int *)arg;
  free(arg);

  // 1. Receive Handshake
  Handshake hs;
  if (recv(client_fd, &hs, sizeof(hs), 0) <= 0) {
    perror("recv handshake");
    close(client_fd);
    return NULL;
  }

  printf("Client connected: Size=%zu, Duration=%ds\n", hs.message_size,
         hs.duration_seconds);

  // 2. Prepare Message (Structure with 8 strings)
  Message msg;
  generate_message(&msg, hs.message_size);

  // 3. Prepare Serialization Buffer (Destination for Copy 1)
  char *send_buffer = (char *)malloc(hs.message_size);
  if (!send_buffer) {
    perror("malloc send_buffer");
    return NULL; // Leak msg for now
  }

  // 4. Send Loop
  time_t start_time = time(NULL);
  long long bytes_sent_total = 0;

  while (time(NULL) - start_time < hs.duration_seconds) {
    // COPY 1: User-to-User (Linearize 8 fields into 1 buffer)
    size_t offset = 0;
    for (int i = 0; i < NUM_FIELDS; i++) {
      memcpy(send_buffer + offset, msg.fields[i], msg.field_sizes[i]);
      offset += msg.field_sizes[i];
    }

    // COPY 2: User-to-Kernel (send syscall)
    // Note: send() copies data from user space to kernel socket buffer
    ssize_t sent = send(client_fd, send_buffer, hs.message_size, 0);
    if (sent <= 0)
      break;
    bytes_sent_total += sent;
  }

  // Cleanup
  free_message(&msg);
  free(send_buffer);
  close(client_fd);
  printf("Finished client: %lld bytes sent\n", bytes_sent_total);
  return NULL;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <port>\n", argv[0]);
    exit(1);
  }
  int port = atoi(argv[1]);

  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    perror("socket");
    exit(1);
  }

  int opt = 1;
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(port);

  if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("bind");
    exit(1);
  }
  if (listen(server_fd, 100) < 0) {
    perror("listen");
    exit(1);
  }

  printf("A1 Server (Two-Copy) listening on port %d\n", port);

  while (1) {
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);
    int *client_fd = malloc(sizeof(int));
    *client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &len);
    if (*client_fd < 0) {
      free(client_fd);
      continue;
    }
    pthread_t tid;
    pthread_create(&tid, NULL, handle_client, client_fd);
    pthread_detach(tid);
  }
  return 0;
}
