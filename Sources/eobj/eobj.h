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
#include "../ewidget/ewidget.h"
#include "../erouter/erouter.h"

/*********************************
 * OBJECT
 *********************************/

void *eobj_alloc(t_eclass *c);
char eobj_getregister(void *x);
void eobj_inletnew(void* x);
int  eobj_getproxy(void* x);

/*********************************
 * PROXY
 *********************************/

//! @cond
void eproxy_setup(t_eclass* c);
void eproxy_anything(t_eproxy *x, t_symbol *s, int argc, t_atom *argv);
void *eproxy_new(t_symbol *s, int argc, t_atom *argv);
void eproxy_init(t_eproxy *x, void *owner, int index);
//! @endcond

/*********************************
 * DSP
 *********************************/

void eobj_dspsetup(void *x, long nins, long nouts);
void eobj_dspfree(void *x);
void eobj_resize_inputs(void *x, long nins);
void eobj_resize_outputs(void *x, long nouts);

//! @cond
void eobj_dsp(t_edspobj *x, t_signal **sp);
t_int* eobj_perform(t_int* w);
void eobj_getconnections(t_edspobj* x, short* count);
void eobj_dsp_add(t_edspobj *x, t_symbol* s, t_object* obj, method m, long flags, void *userparam);
//! @endcond

#endif



