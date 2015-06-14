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

void *eobj_new(t_eclass *c);
void eobj_free(void *x);
t_eproxy* eobj_proxynew(void* x);
int  eobj_getproxy(void* x);
t_eclass* eobj_getclass(void* x);
t_symbol* eobj_getclassname(void* x);
t_canvas* eobj_getcanvas(void *x);
char eobj_isbox(void *x);
char eobj_isdsp(void *x);
void eobj_dspsetup(void *x, long nins, long nouts);
void eobj_dspfree(void *x);
void eobj_resize_inputs(void *x, long nins);
t_sample* eobj_getsignalinput(void *x, long index);
t_sample* eobj_getsignaloutput(void *x, long index);

//! @cond
void eobj_save(t_gobj* x, t_binbuf *b);
void eobj_dosave(t_eobj* x, t_binbuf *b);
void eobj_popup(t_eobj* x, t_symbol* s, float itemid);
void eobj_write(t_eobj* x, t_symbol* s, int argc, t_atom *argv);
void eobj_read(t_eobj* x, t_symbol* s, int argc, t_atom *argv);

void eobj_dsp(void *x, t_signal **sp);
t_int* eobj_perform(t_int* w);
t_int* eobj_perform_no_inplace(t_int* w);
t_int* eobj_perform_box(t_int* w);
t_int* eobj_perform_box_no_inplace(t_int* w);
void eobj_dsp_add(void *x, t_symbol* s, t_object* obj, method m, long flags, void *userparam);
//! @endcond

#endif



