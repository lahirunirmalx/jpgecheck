/*
 * calc2.c
 * 
 * Copyright 2024 lahiru <lahiru@lahiru-Lenovo-V15-G2-ITL>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <stdio.h>
#include <stdbool.h> 
#include <gtk/gtk.h> 
#include <string.h> 
#include <gio/gio.h>  

#define MAX_SIG_LEN 10 

gchar *path ;
GObject *window;
GObject *text_view;

  static void showMessage1(char *message) {
	GtkWindow *gwindow = GTK_WINDOW(window);  
	const gchar *gm[sizeof(message)];
	 snprintf(gm, sizeof(gm), "%s", message);
		  GtkWidget *dialog = gtk_message_dialog_new(gwindow,
                                                   GTK_DIALOG_DESTROY_WITH_PARENT,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   gm);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
	}
 
static void on_file_selected (GtkFileChooserButton *chooser, gpointer user_data) {
 
          gchar *uri = gtk_file_chooser_get_uri(GTK_FILE_CHOOSER(chooser)); 
          if (strncmp(uri, "file://", 7) == 0) {
          path = g_strdup(uri + 7);   
          g_print("File path from URI: %s\n", path);
    
			} else {
				showMessage1("Unsupported URI format.");
			}
    
         
}

static void check_file_type (GtkWidget *widget, gpointer   data) { 
	if(path){ 
		
    FILE* fp = fopen(path, "rb"); 
    unsigned char magic[4];
    if (fp == NULL) {
        showMessage1 ("Error opening file"); 
    }
 if (fread(magic, 1, 4, fp) != 4) {
        if (feof(fp)) 
            showMessage1("File is less than 4 bytes");
        else
            showMessage1("Error reading the file");
 
    }

     fclose(fp);
     char formatted_string[12];  
   char *realDate = "ff d8 ff e0";
   sprintf(formatted_string, "%02hhx %02hhx %02hhx %02hhx",  magic[0], magic[1], magic[2], magic[3]);
   
   char *text[1024];
   char *result;
   if (strcmp(formatted_string, realDate) == 0) {
	     result = "File Type Is Jpeg !";
	   }else{
		   
		    result = "File Type Is Not Jpeg!";
		   
		   }
  sprintf(text, "Expected : %s \n Read From file %s \n %s ",  realDate, formatted_string, result);
 GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
  gtk_text_buffer_set_text(buffer, text, -1); 
  } else { 
	 	  
     showMessage1("Please Select File Before Execute !");
		}
}
 


static void init_ui(int argc, char *argv[]) {
    GtkBuilder *builder;

    GObject *button;
 

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    GError *error = NULL;

    if (gtk_builder_add_from_file(builder, "builder.ui", &error) == 0) {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return;
    }

       window = gtk_builder_get_object(builder, "window1");
       g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
       
       button = gtk_builder_get_object (builder, "exitButton" );
       g_signal_connect (button, "clicked", G_CALLBACK (gtk_main_quit), NULL);
         
       GtkFileChooserButton *file_chooser_button = GTK_FILE_CHOOSER_BUTTON(gtk_builder_get_object(builder, "fileChoose"));
       g_signal_connect(file_chooser_button, "file-set", G_CALLBACK(on_file_selected), NULL);

       button = gtk_builder_get_object (builder, "fileExecute" );
       g_signal_connect (button, "clicked", G_CALLBACK (check_file_type), NULL);
       
       
        text_view = gtk_builder_get_object (builder, "resultDisplay" ); 
       

    gtk_main();
}

int main(int argc, char *argv[]) {
    init_ui(argc, argv);
    return 0;
}
