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

#include "eobj.h"

//! Initialize an ebox
/*
 \ @memberof    ebox
 \ @param x     The ebox pointer
 \ @param flag  The flags to set the ebox behavior
 \ @return      Nothing
*/
void ebox_new(t_ebox *x, long flags)
{
    char buffer[MAXPDSTRING];
    
    x->b_flags = flags;
    sprintf(buffer,"#%s%lx", class_getname(x->b_obj.o_obj.te_g.g_pd), (long unsigned int)x);
    x->b_object_id = gensym(buffer);
    
    pd_bind(&x->b_obj.o_obj.ob_pd, x->b_object_id);
   
    x->b_ready_to_draw      = 0;
    x->b_number_of_layers   = 0;
    x->b_layers             = NULL;
    x->b_editor_id          = NULL;
    x->b_objuser_id         = gensym("(null)");

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
    t_eclass* c = (t_eclass *)x->b_obj.o_obj.te_g.g_pd;
    
    ebox_get_mouse_global_position(x);
    
    x->b_selected_item  = EITEM_NONE;
    x->b_selected_box   = 0;
    x->b_selected_inlet = -1;
    x->b_selected_outlet= -1;
    x->b_mouse_down     = 0;
    
    x->b_boxparameters.d_bordercolor = rgba_black;
    x->b_boxparameters.d_borderthickness = 1;
    x->b_boxparameters.d_boxfillcolor = rgba_white;
    x->b_boxparameters.d_cornersize = 0;
    c->c_widget.w_getdrawparameters(x, NULL, &x->b_boxparameters);
    x->b_ready_to_draw = 1;
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
    pd_unbind(&x->b_obj.o_obj.ob_pd, x->b_object_id);
	if(x->b_objuser_id != gensym("(null)"))
		pd_unbind(&x->b_obj.o_obj.ob_pd, x->b_objuser_id);
	erouter_detach((t_object *)x);
}











