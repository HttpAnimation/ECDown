#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>

#define MAX_URL_LENGTH 256
#define MAX_USERNAME_LENGTH 64
#define MAX_API_KEY_LENGTH 64

// Function to read username and API key from login.json file
void read_login_credentials(char *username, char *api_key) {
    FILE *file = fopen("login.json", "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Failed to open login.json file.\n");
        exit(1);
    }

    // Read JSON data
    json_t *root;
    json_error_t error;
    root = json_loadf(file, 0, &error);
    fclose(file);

    if (!root) {
        fprintf(stderr, "Error: JSON parsing error: %s\n", error.text);
        exit(1);
    }

    // Get username and API key from JSON
    json_t *json_username = json_object_get(root, "username");
    json_t *json_api_key = json_object_get(root, "api_key");

    if (!json_username || !json_api_key || !json_is_string(json_username) || !json_is_string(json_api_key)) {
        fprintf(stderr, "Error: Invalid JSON format in login.json.\n");
        exit(1);
    }

    strcpy(username, json_string_value(json_username));
    strcpy(api_key, json_string_value(json_api_key));

    // Clean up JSON object
    json_decref(root);
}

int main() {
    char username[MAX_USERNAME_LENGTH];
    char api_key[MAX_API_KEY_LENGTH];
    char url[MAX_URL_LENGTH];
    char download_command[MAX_URL_LENGTH + 20]; // Additional characters for wget command

    // Read login credentials from login.json file
    read_login_credentials(username, api_key);

    // Get URL from user input
    printf("Enter the URL of the post: ");
    fgets(url, MAX_URL_LENGTH, stdin);

    // Remove newline character from the end of URL
    url[strcspn(url, "\n")] = 0;

    // Construct wget command
    snprintf(download_command, MAX_URL_LENGTH + 20, "wget --http-user=%s --http-password=%s -O downloaded_image.jpg %s", username, api_key, url);

    // Execute wget command
    int status = system(download_command);

    if (status != 0) {
        fprintf(stderr, "Error: Failed to download the image.\n");
        return 1;
    }

    printf("Image downloaded successfully!\n");

    return 0;
}
