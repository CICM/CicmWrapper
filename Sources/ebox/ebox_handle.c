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
static t_pt mouse_patcher_pos;

static char *my_cursorlist[] = {
    "left_ptr",
    "center_ptr",
    "sb_v_double_arrow",
    "plus",
    "hand2",
    "circle",
    "X_cursor",
    "bottom_side",
    "bottom_right_corner",
    "right_side",
    "double_arrow",
    "exchange"
};


void ebox_set_mouse_global_position(t_ebox* x, float x_p, float y_p)
{
    mouse_global_pos.x = x_p;
    mouse_global_pos.y = y_p;
}

void ebox_set_mouse_patcher_position(t_ebox* x, float x_p, float y_p)
{
    mouse_patcher_pos.x = x_p;
    mouse_patcher_pos.y = y_p;
}

t_pt ebox_get_mouse_global_position(t_ebox* x)
{
    sys_vgui("global_mousepos %s\n", x->e_object_id->s_name);
    sys_vgui("global_mousepos %s\n", x->e_object_id->s_name);
    return mouse_global_pos;
}

t_pt ebox_get_mouse_canvas_position(t_ebox* x)
{
    t_pt point;
    ebox_get_mouse_global_position(x);
    sys_vgui("patcher_mousepos %s %s\n", x->e_object_id->s_name, x->e_canvas_id->s_name);
    sys_vgui("patcher_mousepos %s %s\n", x->e_object_id->s_name, x->e_canvas_id->s_name);
    point.x = mouse_global_pos.x - mouse_patcher_pos.x;
    point.y = mouse_global_pos.y - mouse_patcher_pos.y;
    return point;
}

void ebox_set_cursor(t_ebox* x, int mode)
{
    mode = pd_clip_minmax(mode, 0, 11);
    sys_vgui("%s configure -cursor %s\n", x->e_drawing_id->s_name, my_cursorlist[mode]);
}

void ebox_mouse_enter(t_ebox* x)
{
    t_eclass *c = (t_eclass *)x->e_obj.te_g.g_pd;
    
    if(!x->e_canvas->gl_edit && !x->e_mouse_down)
    {
        if(c->c_widget.w_mouseenter)
            c->c_widget.w_mouseenter(x);
    }
}

void ebox_mouse_leave(t_ebox* x)
{
    t_eclass *c = (t_eclass *)x->e_obj.te_g.g_pd;
    
    if(!x->e_canvas->gl_edit && !x->e_mouse_down)
    {
        if(c->c_widget.w_mouseleave)
            c->c_widget.w_mouseleave(x);
        clock_delay(x->e_deserted_clock, x->e_deserted_time);
        ebox_set_cursor(x, 0);
    }
    else if(x->e_canvas->gl_edit && !x->e_mouse_down)
    {
        ebox_set_cursor(x, 4);
    }
}

void ebox_mouse_move(t_ebox* x, t_symbol* s, long argc, t_atom* argv)
{
    t_atom av[2];
    t_eclass *c = (t_eclass *)x->e_obj.te_g.g_pd;
    ebox_get_mouse_global_position(x);
    
    x->e_modifiers = (long)atom_getfloat(argv+2);
#ifdef _WINDOWS
	
	if(x->e_modifiers >= 131080)
	{
		x->e_modifiers -= 131080;
		x->e_modifiers += EMOD_ALT;
	}
	else
		x->e_modifiers -= 8;
#endif

    if(!x->e_canvas->gl_edit)
    {
        if(!x->e_mouse_down)
        {
            ebox_set_cursor(x, 1);
            x->e_mouse.x = atom_getfloat(argv);
            x->e_mouse.y = atom_getfloat(argv+1);
            
            if(c->c_widget.w_mousemove)
                c->c_widget.w_mousemove(x, x->e_canvas, x->e_mouse, x->e_modifiers);
        }
        else
        {
            ebox_mouse_drag(x, s, argc, argv);
        }
    }
    else
    {
        if(!x->e_mouse_down)
        {
            ebox_mouse_move_editmode(x, atom_getfloat(argv), atom_getfloat(argv+1), x->e_modifiers);
        }
        else
        {
            x->e_mouse.x = atom_getfloat(argv);
            x->e_mouse.y = atom_getfloat(argv+1);
            if(x->e_selected_item == EITEM_NONE)
            {
                x->e_move_box = ebox_get_mouse_canvas_position(x);
                sys_vgui("pdtk_canvas_motion %s %i %i 0\n", x->e_canvas_id->s_name, (int)x->e_move_box.x, (int)x->e_move_box.y);
            }
            else
            {
                if(x->e_flags & EBOX_GROWNO)
                    return;
                else if(x->e_flags & EBOX_GROWLINK)
                {
                    if(x->e_selected_item == EITEM_BOTTOM)
                    {
                        atom_setfloat(av, x->e_rect_last.width + (x->e_mouse.y - x->e_rect_last.height));
                        atom_setfloat(av+1, x->e_mouse.y);
                    }
                    else if(x->e_selected_item == EITEM_RIGHT)
                    {
                        atom_setfloat(av, x->e_mouse.x);
                        atom_setfloat(av+1, x->e_rect_last.height + (x->e_mouse.x - x->e_rect_last.width));
                    }
                    else if(x->e_selected_item == EITEM_CORNER)
                    {
                        if(x->e_mouse.y > x->e_mouse.x)
                        {
                            atom_setfloat(av, x->e_mouse.y);
                            atom_setfloat(av+1, x->e_mouse.y);
                        }
                        else
                        {
                            atom_setfloat(av, x->e_mouse.x);
                            atom_setfloat(av+1, x->e_mouse.x);
                        }
                    }
                }
                else if (x->e_flags & EBOX_GROWINDI)
                {
                    if(x->e_selected_item == EITEM_BOTTOM)
                    {
                        atom_setfloat(av, x->e_rect_last.width);
                        atom_setfloat(av+1, x->e_mouse.y);
                    }
                    else if(x->e_selected_item == EITEM_RIGHT)
                    {
                        atom_setfloat(av, x->e_mouse.x);
                        atom_setfloat(av+1, x->e_rect_last.height);
                    }
                    else if(x->e_selected_item == EITEM_CORNER)
                    {
                        atom_setfloat(av, x->e_mouse.x);
                        atom_setfloat(av+1, x->e_mouse.y);
                    }
                }
                mess3((t_pd *)x, gensym("size"),  gensym("size"), 2, av);
            }
        }
    }
}

