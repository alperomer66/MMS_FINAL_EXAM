#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <time.h>

#define NUM_THREADS 4

void *sum_digits(void *arg) {
    uint64_t num = *((uint64_t *)arg);
    uint64_t sum = 0;

    while (num > 0) {
        sum += num % 10;
        num /= 10;
    }

    pthread_exit((void *)(uintptr_t)sum);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s filename N\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    uint64_t N = atoll(argv[2]);

    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("File open error");
        return 1;
    }

    srand((unsigned int)time(NULL));

    pthread_t threads[NUM_THREADS];
    uint64_t nums[N];
    void *thread_results[NUM_THREADS];

    for (uint64_t i = 0; i < N; i++) {
        uint64_t num = ((uint64_t)rand() << 32) | rand();
        nums[i] = num;
        fwrite(&num, sizeof(uint64_t), 1, file);
    }

    fclose(file);

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, sum_digits, &nums[i]);
    }

    uint64_t total_sum = 0;

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], &thread_results[i]);
        total_sum += (uint64_t)(uintptr_t)thread_results[i];
    }

    printf("Total sum of digits: %lu\n", total_sum);

    return 0;
}

