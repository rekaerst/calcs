#include <gtk/gtk.h>

static char *display_get_text(GtkTextBuffer *buffer) {
	GtkTextIter start, end;
	gtk_text_buffer_get_bounds(buffer, &start, &end);
	return gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
}

static void display_insert(GtkTextBuffer *buffer, const char *str) {
	int display_char_count = gtk_text_buffer_get_char_count(buffer);
	char *text = display_get_text(buffer);
	if (display_char_count < 22) {
		// You are trying to input any thing but a "." and ...
		if (strcmp(str, ".") != 0) {
			// Text on display is a "0"
			if (display_char_count == 1 && text[0] == '0' && strcmp(str, ".") != 0) {
				gtk_text_buffer_set_text(buffer, str, strlen(str));
			} else { // Text on display is NOT a "0"
				gtk_text_buffer_insert_at_cursor(buffer, str, strlen(str));
			}
			// You are tyring to input a "." and ...
		} else if (strchr(text, '.') == NULL) { // Text on display does NOT contain a '.'
			gtk_text_buffer_insert_at_cursor(buffer, ".", 1);
		} // Text on display does contain a '.'. Well, we are not gonna to do anything
	}
}

static void display_backspace(GtkTextBuffer *buffer) {
	int display_char_count = gtk_text_buffer_get_char_count(buffer);
	if (display_char_count > 0) {
		char *text = display_get_text(buffer);

		if (display_char_count == 1) {
			strcpy(text, "0");
		} else {
			text[strlen(text) - 1] = '\0';
		}
		gtk_text_buffer_set_text(buffer, text, strlen(text));
	}
}
static void on_number_button_clicked(GtkWidget *widget, gpointer data) {
	const char *button_id = gtk_button_get_label(GTK_BUTTON(widget));
	display_insert(GTK_TEXT_BUFFER(data), button_id);
}

static void on_button_clicked(GtkWidget *widget, gpointer data) {
	const char *button_id;
	button_id = gtk_button_get_label(GTK_BUTTON(widget));
	if (strcmp(button_id, "C") == 0) {
		gtk_text_buffer_set_text(GTK_TEXT_BUFFER(data), "0", 1);
	} else if (strcmp(button_id, "<-") == 0) {
		display_backspace(GTK_TEXT_BUFFER(data));
	} else if (strcmp(button_id, "/") == 0) {
	} else if (strcmp(button_id, "x") == 0) {
	} else if (strcmp(button_id, "-") == 0) {
	} else if (strcmp(button_id, "+") == 0) {
	} else if (strcmp(button_id, "=") == 0) {
	} else if (strcmp(button_id, "%") == 0) {
	} else if (strcmp(button_id, ".") == 0) {
		display_insert(GTK_TEXT_BUFFER(data), ".");
	}
}

static void activate(GtkApplication *app, gpointer data) {
	GtkWidget *window;
	GtkWidget *display;
	GtkTextBuffer *display_buffer;
	GtkWidget *button;
	char button_id[] = "buttonX";

	GtkBuilder *builder;

	GtkCssProvider *provider;

	provider = gtk_css_provider_new();
	gtk_css_provider_load_from_path(provider, "theme.css", NULL);
	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

	builder = gtk_builder_new_from_file("calc.ui");
	gtk_builder_connect_signals(builder, NULL);

	window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
	gtk_window_set_application(GTK_WINDOW(window), app);
	gtk_window_set_title(GTK_WINDOW(window), "Calculator");
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_widget_destroy), &window);

	display = GTK_WIDGET(gtk_builder_get_object(builder, "display"));
	display_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(display));
	gtk_text_buffer_set_text(display_buffer, "0", 1);

	// initialize number buttons of Calculator
	for (int i = 0; i < 10; i++) {
		sprintf(button_id, "button%i", i);
		button = GTK_WIDGET(gtk_builder_get_object(builder, button_id));
		g_signal_connect(button, "clicked", G_CALLBACK(on_number_button_clicked), display_buffer);
	}

	// clear button
	button = GTK_WIDGET(gtk_builder_get_object(builder, "button_clear"));
	g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), display_buffer);
	// backspace button
	button = GTK_WIDGET(gtk_builder_get_object(builder, "button_back"));
	g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), display_buffer);
	// mod button
	button = GTK_WIDGET(gtk_builder_get_object(builder, "button_mod"));
	g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), display_buffer);
	// divide button
	button = GTK_WIDGET(gtk_builder_get_object(builder, "button_div"));
	g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), display_buffer);
	// multiply button
	button = GTK_WIDGET(gtk_builder_get_object(builder, "button_mul"));
	g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), display_buffer);
	// minus button
	button = GTK_WIDGET(gtk_builder_get_object(builder, "button_min"));
	g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), display_buffer);
	// add button
	button = GTK_WIDGET(gtk_builder_get_object(builder, "button_add"));
	g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), display_buffer);
	// equal button
	button = GTK_WIDGET(gtk_builder_get_object(builder, "button_equ"));
	g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), display_buffer);
	// dot button
	button = GTK_WIDGET(gtk_builder_get_object(builder, "button_dot"));
	g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), display_buffer);

	gtk_widget_set_visible(window, TRUE);
	gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
	GtkApplication *app;
	int status;

	app = gtk_application_new("com.rekaerst.calc", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);

	return status;
}