void ebox_mouse_drag(t_ebox* x, t_symbol* s, long argc, t_atom* argv)
{
    t_eclass *c = (t_eclass *)x->e_obj.te_g.g_pd;
    
    x->e_modifiers -= 256;

    if(!x->e_canvas->gl_edit)
    {
        x->e_mouse.x = atom_getfloat(argv);
        x->e_mouse.y = atom_getfloat(argv+1);
        if(c->c_widget.w_mousedrag && x->e_mouse_down)
            c->c_widget.w_mousedrag(x, x->e_canvas, x->e_mouse, x->e_modifiers);
    }
}

void ebox_mouse_down(t_ebox* x, t_symbol* s, long argc, t_atom* argv)
{
    t_eclass *c = (t_eclass *)x->e_obj.te_g.g_pd;
	
    x->e_modifiers = (long)atom_getfloat(argv+2);
#ifdef _WINDOWS
	
	if(x->e_modifiers >= 131080)
	{
		x->e_modifiers -= 131080;
		x->e_modifiers += EMOD_ALT;
	}
	else
		x->e_modifiers -= 8;
#endif

    if(!x->e_canvas->gl_edit)
    {
        x->e_mouse.x = atom_getfloat(argv);
        x->e_mouse.y = atom_getfloat(argv+1);
        if(c->c_widget.w_mousedown)
            c->c_widget.w_mousedown(x, x->e_canvas, x->e_mouse, x->e_modifiers);
    }
    else
    {
        x->e_mouse.x = atom_getfloat(argv);
        x->e_mouse.y = atom_getfloat(argv+1);
        if(x->e_selected_item == EITEM_NONE)
        {
            x->e_move_box = ebox_get_mouse_canvas_position(x);
            if(x->e_modifiers == EMOD_CMD)
                sys_vgui("pdtk_canvas_rightclick %s %i %i 0\n", x->e_canvas_id->s_name, (int)x->e_move_box.x, (int)x->e_move_box.y);
            else
                sys_vgui("pdtk_canvas_mouse %s %i %i 0 0\n", x->e_canvas_id->s_name, (int)(x->e_move_box.x), (int)(x->e_move_box.y));
        }
        else
        {
            x->e_rect_last = x->e_rect;
        }
    }
    x->e_mouse_down = 1;
}

void ebox_mouse_up(t_ebox* x, t_symbol* s, long argc, t_atom* argv)
{
    t_eclass *c = (t_eclass *)x->e_obj.te_g.g_pd;
   
    x->e_modifiers = (long)atom_getfloat(argv+2);
#ifdef _WINDOWS
	
	if(x->e_modifiers >= 131080)
	{
		x->e_modifiers -= 131080;
		x->e_modifiers += EMOD_ALT;
	}
	else
		x->e_modifiers -= 8;
#endif    
    if(!x->e_canvas->gl_edit)
    {
        x->e_mouse.x = atom_getfloat(argv);
        x->e_mouse.y = atom_getfloat(argv+1);
        if(c->c_widget.w_mouseup)
            c->c_widget.w_mouseup(x, x->e_canvas, x->e_mouse, x->e_modifiers);
    }
    else
    {
        x->e_move_box = ebox_get_mouse_canvas_position(x);
        sys_vgui("pdtk_canvas_mouseup %s %i %i 0\n", x->e_canvas_id->s_name, (int)x->e_move_box.x, (int)x->e_move_box.y);
        if(x->e_selected_inlet != -1 || x->e_selected_outlet != -1)
        {
            x->e_selected_inlet = -1;
            x->e_selected_outlet = -1;
            ebox_redraw(x);
        }
    }
    
    x->e_mouse_down = 0;
}

