#include <gtk/gtk.h>
#include <math.h>
#include <stdbool.h>

typedef enum { CALC_NUL,
			   CALC_ADD,
			   CALC_SUB,
			   CALC_MUL,
			   CALC_DIV,
			   CALC_MOD } CalcOperation;
typedef struct {
	GtkTextBuffer *display_buffer;
	CalcOperation operation;
	bool is_operation_button_clicked;
	double accumulator;
} CalcData;

#define CALC_DATA(__x) ((CalcData *)(__x))

static char *display_get_text(GtkTextBuffer *buffer) {
	GtkTextIter start, end;
	gtk_text_buffer_get_bounds(buffer, &start, &end);
	return gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
}

static void display_set_text(GtkTextBuffer *buffer, const char *str) {
	gtk_text_buffer_set_text(buffer, str, strlen(str));
}

static void display_insert(GtkTextBuffer *buffer, const char *str) {
	int display_char_count = gtk_text_buffer_get_char_count(buffer);
	char *text = display_get_text(buffer);
	if (display_char_count < 22) {
		// You are trying to input any thing but a "." and ...
		if (strcmp(str, ".") != 0) {
			// Text on display is a "0"
			if (display_char_count == 1 && text[0] == '0') {
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

static void display_clear(GtkTextBuffer *buffer) {
	gtk_text_buffer_set_text(GTK_TEXT_BUFFER(buffer), "0", 1);
}

/* Insert a digit after the last digit on display
 * Clear set is_operation_button_clicked to false and clear screen if it is true
 */
static void on_number_button_clicked(GtkWidget *widget, gpointer data) {
	CalcData *calc_data = CALC_DATA(data);
	const char *button_id = gtk_button_get_label(GTK_BUTTON(widget));
	if (calc_data->is_operation_button_clicked == true) {
		display_clear(calc_data->display_buffer);
		calc_data->is_operation_button_clicked = false;
	}
	display_insert(GTK_TEXT_BUFFER(calc_data->display_buffer), button_id);
}

/* Perform calculation on calc_data->accumulator
 * Depending on value of calc_data->operation, either the number on display will be
 * stroed to accumulator, or a calculation of accumulator calc_data->operation number on
 * screen will be performed and the result will be stored to accumulator. In either
 * case, value of calc_data->operation will be set to value of operation paramter
 *
 * Set calc_data->is_operation_button_clicked to true
 */
static void calculate(CalcData *calc_data, CalcOperation operation) {
	char accumulator_str[50];
	// continously operand button hitting prevent;
	if (calc_data->is_operation_button_clicked == false) {
		switch (calc_data->operation) {
		case CALC_ADD:
			calc_data->accumulator += strtod(display_get_text(calc_data->display_buffer), NULL);
			break;
		case CALC_SUB:
			calc_data->accumulator -= strtod(display_get_text(calc_data->display_buffer), NULL);
			break;
		case CALC_MUL:
			calc_data->accumulator *= strtod(display_get_text(calc_data->display_buffer), NULL);
			break;
		case CALC_DIV:
			calc_data->accumulator /= strtod(display_get_text(calc_data->display_buffer), NULL);
			break;
		case CALC_MOD:
			calc_data->accumulator = fmod(calc_data->accumulator, strtod(display_get_text(calc_data->display_buffer), NULL));
			break;
		case CALC_NUL:
			calc_data->accumulator = strtod(display_get_text(calc_data->display_buffer), NULL);
		default:
			break;
		}
	}

	if (ceil(calc_data->accumulator) == calc_data->accumulator) {
		sprintf(accumulator_str, "%.0lf", calc_data->accumulator);
	} else {
		sprintf(accumulator_str, "%.16g", calc_data->accumulator);
	}
	if (strlen(accumulator_str) > 22) {
		if (calc_data->accumulator > 0) {
			sprintf(accumulator_str, "%.16e", calc_data->accumulator);
		} else {
			sprintf(accumulator_str, "%.15e", calc_data->accumulator);
		}
	}

	display_set_text(calc_data->display_buffer, accumulator_str);
	calc_data->is_operation_button_clicked = true;
	calc_data->operation = operation;
}

static void on_button_clicked(GtkWidget *widget, gpointer data) {
	CalcData *calc_data = CALC_DATA(data);
	const char *button_id = gtk_button_get_label(GTK_BUTTON(widget));

	if (strcmp(button_id, "C") == 0) {
		calc_data->accumulator = 0;
		calc_data->is_operation_button_clicked = false;
		calc_data->operation = CALC_NUL;
		display_clear(calc_data->display_buffer);
	} else if (strcmp(button_id, "<-") == 0) {
		display_backspace(GTK_TEXT_BUFFER(calc_data->display_buffer));
	} else if (strcmp(button_id, "+") == 0) {
		calculate(calc_data, CALC_ADD);
	} else if (strcmp(button_id, "-") == 0) {
		calculate(calc_data, CALC_SUB);
	} else if (strcmp(button_id, "x") == 0) {
		calculate(calc_data, CALC_MUL);
	} else if (strcmp(button_id, "/") == 0) {
		calculate(calc_data, CALC_DIV);
	} else if (strcmp(button_id, "%") == 0) {
		calculate(calc_data, CALC_MOD);
	} else if (strcmp(button_id, "=") == 0) {
		calculate(calc_data, CALC_NUL);
	} else if (strcmp(button_id, ".") == 0) {
		display_insert(GTK_TEXT_BUFFER(calc_data->display_buffer), ".");
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

	CalcData *calc_data = CALC_DATA(data);
	calc_data->accumulator = 0;
	calc_data->is_operation_button_clicked = false;
	calc_data->operation = CALC_NUL;

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
	calc_data->display_buffer = display_buffer;
	gtk_text_buffer_set_text(display_buffer, "0", 1);

	// initialize number buttons of Calculator
	for (int i = 0; i < 10; i++) {
		sprintf(button_id, "button%i", i);
		button = GTK_WIDGET(gtk_builder_get_object(builder, button_id));
		g_signal_connect(button, "clicked", G_CALLBACK(on_number_button_clicked), calc_data);
	}

	// clear button
	button = GTK_WIDGET(gtk_builder_get_object(builder, "button_clear"));
	g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), calc_data);
	// backspace button
	button = GTK_WIDGET(gtk_builder_get_object(builder, "button_back"));
	g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), calc_data);
	// mod button
	button = GTK_WIDGET(gtk_builder_get_object(builder, "button_mod"));
	g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), calc_data);
	// divide button
	button = GTK_WIDGET(gtk_builder_get_object(builder, "button_div"));
	g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), calc_data);
	// multiply button
	button = GTK_WIDGET(gtk_builder_get_object(builder, "button_mul"));
	g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), calc_data);
	// minus button
	button = GTK_WIDGET(gtk_builder_get_object(builder, "button_sub"));
	g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), calc_data);
	// add button
	button = GTK_WIDGET(gtk_builder_get_object(builder, "button_add"));
	g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), calc_data);
	// equal button
	button = GTK_WIDGET(gtk_builder_get_object(builder, "button_equ"));
	g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), calc_data);
	// dot button
	button = GTK_WIDGET(gtk_builder_get_object(builder, "button_dot"));
	g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), calc_data);

	gtk_widget_set_visible(window, TRUE);
	gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
	GtkApplication *app;
	CalcData *calc_data = malloc(sizeof(CalcData));
	int status;

	app = gtk_application_new("com.rekaerst.calc", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), calc_data);
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);

	return status;
}
