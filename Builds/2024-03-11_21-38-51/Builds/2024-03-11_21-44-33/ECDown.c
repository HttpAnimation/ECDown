#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <jansson.h>

#define MAX_URL_LENGTH 256
#define MAX_USERNAME_LENGTH 64
#define MAX_API_KEY_LENGTH 64

// Function to perform HTTP GET request using libcurl
static size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    FILE *file = (FILE *)userdata;
    size_t written = fwrite(ptr, size, nmemb, file);
    return written;
}

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
    char download_url[MAX_URL_LENGTH + 10]; // Additional 10 characters for "/download" part

    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_ALL);
    CURL *curl = curl_easy_init();

    if (!curl) {
        fprintf(stderr, "Error: Failed to initialize libcurl.\n");
        return 1;
    }

    // Read login credentials from login.json file
    read_login_credentials(username, api_key);

    // Get URL from user input
    printf("Enter the URL of the post: ");
    fgets(url, MAX_URL_LENGTH, stdin);

    // Remove newline character from the end of URL
    url[strcspn(url, "\n")] = 0;

    // Append "/download" to the URL
    snprintf(download_url, MAX_URL_LENGTH + 10, "%s/download", url);

    // Set URL to download
    curl_easy_setopt(curl, CURLOPT_URL, download_url);

    // Set up headers with username and API key for authorization
    struct curl_slist *headers = NULL;
    char authorization_header[MAX_USERNAME_LENGTH + MAX_API_KEY_LENGTH + 20]; // Additional characters for "Authorization: Basic " and null terminator
    char base64_encoded_credentials[MAX_USERNAME_LENGTH + MAX_API_KEY_LENGTH + 2]; // Additional characters for colon separator and null terminator
    snprintf(base64_encoded_credentials, sizeof(base64_encoded_credentials), "%s:%s", username, api_key);
    char *encoded_credentials = curl_easy_escape(curl, base64_encoded_credentials, 0);
    snprintf(authorization_header, sizeof(authorization_header), "Authorization: Basic %s", encoded_credentials);
    headers = curl_slist_append(headers, authorization_header);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // Set up file to save the downloaded image
    FILE *fp = fopen("downloaded_image.jpg", "wb");
    if (!fp) {
        fprintf(stderr, "Error: Failed to open file for writing.\n");
        return 1;
    }
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

    // Perform the request
    CURLcode res = curl_easy_perform(curl);

    // Check for errors
    if (res != CURLE_OK) {
        fprintf(stderr, "Error: Failed to download the image: %s\n", curl_easy_strerror(res));
    } else {
        printf("Image downloaded successfully!\n");
    }

    // Clean up
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    fclose(fp);
    curl_free(encoded_credentials);
    curl_global_cleanup();

    return 0;
}