void ebox_mouse_dblclick(t_ebox* x, t_symbol* s, long argc, t_atom* argv)
{
    t_eclass *c = (t_eclass *)x->e_obj.te_g.g_pd;
    
    x->e_modifiers = (long)atom_getfloat(argv+2);
#ifdef _WINDOWS
	
	if(x->e_modifiers >= 131080)
	{
		x->e_modifiers -= 131080;
		x->e_modifiers += EMOD_ALT;
	}
	else
		x->e_modifiers -= 8;
#endif    
    if(!x->e_canvas->gl_edit)
    {
        x->e_mouse.x = atom_getfloat(argv);
        x->e_mouse.y = atom_getfloat(argv+1);
        if(c->c_widget.w_dblclick)
            c->c_widget.w_dblclick(x, x->e_canvas, x->e_mouse);
    }
}

void ebox_mouse_wheel(t_ebox* x, t_symbol* s, long argc, t_atom* argv)
{
    float delta;
    t_eclass* c = (t_eclass *)x->e_obj.te_g.g_pd;
    
    x->e_modifiers = (long)atom_getfloat(argv+3);
    delta = atom_getfloat(argv+2);

#ifdef _WINDOWS
	
	if(x->e_modifiers >= 131080)
	{
		x->e_modifiers -= 131080;
		x->e_modifiers += EMOD_ALT;
	}
	else
		x->e_modifiers -= 8;

	delta /= 120.;
#endif
    if(!x->e_canvas->gl_edit)
    {
        x->e_mouse.x = atom_getfloat(argv);
        x->e_mouse.y = atom_getfloat(argv+1);
        if(c->c_widget.w_mousewheel)
            c->c_widget.w_mousewheel(x, x->e_canvas, x->e_mouse, (long)x->e_modifiers, delta, delta);
    }
}

void ebox_keydown(t_ebox* x, t_symbol* s, long argc, t_atom* argv)
{
    //post("keydown");
    if(!x->e_canvas->gl_edit)
    {
        
    }
    else
    {
        ;
    }
    ebox_redraw(x);
}

void ebox_keyup(t_ebox* x, t_symbol* s, long argc, t_atom* argv)
{
    if(!x->e_canvas->gl_edit)
    {
        ;
    }
    else
    {
        ;
    }
}

void ebox_deserted(t_ebox *x)
{
    t_eclass* c = (t_eclass *)x->e_obj.te_g.g_pd;
    if(c->c_widget.w_deserted)
        c->c_widget.w_deserted(x);
    clock_unset(x->e_deserted_clock);
}

