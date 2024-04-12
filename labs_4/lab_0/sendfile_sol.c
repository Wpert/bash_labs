#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
// https://man7.org/linux/man-pages/man2/sendfile.2.html
#include <sys/sendfile.h>
// https://pubs.opengroup.org/onlinepubs/7908799/xsh/sysstat.h.html
#include <sys/stat.h>
#include <errno.h>

// https://stackoverflow.com/questions/2180079/how-can-i-copy-a-file-on-unix-using-c
// is this my lab?

// https://topic.alibabacloud.com/a/font-colorredzerofont-copy-technology-in-linux-part-1_1_16_32002138.html
// maybe need to use splice() instead of sendfile()?

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

    // https://jameshfisher.com/2017/02/24/what-is-mode_t/
    if ((S_IROTH & inputs.st_mode) != 0 || (S_IRGRP & inputs.st_mode) != 0 || (S_IRUSR & inputs.st_mode) != 0) {
    // if ((S_IRGRP & inputs.st_mode) != 0) {
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
    
    if ((output = creat(destination, S_IRWXU | S_IRWXG | S_IRWXO)) == -1) {
        printf("Error creating destination file\n");
        printf("%s\n", strerror(errno));
        close(input);
        return -1;
    }

    //Here we use kernel-space copying for performance reasons
    //sendfile will work with non-socket output (i.e. regular file) on Linux 2.6.33+
    off_t bytesCopied = 0;
    int result = sendfile(output, input, &bytesCopied, inputs.st_size);
    // printf("%d\n", result);
    // printf("%s\n", strerror(errno));
    // EBADF, but why? 
    // ok boomer i'll fix it with errno checking

    // chmod https://man7.org/linux/man-pages/man2/chmod.2.html
    fchmod(output, inputs.st_mode);
    fchmod(input, inputs.st_mode);
    close(input);
    close(output);
    return result;
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