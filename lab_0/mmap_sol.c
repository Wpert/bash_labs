#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>

char* getBackupName(char* filename) {
    char* backupname = strcpy(malloc(strlen(filename) + 5), filename);
    backupname = strcat(backupname, ".bak");
    return backupname;
}

int OSCopyFile(const char* source, const char* destination) {
    int input, output;
    // i want to know file permissions
    struct stat inputs = {0};
    if (stat(source, &inputs) != 0) {
        printf("Error opening mode of source file\n");
        printf("%s\n", strerror(errno));
        return -1;
    }

    if ((S_IROTH & inputs.st_mode) == 0 || (S_IRGRP & inputs.st_mode) == 0 || (S_IRUSR & inputs.st_mode) == 0) {
        if ( chmod(source, inputs.st_mode | S_IROTH | S_IRUSR | S_IRGRP) != 0) {
            printf("Error changing permissions on source file\n");
            printf("%s\n", strerror(errno));
            return -1;
        }
        printf("Source file has changes permissions\n");
    }

    input = open(source, O_RDONLY);
    if (input < 0) {
        printf("Error opening source file\n");
        printf("%s\n", strerror(errno));
        return -1;
    }

    void *mem = mmap(NULL, inputs.st_size, PROT_READ, MAP_SHARED, input, 0);
    if(mem == MAP_FAILED) {
        printf("Error mapping source file\n");
        printf("%s\n", strerror(errno));
        close(input);
        return -1;
    }

    if ((output = creat(destination, S_IRWXU | S_IRWXG | S_IRWXO)) == -1) {
        printf("Error creating destination file\n");
        printf("%s\n", strerror(errno));
        close(input);
        return -1;
    }

    ssize_t nwritten = write(output, mem, inputs.st_size);
    if(nwritten < inputs.st_size) {
        printf("Error writing to destination file\n");
        printf("%s\n", strerror(errno));
        close(input);
        close(output);
    }

    fchmod(output, inputs.st_mode);
    fchmod(input, inputs.st_mode);
    close(input);
    close(output);
    return 0;
}

int main(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        char* filename = argv[i];
        char* backupname = getBackupName(filename);

        if ( OSCopyFile(filename, backupname) == -1 ) {
            free(backupname);
            return 1;
        }
        // do i reallly need this?
        free(backupname);
    }
    return 0;
}