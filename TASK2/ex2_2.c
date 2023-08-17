#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

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

    for (uint64_t i = 0; i < N; i++) {
        uint64_t num = ((uint64_t)rand() << 32) | rand();  
        fwrite(&num, sizeof(uint64_t), 1, file);
    }

    fclose(file);

    return 0;
}
