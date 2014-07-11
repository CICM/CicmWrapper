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

#include "../c.library.h"

typedef struct _rslider
{
	t_ebox      j_box;
    
    t_outlet*   f_out_left;
    t_outlet*   f_out_right;
    long        f_mode;
    
	t_rgba		f_color_background;
	t_rgba		f_color_border;
	t_rgba		f_color_knob;

    char        f_direction;
    char        f_loworhigh;
    float       f_min;
    float       f_max;
    float       f_value_low;
    float       f_value_high;

} t_rslider;

t_eclass *rslider_class;

void *rslider_new(t_symbol *s, int argc, t_atom *argv);
void rslider_free(t_rslider *x);
void rslider_assist(t_rslider *x, void *b, long m, long a, char *s);

void rslider_float(t_rslider *x, float f);
void rslider_set(t_rslider *x, t_symbol* s, long argc, t_atom* argv);
void rslider_list(t_rslider *x, t_symbol* s, long argc, t_atom* argv);

void rslider_bang(t_rslider *x);
void rslider_output(t_rslider *x);

t_pd_err rslider_notify(t_rslider *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

void rslider_preset(t_rslider *x, t_binbuf *b);

void rslider_getdrawparams(t_rslider *x, t_object *patcherview, t_edrawparams *params);
void rslider_oksize(t_rslider *x, t_rect *newrect);

void rslider_paint(t_rslider *x, t_object *view);
void draw_background(t_rslider *x,  t_object *view, t_rect *rect);
void draw_knob(t_rslider *x, t_object *view, t_rect *rect);

void rslider_mousedown(t_rslider *x, t_object *patcherview, t_pt pt, long modifiers);
void rslider_mousedrag(t_rslider *x, t_object *patcherview, t_pt pt, long modifiers);

extern "C" void setup_c0x2erslider(void)
{
	t_eclass *c;
    
	c = eclass_new("c.rslider", (method)rslider_new, (method)rslider_free, (short)sizeof(t_rslider), 0L, A_GIMME, 0);
    
	eclass_init(c, 0);
    cream_initclass(c);
	
	eclass_addmethod(c, (method) rslider_assist,          "assist",           A_CANT, 0);
	eclass_addmethod(c, (method) rslider_paint,           "paint",            A_CANT, 0);
	eclass_addmethod(c, (method) rslider_notify,          "notify",           A_CANT, 0);
    eclass_addmethod(c, (method) rslider_getdrawparams,   "getdrawparams",    A_CANT, 0);
    eclass_addmethod(c, (method) rslider_oksize,          "oksize",           A_CANT, 0);
    eclass_addmethod(c, (method) rslider_set,             "set",              A_GIMME,0);
    eclass_addmethod(c, (method) rslider_list,            "list",             A_GIMME,0);
    eclass_addmethod(c, (method) rslider_float,           "float",            A_FLOAT,0);
    eclass_addmethod(c, (method) rslider_bang,            "bang",             A_CANT, 0);
    eclass_addmethod(c, (method) rslider_mousedown,       "mousedown",        A_CANT, 0);
    eclass_addmethod(c, (method) rslider_mousedrag,       "mousedrag",        A_CANT, 0);
    eclass_addmethod(c, (method) rslider_preset,          "preset",           A_CANT, 0);
    
    CLASS_ATTR_INVISIBLE            (c, "fontname", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontweight", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontslant", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontsize", 1);
	CLASS_ATTR_DEFAULT              (c, "size", 0, "15. 120.");
    
    CLASS_ATTR_LONG                 (c, "listmode", 0, t_rslider, f_mode);
	CLASS_ATTR_LABEL                (c, "listmode", 0, "List Mode");
    CLASS_ATTR_FILTER_CLIP          (c, "listmode", 0, 1);
	CLASS_ATTR_ORDER                (c, "listmode", 0, "1");
    CLASS_ATTR_DEFAULT              (c, "listmode", 0, "0");
    CLASS_ATTR_SAVE                 (c, "listmode", 1);
    CLASS_ATTR_STYLE                (c, "listmode", 0, "onoff");
    
    CLASS_ATTR_FLOAT                (c, "min", 0, t_rslider, f_min);
	CLASS_ATTR_LABEL                (c, "min", 0, "Minimum Value");
	CLASS_ATTR_ORDER                (c, "min", 0, "1");
    CLASS_ATTR_DEFAULT              (c, "min", 0, "0.");
    CLASS_ATTR_SAVE                 (c, "min", 1);
    CLASS_ATTR_STYLE                (c, "min", 0, "number");
    
    CLASS_ATTR_FLOAT                (c, "max", 0, t_rslider, f_max);
	CLASS_ATTR_LABEL                (c, "max", 0, "Maximum Value");
	CLASS_ATTR_ORDER                (c, "max", 0, "1");
    CLASS_ATTR_DEFAULT              (c, "max", 0, "1.");
    CLASS_ATTR_SAVE                 (c, "max", 1);
    CLASS_ATTR_STYLE                (c, "max", 0, "number");
    
	CLASS_ATTR_RGBA                 (c, "bgcolor", 0, t_rslider, f_color_background);
	CLASS_ATTR_LABEL                (c, "bgcolor", 0, "Background Color");
	CLASS_ATTR_ORDER                (c, "bgcolor", 0, "1");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bgcolor", 0, "0.75 0.75 0.75 1.");
	CLASS_ATTR_STYLE                (c, "bgcolor", 0, "color");
    
	CLASS_ATTR_RGBA                 (c, "bdcolor", 0, t_rslider, f_color_border);
	CLASS_ATTR_LABEL                (c, "bdcolor", 0, "Border Color");
	CLASS_ATTR_ORDER                (c, "bdcolor", 0, "2");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bdcolor", 0, "0.5 0.5 0.5 1.");
	CLASS_ATTR_STYLE                (c, "bdcolor", 0, "color");
    
	CLASS_ATTR_RGBA                 (c, "kncolor", 0, t_rslider, f_color_knob);
	CLASS_ATTR_LABEL                (c, "kncolor", 0, "Knob Color");
	CLASS_ATTR_ORDER                (c, "kncolor", 0, "3");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "kncolor", 0, "0.5 0.5 0.5 1.");
	CLASS_ATTR_STYLE                (c, "kncolor", 0, "color");
    
    eclass_register(CLASS_BOX, c);
	rslider_class = c;
}

void *rslider_new(t_symbol *s, int argc, t_atom *argv)
{
	t_rslider *x =  NULL;
	t_binbuf* d;
    long flags;
	if (!(d = binbuf_via_atoms(argc,argv)))
		return NULL;
    
	x = (t_rslider *)eobj_new(rslider_class);
    flags = 0
    | EBOX_GROWINDI
    ;
	ebox_new((t_ebox *)x, flags);
    eobj_proxynew(x);
    x->f_out_left = (t_outlet *)listout(x);
    x->f_out_right = (t_outlet *)floatout(x);
    
	ebox_attrprocess_viabinbuf(x, d);
    x->f_value_low = x->f_min;
	ebox_ready((t_ebox *)x);
	return (x);
}

void rslider_getdrawparams(t_rslider *x, t_object *patcherview, t_edrawparams *params)
{
	params->d_borderthickness   = 2;
	params->d_cornersize        = 2;
    params->d_bordercolor       = x->f_color_border;
    params->d_boxfillcolor      = x->f_color_background;
}

void rslider_oksize(t_rslider *x, t_rect *newrect)
{
    newrect->width = pd_clip_min(newrect->width, 16.);
    newrect->height = pd_clip_min(newrect->height, 16.);
    
    if(newrect->width > newrect->height)
        x->f_direction = 1;
    else
        x->f_direction = 0;
    
    if(x->f_direction)
    {
        newrect->width = pd_clip_min(newrect->width, 50.);
        if((int)newrect->height % 2 == 1)
            newrect->height++;
    }
    else
    {
        newrect->height = pd_clip_min(newrect->height, 50.);
        if((int)newrect->width % 2 == 1)
            newrect->width++;
    }
}

void rslider_list(t_rslider *x, t_symbol* s, long argc, t_atom* argv)
{
    rslider_set(x, s, argc, argv);
    rslider_output(x);
}

void rslider_set(t_rslider *x, t_symbol* s, long argc, t_atom* argv)
{
    if(argc > 0 && atom_gettype(argv) == A_FLOAT)
    {
        if(x->f_min < x->f_max)
        {
            x->f_value_low = pd_clip_minmax(atom_getfloat(argv), x->f_min, x->f_max);
        }
        else
        {
            x->f_value_low = pd_clip_minmax(atom_getfloat(argv), x->f_max, x->f_min);
        }
    }
    if(argc > 1 && atom_gettype(argv+1) == A_FLOAT)
    {
        if(x->f_min < x->f_max)
        {
            x->f_value_high = pd_clip_minmax(atom_getfloat(argv+1), x->f_min, x->f_max);
        }
        else
        {
            x->f_value_high = pd_clip_minmax(atom_getfloat(argv+1), x->f_max, x->f_min);
        }
    }

    ebox_invalidate_layer((t_ebox *)x, gensym("knob_layer"));
    ebox_redraw((t_ebox *)x);
}

void rslider_float(t_rslider *x, float f)
{
    if(eobj_getproxy(x) == 0)
    {
        if(x->f_min < x->f_max)
        {
            x->f_value_low = pd_clip_minmax(f, x->f_min, x->f_max);
        }
        else
        {
            x->f_value_low = pd_clip_minmax(f, x->f_max, x->f_min);
        }
    }
    else
    {
        if(x->f_min < x->f_max)
        {
            x->f_value_high = pd_clip_minmax(f, x->f_min, x->f_max);
        }
        else
        {
            x->f_value_high = pd_clip_minmax(f, x->f_max, x->f_min);
        }
    }
    rslider_output(x);
    ebox_invalidate_layer((t_ebox *)x, gensym("knob_layer"));
    ebox_redraw((t_ebox *)x);
}

void rslider_bang(t_rslider *x)
{
    ebox_invalidate_layer((t_ebox *)x, gensym("knob_layer"));
    ebox_redraw((t_ebox *)x);
    rslider_output(x);
}

void rslider_output(t_rslider *x)
{
    t_atom argv[2];
    atom_setfloat(argv, x->f_value_low);
    atom_setfloat(argv+1, x->f_value_high);
    if(x->f_mode)
        outlet_list(x->f_out_left, &s_list, 2, argv);
    else
    {
        outlet_float(x->f_out_left, (float)x->f_value_low);
        outlet_float(x->f_out_right, (float)x->f_value_high);
    }
    if(ebox_getsender((t_ebox *) x))
        pd_list(ebox_getsender((t_ebox *) x), &s_list, 2, argv);
}

void rslider_free(t_rslider *x)
{
	ebox_free((t_ebox *)x);
}

void rslider_assist(t_rslider *x, void *b, long m, long a, char *s)
{
	;
}

t_pd_err rslider_notify(t_rslider *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	if (msg == gensym("attr_modified"))
	{
		if(s == gensym("bgcolor") || s == gensym("bdcolor") || s == gensym("kncolor"))
		{
			ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
            ebox_invalidate_layer((t_ebox *)x, gensym("knob_layer"));
		}
        ebox_redraw((t_ebox *)x);
	}
	return 0;
}

void rslider_paint(t_rslider *x, t_object *view)
{
	t_rect rect;
	ebox_get_rect_for_view((t_ebox *)x, &rect);
    draw_background(x, view, &rect);
    draw_knob(x, view, &rect);
}

void draw_background(t_rslider *x, t_object *view, t_rect *rect)
{
	t_elayer *g = ebox_start_layer((t_ebox *)x, gensym("background_layer"), rect->width, rect->height);
    
	if (g)
	{
        egraphics_set_color_rgba(g, &x->f_color_border);
        egraphics_set_line_width(g, 2);
        if(x->f_direction)
        {
            egraphics_line_fast(g,  -2, rect->height * 0.5, rect->width + 4, rect->height * 0.5);
        }
        else
        {
            egraphics_line_fast(g, rect->width * 0.5, -2, rect->width * 0.5, rect->height  + 4);
        }
        egraphics_stroke(g);
        ebox_end_layer((t_ebox*)x, gensym("background_layer"));
	}
	ebox_paint_layer((t_ebox *)x, gensym("background_layer"), 0., 0.);
}

void draw_knob(t_rslider *x, t_object *view, t_rect *rect)
{
    t_rgba linecolor;
    t_rgba rectcolor;
    t_elayer *g = ebox_start_layer((t_ebox *)x, gensym("knob_layer"), rect->width, rect->height);
    
	if (g)
	{
        float value_low = (x->f_value_low - x->f_min) / (x->f_max - x->f_min);
        float value_high = (x->f_value_high - x->f_min) / (x->f_max - x->f_min);
        rgba_set(&rectcolor, (x->f_color_background.red + x->f_color_knob.red) * 0.5, (x->f_color_background.green + x->f_color_knob.green) * 0.5, (x->f_color_background.blue + x->f_color_knob.blue) * 0.5, 1.);
        
        rgba_set(&linecolor, (x->f_color_border.red + x->f_color_knob.red) * 0.5, (x->f_color_border.green + x->f_color_border.green) * 0.5, (x->f_color_border.blue + x->f_color_knob.blue) * 0.5, 1.);
        
        egraphics_set_line_width(g, 2);
        if(x->f_direction)
        {
            egraphics_set_color_rgba(g, &rectcolor);
            egraphics_rectangle(g, value_low * rect->width, -2, value_high * rect->width - value_low * rect->width, rect->height + 4);
            egraphics_fill(g);
            
            egraphics_set_color_rgba(g, &linecolor);
            egraphics_line_fast(g,  value_low * rect->width, rect->height * 0.5, value_high * rect->width, rect->height * 0.5);
            
            egraphics_set_color_rgba(g, &x->f_color_knob);
            egraphics_line_fast(g, value_low * rect->width, -2, value_low * rect->width, rect->height + 4);
            egraphics_line_fast(g, value_high * rect->width, -2, value_high * rect->width, rect->height + 4);
        }
        else
        {
            egraphics_set_color_rgba(g, &rectcolor);
            egraphics_rectangle(g, -2, value_low * rect->height, rect->width + 4, value_high * rect->height - value_low * rect->height);
            egraphics_fill(g);
            
            egraphics_set_color_rgba(g, &linecolor);
            egraphics_line_fast(g, rect->width * 0.5, value_low * rect->height, rect->width * 0.5, value_high * rect->height);
            
            egraphics_set_color_rgba(g, &x->f_color_knob);
            egraphics_line_fast(g, -2, value_low * rect->height, rect->width + 4, value_low * rect->height);
            egraphics_line_fast(g, -2, value_high * rect->height, rect->width + 4, value_high * rect->height);
        }
        egraphics_stroke(g);
        
        ebox_end_layer((t_ebox*)x, gensym("knob_layer"));
	}
	ebox_paint_layer((t_ebox *)x, gensym("knob_layer"), 0., 0.);
}

void rslider_mousedown(t_rslider *x, t_object *patcherview, t_pt pt, long modifiers)
{
    float ratio;
    float value;
    if(x->f_min < x->f_max)
        ratio = x->f_max - x->f_min;
    else
        ratio = x->f_min - x->f_max;
    
    if(x->f_direction)
    {
        if(x->f_min < x->f_max)
            value = pd_clip_minmax(pt.x / x->j_box.b_rect.width * ratio + x->f_min, x->f_min, x->f_max);
        else
            value= pd_clip_minmax((x->j_box.b_rect.width - pt.x) / x->j_box.b_rect.width * ratio + x->f_max, x->f_max, x->f_min);
    }
    else
    {
        if(x->f_min < x->f_max)
            value = pd_clip_minmax(pt.y / x->j_box.b_rect.height * ratio + x->f_min, x->f_min, x->f_max);
        else
            value = pd_clip_minmax((x->j_box.b_rect.height - pt.y) / x->j_box.b_rect.height * ratio + x->f_max, x->f_max, x->f_min);
    }
    
    if(modifiers == EMOD_SHIFT)
    {
        if(fabs(x->f_value_high - value) < fabs(x->f_value_low - value))
        {
            x->f_value_high = value;
            x->f_loworhigh = 0;
        }
        else
        {
            x->f_value_low = value;
            x->f_loworhigh = 1;
        }
    }
    else
    {
        x->f_value_high = x->f_value_low = value;
        x->f_loworhigh = 0;
    }
    
    ebox_invalidate_layer((t_ebox *)x, gensym("knob_layer"));
    ebox_redraw((t_ebox *)x);
    rslider_output(x);
}

void rslider_mousedrag(t_rslider *x, t_object *patcherview, t_pt pt, long modifiers)
{
    float ratio;
    float value;
    if(x->f_min < x->f_max)
        ratio = x->f_max - x->f_min;
    else
        ratio = x->f_min - x->f_max;
    
    if(x->f_direction)
    {
        if(x->f_min < x->f_max)
            value = pd_clip_minmax(pt.x / x->j_box.b_rect.width * ratio + x->f_min, x->f_min, x->f_max);
        else
            value = pd_clip_minmax((x->j_box.b_rect.width - pt.x) / x->j_box.b_rect.width * ratio + x->f_max, x->f_max, x->f_min);
    }
    else
    {
        if(x->f_min < x->f_max)
            value = pd_clip_minmax(pt.y / x->j_box.b_rect.height * ratio + x->f_min, x->f_min, x->f_max);
        else
            value = pd_clip_minmax((x->j_box.b_rect.height - pt.y) / x->j_box.b_rect.height * ratio + x->f_max, x->f_max, x->f_min);
    }
    
    if(x->f_loworhigh)
    {
        x->f_value_low = value;
    }
    else
    {
        x->f_value_high = value;
    }
    
    ebox_invalidate_layer((t_ebox *)x, gensym("knob_layer"));
    ebox_redraw((t_ebox *)x);
    rslider_output(x);
}

void rslider_preset(t_rslider *x, t_binbuf *b)
{
    binbuf_addv(b, "sff", gensym("list"), (float)x->f_value_low, (float)x->f_value_high);
}






