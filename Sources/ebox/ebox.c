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
    if(!c)
        bug("pd_new: apparently called before setup routine");
    x = (t_pd *)t_getbytes(c->c_class.c_size);
    *x = (t_pd)c;
    
    if (c->c_class.c_patchable)
    {
        ((t_object *)x)->ob_inlet = 0;
        ((t_object *)x)->ob_outlet = 0;
    }
    
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
    x->e_deserted_time      = 3000.;
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
    x->e_deserted_clock = clock_new(x, (t_method)ebox_deserted);
    x->e_ready_to_draw = 1;
}

//! Free an UI ebox
/*
 \ @memberof    ebox
 \ @param x     The ebox pointer
 \ @return      Nothing
*/
void ebox_free(t_ebox* x)
{
    ebox_router(x, gensym("detach"), 0, NULL);
    clock_free(x->e_deserted_clock);
    pd_unbind(&x->e_obj.ob_pd, x->e_object_id);
    gfxstub_deleteforkey(x);
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










