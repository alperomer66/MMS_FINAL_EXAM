#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

int digitSum(uint64_t num) {
    int sum = 0;
    while (num > 0) {
        sum += num % 10;
        num /= 10;
    }
    return sum;
}

void *processFile(void *arg) {
    const char *filename = (const char *)arg;

    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror(filename);
        pthread_exit(NULL);
    }

    uint64_t num;
    int sum = 0;

    while (fread(&num, sizeof(uint64_t), 1, file) == 1) {
        sum += digitSum(num);
    }

    printf("%s - %d\n", filename, sum);

    fclose(file);
    int *result = malloc(sizeof(int));
    *result = sum;
    pthread_exit(result);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s file1 file2 file3 ...\n", argv[0]);
        return 1;
    }

    int totalSum = 0;
    pthread_t threads[argc - 1];

    for (int i = 1; i < argc; i++) {
        const char *filename = argv[i];

        if (pthread_create(&threads[i - 1], NULL, processFile, (void *)filename) != 0) {
            perror("Thread creation error");
            return 1;
        }
    }

    for (int i = 0; i < argc - 1; i++) {
        int *childSum;
        if (pthread_join(threads[i], (void **)&childSum) != 0) {
            perror("Thread join error");
            return 1;
        }
        totalSum += *childSum;
        free(childSum);
    }

    printf("Total sum of digits - %d\n", totalSum);

    return 0;
}
