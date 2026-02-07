//This file is created using chatGPT
#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <time.h>
#include <errno.h>

#define NUM_FIELDS 8

// Structure with 8 dynamically allocated fields
typedef struct {
    char *fields[NUM_FIELDS];
    size_t field_sizes[NUM_FIELDS];
} Message;

// Handshake structure
typedef struct {
    int num_threads;
    size_t message_size; // Total payload size per message
    int duration_seconds;
} Handshake;

// Function to generate a message structure
void generate_message(Message *msg, size_t total_size) {
    size_t part_size = total_size / NUM_FIELDS;
    for (int i = 0; i < NUM_FIELDS; i++) {
        // Adjust last part to handle rounding
        if (i == NUM_FIELDS - 1) {
            part_size = total_size - (part_size * (NUM_FIELDS - 1));
        }
        msg->field_sizes[i] = part_size;
        msg->fields[i] = (char *)malloc(part_size);
        if (!msg->fields[i]) {
            perror("malloc");
            exit(1);
        }
        memset(msg->fields[i], 'A' + i, part_size); // Fill with dummy data
    }
}

void free_message(Message *msg) {
    for (int i = 0; i < NUM_FIELDS; i++) {
        free(msg->fields[i]);
    }
}

#endif
