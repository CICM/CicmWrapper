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

extern "C"  {
#include "../../../PdEnhanced/Sources/pd_enhanced.h"
}

typedef struct _toggle
{
	t_edspbox   j_box;

    t_outlet*   f_out;
	t_rgba		f_color_background;
	t_rgba		f_color_border;
	t_rgba		f_color_cross;
    char        f_active;

} t_toggle;

t_eclass *toggle_class;

void *toggle_new(t_symbol *s, int argc, t_atom *argv);
void toggle_free(t_toggle *x);
void toggle_assist(t_toggle *x, void *b, long m, long a, char *s);

void toggle_float(t_toggle *x, float f);
void toggle_bang(t_toggle *x);
void toggle_output(t_toggle *x);

t_pd_err toggle_notify(t_toggle *x, t_symbol *s, t_symbol *msg, void *sender, void *data);
void toggle_preset(t_toggle *x, t_binbuf *b);

void toggle_getdrawparams(t_toggle *x, t_object *patcherview, t_edrawparams *params);
void toggle_oksize(t_toggle *x, t_rect *newrect);

void toggle_paint(t_toggle *x, t_object *view);
void draw_background(t_toggle *x,  t_object *view, t_rect *rect);

void toggle_mousedown(t_toggle *x, t_object *patcherview, t_pt pt, long modifiers);

extern "C" void setup_c0x2etoggle(void)
{
	t_eclass *c;

	c = eclass_new("c.toggle", (method)toggle_new, (method)toggle_free, (short)sizeof(t_toggle), 0L, A_GIMME, 0);

	eclass_init(c, 0);
    eclass_dspinit(c);
    
	eclass_addmethod(c, (method) toggle_assist,          "assist",           A_CANT, 0);
	eclass_addmethod(c, (method) toggle_paint,           "paint",            A_CANT, 0);
	eclass_addmethod(c, (method) toggle_notify,          "notify",           A_CANT, 0);
    eclass_addmethod(c, (method) toggle_getdrawparams,   "getdrawparams",    A_CANT, 0);
    eclass_addmethod(c, (method) toggle_oksize,          "oksize",           A_CANT, 0);
    eclass_addmethod(c, (method) toggle_float,           "float",            A_FLOAT,0);
    eclass_addmethod(c, (method) toggle_bang,            "bang",             A_CANT, 0);
    eclass_addmethod(c, (method) toggle_mousedown,       "mousedown",        A_CANT, 0);
    eclass_addmethod(c, (method) toggle_preset,          "preset",           A_CANT, 0);

	CLASS_ATTR_DEFAULT              (c, "size", 0, "15. 15.");

	CLASS_ATTR_RGBA                 (c, "bgcolor", 0, t_toggle, f_color_background);
	CLASS_ATTR_LABEL                (c, "bgcolor", 0, "Background Color");
	CLASS_ATTR_ORDER                (c, "bgcolor", 0, "1");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bgcolor", 0, "0.75 0.75 0.75 1.");

	CLASS_ATTR_RGBA                 (c, "bdcolor", 0, t_toggle, f_color_border);
	CLASS_ATTR_LABEL                (c, "bdcolor", 0, "Box Border Color");
	CLASS_ATTR_ORDER                (c, "bdcolor", 0, "2");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bdcolor", 0, "0.5 0.5 0.5 1.");

	CLASS_ATTR_RGBA                 (c, "crcolor", 0, t_toggle, f_color_cross);
	CLASS_ATTR_LABEL                (c, "crcolor", 0, "Cross Color");
	CLASS_ATTR_ORDER                (c, "crcolor", 0, "3");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "crcolor", 0, "0.5 0.5 0.5 1.");


    eclass_register(CLASS_NOBOX, c);
	toggle_class = c;
}

void *toggle_new(t_symbol *s, int argc, t_atom *argv)
{
	t_toggle *x =  NULL;
	t_binbuf* d;
    long flags;
	if (!(d = binbuf_via_atoms(argc,argv)))
		return NULL;

	x = (t_toggle *)eobj_new(toggle_class);
    flags = 0
    | EBOX_GROWLINK
    ;
	ebox_new((t_ebox *)x, flags);
    x->f_active = 0;
    x->f_out = (t_outlet *)floatout(x);

	ebox_attrprocess_viabinbuf(x, d);
	ebox_ready((t_ebox *)x);
	return (x);
}

void toggle_getdrawparams(t_toggle *x, t_object *patcherview, t_edrawparams *params)
{
	params->d_borderthickness   = 2;
	params->d_cornersize        = 2;
    params->d_bordercolor       = x->f_color_border;
    params->d_boxfillcolor      = x->f_color_background;
}

void toggle_oksize(t_toggle *x, t_rect *newrect)
{
    newrect->width = pd_clip_min(newrect->width, 15.);
    newrect->height = pd_clip_min(newrect->height, 15.);
}

void toggle_float(t_toggle *x, float f)
{
    if(f == 0)
        x->f_active = 0;
    else
        x->f_active = 1;
    toggle_output(x);
}

void toggle_bang(t_toggle *x)
{
    if(x->f_active == 1)
        x->f_active = 0;
    else
        x->f_active = 1;
    toggle_output(x);
}

void toggle_output(t_toggle *x)
{
    ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
    ebox_redraw((t_ebox *)x);
    outlet_float((t_outlet*)x->f_out, (float)x->f_active);
}

void toggle_free(t_toggle *x)
{
	ebox_free((t_ebox *)x);
}

void toggle_assist(t_toggle *x, void *b, long m, long a, char *s)
{
	;
}

t_pd_err toggle_notify(t_toggle *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
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

void toggle_paint(t_toggle *x, t_object *view)
{
	t_rect rect;
	ebox_get_rect_for_view((t_ebox *)x, &rect);
    draw_background(x, view, &rect);
}

void draw_background(t_toggle *x, t_object *view, t_rect *rect)
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

void toggle_mousedown(t_toggle *x, t_object *patcherview, t_pt pt, long modifiers)
{
    toggle_bang(x);
}

void toggle_preset(t_toggle *x, t_binbuf *b)
{
    binbuf_addv(b, "sf", gensym("float"), (float)x->f_active);
}







