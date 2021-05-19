/**
 * Maemo Screen Breaker (braek)
 * Copyright (c) 2009 Thomas Perl.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of Thomas Perl nor the names of any contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <cairo/cairo.h>
#include <gtk/gtk.h>
#include <assert.h>

#include "braek.h"

typedef struct { const char* buf; const char* end; } bclosure;

cairo_status_t read_braek_image(void* closure, unsigned char* data,
        unsigned int length)
{
    bclosure* c = (bclosure*)closure;
    int take = MIN(length, (c->end - c->buf));
    memcpy(data, c->buf, take);
    c->buf += take;
    return CAIRO_STATUS_SUCCESS;
}


gboolean on_expose(GtkWidget* widget, GdkEventExpose* event, gpointer p)
{
    cairo_t* cr = gdk_cairo_create(GDK_DRAWABLE(widget->window));
    cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
    cairo_set_source(cr, (cairo_pattern_t*)p);
    cairo_paint(cr);
    cairo_destroy(cr);
    return TRUE;
}

int main(int argc, char** argv)
{
    GtkWidget* win;
    cairo_surface_t* surface;
    cairo_pattern_t* pattern;
    GdkScreen* screen;
    GdkColormap* colormap;
    int iw, ih, w, h;
    bclosure c = { braekdata, braekdata+sizeof(braekdata) };

    gtk_init(&argc, &argv);

    win = gtk_window_new(GTK_WINDOW_POPUP);

    screen = gtk_widget_get_screen(win);
    colormap = gdk_screen_get_rgba_colormap(screen);
    gtk_widget_set_colormap(win, colormap);

    surface = cairo_image_surface_create_from_png_stream(read_braek_image, &c);
    pattern = cairo_pattern_create_for_surface(surface);

    g_signal_connect(win, "expose-event", (GCallback)on_expose,
            (gpointer)pattern);

    w = gdk_screen_get_width(screen);
    h = gdk_screen_get_height(screen);
    iw = cairo_image_surface_get_width(surface);
    ih = cairo_image_surface_get_height(surface);
    gtk_window_set_default_size(GTK_WINDOW(win), iw, ih);

    srand((int)time(NULL));
    gtk_window_move(GTK_WINDOW(win), rand()%(w-iw), rand()%(h-ih));
    gtk_widget_show(win);

    gtk_main();
    return 0;
}

