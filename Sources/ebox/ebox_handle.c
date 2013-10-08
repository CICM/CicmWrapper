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


void ebox_mouse_enter(t_ebox* x)
{
    t_eclass *c = (t_eclass *)x->e_obj.te_g.g_pd;
    
    if(c->c_widget.w_mouseenter)
        c->c_widget.w_mouseenter(x);
}

void ebox_mouse_leave(t_ebox* x)
{
    t_eclass *c = (t_eclass *)x->e_obj.te_g.g_pd;
    
    if(c->c_widget.w_mouseleave)
        c->c_widget.w_mouseleave(x);
}

void ebox_mouse_move(t_ebox* x, float x_p, float y_p, float key)
{
    t_eclass *c = (t_eclass *)x->e_obj.te_g.g_pd;
    if(!x->e_mouse_down)
    {
        if(!x->e_canvas->gl_edit)
        {
            x->e_mouse.x = x_p;
            x->e_mouse.y = y_p;
            
            if(c->c_widget.w_mousemove)
                c->c_widget.w_mousemove(x, x->e_canvas, x->e_mouse, (long)key);
        }
    }
    else
    {
        ebox_mouse_drag(x, x_p, y_p, key);
    }
}

void ebox_mouse_down(t_ebox* x, float x_p, float y_p, float key)
{
    t_eclass *c = (t_eclass *)x->e_obj.te_g.g_pd;
    if(!x->e_canvas->gl_edit)
    {
        x->e_mouse.x = x_p;
        x->e_mouse.y = y_p;
        if(c->c_widget.w_mousedown)
            c->c_widget.w_mousedown(x, x->e_canvas, x->e_mouse, (long)key);
        x->e_mouse_down = 1;
    }
}

void ebox_mouse_up(t_ebox* x, float x_p, float y_p, float key)
{
    t_eclass *c = (t_eclass *)x->e_obj.te_g.g_pd;
    if(!x->e_canvas->gl_edit)
    {
        x->e_mouse.x = x_p;
        x->e_mouse.y = y_p;
        if(c->c_widget.w_mouseup)
            c->c_widget.w_mouseup(x, x->e_canvas, x->e_mouse, (long)key);
        x->e_mouse_down = 0;
    }
}

void ebox_mouse_drag(t_ebox* x, float x_p, float y_p, float key)
{
    t_eclass *c = (t_eclass *)x->e_obj.te_g.g_pd;
    
    if(!x->e_canvas->gl_edit)
    {
        x->e_mouse.x = x_p;
        x->e_mouse.y = y_p;
        if(c->c_widget.w_mousedrag && x->e_mouse_down)
            c->c_widget.w_mousedrag(x, x->e_canvas, x->e_mouse, (long)key);
    }
}
