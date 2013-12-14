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
#include "../erouter/erouter.h"

// ebox.c functions //
////////////////////////////////////////////////////////////////////////////////////////////////////////
void *ebox_alloc(t_eclass *c);
void ebox_new(t_ebox *b, long flags);
void ebox_ready(t_ebox *x);
void ebox_free(t_ebox* x);
char ebox_getregister(t_ebox *x);

// ebox_dsp.c functions//
////////////////////////////////////////////////////////////////////////////////////////////////////////
void ebox_dspsetup(t_ebox *x, long nins, long nouts);
void ebox_dspfree(t_ebox *x);
void ebox_resize_inputs(t_ebox *x, long nins);
void ebox_resize_outputs(t_ebox *x, long nouts);

void ebox_dsp(t_ebox *x, t_signal **sp);                                                            // PRIVATE
t_int* ebox_perform(t_int* w);                                                                      // PRIVATE
void ebox_getconnections(t_ebox* x, short* count);                                                  // PRIVATE
void ebox_dsp_add(t_ebox *x, t_symbol* s, t_object* obj, method m, long flags, void *userparam);    // PRIVATE

// ebox_draw.c functions//
////////////////////////////////////////////////////////////////////////////////////////////////////////
void ebox_redraw(t_ebox *x);
void ebox_get_rect_for_view(t_ebox* x, t_rect *rect);
t_elayer* ebox_start_layer(t_ebox *b, t_symbol *name, float width, float height);
t_pd_err ebox_end_layer(t_ebox *b, t_symbol *name);
t_pd_err ebox_invalidate_layer(t_ebox *b,t_symbol *name);
t_pd_err ebox_paint_layer(t_ebox *b, t_symbol *name, float x, float y);

void ebox_draw_border(t_ebox* x);                                                                   // PRIVATE
void ebox_draw_iolets(t_ebox* x);                                                                   // PRIVATE
void ebox_invalidate_all(t_ebox *x);                                                                // PRIVATE
void ebox_update(t_ebox *x);                                                                        // PRIVATE
void ebox_select(t_ebox* x);                                                                        // PRIVATE
void ebox_move(t_ebox* x);                                                                          // PRIVATE
void ebox_erase(t_ebox* x);                                                                         // PRIVATE

// ebox_attr.c functions//
////////////////////////////////////////////////////////////////////////////////////////////////////////
void binbuf_attr_process(void *x, t_binbuf *d);
void atoms_attr_process(void *x, long argc, t_atom *argv);
void default_attr_process(void *x);
t_symbol* ebox_get_fontname(t_ebox* x);
t_symbol* ebox_font_slant(t_ebox* x);
t_symbol* ebox_font_weight(t_ebox* x);
float ebox_font_size(t_ebox* x);

void ebox_properties(t_gobj *y, t_glist *x);                                                        // PRIVATE
void ebox_dialog(t_object *x, t_symbol *s, long argc, t_atom* argv);                                // PRIVATE
t_pd_err ebox_notify(t_ebox *x, t_symbol *s, t_symbol *msg, void *sender, void *data);              // PRIVATE
t_pd_err ebox_size_set(t_ebox *x, t_object *attr, long argc, t_atom *argv);                         // PRIVATE
void ebox_attrprint(t_ebox* x);                                                                     // PRIVATE

// ebox_window.c functions//
////////////////////////////////////////////////////////////////////////////////////////////////////////
void ebox_tk_ids(t_ebox *x, t_canvas *canvas);                                                      // PRIVATE
void ebox_bind_events(t_ebox* x);                                                                   // PRIVATE
void ebox_create_widget(t_ebox* x);                                                                 // PRIVATE
void ebox_create_window(t_ebox* x, t_glist* glist);                                                 // PRIVATE

// ebox_handle.c functions//
////////////////////////////////////////////////////////////////////////////////////////////////////////
t_pt ebox_get_mouse_global_position(t_ebox* x);
t_pt ebox_get_mouse_canvas_position(t_ebox* x);
void ebox_set_cursor(t_ebox* x, int mode);

void ebox_set_mouse_global_position(t_ebox* x, float x_p, float y_p);                               // PRIVATE
void ebox_set_mouse_canvas_position(t_ebox* x, float x_p, float y_p);                               // PRIVATE
void ebox_mouse_enter(t_ebox* x);                                                                   // PRIVATE
void ebox_mouse_leave(t_ebox* x);                                                                   // PRIVATE
void ebox_mouse_move(t_ebox* x, t_symbol* s, long argc, t_atom* argv);                              // PRIVATE
void ebox_mouse_up(t_ebox* x, t_symbol* s, long argc, t_atom* argv);                                // PRIVATE
void ebox_mouse_down(t_ebox* x, t_symbol* s, long argc, t_atom* argv);                              // PRIVATE
void ebox_mouse_drag(t_ebox* x, t_symbol* s, long argc, t_atom* argv);                              // PRIVATE
void ebox_mouse_dblclick(t_ebox* x, t_symbol* s, long argc, t_atom* argv);                          // PRIVATE
void ebox_mouse_wheel(t_ebox* x, t_symbol* s, long argc, t_atom* argv);                             // PRIVATE
void ebox_mouse_move_editmode(t_ebox* x, float x_p, float y_p, float key);                          // PRIVATE
void ebox_popup(t_ebox* x, t_symbol* s, float itemid);                                              // PRIVATE
void ebox_key(t_ebox* x, t_symbol* s, long argc, t_atom* argv);                                     // PRIVATE
void ebox_setdeserted_time(t_ebox *x, float time);                                                  // PRIVATE
void ebox_deserted(t_ebox *x);                                                                      // PRIVATE
void ebox_dosave_box(t_gobj* z, t_binbuf *b);                                                       // PRIVATE
void ebox_dosave_nobox(t_gobj* z, t_binbuf *b);                                                     // PRIVATE
void ebox_save(t_gobj* z, t_binbuf *b);                                                             // PRIVATE
t_pd_err ebox_set_id(t_ebox *x, t_object *attr, long argc, t_atom *argv);                           // PRIVATE

// ebox_patcher.c functions//
////////////////////////////////////////////////////////////////////////////////////////////////////////
void ebox_patcher_editmode(t_ebox* x, t_symbol* s, long argc, t_atom* argv);                        // PRIVATE
void ebox_patcher_motion(t_ebox* x, t_symbol* s, long argc, t_atom* argv);                          // PRIVATE
void ebox_patcher_key(t_ebox* x, t_symbol* s, long argc, t_atom* argv);                             // PRIVATE
void ebox_patcher_mouse(t_ebox* x, t_symbol* s, long argc, t_atom* argv);                           // PRIVATE
void ebox_patcher_focus(t_ebox* x, t_symbol* s, long argc, t_atom* argv);                           // PRIVATE

#endif



