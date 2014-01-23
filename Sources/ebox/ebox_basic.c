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

#include "ebox.h"

//! Initialize an ebox
/*
 \ @memberof    ebox
 \ @param x     The ebox pointer
 \ @param flag  The flags to set the ebox behavior
 \ @return      Nothing
*/
void ebox_new(t_ebox *x, long flags)
{
    x->b_flags = flags;
    x->b_ready_to_draw      = 0;
    x->b_have_window        = 0;
    x->b_number_of_layers   = 0;
    x->b_layers             = NULL;
    x->b_editor_id          = NULL;
    x->b_receive_id         = gensym("(null)");
    x->b_send_id            = gensym("(null)");
    x->b_objpreset_id       = gensym("(null)");
    x->b_visible            = 1;
    eobj_getclass(x)->c_widget.w_dosave = (method)ebox_dosave;
    ebox_attrprocess_default(x);
}

//! Indicate that an ebox is ready
/*
 \ @memberof    ebox
 \ @param x     The ebox pointer
 \ @return      Nothing
*/
void ebox_ready(t_ebox *x)
{
    t_eclass* c = (t_eclass *)x->b_obj.o_obj.te_g.g_pd;
    
    eobj_get_mouse_global_position((t_eobj *)x);
    
    x->b_selected_item  = EITEM_NONE;
    x->b_selected_box   = 0;
    x->b_selected_inlet = -1;
    x->b_selected_outlet= -1;
    x->b_mouse_down     = 0;
    
    x->b_boxparameters.d_bordercolor = rgba_black;
    x->b_boxparameters.d_borderthickness = 1;
    x->b_boxparameters.d_boxfillcolor = rgba_white;
    x->b_boxparameters.d_cornersize = 0;
    if(c->c_widget.w_getdrawparameters)
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
	if(x->b_receive_id != gensym("(null)"))
		pd_unbind(&x->b_obj.o_obj.ob_pd, x->b_receive_id);
	eobj_detach_torouter((t_object *)x);
    if(eobj_isdsp(x))
        eobj_dspfree(x);
    eobj_free(x);
}

//! Retrieve the font name of an ebox
/*
 \ @memberof        ebox
 \ @param x         The ebox
 \ @return          The font name
 */
t_symbol* ebox_getfontname(t_ebox* x)
{
    return x->b_font.c_family;
}

//! Retrieve the font slant of an ebox
/*
 \ @memberof        ebox
 \ @param x         The ebox
 \ @return          The font slant
 */
t_symbol* ebox_getfontslant(t_ebox* x)
{
    return x->b_font.c_slant;
}

//! Retrieve the font weight of an ebox
/*
 \ @memberof        ebox
 \ @param x         The ebox
 \ @return          The font weight
 */
t_symbol* ebox_getfontweight(t_ebox* x)
{
    return x->b_font.c_weight;
}

//! Retrieve the font size of an ebox
/*
 \ @memberof        ebox
 \ @param x         The ebox
 \ @return          The font size
 */
float ebox_getfontsize(t_ebox* x)
{
    return x->b_font.c_size;
}

//! Retrieve the sender object
/*
 \ @memberof        ebox
 \ @param x         The ebox
 \ @return          The font size
 */
t_pd* ebox_getsender(t_ebox* x)
{
    if(x->b_send_id != gensym("(null)") && x->b_send_id->s_thing)
    {
        return x->b_send_id->s_thing;
    }
    return NULL;
}

//! Retrieve if an ebox is drawable
/*
 \ @memberof        ebox
 \ @param x         The ebox
 \ @return          This function return 1 is the ebox is drawable and 0 if not
 */
char ebox_isdrawable(t_ebox* x)
{
    if(eobj_isbox(x) && x->b_obj.o_canvas)
    {
        if(x->b_ready_to_draw && glist_isvisible(x->b_obj.o_canvas))
            return 1;
    }
    return 0;
}

//! Change the cursor
/*
 \ @memberof        ebox
 \ @param x         The ebox pointer
 \ @param mode      The index of the cursor list
 \ @return          Nothing
 */
void ebox_set_cursor(t_ebox* x, int mode)
{
    mode = pd_clip_minmax(mode, 0, 12);
    sys_vgui("%s configure -cursor %s\n", x->b_drawing_id->s_name, my_cursorlist[mode]);
}









