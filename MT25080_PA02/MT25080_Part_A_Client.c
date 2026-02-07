//This file is created using chatGPT
#include "MT25080_Part_A_Common.h"

// Client Thread Args
typedef struct {
  char *server_ip;
  int port;
  int duration;
  size_t message_size;
  long long bytes_received;
  double total_latency_us;
  long long messages_received;
} ClientThreadArgs;

void *client_worker(void *arg) {
  ClientThreadArgs *args = (ClientThreadArgs *)arg;

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    perror("socket");
    return NULL;
  }

  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(args->port);
  if (inet_pton(AF_INET, args->server_ip, &serv_addr.sin_addr) <= 0) {
    perror("inet_pton");
    return NULL;
  }

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("connect");
    return NULL;
  }

  // Send Handshake
  Handshake hs;
  hs.num_threads = 1; // Per connection context
  hs.message_size = args->message_size;
  hs.duration_seconds = args->duration;

  if (send(sock, &hs, sizeof(hs), 0) < 0) {
    perror("send handshake");
    close(sock);
    return NULL;
  }

  // Receive Loop
  char *buffer = malloc(args->message_size); // Receive buffer
  // long long total = 0; // Unused

  // We want to measure "Latency per Message".
  // We expect the server to send `args->message_size` chunks.
  // However, TCP is a stream. We must define a message boundary.
  // We will attempt to recv exactly `args->message_size` in a loop to define
  // one "message".

  while (1) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    size_t received_for_msg = 0;
    while (received_for_msg < args->message_size) {
      ssize_t n = recv(sock, buffer + received_for_msg,
                       args->message_size - received_for_msg, 0);
      if (n <= 0)
        goto disconnected;
      received_for_msg += n;
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    // Message Completed
    args->messages_received++;
    args->bytes_received += received_for_msg;

    double lat_us =
        (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_nsec - start.tv_nsec) / 1e3;
    args->total_latency_us += lat_us;
  }

disconnected:
  close(sock);
  free(buffer);
  return NULL;
}

int main(int argc, char *argv[]) {
  // Usage: ./client <IP> <PORT> <THREADS> <MSG_SIZE> <DURATION>
  if (argc != 6) {
    fprintf(stderr, "Usage: %s <IP> <PORT> <THREADS> <MSG_SIZE> <DURATION>\n",
            argv[0]);
    exit(1);
  }

  char *ip = argv[1];
  int port = atoi(argv[2]);
  int threads_count = atoi(argv[3]);
  size_t msg_size = atol(argv[4]);
  int duration = atoi(argv[5]);

  pthread_t *threads = malloc(sizeof(pthread_t) * threads_count);
  ClientThreadArgs *t_args = malloc(sizeof(ClientThreadArgs) * threads_count);

  printf("Starting Client: %d threads, %zu bytes, %ds duration\n",
         threads_count, msg_size, duration);

  for (int i = 0; i < threads_count; i++) {
    t_args[i].server_ip = ip;
    t_args[i].port = port;
    t_args[i].message_size = msg_size;
    t_args[i].duration = duration;
    t_args[i].bytes_received = 0;
    t_args[i].total_latency_us = 0;
    t_args[i].messages_received = 0;
    pthread_create(&threads[i], NULL, client_worker, &t_args[i]);
  }

  long long total_bytes = 0;
  long long total_msgs = 0;
  double total_latency_sum = 0;

  for (int i = 0; i < threads_count; i++) {
    pthread_join(threads[i], NULL);
    total_bytes += t_args[i].bytes_received;
    total_msgs += t_args[i].messages_received;
    total_latency_sum += t_args[i].total_latency_us;
  }

  double throughput_bits = (double)total_bytes * 8;
  double throughput_gbps =
      throughput_bits /
      (duration * 1e9); // Approximation using requested duration
  // A better throughput calc would use actual elapsed time of the threads, but
  // duration is close enough for heavy load.

  double avg_latency = 0;
  if (total_msgs > 0)
    avg_latency = total_latency_sum / total_msgs;

  printf("Results:\n");
  printf("Throughput: %.4f Gbps\n", throughput_gbps);
  printf("Latency: %.2f us\n", avg_latency);

  // CSV format output to stderr for script capture? Or just stdout is fine.
  // Script will likely parse this.

  free(threads);
  free(t_args);
  return 0;
}
