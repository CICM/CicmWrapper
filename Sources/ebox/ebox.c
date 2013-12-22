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

#include "ebox.h"

//! Allocate the memory for an intance of an ebox that contains a specific eclass
/*
 \ @memberof    ebox
 \ @param c     The eclass pointer
 \ @return      This function return a new instance of an ebox
*/
void *ebox_alloc(t_eclass *c)
{
    t_pd *x;
    t_ebox* z;
    if(!c)
        bug("pd_new: apparently called before setup routine");
    x = (t_pd *)t_getbytes(c->c_class.c_size);
    *x = (t_pd)c;
    
    if (c->c_class.c_patchable)
    {
        ((t_object *)x)->ob_inlet = 0;
        ((t_object *)x)->ob_outlet = 0;
    }
    z = (t_ebox *)x;
    z->e_nproxy = 0;
    
    return (x);
}

//! Initialize an UI ebox
/*
 \ @memberof    ebox
 \ @param x     The ebox pointer
 \ @param flag  The flags to set the ebox behavior
 \ @return      Nothing
*/
void ebox_new(t_ebox *x, long flags)
{
    char buffer[MAXPDSTRING];
    
    x->e_flags = flags;
    sprintf(buffer,"#%s%lx", class_getname(x->e_obj.te_g.g_pd), (long unsigned int)x);
    x->e_object_id = gensym(buffer);
    
    pd_bind(&x->e_obj.ob_pd, x->e_object_id);
   
    x->e_ready_to_draw      = 0;
    x->z_misc               = 1;
    x->e_number_of_layers   = 0;
    x->e_layers             = NULL;
    x->e_editor_id          = NULL;
    x->e_objuser_id         = gensym("(null)");

    pd_bind(&x->e_obj.ob_pd, x->e_objuser_id);
    default_attr_process(x);
}

//! Indicate that an UI ebox is ready
/*
 \ @memberof    ebox
 \ @param x     The ebox pointer
 \ @return      Nothing
*/
void ebox_ready(t_ebox *x)
{
    t_eclass* c = (t_eclass *)x->e_obj.te_g.g_pd;
    
    ebox_get_mouse_global_position(x);
    
    x->e_selected_item  = EITEM_NONE;
    x->e_selected_box   = 0;
    x->e_selected_inlet = -1;
    x->e_selected_outlet= -1;
    x->e_mouse_down     = 0;
    
    x->e_boxparameters.d_bordercolor = rgba_black;
    x->e_boxparameters.d_borderthickness = 1;
    x->e_boxparameters.d_boxfillcolor = rgba_white;
    x->e_boxparameters.d_cornersize = 0;
    c->c_widget.w_getdrawparameters(x, NULL, &x->e_boxparameters);
    x->e_ready_to_draw = 1;
}

void eproxy_init(t_eproxy *x, t_ebox *owner, int index)
{
	x->p_owner = (t_object *)owner;
	x->p_pd    = eproxy_class;
    x->p_index = index;
}

void *eproxy_new(t_symbol *s, int argc, t_atom *argv)
{
	t_eproxy *x = NULL;
    x = (t_eproxy *)pd_new(eproxy_class);
    return x;
}

void eproxy_anything(t_eproxy *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ebox *z = (t_ebox *)x->p_owner;
    z->e_current_proxy = x->p_index;
	pd_typedmess((t_pd *)x->p_owner, s, argc, argv);
    z->e_current_proxy = 0;
}

void eproxy_setup(t_eclass* c)
{
    char buffer[MAXPDSTRING];
    sprintf(buffer, "eproxy%s", c->c_class.c_name->s_name);
	eproxy_class = class_new(gensym(buffer),(t_newmethod)eproxy_new, (t_method)NULL, sizeof(t_eproxy), 0, A_GIMME, 0);
	class_addanything(eproxy_class, (t_method)eproxy_anything);
}

//! Add a proxy inlet
/*
 \ @memberof    ebox
 \ @param x     The ebox pointer
 \ @return      Nothing
 */
void ebox_inletnew(t_ebox* x)
{
    eproxy_init(&x->e_proxy[x->e_nproxy], x, x->e_nproxy+1);
    inlet_new(&x->e_obj, &x->e_proxy[x->e_nproxy].p_pd, NULL, NULL);
    x->e_nproxy++;
}

int ebox_getproxy(t_ebox* x)
{
    return x->e_current_proxy;
}

//! Free an UI ebox
/*
 \ @memberof    ebox
 \ @param x     The ebox pointer
 \ @return      Nothing
*/
void ebox_free(t_ebox* x)
{
    gfxstub_deleteforkey(x);
    erouter_detach((t_object *)x);
    pd_unbind(&x->e_obj.ob_pd, x->e_object_id);
    pd_unbind(&x->e_obj.ob_pd, x->e_objuser_id);
}

//! Return if the ebox is an UI object or not
/*
 \ @memberof    ebox
 \ @param x     The ebox pointer
 \ @return      This function return true the object is a box and flase if the object is an UI
*/
char ebox_getregister(t_ebox *x)
{
    t_eclass* c = (t_eclass *)x->e_obj.te_g.g_pd;
    return c->c_box;
}










