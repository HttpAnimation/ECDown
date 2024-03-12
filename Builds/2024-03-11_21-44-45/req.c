#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PACKAGES 10
#define MAX_LENGTH 100

void installPackage(const char *package) {
    char command[MAX_LENGTH];
    snprintf(command, sizeof(command), "sudo apt-get install -y %s", package);
    system(command);
}

int main() {
    FILE *file;
    char line[MAX_LENGTH];
    char distro[MAX_LENGTH];
    char packages[MAX_PACKAGES][MAX_LENGTH];
    int package_count = 0;

    file = fopen("req.json", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Read file line by line
    while (fgets(line, sizeof(line), file)) {
        // Parse JSON content
        sscanf(line, "\"%[^\"]\":", distro);
        if (strcmp(distro, "fedora") == 0 || strcmp(distro, "debian") == 0) {
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, "}")) break;
                sscanf(line, "\"%d\": \"%[^\"]\",", &package_count, packages[package_count-1]);
            }
            if (strcmp(distro, "fedora") == 0) {
                // Install packages for Fedora
                for (int i = 0; i < package_count; i++) {
                    char command[MAX_LENGTH];
                    snprintf(command, sizeof(command), "sudo dnf install -y %s", packages[i]);
                    system(command);
                }
            } else if (strcmp(distro, "debian") == 0) {
                // Install packages for Debian
                for (int i = 0; i < package_count; i++) {
                    installPackage(packages[i]);
                }
            }
        }
    }

    fclose(file);
    return 0;
}
