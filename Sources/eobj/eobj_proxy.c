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



//! Intialize a proxy inlet
/*
 * @memberof    eobj
 * @param owner The eobj pointer
 * @return      This function return the proxy pointor
 */
t_eproxy* eproxy_new(void *owner, t_symbol* s)
{
    t_eproxy* proxy;
    t_eobj *z = (t_eobj *)owner;
    t_class *eproxy_class = eproxy_setup();
    if(z->o_proxy)
    {
        z->o_proxy = (t_eproxy **)realloc(z->o_proxy, (z->o_nproxy + 1) * sizeof(t_eproxy));
    }
    else
    {
        z->o_proxy = (t_eproxy **)malloc(1 * sizeof(t_eproxy));
    }
    if(z->o_proxy)
    {
        proxy = (t_eproxy *)pd_new(eproxy_class);
        proxy->p_owner = (t_object *)owner;
        proxy->p_pd    = eproxy_class;
        proxy->p_index = z->o_nproxy;
        proxy->p_inlet = inlet_new((t_object *)owner, (t_pd *)proxy, s, s);
        
        z->o_proxy[z->o_nproxy] = proxy;
        z->o_nproxy++;
        return proxy;
    }
    else
    {
        pd_error(z, "cons't allocate memory for a new proxy inlet.");
        return NULL;
    }
}

//! Free a proxy inlet
/*
 * @memberof    eobj
 * @param x     The proxy pointer
 * @param owner The eobj pointer
 * @return      Nothing
 */
void eproxy_free(void *owner, t_eproxy* proxy)
{
    t_eobj *z   = (t_eobj *)owner;
    if(z && proxy && proxy->p_owner == (t_object *)z)
    {
        if(z->o_nproxy == proxy->p_index + 1)
        {
            inlet_free(proxy->p_inlet);
            if(z->o_nproxy == 1)
            {
                free(z->o_proxy);
                z->o_nproxy = 0;
            }
            else
            {
                z->o_proxy = realloc(z->o_proxy, (z->o_nproxy - 1) * sizeof(t_eproxy));
                z->o_nproxy--;
            }
        }
        else
        {
            pd_error(owner, "can't free a proxy if it's not the last one.");
        }
    }
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
}


void eproxy_bang(t_eproxy *x)
{
    t_eobj *z = (t_eobj *)x->p_owner;
    z->o_current_proxy = x->p_index;
    pd_bang((t_pd *)x->p_owner);
}

void eproxy_float(t_eproxy *x, float f)
{
    t_eobj *z = (t_eobj *)x->p_owner;
    z->o_current_proxy = x->p_index;
    pd_float((t_pd *)x->p_owner, f);
}

void eproxy_symbol(t_eproxy *x, t_symbol* s)
{
    t_eobj *z = (t_eobj *)x->p_owner;
    z->o_current_proxy = x->p_index;
    pd_symbol((t_pd *)x->p_owner, s);
}

void eproxy_list(t_eproxy *x, t_symbol* s, int argc, t_atom* argv)
{
    t_eobj *z = (t_eobj *)x->p_owner;
    z->o_current_proxy = x->p_index;
    pd_list((t_pd *)x->p_owner, s, argc, argv);
}

//! Initialize the proxy inlet classe
/*
 * @memberof    eobj
 * @param c     The proxy inlet classe
 * @return      This function return nothing
 */
t_class* eproxy_setup()
{
    t_class* eproxy_class;
    t_symbol* eproxy1572_sym = gensym("eproxy1572");
    if(!eproxy1572_sym->s_thing)
    {
        eproxy_class = class_new(gensym("eproxy"), NULL, (t_method)NULL, sizeof(t_eproxy), CLASS_PD, A_GIMME, 0);
        eproxy1572_sym->s_thing = (t_class **)eproxy_class;
        class_addanything(eproxy_class, (t_method)eproxy_anything);
        class_addbang(eproxy_class,  (t_method)eproxy_bang);
        class_addfloat(eproxy_class,  (t_method)eproxy_float);
        class_addsymbol(eproxy_class,  (t_method)eproxy_symbol);
        class_addlist(eproxy_class, (t_method)eproxy_list);
        return eproxy_class;
    }
    else
    {
        return (t_class *)eproxy1572_sym->s_thing;
    }
}

//! @endcond







