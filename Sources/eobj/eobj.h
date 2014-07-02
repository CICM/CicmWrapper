/*
 * CicmWrapper
 *
 * A wrapper for Pure Data
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

/*!
 * @file eobj.h
 * @author Pierre Guillot
 * @version 0.1
 * @date 2013-2014
 */

#ifndef DEF_EOBJ
#define DEF_EOBJ

#include "../estruct.h"
#include "../egraphics/egraphics.h"
#include "../ecommon/ecommon.h"

/*********************************
 * OBJECT
 *********************************/

void *eobj_new(t_eclass *c);
void eobj_free(void *x);
t_eproxy* eobj_proxynew(void* x);
int  eobj_getproxy(void* x);
t_eclass* eobj_getclass(void* x);
t_symbol* eobj_getclassname(void* x);
t_canvas* eobj_getcanvas(void *x);
char eobj_isbox(void *x);
char eobj_isdsp(void *x);

/*********************************
 * HANDLE
 *********************************/

t_pt eobj_get_mouse_global_position(void* x);
t_pt eobj_get_mouse_canvas_position(void* x);
char eobj_get_mouse_status(void* x);
long eobj_get_mouse_modifier(void* x);

//! @cond
void eobj_save(t_eobj* x, t_binbuf *b);
void eobj_dosave(t_eobj* x, t_binbuf *b);
void eobj_popup(t_eobj* x, t_symbol* s, float itemid);
void eobj_write(t_eobj* x, t_symbol* s, long argc, t_atom* argv);
void eobj_read(t_eobj* x, t_symbol* s, long argc, t_atom* argv);
void eobj_tick(t_eobj* x);
void eobj_poll_mouse(void* x);
void eobj_nopoll_mouse(void* x);
void eobj_mousecanvas(t_eobj* x, float px, float py);
//! @endcond

/*********************************
 * PROXY
 *********************************/

//! @cond

void inlet_wrong(t_inlet *x, t_symbol *s);
void new_inlet_bang(t_inlet *x);
void new_inlet_pointer(t_inlet *x, t_gpointer *gp);
void new_inlet_float(t_inlet *x, t_float f);
void new_inlet_symbol(t_inlet *x, t_symbol *s);
void new_inlet_list(t_inlet *x, t_symbol *s, int argc, t_atom *argv);
void new_inlet_anything(t_inlet *x, t_symbol *s, int argc, t_atom *argv);

void eproxy_setup();
void eproxy_anything(t_eproxy *x, t_symbol *s, int argc, t_atom *argv);
t_eproxy* eproxy_new(void *owner);
t_eproxy* eproxy_signalnew(void *owner, float f);
void eproxy_free(t_eproxy* proxy);

/*
void proxlet_init();
t_proxlet *proxlet_new(t_object *owner);
t_proxlet *signalproxlet_new(t_object *owner, t_float f);
void proxlet_free(t_proxlet *x);

void proxlet_bang(t_proxlet *x);
void proxlet_float(t_proxlet *x, t_float f);
void proxlet_symbol(t_proxlet *x, t_symbol *s);
void proxlet_list(t_proxlet *x, t_symbol *s, int argc, t_atom *argv);
void proxlet_anything(t_proxlet *x, t_symbol *s, int argc, t_atom *argv);
 */
//! @endcond

/*********************************
 * DSP
 *********************************/

void eobj_dspsetup(void *x, long nins, long nouts);
void eobj_dspfree(void *x);
void eobj_resize_inputs(void *x, long nins);
void eobj_resize_outputs(void *x, long nouts);
t_eproxy* eobj_getdspproxlet(void *x, long index);
t_outlet* eobj_getdspoutlet(void *x, long index);
t_sample* eobj_getsignalinput(void *x, long index);
t_sample* eobj_getsignaloutput(void *x, long index);

//! @cond
void eobj_dsp(void *x, t_signal **sp);
t_int* eobj_perform(t_int* w);
t_int* eobj_perform_no_inplace(t_int* w);

t_int* eobj_perform_box(t_int* w);
t_int* eobj_perform_box_no_inplace(t_int* w);
void eobj_getconnections(void* x, short* count);
void eobj_dsp_add(void *x, t_symbol* s, t_object* obj, method m, long flags, void *userparam);
//! @endcond

/*********************************
 * ROUTER
 *********************************/

void eobj_attach_torouter(t_object* child);
void eobj_detach_torouter(t_object* child);

//! @cond
void erouter_setup();
void erouter_free(t_erouter *x);
void erouter_anything(t_erouter *x, t_symbol *s, long argc, t_atom *argv);

void erouter_tick(t_erouter * x);
void erouter_mousedown(t_erouter *x, t_symbol *s, int argc, t_atom *argv);
void erouter_mouseup(t_erouter *x, t_symbol *s, int argc, t_atom *argv);
void erouter_mousemove(t_erouter *x, t_symbol *s, int argc, t_atom *argv);

void erouter_mouseglobal(t_erouter *x, float px, float py);
void erouter_mousetopcanvas(t_erouter *x, float px, float py);

t_pt erouter_getmouse_topcanvas_position();
t_pt erouter_getmouse_global_position();
long erouter_getmouse_modifier();
char erouter_getmouse_status();
//! @endcond

#endif



