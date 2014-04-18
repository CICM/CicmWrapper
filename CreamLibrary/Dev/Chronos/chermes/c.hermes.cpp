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
#include "../../../PdEnhanced/Sources/cicm_wrapper.h"
}

typedef struct _hermes
{
	t_ebox      j_box;
    
    t_outlet*   f_out;
	t_rgba		f_color_background;
	t_rgba		f_color_border;
} t_hermes;

t_eclass *hermes_class;

void *hermes_new(t_symbol *s, int argc, t_atom *argv);
void hermes_free(t_hermes *x);
void hermes_assist(t_hermes *x, void *b, long m, long a, char *s);

void hermes_output(t_hermes *x, t_symbol* s, long argc, t_atom* argv);

t_pd_err hermes_notify(t_hermes *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

void hermes_getdrawparams(t_hermes *x, t_object *patcherview, t_edrawparams *params);
void hermes_oksize(t_hermes *x, t_rect *newrect);

void hermes_paint(t_hermes *x, t_object *view);
void draw_background(t_hermes *x,  t_object *view, t_rect *rect);

void hermes_mousedown(t_hermes *x, t_object *patcherview, t_pt pt, long modifiers);
void hermes_mouseup(t_hermes *x, t_object *patcherview, t_pt pt, long modifiers);

extern "C" void setup_c0x2ehermes(void)
{
	t_eclass *c;
    
	c = eclass_new("c.hermes", (method)hermes_new, (method)hermes_free, (short)sizeof(t_hermes), 0L, A_GIMME, 0);
    
	eclass_init(c, 0);
	
	eclass_addmethod(c, (method) hermes_assist,          "assist",           A_CANT, 0);
	eclass_addmethod(c, (method) hermes_paint,           "paint",            A_CANT, 0);
	eclass_addmethod(c, (method) hermes_notify,          "notify",           A_CANT, 0);
    eclass_addmethod(c, (method) hermes_getdrawparams,   "getdrawparams",    A_CANT, 0);
    eclass_addmethod(c, (method) hermes_oksize,          "oksize",           A_CANT, 0);
    eclass_addmethod(c, (method) hermes_output,          "float",            A_FLOAT,0);
    eclass_addmethod(c, (method) hermes_output,          "bang",             A_CANT, 0);
    eclass_addmethod(c, (method) hermes_output,          "list",             A_GIMME,0);
    eclass_addmethod(c, (method) hermes_output,          "anything",         A_GIMME,0);
    
    eclass_addmethod(c, (method) hermes_mousedown,       "mousedown",        A_CANT, 0);
    eclass_addmethod(c, (method) hermes_mouseup,         "mouseup",          A_CANT, 0);
    
    CLASS_ATTR_INVISIBLE            (c, "fontname", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontweight", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontslant", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontsize", 1);
	CLASS_ATTR_DEFAULT              (c, "size", 0, "15. 15.");
    
	CLASS_ATTR_RGBA                 (c, "bgcolor", 0, t_hermes, f_color_background);
	CLASS_ATTR_LABEL                (c, "bgcolor", 0, "Background Color");
	CLASS_ATTR_ORDER                (c, "bgcolor", 0, "1");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bgcolor", 0, "1. 1. 1. 1.");
    CLASS_ATTR_STYLE                (c, "bgcolor", 0, "color");
	
	CLASS_ATTR_RGBA                 (c, "bdcolor", 0, t_hermes, f_color_border);
	CLASS_ATTR_LABEL                (c, "bdcolor", 0, "Box Border Color");
	CLASS_ATTR_ORDER                (c, "bdcolor", 0, "2");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bdcolor", 0, "0.41 0.05 0.07 1.");
	CLASS_ATTR_STYLE                (c, "bdcolor", 0, "color");
    
    eclass_register(CLASS_BOX, c);
    cicm_post();
	hermes_class = c;
}

void *hermes_new(t_symbol *s, int argc, t_atom *argv)
{
	t_hermes *x =  NULL;
	t_binbuf* d;
    long flags;
	if (!(d = binbuf_via_atoms(argc,argv)))
		return NULL;
    
	x = (t_hermes *)eobj_new(hermes_class);
	flags = 0
    | EBOX_GROWINDI
    ;
	ebox_new((t_ebox *)x, flags);
    x->f_out = (t_outlet *)bangout((t_object *)x);
	ebox_attrprocess_viabinbuf(x, d);
	ebox_ready((t_ebox *)x);
	return (x);
}

void hermes_getdrawparams(t_hermes *x, t_object *patcherview, t_edrawparams *params)
{
	params->d_borderthickness   = 2;
	params->d_cornersize        = 2;
    params->d_bordercolor       = x->f_color_border;
    params->d_boxfillcolor      = x->f_color_background;
}

void hermes_oksize(t_hermes *x, t_rect *newrect)
{
    newrect->width = pd_clip_min(newrect->width, 15.);
    newrect->height = pd_clip_min(newrect->height, 15.);
}

void hermes_output(t_hermes *x, t_symbol* s, long argc, t_atom* argv)
{
    ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
    ebox_redraw((t_ebox *)x);
}

void hermes_free(t_hermes *x)
{
	ebox_free((t_ebox *)x);
}

void hermes_assist(t_hermes *x, void *b, long m, long a, char *s)
{
	;
}

t_pd_err hermes_notify(t_hermes *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
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

void hermes_paint(t_hermes *x, t_object *view)
{
	t_rect rect;
	ebox_get_rect_for_view((t_ebox *)x, &rect);
    draw_background(x, view, &rect);
}

void draw_background(t_hermes *x, t_object *view, t_rect *rect)
{
	t_elayer *g = ebox_start_layer((t_ebox *)x, gensym("background_layer"), rect->width, rect->height);
    
	if (g)
	{
        ebox_end_layer((t_ebox*)x, gensym("background_layer"));
	}
	ebox_paint_layer((t_ebox *)x, gensym("background_layer"), 0., 0.);
}

void hermes_mousedown(t_hermes *x, t_object *patcherview, t_pt pt, long modifiers)
{
    ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
    ebox_redraw((t_ebox *)x);
}

void hermes_mouseup(t_hermes *x, t_object *patcherview, t_pt pt, long modifiers)
{
    ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
    ebox_redraw((t_ebox *)x);
}







