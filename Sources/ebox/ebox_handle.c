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

static t_pt mouse_global_pos;
static int ebox_check_global_move = 0;

static char *cursorlist[] = {
    "$cursor_runmode_nothing",
    "$cursor_runmode_clickme",
    "$cursor_runmode_thicken",
    "$cursor_runmode_addpoint",
    "$cursor_editmode_nothing",
    "$cursor_editmode_connect",
    "$cursor_editmode_disconnect"
};


void ebox_set_mouse_global_position(t_ebox* x, float x_p, float y_p)
{
    mouse_global_pos.x = x_p;
    mouse_global_pos.y = y_p;
}

t_pt ebox_get_mouse_global_position(t_ebox* x)
{
    t_pt point;
    sys_vgui("global_mousepos %s\n", x->e_name_rcv->s_name);
    point = mouse_global_pos;
    return point;
}

t_pt ebox_get_mouse_canvas_position(t_ebox* x)
{
    t_pt point;
    sys_vgui("global_mousepos %s\n", x->e_name_rcv->s_name);
    point.x = mouse_global_pos.x - x->e_canvas->gl_screenx1;
    point.y = mouse_global_pos.y - x->e_canvas->gl_screeny1;
    
    return point;
}

void ebox_set_cursor(t_ebox* x, int mode)
{
    mode = pd_clip_minmax(mode, 0, 6);
    sys_vgui("%s configure -cursor %s\n", x->e_drawing_id->s_name, cursorlist[mode]);
}

void ebox_mouse_enter(t_ebox* x)
{
    t_eclass *c = (t_eclass *)x->e_obj.te_g.g_pd;

    if(!x->e_canvas->gl_edit)
    {
        if(c->c_widget.w_mouseenter && !x->e_mouse_down)
        {
            ebox_set_cursor(x, 1);
            c->c_widget.w_mouseenter(x);
        }
    }
    else
    {
        ebox_set_cursor(x, 4);
    }
}

void ebox_mouse_leave(t_ebox* x)
{
    t_eclass *c = (t_eclass *)x->e_obj.te_g.g_pd;
    
    if(!x->e_canvas->gl_edit)
    {
        if(c->c_widget.w_mouseleave && !x->e_mouse_down)
        {
            ebox_set_cursor(x, 0);
            c->c_widget.w_mouseleave(x);
            clock_delay(x->e_deserted_clock, x->e_deserted_time);
        }
    }
    else
        ebox_set_cursor(x, 4);
}

void ebox_mouse_move(t_ebox* x, float x_p, float y_p, float key)
{
    t_pt point;
    t_selection *y;
    t_class *cl;
    int resortin = 0, resortout = 0;
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
        else
            ebox_set_cursor(x, 4);
    }
    else
    {
        if(!x->e_canvas->gl_edit)
        {
            ebox_mouse_drag(x, x_p, y_p, key);
        }
        else
        {
            // stupid error somewhere .... sorry
            if(ebox_check_global_move == 0)
            {
                x->e_move_box = ebox_get_mouse_canvas_position(x);
                ebox_check_global_move = 1;
            }
            point = ebox_get_mouse_canvas_position(x);
            // ... and I do evething : so stupid !
            for (y = x->e_canvas->gl_editor->e_selection; y; y = y->sel_next)
            {
                cl = pd_class(&y->sel_what->g_pd);
                gobj_displace(y->sel_what, x->e_canvas, point.x - x->e_move_box.x, point.y - x->e_move_box.y);
                if (cl == vinlet_class) resortin = 1;
                else if (cl == voutlet_class) resortout = 1;
            }
            if (resortin) canvas_resortinlets(x->e_canvas);
            if (resortout) canvas_resortoutlets(x->e_canvas);
            sys_vgui("pdtk_canvas_getscroll .x%lx.c\n", x->e_canvas);
            if (x->e_canvas->gl_editor->e_selection)
                canvas_dirty(x->e_canvas, 1);
         
            x->e_move_box = point;
        }
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
    }
    else
    {
        ebox_check_global_move = 0;
        x->e_move_box = ebox_get_mouse_canvas_position(x);
        t_gotfn selmethod = getfn((t_pd *)x->e_canvas, gensym("mouse"));
        selmethod(x->e_canvas, gensym("mouse"), x->e_move_box.x, x->e_move_box.y, 0, 0);
    }
    x->e_mouse_down = 1;
}

void ebox_mouse_up(t_ebox* x, float x_p, float y_p, float key)
{
    t_pt point;
    t_eclass *c = (t_eclass *)x->e_obj.te_g.g_pd;
    if(!x->e_canvas->gl_edit)
    {
        point = ebox_get_mouse_canvas_position(x);
        x->e_mouse.x = point.x;
        x->e_mouse.y = point.y;
        if(c->c_widget.w_mouseup)
            c->c_widget.w_mouseup(x, x->e_canvas, x->e_mouse, (long)key);
    }
    else
    {
        t_gotfn selmethod = getfn((t_pd *)x->e_canvas, gensym("mouseup"));
        selmethod(x->e_canvas, gensym("mouseup"), x_p + x->e_obj.te_xpix , y_p + x->e_obj.te_ypix, 0, 0);
    }
    x->e_mouse_down = 0;
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

void ebox_mouse_dblclick(t_ebox* x, float x_p, float y_p)
{
    t_eclass *c = (t_eclass *)x->e_obj.te_g.g_pd;
    
    if(!x->e_canvas->gl_edit)
    {
        x->e_mouse.x = x_p;
        x->e_mouse.y = y_p;
        if(c->c_widget.w_dblclick)
            c->c_widget.w_dblclick(x, x->e_canvas, x->e_mouse);
    }
}

void ebox_mouse_rightclick(t_ebox* x, float x_p, float y_p)
{
    
}

void ebox_deserted(t_ebox *x)
{
    t_eclass* c = (t_eclass *)x->e_obj.te_g.g_pd;
    if(c->c_widget.w_deserted)
        c->c_widget.w_deserted(x);
    clock_unset(x->e_deserted_clock);
}

