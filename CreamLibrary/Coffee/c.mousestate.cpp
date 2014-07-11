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

#include "../c.library.h"

typedef struct  _mousestate
{
	t_eobj      l_box;
	t_outlet*   l_mouse_pressed;
    t_outlet*   l_mouse_x;
    t_outlet*   l_mouse_y;
    t_outlet*   l_mouse_deltax;
    t_outlet*   l_mouse_deltay;
    t_outlet*   l_mouse_modifier;
    t_clock*    l_clock;
    t_pt        l_mouse;
    t_pt        l_mouse_zero;
    char        l_zero;
    int         l_mode;
} t_mousestate;

t_eclass *mousestate_class;

void *mousestate_new(t_symbol *s, int argc, t_atom *argv);
void mousestate_free(t_mousestate *x);
void mousestate_assist(t_mousestate *x, void *b, long m, long a, char *s);
void mousestate_tick(t_mousestate *x);
void mousestate_poll(t_mousestate *x);
void mousestate_nopoll(t_mousestate *x);
void mousestate_mode(t_mousestate *x, float f);
void mousestate_zero(t_mousestate *x);
void mousestate_reset(t_mousestate *x);

extern "C" void setup_c0x2emousestate(void)
{
	t_eclass *c;
    
	c = eclass_new("c.mousestate", (method)mousestate_new, (method)mousestate_free, (short)sizeof(t_mousestate), 0L, A_GIMME, 0);
    cream_initclass(c);
    
    eclass_addmethod(c, (method) mousestate_assist,     "assist",          A_CANT, 0);
    eclass_addmethod(c, (method) mousestate_poll,       "poll",            A_CANT, 0);
    eclass_addmethod(c, (method) mousestate_nopoll,     "nopoll",          A_CANT, 0);
    eclass_addmethod(c, (method) mousestate_reset,      "reset",           A_CANT, 0);
    eclass_addmethod(c, (method) mousestate_zero,       "zero",            A_CANT, 0);
    eclass_addmethod(c, (method) mousestate_mode,       "mode",            A_FLOAT, 0);

    eclass_register(CLASS_OBJ, c);
	mousestate_class = c;
}

void *mousestate_new(t_symbol *s, int argc, t_atom *argv)
{
	t_mousestate *x =  NULL;
	t_binbuf* d;
    
    if (!(d = binbuf_via_atoms(argc,argv)))
        return NULL;
    
    x = (t_mousestate *)eobj_new(mousestate_class);
    x->l_mouse_pressed = (t_outlet *)floatout(x);
    x->l_mouse_x       = (t_outlet *)floatout(x);
    x->l_mouse_y       = (t_outlet *)floatout(x);
    x->l_mouse_deltax  = (t_outlet *)floatout(x);
    x->l_mouse_deltay  = (t_outlet *)floatout(x);
    x->l_mouse_modifier= (t_outlet *)floatout(x);
    x->l_mode          = 0;
    x->l_zero          = 0;
    x->l_mouse         = eobj_get_mouse_global_position((t_object *)x);
    x->l_clock         = clock_new(x, (t_method)mousestate_tick);
    
    return (x);
}

void mousestate_reset(t_mousestate *x)
{
    x->l_zero = 0;
}

void mousestate_zero(t_mousestate *x)
{
    if(x->l_mode == 1)
        x->l_mouse_zero = eobj_get_mouse_canvas_position(x);
    else
        x->l_mouse_zero = eobj_get_mouse_global_position(x);
    x->l_zero = 1;
}

void mousestate_mode(t_mousestate *x, float f)
{
    int mode;
    if(f == 0)
        mode = 0;
    else
        mode = 1;
    if(x->l_mode != mode)
    {
        x->l_mode = mode;
        if(x->l_mode == 1)
        {
            eobj_poll_mouse(x);
            x->l_mouse = eobj_get_mouse_canvas_position(x);
        }
        else
        {
            eobj_nopoll_mouse(x);
            x->l_mouse = eobj_get_mouse_global_position(x);
        }
        x->l_zero = 0;
    }
}

void mousestate_tick(t_mousestate *x)
{
    t_pt mouse;
    if(x->l_mode == 1)
        mouse = eobj_get_mouse_canvas_position(x);
    else
        mouse = eobj_get_mouse_global_position(x);
    if(x->l_zero)
    {
        mouse.x -= x->l_mouse_zero.x;
        mouse.y -= x->l_mouse_zero.y;
    }
    
    outlet_float(x->l_mouse_modifier, eobj_get_mouse_modifier(x));
    outlet_float(x->l_mouse_deltay, mouse.y - x->l_mouse.y);
    outlet_float(x->l_mouse_deltax, mouse.x - x->l_mouse.x);
    outlet_float(x->l_mouse_y, mouse.y);
    outlet_float(x->l_mouse_x, mouse.x);
    outlet_float(x->l_mouse_pressed, eobj_get_mouse_status(x));
    
    x->l_mouse = mouse;
    clock_delay(x->l_clock, 20);
}

void mousestate_poll(t_mousestate *x)
{
    t_pt mouse;
    if(x->l_mode == 1)
        mouse = eobj_get_mouse_canvas_position(x);
    else
        mouse = eobj_get_mouse_global_position(x);
    if(x->l_zero)
    {
        mouse.x -= x->l_mouse_zero.x;
        mouse.y -= x->l_mouse_zero.y;
    }
    
    outlet_float(x->l_mouse_modifier, eobj_get_mouse_modifier(x));
    outlet_float(x->l_mouse_deltay, mouse.y - x->l_mouse.y);
    outlet_float(x->l_mouse_deltax, mouse.x - x->l_mouse.x);
    outlet_float(x->l_mouse_y, mouse.y);
    outlet_float(x->l_mouse_x, mouse.x);
    outlet_float(x->l_mouse_pressed, eobj_get_mouse_status(x));
    
    x->l_mouse = mouse;
    clock_set(x->l_clock, 20);
}

void mousestate_nopoll(t_mousestate *x)
{
    clock_unset(x->l_clock);
}

void mousestate_free(t_mousestate *x)
{
    eobj_nopoll_mouse(x);
    clock_free(x->l_clock);
	eobj_free(x);
}

void mousestate_assist(t_mousestate *x, void *b, long m, long a, char *s)
{
	;
}


