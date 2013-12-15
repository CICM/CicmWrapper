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

typedef struct _toggl
{
	t_ebox      j_box;
    
    t_outlet*   f_out;
	t_rgba		f_color_background;
	t_rgba		f_color_border;
	t_rgba		f_color_cross;
    char        f_active;
    
} t_toggl;

t_eclass *toggl_class;

void toggl_tilde_setup(void);
void *toggl_new(t_symbol *s, int argc, t_atom *argv);
void toggl_free(t_toggl *x);
void toggl_assist(t_toggl *x, void *b, long m, long a, char *s);

void toggl_float(t_toggl *x, float f);
void toggl_bang(t_toggl *x);
void toggl_output(t_toggl *x);

t_pd_err toggl_notify(t_toggl *x, t_symbol *s, t_symbol *msg, void *sender, void *data);
void toggl_preset(t_toggl *x, t_binbuf *b);

void toggl_getdrawparams(t_toggl *x, t_object *patcherview, t_edrawparams *params);
void toggl_oksize(t_toggl *x, t_rect *newrect);

void toggl_paint(t_toggl *x, t_object *view);
void draw_background(t_toggl *x,  t_object *view, t_rect *rect);

void toggl_mousedown(t_toggl *x, t_object *patcherview, t_pt pt, long modifiers);

extern "C" void toggl_setup(void)
{
	t_eclass *c;
    
	c = eclass_new("toggl", (method)toggl_new, (method)toggl_free, (short)sizeof(t_toggl), 0L, A_GIMME, 0);
    
	eclass_init(c, 0);
	
	eclass_addmethod(c, (method) toggl_assist,          "assist",           A_CANT, 0);
	eclass_addmethod(c, (method) toggl_paint,           "paint",            A_CANT, 0);
	eclass_addmethod(c, (method) toggl_notify,          "notify",           A_CANT, 0);
    eclass_addmethod(c, (method) toggl_getdrawparams,   "getdrawparams",    A_CANT, 0);
    eclass_addmethod(c, (method) toggl_oksize,          "oksize",           A_CANT, 0);
    eclass_addmethod(c, (method) toggl_float,           "float",            A_FLOAT,0);
    eclass_addmethod(c, (method) toggl_bang,            "bang",             A_CANT, 0);
    eclass_addmethod(c, (method) toggl_mousedown,       "mousedown",        A_CANT, 0);
    eclass_addmethod(c, (method) toggl_preset,          "preset",           A_CANT, 0);
    
	CLASS_ATTR_DEFAULT              (c, "size", 0, "15. 15.");
    
	CLASS_ATTR_RGBA                 (c, "bgcolor", 0, t_toggl, f_color_background);
	CLASS_ATTR_LABEL                (c, "bgcolor", 0, "Background Color");
	CLASS_ATTR_ORDER                (c, "bgcolor", 0, "1");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bgcolor", 0, "0.75 0.75 0.75 1.");
	
	CLASS_ATTR_RGBA                 (c, "bdcolor", 0, t_toggl, f_color_border);
	CLASS_ATTR_LABEL                (c, "bdcolor", 0, "Box Border Color");
	CLASS_ATTR_ORDER                (c, "bdcolor", 0, "2");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bdcolor", 0, "0.5 0.5 0.5 1.");
	
	CLASS_ATTR_RGBA                 (c, "crcolor", 0, t_toggl, f_color_cross);
	CLASS_ATTR_LABEL                (c, "crcolor", 0, "Cross Color");
	CLASS_ATTR_ORDER                (c, "crcolor", 0, "3");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "crcolor", 0, "0.5 0.5 0.5 1.");
	
	
    eclass_register(CLASS_NOBOX, c);
	toggl_class = c;
}

void *toggl_new(t_symbol *s, int argc, t_atom *argv)
{
	t_toggl *x =  NULL;
	t_binbuf* d;
    long flags;
	if (!(d = binbuf_via_atoms(argc,argv)))
		return NULL;
    
	x = (t_toggl *)ebox_alloc(toggl_class);
    flags = 0
    | EBOX_GROWLINK
    ;
	ebox_new((t_ebox *)x, flags);
	x->j_box.b_firstin = (t_object *)x;
    x->f_active = 0;
    x->f_out = (t_outlet *)floatout(x);
    
	binbuf_attr_process(x, d);
	ebox_ready((t_ebox *)x);
	return (x);
}

void toggl_getdrawparams(t_toggl *x, t_object *patcherview, t_edrawparams *params)
{
	params->d_borderthickness   = 2;
	params->d_cornersize        = 2;
    params->d_bordercolor       = x->f_color_border;
    params->d_boxfillcolor      = x->f_color_background;
}

void toggl_oksize(t_toggl *x, t_rect *newrect)
{
    newrect->width = pd_clip_min(newrect->width, 15.);
    newrect->height = pd_clip_min(newrect->height, 15.);
}

void toggl_float(t_toggl *x, float f)
{
    if(f == 0)
        x->f_active = 0;
    else
        x->f_active = 1;
    toggl_output(x);
}

void toggl_bang(t_toggl *x)
{
    if(x->f_active == 1)
        x->f_active = 0;
    else
        x->f_active = 1;
    toggl_output(x);
}

void toggl_output(t_toggl *x)
{
    ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
    ebox_redraw((t_ebox *)x);
    outlet_float((t_outlet*)x->f_out, (float)x->f_active);
}

void toggl_free(t_toggl *x)
{
	ebox_free((t_ebox *)x);
}

void toggl_assist(t_toggl *x, void *b, long m, long a, char *s)
{
	;
}

t_pd_err toggl_notify(t_toggl *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	if (msg == gensym("attr_modified"))
	{
		if(s == gensym("bgcolor") || s == gensym("bdcolor") || s == gensym("crcolor"))
		{
			ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
		}
        ebox_redraw((t_ebox *)x);
	}
	return 0;
}

void toggl_paint(t_toggl *x, t_object *view)
{
	t_rect rect;
	ebox_get_rect_for_view((t_ebox *)x, &rect);
    draw_background(x, view, &rect);
}

void draw_background(t_toggl *x, t_object *view, t_rect *rect)
{
	t_elayer *g = ebox_start_layer((t_ebox *)x, gensym("background_layer"), rect->width, rect->height);
    
	if (g)
	{
        if(x->f_active)
        {
            egraphics_set_color_rgba(g, &x->f_color_cross);
            egraphics_set_line_width(g, 2);
            egraphics_line_fast(g, 0, 0, rect->width, rect->height);
            egraphics_line_fast(g, 0, rect->height, rect->width, 0);
        }
        ebox_end_layer((t_ebox*)x, gensym("background_layer"));
	}
	ebox_paint_layer((t_ebox *)x, gensym("background_layer"), 0., 0.);
}

void toggl_mousedown(t_toggl *x, t_object *patcherview, t_pt pt, long modifiers)
{
    toggl_bang(x);
}

void toggl_preset(t_toggl *x, t_binbuf *b)
{
    binbuf_addv(b, "sf", gensym("float"), (float)x->f_active);
}







