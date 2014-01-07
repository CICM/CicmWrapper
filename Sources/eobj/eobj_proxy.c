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

//! @cond

#include "eobj.h"

t_class *eproxy_class;

//! Intialize a proxy inlet
/*
 * @memberof    eobj
 * @param x     The proxy pointer
 * @param owner The eobj pointer
 * @param index The index of the proxy
 * @return      This function return nothing
 */
void eproxy_init(t_eproxy *x, void *owner, int index)
{
	x->p_owner = (t_object *)owner;
	x->p_pd    = eproxy_class;
    x->p_index = index;
}

//! Allocate the memory for a proxy inlet
/*
 * @memberof    eobj
 * @param s     A symbol unsued
 * @param argc  The size of the array of atoms unusued
 * @param argv  The array of atoms unusued
 * @return      This function return nothing
 */
void *eproxy_new(t_symbol *s, int argc, t_atom *argv)
{
	t_eproxy *x = NULL;
    x = (t_eproxy *)pd_new(eproxy_class);
    return x;
}

//! Anything method of the proxy inlet
/*
 * @memberof    eobj
 * @param x     The proxy pointer
 * @param s     The symbol selector
 * @param argc  The size of the array of atoms
 * @param argv  The array of atoms
 * @return      This function return nothing
 * @details     This function record the current index of the proxy and call the eobj method correspondinf to the selector.
 */
void eproxy_anything(t_eproxy *x, t_symbol *s, int argc, t_atom *argv)
{
    t_eobj *z = (t_eobj *)x->p_owner;
    z->o_current_proxy = x->p_index;
	pd_typedmess((t_pd *)x->p_owner, s, argc, argv);
    z->o_current_proxy = 0;
}

//! Initialize the proxy inlet classe
/*
 * @memberof    eobj
 * @param c     The proxy inlet classe
 * @return      This function return nothing
 */
void eproxy_setup(t_eclass* c)
{
    char buffer[MAXPDSTRING];
    sprintf(buffer, "eproxy%s", c->c_class.c_name->s_name);
	eproxy_class = class_new(gensym(buffer),(t_newmethod)eproxy_new, (t_method)NULL, sizeof(t_eproxy), 0, A_GIMME, 0);
	class_addanything(eproxy_class, (t_method)eproxy_anything);
}

//! @endcond







