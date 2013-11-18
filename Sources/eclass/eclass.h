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

#ifndef DEF_ECLASS
#define DEF_ECLASS

#include "../estruct.h"
#include "../eattr/eattr.h"

t_eclass* eclass_new(char *name, method newmethod, method freemethod, size_t size, int flags, t_atomtype arg1, int arg2);
void eclass_init(t_eclass* c, long flags);
void eclass_dspinit(t_eclass* c);
void eclassbox_dspinit(t_eclass* c);
void eclass_addmethod(t_eclass* c, method m, char* name, t_atomtype type, long anything);
t_pd_err eclass_register(t_symbol *name_space, t_eclass *c);

void eclass_default_attributes(t_eclass* c);
void eclass_properties_dialog(t_eclass* c);

#endif