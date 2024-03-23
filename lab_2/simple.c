#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/wait.h>

// exeptions handling
#include <errno.h>
#include <string.h>

int main(int argv, char* argc[]) {
    if (argv < 3) {
        fprintf(stderr, "The argument list is too small\n");
        exit(EXIT_FAILURE);
    }

    mode_t out_mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    char* outputname = argc[1];
    int output_fd = open(outputname, O_CREAT | O_WRONLY, out_mode);

    if (output_fd < 0) {
        perror("output file");
        exit(EXIT_FAILURE);
    }

    for (int i = 2; i < argv; ++i) {
        char* const inputfile = argc[i];

        mode_t in_mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
        int input_fd = open(inputfile, O_RDWR);
        if (input_fd < 0) {
            // perror(inputfile);
            perror("input file");
            close(output_fd);
            exit(EXIT_FAILURE);
        }

        int pipe_fd[2];
        pid_t child1, child2;

        if (pipe(pipe_fd) == -1) {
            perror("pipe opening");
            close(input_fd);
            close(output_fd);
            exit(EXIT_FAILURE);
        }

        child1 = fork();
        if (child1 < 0) {
            perror("First child err");
            // do i really have to handle every error here??
            // i do want to program in C++ :)
            exit(EXIT_FAILURE);
        }
        if (child1 == 0) {
            if (close(STDOUT_FILENO) < 0) {
                perror("ch1 close stdout");
                exit(EXIT_FAILURE);
            }
            if (close(input_fd) < 0) {
                perror("ch1 close input");
                exit(EXIT_FAILURE);
            }
            if (close(output_fd) < 0) {
                perror("ch1 close output");
                exit(EXIT_FAILURE);
            }

            // pipe_fd[0] = write end of pipe
            if (dup2(pipe_fd[1], STDOUT_FILENO) < 0) {
                perror("ch1 dup2");
                exit(EXIT_FAILURE);
            }

            size_t n = strlen("/usr/bin/cat") + 1+ strlen(inputfile) + 1;
            char command[100];
            snprintf(command, n * sizeof(char), "/usr/bin/cat %s", inputfile);

            // how do i need to call it??
            // if (execl("/usr/bin/bash", "/usr/bin/bash", "-c", command, (char *) NULL) < 0) {
            //     perror("ch1 execl");
            //     exit(EXIT_FAILURE);
            // }
            if (execl("/usr/bin/cat", "cat", inputfile, (char *) NULL) < 0) {
                perror("ch1 execl");
                exit(EXIT_FAILURE);
            }
        }
        else {
            child2 = fork();

            if (child1 < 0) {
                perror("Second child");
                exit(EXIT_FAILURE);
            }

            if (child2 == 0) {
                close(STDIN_FILENO);
                close(STDOUT_FILENO);
                close(pipe_fd[1]);

                // read end of pipe
                dup2(pipe_fd[0], STDIN_FILENO);
                dup2(output_fd, STDOUT_FILENO);

                // if (execl("/usr/bin/bash", "/usr/bin/bash", "-c", "grep -E '^[0-9]+$'", (char *) NULL) < 0) {
                //     perror("ch2 execl");
                //     exit(EXIT_FAILURE);
                // }
                if (execl("/usr/bin/grep", "grep", "-E", "^[0-9]+$", (char *) NULL) < 0) {
                    perror("ch2 execl");
                    exit(EXIT_FAILURE);
                }
            }
            else {
                // perror("parent");
            }
        }
    }

    return 0;
}
