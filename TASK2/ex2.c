#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/wait.h>

int digitSum(uint64_t num) {
    int sum = 0;
    while (num > 0) {
        sum += num % 10;
        num /= 10;
    }
    return sum;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s file1 file2 file3 ...\n", argv[0]);
        return 1;
    }

    int totalSum = 0;

    for (int i = 1; i < argc; i++) {
        const char *filename = argv[i];

        pid_t pid = fork();

        if (pid == 0) {  // dete
            FILE *file = fopen(filename, "rb");
            if (!file) {
                perror(filename);
                return 1;
            }

            uint64_t num;
            int sum = 0;

            while (fread(&num, sizeof(uint64_t), 1, file) == 1) {
                sum += digitSum(num);
            }

            printf("%s - %d\n", filename, sum);

            fclose(file);
            return sum; // suma ot dete
        } else if (pid < 0) {
            perror("Fork error");
            return 1;
        }
    }

    for (int i = 1; i < argc; i++) {
        int childStatus;
        wait(&childStatus);
        totalSum += WEXITSTATUS(childStatus);
    }

    printf("Total sum of digits - %d\n", totalSum);

    return 0;
}
