#include <gtk/gtk.h>
#include <curl/curl.h>

#define MAX_URL_LENGTH 1000

GtkWidget *url_entry;
GtkWidget *download_button;

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    return fwrite(ptr, size, nmemb, stream);
}

void download_image(const gchar *url) {
    CURL *curl;
    FILE *fp;
    gchar filename[MAX_URL_LENGTH];
    
    curl = curl_easy_init();
    
    if (curl) {
        fp = fopen("downloaded_image.png", "wb");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
    }
}

void on_download_clicked(GtkWidget *widget, gpointer data) {
    const gchar *url = gtk_entry_get_text(GTK_ENTRY(url_entry));
    download_image(url);
    g_print("Image downloaded successfully!\n");
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *vbox;
    
    gtk_init(&argc, &argv);
    
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_window_set_title(GTK_WINDOW(window), "e621 Image Downloader");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);
    
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    
    url_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(url_entry), "Enter e621 URL");
    gtk_box_pack_start(GTK_BOX(vbox), url_entry, TRUE, TRUE, 0);
    
    download_button = gtk_button_new_with_label("Download");
    g_signal_connect(download_button, "clicked", G_CALLBACK(on_download_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), download_button, TRUE, TRUE, 0);
    
    gtk_widget_show_all(window);
    
    gtk_main();
    
    return 0;
}
