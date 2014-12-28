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

#include "eclass.h"

void ewidget_init(t_eclass* c)
{
    c->c_widget.w_getrectfn         = ewidget_getrect;
    c->c_widget.w_visfn             = ewidget_vis;
    c->c_widget.w_displacefn        = ewidget_displace;
    c->c_widget.w_selectfn          = ewidget_select;
    c->c_widget.w_activatefn        = NULL;
    c->c_widget.w_deletefn          = ewidget_delete;
    c->c_widget.w_clickfn           = NULL;
    
    c->c_widget.w_paint             = NULL;
    c->c_widget.w_mouseenter        = NULL;
    c->c_widget.w_mouseleave        = NULL;
    c->c_widget.w_mousemove         = NULL;
    c->c_widget.w_mousedown         = NULL;
    c->c_widget.w_mousedrag         = NULL;
    c->c_widget.w_mouseup           = NULL;
    c->c_widget.w_mousewheel        = NULL;
    c->c_widget.w_dblclick          = NULL;
    c->c_widget.w_key               = NULL;
    c->c_widget.w_keyfilter         = NULL;
    c->c_widget.w_deserted          = NULL;
    c->c_widget.w_getdrawparameters = NULL;
    c->c_widget.w_notify            = NULL;
    c->c_widget.w_save              = NULL;
    c->c_widget.w_dosave            = NULL;
    c->c_widget.w_popup             = NULL;
    c->c_widget.w_dsp               = NULL;
    c->c_widget.w_oksize            = NULL;
    c->c_widget.w_write             = NULL;
    c->c_widget.w_read              = NULL;
    c->c_widget.w_notify_juce       = NULL;
    c->c_widget.w_paint_juce        = NULL;
}

void ewidget_getrect(t_gobj *z, t_glist *glist, int *xp1, int *yp1, int *xp2, int *yp2)
{
    t_ebox *x = (t_ebox *)z;
    *xp1 = text_xpix(&x->b_obj.o_obj, glist);
    *yp1 = text_ypix(&x->b_obj.o_obj, glist) - (int)(x->b_boxparameters.d_borderthickness);
    *xp2 = text_xpix(&x->b_obj.o_obj, glist) + (int)x->b_rect.width + (int)(x->b_boxparameters.d_borderthickness);
    *yp2 = text_ypix(&x->b_obj.o_obj, glist) + (int)x->b_rect.height + (int)(x->b_boxparameters.d_borderthickness);
}

void ewidget_vis(t_gobj *z, t_glist *glist, int vis)
{
    t_ebox* x   = (t_ebox *)z;
    t_eclass* c = eobj_getclass(x);
    if(vis)
    {
        if(eobj_isbox(x))
        {
            if(x->b_ready_to_draw && x->b_visible)
            {
                ebox_create_window(x, glist);
                ebox_invalidate_all(x);
                // No redraw for the 1st paint
                
                if(x->b_obj.o_canvas && x->b_ready_to_draw && eobj_isbox(x)) // do not call ebox_isdrawable !!
                {
                    ebox_invalidate_layer(x, gensym("eboxbd"));
                    ebox_invalidate_layer(x, gensym("eboxio"));
                    
                    ebox_update(x);
                    if(c->c_widget.w_paint)
                        c->c_widget.w_paint(x, (t_object *)x->b_obj.o_canvas);
                    ebox_draw_border(x);
                    if(x->b_obj.o_canvas->gl_edit)
                        ebox_draw_iolets(x);
                }
            }
        }
    }
    else
    {
        eobj_nopoll_mouse(x);
        ebox_erase(x);
    }
    canvas_fixlinesfor(glist_getcanvas(glist), (t_text*)x);
}

void ewidget_displace(t_gobj *z, t_glist *glist, int dx, int dy)
{
	t_ebox *x;
#ifdef _WINDOWS
	t_gotfn m;
    
#endif
	x = (t_ebox *)z;
    
    x->b_rect.x += dx;
    x->b_rect.y += dy;
    x->b_obj.o_obj.te_xpix += dx;
    x->b_obj.o_obj.te_ypix += dy;
    
    ebox_move(x);

#ifdef _WINDOWS
	if(!x->b_selected_box)
	{
		m = getfn((t_pd *)x->b_obj.o_canvas, gensym("setbounds"));
		m(x->b_obj.o_canvas, glist->gl_screenx1, glist->gl_screeny1, glist->gl_screenx2, glist->gl_screeny2);
	}
#endif
}

void ewidget_select(t_gobj *z, t_glist *glist, int selected)
{
    t_ebox *x = (t_ebox *)z;
    if(selected)
        x->b_selected_box = 1;
    else
        x->b_selected_box = 0;
    ebox_select(x);
}

void ewidget_delete(t_gobj *z, t_glist *glist)
{
    t_ebox *x = (t_ebox *)z;
    eobj_nopoll_mouse(x);
    ebox_erase(x);
    canvas_deletelinesfor(glist, (t_text *)z);
}






