//This file is created using chatGPT
#include "MT25080_Part_A_Common.h"
#include <linux/errqueue.h>
#include <sys/poll.h>

#ifndef SO_ZEROCOPY
#define SO_ZEROCOPY 60
#endif
#ifndef MSG_ZEROCOPY
#define MSG_ZEROCOPY 0x4000000
#endif

// Function to read the error queue (acknowledge zero-copy completions)
void read_zerocopy_completions(int fd) {
  char control[100];
  struct msghdr msg = {0};
  // struct cmsghdr *cmsg; // Unused in this simple drain
  // struct sock_extended_err *serr; // Unused
  // struct iovec iov = {0}; // Unused

  msg.msg_control = control;
  msg.msg_controllen = sizeof(control);

  // Non-blocking read of error queue
  if (recvmsg(fd, &msg, MSG_ERRQUEUE | MSG_DONTWAIT) < 0) {
    if (errno != EAGAIN)
      perror("recvmsg ERRQUEUE");
    return;
  }

  // Process completions if rigorous tracking is needed, but we just drain here
  // to avoid limits
}

// Thread function
void *handle_client(void *arg) {
  int client_fd = *(int *)arg;
  free(arg);

  // Enable Zero Copy
  int one = 1;
  if (setsockopt(client_fd, SOL_SOCKET, SO_ZEROCOPY, &one, sizeof(one)) < 0) {
    perror("setsockopt SO_ZEROCOPY");
    // Fallback or exit? If kernel doesn't support, we can't do Part A3
    // correctly. We will continue but warn.
  }

  // 1. Receive Handshake
  Handshake hs;
  if (recv(client_fd, &hs, sizeof(hs), 0) <= 0) {
    close(client_fd);
    return NULL;
  }

  printf("Client connected: Size=%zu, Duration=%ds\n", hs.message_size,
         hs.duration_seconds);

  // 2. Prepare Message
  Message msg;
  generate_message(&msg, hs.message_size);

  // 3. Prepare iovec
  struct iovec iov[NUM_FIELDS];
  for (int i = 0; i < NUM_FIELDS; i++) {
    iov[i].iov_base = msg.fields[i];
    iov[i].iov_len = msg.field_sizes[i];
  }

  struct msghdr msgh;
  memset(&msgh, 0, sizeof(msgh));
  msgh.msg_iov = iov;
  msgh.msg_iovlen = NUM_FIELDS;

  // 4. Send Loop
  time_t start_time = time(NULL);
  long long bytes_sent_total = 0;

  // We need to poll for completion to assume safe reuse (conceptual) and
  // draining queue
  while (time(NULL) - start_time < hs.duration_seconds) {
    ssize_t sent = sendmsg(client_fd, &msgh, MSG_ZEROCOPY);
    if (sent < 0) {
      if (errno == ENOBUFS) {
        // Read completions to free up space
        read_zerocopy_completions(client_fd);
        continue;
      }
      perror("sendmsg");
      break;
    }
    bytes_sent_total += sent;

    // Periodically drain completions
    if (bytes_sent_total % 10 == 0)
      read_zerocopy_completions(client_fd);
  }

  // Final drain
  read_zerocopy_completions(client_fd);

  // Cleanup
  free_message(&msg);
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

  printf("A3 Server (Zero-Copy) listening on port %d\n", port);

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
