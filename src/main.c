#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>

void myCSS(void){

    GtkCssProvider *provider;
    GdkDisplay *display;
    GdkScreen *screen;

    provider = gtk_css_provider_new ();
    display = gdk_display_get_default ();
    screen = gdk_display_get_default_screen (display);
    gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    const gchar *myCssFile = "myStyle.css";
    GError *error = 0;

    gtk_css_provider_load_from_file(provider, g_file_new_for_path(myCssFile), &error);
    g_object_unref (provider);
}

int main(int argc, char *argv[])
{
	GtkBuilder      *builder; 
	GtkWidget       *window;

	gtk_init(&argc, &argv);
	myCSS();

	builder = gtk_builder_new();
	gtk_builder_add_from_file (builder, "glade/window_main.glade", NULL);

	window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));

	gtk_builder_connect_signals(builder, NULL);
	g_object_unref(builder);
	gtk_widget_show(window);                
	gtk_main();

	return 0;
}

void on_btn_program1_clicked()
{
	system("./bin/cruces &"); 
}

void on_btn_exit_clicked()
{
	gtk_main_quit();
	
}
 
// called when window is closed
void on_window_main_destroy()
{
	gtk_main_quit();
}