void ebox_mouse_move_editmode(t_ebox* x, float x_p, float y_p, float key)
{
    int i;
    int top, left, right, bottom;
    
    x->e_selected_outlet    = -1;
    x->e_selected_inlet     = -1;
    x->e_selected_item      = EITEM_NONE;
    x->e_move_box = ebox_get_mouse_canvas_position(x);
    sys_vgui("pdtk_canvas_motion %s %i %i 0\n", x->e_canvas_id->s_name, (int)x->e_move_box.x, (int)x->e_move_box.y);
    
    top     = 0;
    left    = 0;
    right   = x->e_rect.width + x->e_boxparameters.d_borderthickness * 2.;
    bottom  = x->e_rect.height + x->e_boxparameters.d_borderthickness * 2.;
    
    // TOP //
    if(y_p >= 0 && y_p < 3)
    {
        for(i = 0; i < obj_noutlets((t_object *)x); i++)
        {
            int pos_x_inlet = 0;
            if(obj_ninlets((t_object *)x) != 1)
                pos_x_inlet = (int)(i / (float)(obj_ninlets((t_object *)x) - 1) * (x->e_rect.width - 8));
            
            if(x_p >= pos_x_inlet && x_p <= pos_x_inlet +7)
            {
                //sys_vgui("pdtk_gettip .x%lx.c inlet %d [list %s] [list %s] [list %s]\n", x->e_canvas, i, g->g_pd->c_name->s_name, g->g_pd->c_helpname->s_name, g->g_pd->c_externdir);
                x->e_selected_inlet = i;
                ebox_set_cursor(x, 4);
                break;
            }
        }
        ebox_invalidate_layer((t_object *)x, NULL, gensym("eboxio"));
        ebox_redraw(x);
        return;
    }
    // BOTTOM & RIGHT //
    else if(y_p > bottom - 3 && y_p <= bottom && x_p > right - 3 && x_p <= right)
    {
        x->e_selected_item = EITEM_CORNER;
        ebox_set_cursor(x, 8);
        return;
    }
    // BOTTOM //
    else if(y_p > bottom - 3 && y_p < bottom)
    {
        for(i = 0; i < obj_noutlets((t_object *)x); i++)
        {
            int pos_x_outlet = 0;
            if(obj_noutlets((t_object *)x) != 1)
                pos_x_outlet = (int)(i / (float)(obj_noutlets((t_object *)x) - 1) * (x->e_rect.width - 8));
            
            if(x_p >= pos_x_outlet && x_p <= pos_x_outlet +7)
            {
                x->e_selected_outlet = i;
                ebox_set_cursor(x, 5);
                break;
            }
        }
        if(x->e_selected_outlet == -1)
        {
            x->e_selected_item = EITEM_BOTTOM;
            ebox_set_cursor(x, 7);
        }
        ebox_invalidate_layer((t_object *)x, NULL, gensym("eboxio"));
        ebox_redraw(x);
        return;
    }
    // RIGHT //
    else if(x_p > right - 3 && x_p <= right)
    {
        x->e_selected_item = EITEM_RIGHT;
        ebox_set_cursor(x, 9);
        return;
    }
    
    // BOX //
    ebox_set_cursor(x, 4);

    ebox_invalidate_layer((t_object *)x, NULL, gensym("eboxio"));
    ebox_redraw(x);
}

void ebox_popup(t_ebox* x, t_symbol* s, float itemid)
{
    t_eclass* c = (t_eclass *)x->e_obj.te_g.g_pd;

    if(c->c_widget.w_popup)
        c->c_widget.w_popup(x, s, (long)itemid);
}

void ebox_dosave_box(t_gobj* z, t_binbuf *b)
{
    t_ebox*   x = (t_ebox *)z;
    t_eclass* c = (t_eclass *)x->e_obj.te_g.g_pd;
    
    binbuf_addv(b, "ssii", gensym("#X"), gensym("obj"), (t_int)x->e_obj.te_xpix, (t_int)x->e_obj.te_ypix);
    binbuf_add(b, binbuf_getnatom(x->e_obj.te_binbuf), binbuf_getvec(x->e_obj.te_binbuf));
    

    if(c->c_widget.w_save != NULL)
        c->c_widget.w_save(x, b);
    
    binbuf_addv(b, ";");
}

void ebox_dosave_nobox(t_gobj* z, t_binbuf *b)
{
    int         i;
    char        attr_name[MAXPDSTRING];
    long        argc    = 0;
    t_atom*     argv    = NULL;
    t_ebox*   x = (t_ebox *)z;
    t_eclass* c = (t_eclass *)x->e_obj.te_g.g_pd;
    
    binbuf_addv(b, "ssiis", gensym("#X"), gensym("obj"), (int)x->e_obj.te_xpix, (int)x->e_obj.te_ypix, gensym(class_getname(x->e_obj.te_g.g_pd)));
    
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i].save)
        {
            sprintf(attr_name, "@%s", c->c_attr[i].name->s_name);
            object_attr_getvalueof((t_object *)x, c->c_attr[i].name, &argc, &argv);
            if(argc && argv)
            {
                binbuf_append_atoms(b, gensym(attr_name), argc, argv);
                argc = 0;
                free(argv);
                argv = NULL;
            }
        }
    }

    if(c->c_widget.w_save != NULL)
        c->c_widget.w_save(x, b);
    
    binbuf_addv(b, ";");
}

void ebox_save(t_gobj* z, t_binbuf *b)
{
    t_ebox*   x = (t_ebox *)z;
    t_eclass* c = (t_eclass *)x->e_obj.te_g.g_pd;
    if(c->c_box)
    {
        ebox_dosave_box(z, b);
    }
    else if(x->e_ready_to_draw)
    {
       ebox_dosave_nobox(z, b);
    }
}

void ebox_resize_patch(t_ebox* x, t_symbol* s, long argc, t_atom* argv)
{
	//t_gotfn m;
	//post("%f %f %f %f", atom_getfloat(argv), atom_getfloat(argv+1), atom_getfloat(argv+2), atom_getfloat(argv+3));

	//m = getfn((t_pd *)x->e_canvas, gensym("setbounds"));
	//m(x->e_canvas, atom_getfloat(argv), atom_getfloat(argv+1), atom_getfloat(argv+2), atom_getfloat(argv+3));
}