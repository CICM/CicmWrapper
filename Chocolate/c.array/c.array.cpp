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

extern "C" {
#include "../../../PdEnhanced/Sources/cicm_wrapper.h"
}
typedef struct _carray
{
	t_ebox      j_box;
    
    t_outlet*   f_out;
	t_rgba		f_color_background;
	t_rgba		f_color_border;
	
    t_symbol*   f_name;
    float       f_ms;
    long        f_channels;
    
    t_clock*    f_clock;
    char        f_active;
    
} t_carray;

t_eclass *carray_class;

void *carray_new(t_symbol *s, int argc, t_atom *argv);
void carray_free(t_carray *x);
void carray_assist(t_carray *x, void *b, long m, long a, char *s);

void carray_output(t_carray *x, t_symbol* s, long argc, t_atom* argv);

t_pd_err carray_notify(t_carray *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

void carray_getdrawparams(t_carray *x, t_object *patcherview, t_edrawparams *params);
void carray_oksize(t_carray *x, t_rect *newrect);

void carray_paint(t_carray *x, t_object *view);
void draw_background(t_carray *x,  t_object *view, t_rect *rect);

void carray_mousedown(t_carray *x, t_object *patcherview, t_pt pt, long modifiers);
void carray_mouseup(t_carray *x, t_object *patcherview, t_pt pt, long modifiers);

extern "C" void setup_c0x2earray(void)
{
	t_eclass *c;
    
	c = eclass_new("c.array", (method)carray_new, (method)carray_free, (short)sizeof(t_carray), 0L, A_GIMME, 0);
    
	eclass_init(c, 0);
	
	eclass_addmethod(c, (method) carray_assist,          "assist",           A_CANT, 0);
	eclass_addmethod(c, (method) carray_paint,           "paint",            A_CANT, 0);
	eclass_addmethod(c, (method) carray_notify,          "notify",           A_CANT, 0);
    eclass_addmethod(c, (method) carray_getdrawparams,   "getdrawparams",    A_CANT, 0);
    eclass_addmethod(c, (method) carray_oksize,          "oksize",           A_CANT, 0);
    eclass_addmethod(c, (method) carray_output,          "float",            A_FLOAT,0);
    eclass_addmethod(c, (method) carray_output,          "bang",             A_CANT, 0);
    eclass_addmethod(c, (method) carray_output,          "list",             A_GIMME,0);
    eclass_addmethod(c, (method) carray_output,          "anything",         A_GIMME,0);
    
    eclass_addmethod(c, (method) carray_mousedown,       "mousedown",        A_CANT, 0);
    eclass_addmethod(c, (method) carray_mouseup,         "mouseup",          A_CANT, 0);
    
    CLASS_ATTR_INVISIBLE            (c, "fontname", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontweight", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontslant", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontsize", 1);
	CLASS_ATTR_DEFAULT              (c, "size", 0, "16. 16.");
    
	CLASS_ATTR_RGBA                 (c, "bgcolor", 0, t_carray, f_color_background);
	CLASS_ATTR_LABEL                (c, "bgcolor", 0, "Background Color");
	CLASS_ATTR_ORDER                (c, "bgcolor", 0, "1");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bgcolor", 0, "0.75 0.75 0.75 1.");
	CLASS_ATTR_STYLE                (c, "bgcolor", 0, "color");
    
	CLASS_ATTR_RGBA                 (c, "bdcolor", 0, t_carray, f_color_border);
	CLASS_ATTR_LABEL                (c, "bdcolor", 0, "Border Color");
	CLASS_ATTR_ORDER                (c, "bdcolor", 0, "2");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bdcolor", 0, "0.5 0.5 0.5 1.");
	CLASS_ATTR_STYLE                (c, "bdcolor", 0, "color");
	
    eclass_register(CLASS_BOX, c);
    cicm_post();
	carray_class = c;
}

void *carray_new(t_symbol *s, int argc, t_atom *argv)
{
	t_carray *x =  NULL;
	t_binbuf* d;
    long flags;
	if (!(d = binbuf_via_atoms(argc,argv)))
		return NULL;
    
	x = (t_carray *)eobj_new(carray_class);
    flags = 0
    | EBOX_GROWLINK
    ;
	ebox_new((t_ebox *)x, flags);
    x->f_out = (t_outlet *)bangout((t_object *)x);
    x->f_active = 0;
    x->f_clock          = clock_new(x,(t_method)carray_mouseup);
	ebox_attrprocess_viabinbuf(x, d);
	ebox_ready((t_ebox *)x);
	return (x);
}

void carray_getdrawparams(t_carray *x, t_object *patcherview, t_edrawparams *params)
{
	params->d_borderthickness   = 2;
	params->d_cornersize        = 2;
    params->d_bordercolor       = x->f_color_border;
    params->d_boxfillcolor      = x->f_color_border;
}

void carray_oksize(t_carray *x, t_rect *newrect)
{
    newrect->width = pd_clip_min(newrect->width, 16.);
    newrect->height = pd_clip_min(newrect->height, 16.);
    if((int)newrect->width % 2 == 0)
        newrect->width++;
    if((int)newrect->height % 2 == 0)
        newrect->height++;
}

void carray_output(t_carray *x, t_symbol* s, long argc, t_atom* argv)
{
    x->f_active = 1;
    ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
    ebox_redraw((t_ebox *)x);
    
    clock_delay(x->f_clock, 100);
}

void carray_free(t_carray *x)
{
	ebox_free((t_ebox *)x);
    clock_free(x->f_clock);
}

void carray_assist(t_carray *x, void *b, long m, long a, char *s)
{
	;
}

t_pd_err carray_notify(t_carray *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
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

void carray_paint(t_carray *x, t_object *view)
{
	t_rect rect;
	ebox_get_rect_for_view((t_ebox *)x, &rect);
    draw_background(x, view, &rect);
}

void draw_background(t_carray *x, t_object *view, t_rect *rect)
{
	t_elayer *g = ebox_start_layer((t_ebox *)x, gensym("background_layer"), rect->width, rect->height);
    
	if (g)
	{
        ebox_end_layer((t_ebox*)x, gensym("background_layer"));
	}
	ebox_paint_layer((t_ebox *)x, gensym("background_layer"), 0., 0.);
}

void carray_mousedown(t_carray *x, t_object *patcherview, t_pt pt, long modifiers)
{
    x->f_active = 1;
    ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
    ebox_redraw((t_ebox *)x);
}

void carray_mouseup(t_carray *x, t_object *patcherview, t_pt pt, long modifiers)
{
    x->f_active = 0;
    ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
    ebox_redraw((t_ebox *)x);
}







