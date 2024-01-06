#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char buffer[32];

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';  // Remove newline character if present
        fprintf(stdout, "%s\n", buffer);
        fflush(stdout);
    }

    return 0;
}