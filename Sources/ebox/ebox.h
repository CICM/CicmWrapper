/*
 * PdEnhanced - Pure Data Enhanced 
 *
 * An add-on for Pure Data
 *
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 *
 * Website  : http://www.mshparisnord.fr/HoaLibrary/
 * Contacts : cicm.mshparisnord@gmail.com
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Library General Public License as published
 * by the Free Software Foundation; either version 2 of the License.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 */

#ifndef DEF_EBOX
#define DEF_EBOX

#include "../estruct.h"
#include "../egraphics/egraphics.h"
#include "../ewidget/ewidget.h"

void *ebox_alloc(t_eclass *c);
void ebox_new(t_ebox *b, long flags, long argc, t_atom *argv);
void ebox_free(t_ebox* x);
char ebox_getregister(t_ebox *x);

void ebox_tk_ids(t_ebox *x, t_canvas *canvas);
void ebox_bind_events(t_ebox* x);
void ebox_create_widget(t_ebox* x);
void ebox_create_window(t_ebox* x, t_glist* glist);

void ebox_dspsetup(t_ebox *x, long nins, long nout);
void ebox_ready(t_ebox *x);

void ebox_dspfree(t_ebox *x);
void ebox_redraw(t_ebox *x);
void ebox_resize_inputs(t_ebox *x, long nins);
void ebox_resize_outputs(t_ebox *x, long nouts);
void ebox_get_rect_for_view(t_object* x, t_object* patcherview, t_rect *rect);
void ebox_dsp(t_ebox *x, t_signal **sp);
t_int* ebox_perform(t_int* w);
void ebox_dsp_add(t_ebox *x, t_symbol* s, t_object* obj, method m, long flags, void *userparam);

void ebox_setdeserted_time(t_ebox *x, float time);
void ebox_deserted(t_ebox *x);

t_elayer* ebox_start_layer(t_object *b, t_object *view, t_symbol *name, float width, float height);
t_pd_err ebox_end_layer(t_object *b, t_object *view, t_symbol *name);
t_pd_err ebox_invalidate_layer(t_object *b, t_object *view, t_symbol *name);
t_pd_err ebox_paint_layer(t_object *b, t_object *view, t_symbol *name, float x, float y);
t_pd_err ebox_notify(t_ebox *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

void ebox_draw_background(t_ebox* x, t_glist* glist);
void ebox_draw_border(t_ebox* x, t_glist* glist);

void ebox_select(t_ebox* x, t_glist* glist);
void ebox_move(t_ebox* x, t_glist* glist);
void ebox_erase(t_ebox* x, t_glist* glist);
void ebox_update(t_ebox *x, t_glist *glist);
void ebox_invalidate_all(t_ebox *x, t_glist *glist);

t_binbuf* binbuf_via_atoms(long ac, t_atom *av);
void attr_binbuf_process(void *x, t_binbuf *d);
void ebox_properties(t_gobj *y, t_glist *x);

t_symbol* ebox_get_fontname(t_ebox* x);
t_symbol* ebox_font_slant(t_ebox* x);
t_symbol* ebox_font_weight(t_ebox* x);
float ebox_font_size(t_ebox* x);

void ebox_set_cursor(t_ebox* x, int mode);
void ebox_mouse_enter(t_ebox* x);
void ebox_mouse_leave(t_ebox* x);
void ebox_mouse_move(t_ebox* x, float x_p, float y_p, float key);
void ebox_mouse_up(t_ebox* x, float x_p, float y_p, float key);
void ebox_mouse_down(t_ebox* x, float x_p, float y_p, float key);
void ebox_mouse_drag(t_ebox* x, float x_p, float y_p, float key);
void ebox_mouse_dblclick(t_ebox* x, float x_p, float y_p);
void ebox_mouse_rightclick(t_ebox* x, float x_p, float y_p);

void ebox_set_mouse_global_position(t_ebox* x, float x_p, float y_p);
t_pt ebox_get_mouse_global_position(t_ebox* x);
t_pt ebox_get_mouse_canvas_position(t_ebox* x);




#endif