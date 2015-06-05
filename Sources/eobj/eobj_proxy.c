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
t_symbol* eproxy1572_sym;

void inlet_wrong(t_inlet *x, t_symbol *s)
{
    pd_error(x->i_owner, "inlet: expected '%s' but got '%s'",
             x->i_symfrom->s_name, s->s_name);
}

void new_inlet_bang(t_inlet *x)
{
    if (x->i_symfrom == &s_bang)
        pd_vmess(x->i_dest, x->i_un.iu_symto, "");
    else if (!x->i_symfrom)
        pd_bang(x->i_dest);
    else if (x->i_symfrom == &s_list)
        new_inlet_list(x, &s_bang, 0, 0);
    else if (*x->i_dest == eproxy_class)
    {
        t_eproxy* proxy = (t_eproxy *)x->i_dest;
        t_eobj *z = (t_eobj *)proxy->p_owner;
        z->o_current_proxy = proxy->p_index;
        pd_typedmess((t_pd *)proxy->p_owner, &s_bang, 0, NULL);
        z->o_current_proxy = 0;
    }
    else inlet_wrong(x, &s_bang);
}

void new_inlet_pointer(t_inlet *x, t_gpointer *gp)
{
    if (x->i_symfrom == &s_pointer)
        pd_vmess(x->i_dest, x->i_un.iu_symto, "p", gp);
    else if (!x->i_symfrom)
        pd_pointer(x->i_dest, gp);
    else if (x->i_symfrom == &s_list)
    {
        t_atom a;
        SETPOINTER(&a, gp);
        new_inlet_list(x, &s_pointer, 1, &a);
    }
    else if (*x->i_dest == eproxy_class)
    {
        t_atom a;
        t_eproxy* proxy = (t_eproxy *)x->i_dest;
        t_eobj *z = (t_eobj *)proxy->p_owner;
        z->o_current_proxy = proxy->p_index;
		SETPOINTER(&a, gp);
        pd_typedmess((t_pd *)x->i_dest, &s_pointer, 1, &a);
        z->o_current_proxy = 0;
    }
    else inlet_wrong(x, &s_pointer);
}

void new_inlet_float(t_inlet *x, t_float f)
{
    if (x->i_symfrom == &s_float)
        pd_vmess(x->i_dest, x->i_un.iu_symto, "f", (t_floatarg)f);
    else if (*x->i_dest == eproxy_class)
    {
        t_atom a;
        t_eproxy* proxy = (t_eproxy *)x->i_dest;
        t_eobj *z = (t_eobj *)proxy->p_owner;
        z->o_current_proxy = proxy->p_index;
		SETFLOAT(&a, f);
        pd_typedmess((t_pd *)x->i_dest, &s_float, 1, &a);
        z->o_current_proxy = 0;
    }
    else if (x->i_symfrom == &s_signal)
        x->i_un.iu_floatsignalvalue = f;
    else if (!x->i_symfrom)
        pd_float(x->i_dest, f);
    else if (x->i_symfrom == &s_list)
    {
        t_atom a;
        SETFLOAT(&a, f);
        new_inlet_list(x, &s_float, 1, &a);
    }
    else inlet_wrong(x, &s_float);
}

void new_inlet_symbol(t_inlet *x, t_symbol *s)
{
    if (x->i_symfrom == &s_symbol)
        pd_vmess(x->i_dest, x->i_un.iu_symto, "s", s);
    else if (!x->i_symfrom)
        pd_symbol(x->i_dest, s);
    else if (x->i_symfrom == &s_list)
    {
        t_atom a;
        SETSYMBOL(&a, s);
        new_inlet_list(x, &s_symbol, 1, &a);
    }
    else if (*x->i_dest == eproxy_class)
    {
        t_atom a;
        t_eproxy* proxy = (t_eproxy *)x->i_dest;
        t_eobj *z = (t_eobj *)proxy->p_owner;
        z->o_current_proxy = proxy->p_index;
		SETSYMBOL(&a, s);
        pd_typedmess((t_pd *)x->i_dest, &s_symbol, 1, &a);
        z->o_current_proxy = 0;
    }
    else inlet_wrong(x, &s_symbol);
}

void new_inlet_list(t_inlet *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->i_symfrom == &s_list || x->i_symfrom == &s_float || x->i_symfrom == &s_symbol || x->i_symfrom == &s_pointer)
        typedmess(x->i_dest, x->i_un.iu_symto, argc, argv);
    else if (*x->i_dest == eproxy_class)
    {
        t_eproxy* proxy = (t_eproxy *)x->i_dest;
        t_eobj *z = (t_eobj *)proxy->p_owner;
        z->o_current_proxy = proxy->p_index;
        pd_typedmess((t_pd *)x->i_dest, s, argc, argv);
        z->o_current_proxy = 0;
    }
    else if (!x->i_symfrom)
    {
        typedmess(x->i_dest, s, argc, argv);
        //pd_list(x->i_dest, s, argc, argv);
    }
    else if (!argc)
        new_inlet_bang(x);
    else if (argc==1 && argv->a_type == A_FLOAT)
        new_inlet_float(x, atom_getfloat(argv));
    else if (argc==1 && argv->a_type == A_SYMBOL)
        new_inlet_symbol(x, atom_getsymbol(argv));
    else inlet_wrong(x, &s_list);
    
}

