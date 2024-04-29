#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <vte/vte.h>
#include "rgba.h"

static void on_terminal_spawn_async(VteTerminal *terminal,
                                    GPid pid,
                                    GError *error,
                                    gpointer user_data) {

    // Destroy the window when the child exits
    GtkWindow *window = gtk_widget_get_parent(GTK_WIDGET(terminal));
    gtk_window_destroy(GTK_WINDOW(window));
}

static void spawn_child_process (VteTerminal *terminal) {
    char *command_argv[2] = { "btop", NULL };

    vte_terminal_spawn_async (
        VTE_TERMINAL (terminal),
	VTE_PTY_DEFAULT,
	NULL,
	command_argv,
	NULL,
	G_SPAWN_SEARCH_PATH,
	NULL,
	NULL,
	NULL,
	2000,
	NULL,
	NULL,
	NULL
    );
}

static void activate (GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *terminal;

    // Create a new window
    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "BTop Desktop");

    // Prefer Dark Theme
    g_object_set(gtk_settings_get_default(),
                "gtk-application-prefer-dark-theme", TRUE, NULL);

    // Create a new VTE terminal widget
    terminal = vte_terminal_new ();

    // Connect the "spawn-async" signal to the callback function
    g_signal_connect(terminal, "child-exited", G_CALLBACK(on_terminal_spawn_async), NULL);

    GdkRGBA fgcolor;
    GdkRGBA bgcolor;
    fgcolor = (GdkRGBA) { 1.0, 1.0, 1.0, 1.0}; // White
    bgcolor = (GdkRGBA) { 0.17, 0.0, 0.11, 1.0 }; // Aubergine

    // Workings of GDK_RGBA prevent this being static
    GdkRGBA palette[16] = {
      GDK_RGBA ("2c001e"), // Aubergine
      GDK_RGBA ("c01c28"), // Red
      GDK_RGBA ("2ec27e"), // Green
      GDK_RGBA ("f5c211"), // Yellow
      GDK_RGBA ("1e78e4"), // Blue
      GDK_RGBA ("9841bb"), // Magenta
      GDK_RGBA ("0ab9dc"), // Cyan
      GDK_RGBA ("c0bfbc"), // White
      GDK_RGBA ("5e5c64"), // Bright Black
      GDK_RGBA ("ed333b"), // Bright Red
      GDK_RGBA ("57e389"), // Bright Green
      GDK_RGBA ("f8e45c"), // Bright Yellow
      GDK_RGBA ("51a1ff"), // Bright Blue
      GDK_RGBA ("c061cb"), // Bright Magenta
      GDK_RGBA ("4fd2fd"), // Bright Cyan
      GDK_RGBA ("f6f5f4"), // Bright White
    };

    // Set the foreground and background colors
    vte_terminal_set_colors (VTE_TERMINAL (terminal), &fgcolor, &bgcolor, &palette, 16);

    // Disable cursor blink
    vte_terminal_set_cursor_blink_mode (VTE_TERMINAL (terminal), VTE_CURSOR_BLINK_OFF);

    gtk_widget_set_vexpand (terminal, TRUE);
    gtk_widget_set_hexpand (terminal, TRUE);
    gtk_window_set_child (GTK_WINDOW (window), terminal);
    spawn_child_process (VTE_TERMINAL (terminal));

    // Show the window
    gtk_window_present (GTK_WINDOW (window));
}

int main (int argc, char *argv[]) {
    GtkApplication *app;
    int status;

    // Create a new GTK application
    app = gtk_application_new ("org.vandine.ken.btop", G_APPLICATION_NON_UNIQUE);

    // Connect the activate function to the activate signal
    g_signal_connect(app, "activate", G_CALLBACK (activate), NULL);

    // Run the GTK application
    status = g_application_run (G_APPLICATION (app), argc, argv);

    // Clean up the application
    g_object_unref (app);

    return status;
}
