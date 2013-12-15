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

extern "C"
{
#include "../../../PdEnhanced/Sources/pd_enhanced.h"
}

typedef struct _buttn
{
	t_ebox      j_box;
    
    t_outlet*   f_out;
	t_rgba		f_color_background;
	t_rgba		f_color_border;
	t_rgba		f_color_bang;
    t_clock*    f_clock;
    char        f_active;
    
} t_buttn;

t_eclass *buttn_class;

void buttn_tilde_setup(void);
void *buttn_new(t_symbol *s, int argc, t_atom *argv);
void buttn_free(t_buttn *x);
void buttn_assist(t_buttn *x, void *b, long m, long a, char *s);

void buttn_output(t_buttn *x, t_symbol* s, long argc, t_atom* argv);

t_pd_err buttn_notify(t_buttn *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

void buttn_getdrawparams(t_buttn *x, t_object *patcherview, t_edrawparams *params);
void buttn_oksize(t_buttn *x, t_rect *newrect);

void buttn_paint(t_buttn *x, t_object *view);
void draw_background(t_buttn *x,  t_object *view, t_rect *rect);

void buttn_mousedown(t_buttn *x, t_object *patcherview, t_pt pt, long modifiers);
void buttn_mouseup(t_buttn *x, t_object *patcherview, t_pt pt, long modifiers);

extern "C" void buttn_setup(void)
{
	t_eclass *c;
    
	c = eclass_new("buttn", (method)buttn_new, (method)buttn_free, (short)sizeof(t_buttn), 0L, A_GIMME, 0);
    
	eclass_init(c, 0);
	
	eclass_addmethod(c, (method) buttn_assist,          "assist",           A_CANT, 0);
	eclass_addmethod(c, (method) buttn_paint,           "paint",            A_CANT, 0);
	eclass_addmethod(c, (method) buttn_notify,          "notify",           A_CANT, 0);
    eclass_addmethod(c, (method) buttn_getdrawparams,   "getdrawparams",    A_CANT, 0);
    eclass_addmethod(c, (method) buttn_oksize,          "oksize",           A_CANT, 0);
    eclass_addmethod(c, (method) buttn_output,          "float",            A_FLOAT,0);
    eclass_addmethod(c, (method) buttn_output,          "bang",             A_CANT, 0);
    eclass_addmethod(c, (method) buttn_output,          "list",             A_GIMME,0);
    eclass_addmethod(c, (method) buttn_output,          "anything",         A_GIMME,0);
    
    eclass_addmethod(c, (method) buttn_mousedown,       "mousedown",        A_CANT, 0);
    eclass_addmethod(c, (method) buttn_mouseup,         "mouseup",          A_CANT, 0);
    
	CLASS_ATTR_DEFAULT              (c, "size", 0, "15. 15.");
    
	CLASS_ATTR_RGBA                 (c, "bgcolor", 0, t_buttn, f_color_background);
	CLASS_ATTR_LABEL                (c, "bgcolor", 0, "Background Color");
	CLASS_ATTR_ORDER                (c, "bgcolor", 0, "1");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bgcolor", 0, "0.75 0.75 0.75 1.");
	
	CLASS_ATTR_RGBA                 (c, "bdcolor", 0, t_buttn, f_color_border);
	CLASS_ATTR_LABEL                (c, "bdcolor", 0, "Box Border Color");
	CLASS_ATTR_ORDER                (c, "bdcolor", 0, "2");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bdcolor", 0, "0.5 0.5 0.5 1.");
	
	CLASS_ATTR_RGBA                 (c, "bacolor", 0, t_buttn, f_color_bang);
	CLASS_ATTR_LABEL                (c, "bacolor", 0, "Bang Color");
	CLASS_ATTR_ORDER                (c, "bacolor", 0, "3");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bacolor", 0, "0. 0. 0. 1.");
	
	
    eclass_register(CLASS_NOBOX, c);
	buttn_class = c;
}

void *buttn_new(t_symbol *s, int argc, t_atom *argv)
{
	t_buttn *x =  NULL;
	t_binbuf* d;
    long flags;
	if (!(d = binbuf_via_atoms(argc,argv)))
		return NULL;
    
	x = (t_buttn *)ebox_alloc(buttn_class);
    flags = 0
    | EBOX_GROWLINK
    ;
	ebox_new((t_ebox *)x, flags);
	x->j_box.b_firstin = (t_object *)x;
    x->f_out = (t_outlet *)bangout((t_object *)x);
    x->f_active = 0;
    x->f_clock          = clock_new(x,(t_method)buttn_mouseup);
	binbuf_attr_process(x, d);
	ebox_ready((t_ebox *)x);
	return (x);
}

void buttn_getdrawparams(t_buttn *x, t_object *patcherview, t_edrawparams *params)
{
	params->d_borderthickness   = 2;
	params->d_cornersize        = 2;
    params->d_bordercolor       = x->f_color_border;
    params->d_boxfillcolor      = x->f_color_border;
}

void buttn_oksize(t_buttn *x, t_rect *newrect)
{
    newrect->width = pd_clip_min(newrect->width, 15.);
    newrect->height = pd_clip_min(newrect->height, 15.);
}

void buttn_output(t_buttn *x, t_symbol* s, long argc, t_atom* argv)
{
    x->f_active = 1;
    ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
    ebox_redraw((t_ebox *)x);
    outlet_bang(x->f_out);
    clock_delay(x->f_clock, 100);
}

void buttn_free(t_buttn *x)
{
	ebox_free((t_ebox *)x);
    clock_free(x->f_clock);
}

void buttn_assist(t_buttn *x, void *b, long m, long a, char *s)
{
	;
}

t_pd_err buttn_notify(t_buttn *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	if (msg == gensym("attr_modified"))
	{
		if(s == gensym("bgcolor") || s == gensym("bdcolor") || s == gensym("bacolor"))
		{
			ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
		}
        ebox_redraw((t_ebox *)x);
	}
	return 0;
}

void buttn_paint(t_buttn *x, t_object *view)
{
	t_rect rect;
	ebox_get_rect_for_view((t_ebox *)x, &rect);
    draw_background(x, view, &rect);
}

void draw_background(t_buttn *x, t_object *view, t_rect *rect)
{
    float size;
	t_elayer *g = ebox_start_layer((t_ebox *)x, gensym("background_layer"), rect->width, rect->height);
    
	if (g)
	{
        size = rect->width * 0.5;
        if(x->f_active)
        {
            egraphics_set_color_rgba(g, &x->f_color_bang);
        }
        else
        {
            egraphics_set_color_rgba(g, &x->f_color_background);
        }
        egraphics_circle(g, size, size, size * 0.9);
        egraphics_fill(g);
        ebox_end_layer((t_ebox*)x, gensym("background_layer"));
	}
	ebox_paint_layer((t_ebox *)x, gensym("background_layer"), 0., 0.);
}

void buttn_mousedown(t_buttn *x, t_object *patcherview, t_pt pt, long modifiers)
{
    x->f_active = 1;
    ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
    ebox_redraw((t_ebox *)x);
    outlet_bang(x->f_out);
}

void buttn_mouseup(t_buttn *x, t_object *patcherview, t_pt pt, long modifiers)
{
    x->f_active = 0;
    ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
    ebox_redraw((t_ebox *)x);
}







