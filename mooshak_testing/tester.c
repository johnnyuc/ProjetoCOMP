#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void executeProgram(const char *program, const char *inputFile, const char *outputFile) {
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        freopen(outputFile, "w", stdout); // Redirect stdout to the output file
        execl(program, program, inputFile, NULL); // Execute the program
        perror("execl");
        exit(1);
    } else {
        int status;
        waitpid(pid, &status, 0); // Wait for the child to finish
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
            fprintf(stderr, "Error: %s failed\n", program);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <folder> <program>\n", argv[0]);
        return 1;
    }

    DIR *dir;
    struct dirent *file;

    if ((dir = opendir(argv[1])) == NULL) {
        perror("opendir");
        return 1;
    }

    char outputFileName[256];

    while ((file = readdir(dir)) != NULL) {
        char *filename = file->d_name;
        size_t filename_len = strlen(filename);

        if ((filename_len >= 2 && strcmp(filename + filename_len - 2, ".c") == 0) ||
            (filename_len >= 3 && strcmp(filename + filename_len - 3, ".uc") == 0)) {
            snprintf(outputFileName, sizeof(outputFileName), "%s.res", filename);
            executeProgram(argv[2], filename, outputFileName);
        }
    }

    closedir(dir);

    return 0;
}