void new_inlet_anything(t_inlet *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->i_symfrom == s)
        typedmess(x->i_dest, x->i_un.iu_symto, argc, argv);
    else if (!x->i_symfrom)
        typedmess(x->i_dest, s, argc, argv);
    else if (*x->i_dest == eproxy_class)
    {
        t_eproxy* proxy = (t_eproxy *)x->i_dest;
        t_eobj *z = (t_eobj *)proxy->p_owner;
        z->o_current_proxy = proxy->p_index;
        pd_typedmess((t_pd *)x->i_dest, s, argc, argv);
        z->o_current_proxy = 0;
    }
    else inlet_wrong(x, s);
}

//! Intialize a proxy inlet
/*
 * @memberof    eobj
 * @param owner The eobj pointer
 * @return      This function return the proxy pointor
 */
t_eproxy* eproxy_new(void *owner)
{
    t_class* inlet_class;
    t_eobj *z = (t_eobj *)owner;
    eproxy1572_sym = s_eproxy1572;
    if(!eproxy1572_sym->s_thing)
        eproxy_setup();
    else
        eproxy_class = (t_class *)eproxy1572_sym->s_thing;
    if(z->o_proxy)
    {
        z->o_proxy = (t_eproxy *)realloc(z->o_proxy, (z->o_nproxy + 1) * sizeof(t_eproxy));
    }
    else
    {
        z->o_proxy = (t_eproxy *)malloc(sizeof(t_eproxy));
    }
    if(z->o_proxy)
    {
        z->o_proxy[z->o_nproxy].p_owner = (t_object *)owner;
        z->o_proxy[z->o_nproxy].p_pd    = eproxy_class;
        z->o_proxy[z->o_nproxy].p_index = z->o_nproxy;
        z->o_proxy[z->o_nproxy].p_inlet = inlet_new(&z->o_obj, &z->o_proxy[z->o_nproxy].p_pd, NULL, NULL);
        
        inlet_class = z->o_proxy[z->o_nproxy].p_inlet->i_pd;
        
        inlet_class->c_bangmethod = (t_bangmethod)new_inlet_bang;
        inlet_class->c_pointermethod = (t_pointermethod)new_inlet_pointer;
        inlet_class->c_floatmethod = (t_floatmethod)new_inlet_float;
        inlet_class->c_symbolmethod = (t_symbolmethod)new_inlet_symbol;
        inlet_class->c_listmethod = (t_listmethod)new_inlet_list;
        inlet_class->c_anymethod = (t_anymethod)new_inlet_anything;
        
        z->o_nproxy++;
        return &z->o_proxy[z->o_nproxy-1];
    }
    else
    {
        pd_error(z, "cons't allocate memory for a new proxy inlet.");
        return NULL;
    }
}

//! Intialize a signal proxy inlet
/*
 * @memberof    eobj
 * @param owner The eobj pointer
 * @param f     The float for signal
 * @return      his function return the proxy pointor
 */
t_eproxy* eproxy_signalnew(void *owner, float f)
{
    t_class* inlet_class;
    t_eobj *z = (t_eobj *)owner;
    eproxy1572_sym = s_eproxy1572;
    if(!eproxy1572_sym->s_thing)
        eproxy_setup();
    else
        eproxy_class = (t_class *)eproxy1572_sym->s_thing;
    
    if(z->o_proxy)
    {
        z->o_proxy = (t_eproxy *)realloc(z->o_proxy, (z->o_nproxy + 1) * sizeof(t_eproxy));
    }
    else
    {
        z->o_proxy = (t_eproxy *)malloc(sizeof(t_eproxy));
    }
    if(z->o_proxy)
    {
        z->o_proxy[z->o_nproxy].p_owner = (t_object *)owner;
        z->o_proxy[z->o_nproxy].p_pd    = eproxy_class;
        z->o_proxy[z->o_nproxy].p_index = z->o_nproxy;
        
        z->o_proxy[z->o_nproxy].p_inlet = inlet_new(&z->o_obj, &z->o_proxy[z->o_nproxy].p_pd, &s_signal, &s_signal);
        z->o_proxy[z->o_nproxy].p_inlet->i_un.iu_floatsignalvalue = f;
        
        inlet_class = z->o_proxy[z->o_nproxy].p_inlet->i_pd;
        
        inlet_class->c_bangmethod = (t_bangmethod)new_inlet_bang;
        inlet_class->c_pointermethod = (t_pointermethod)new_inlet_pointer;
        inlet_class->c_floatmethod = (t_floatmethod)new_inlet_float;
        inlet_class->c_symbolmethod = (t_symbolmethod)new_inlet_symbol;
        
        inlet_class->c_listmethod = (t_listmethod)new_inlet_list;
        inlet_class->c_anymethod = (t_anymethod)new_inlet_anything;
        
        z->o_nproxy++;
        
        return &z->o_proxy[z->o_nproxy-1];
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
void eproxy_free(t_eproxy* proxy)
{
    t_eobj *z   = (t_eobj *)proxy->p_owner;
    z->o_nproxy--;
    inlet_free(proxy->p_inlet);
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
void eproxy_setup()
{
    eproxy1572_sym = s_eproxy1572;
    
	eproxy_class = class_new(gensym("eproxy"), NULL, (t_method)NULL, sizeof(t_eproxy), CLASS_PD, A_GIMME, 0);
    eproxy1572_sym->s_thing = (t_class **)eproxy_class;
	class_addanything(eproxy_class, (t_method)eproxy_anything);
    class_addbang(eproxy_class,  (t_method)eproxy_bang);
    class_addfloat(eproxy_class,  (t_method)eproxy_float);
    class_addsymbol(eproxy_class,  (t_method)eproxy_symbol);
    class_addlist(eproxy_class, (t_method)eproxy_list);
}

//! @endcond







