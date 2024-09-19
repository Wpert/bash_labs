#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

// i dont want to rewrite it

char* getBackupName(char* filename) {
    char* backupname = strcpy(malloc(strlen(filename) + 5), filename);
    backupname = strcat(backupname, ".bak");
    return backupname;
}

int fcopy(char* source, char* destination) {
    char* filename = source;
    FILE* input = fopen(filename, "r"); 
    if (input == NULL) {
        printf("Cannot open file %s \n", filename);
        return -1;
    }

    char* backupname = getBackupName(filename);
    FILE* output = fopen(backupname, "w"); 
    if (output == NULL) {
        fclose(input); 
        printf("Cannot open file %s \n", backupname); 
        return -1;
    } 

    char c = fgetc(input); 
    while (c != EOF) {
        fputc(c, output);
        c = fgetc(input);
    }

    free(backupname);
    fclose(input); 
    fclose(output);
}

int main(int argc, char *argv[argc]) {
    for (int i = 1; i < argc; i++) {
        char* filename = argv[i];
        char* backupname = getBackupName(filename);

        if ( fcopy(filename, backupname) == -1 ) {
            printf("Error copying file\n");
            return 1;
        }

        free(backupname);
    }
    return 0; 
}