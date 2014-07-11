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

typedef struct _knob
{
	t_ebox      j_box;
    
    t_outlet*   f_out;
	t_rgba		f_color_background;
	t_rgba		f_color_border;
	t_rgba		f_color_needle;
    long        f_endless;
    long        f_mode;
    float       f_value;
    float       f_min;
    float       f_max;
    float       f_ref_y;
    float       f_ref_value;
} t_knob;

t_eclass *knob_class;

void *knob_new(t_symbol *s, int argc, t_atom *argv);
void knob_free(t_knob *x);
void knob_assist(t_knob *x, void *b, long m, long a, char *s);

void knob_float(t_knob *x, float f);
void knob_set(t_knob *x, float f);
void knob_bang(t_knob *x);
void knob_output(t_knob *x);

t_pd_err knob_notify(t_knob *x, t_symbol *s, t_symbol *msg, void *sender, void *data);
void knob_preset(t_knob *x, t_binbuf *b);

void knob_getdrawparams(t_knob *x, t_object *patcherview, t_edrawparams *params);
void knob_oksize(t_knob *x, t_rect *newrect);

void knob_paint(t_knob *x, t_object *view);
void draw_background(t_knob *x,  t_object *view, t_rect *rect);
void draw_needle(t_knob *x, t_object *view, t_rect *rect);

void knob_mousedown(t_knob *x, t_object *patcherview, t_pt pt, long modifiers);
void knob_mousedrag(t_knob *x, t_object *patcherview, t_pt pt, long modifiers);

extern "C" void setup_c0x2eknob(void)
{
	t_eclass *c;
    
	c = eclass_new("c.knob", (method)knob_new, (method)knob_free, (short)sizeof(t_knob), 0L, A_GIMME, 0);
    
	eclass_init(c, 0);
    cream_initclass(c);
	
	eclass_addmethod(c, (method) knob_assist,          "assist",           A_CANT, 0);
	eclass_addmethod(c, (method) knob_paint,           "paint",            A_CANT, 0);
	eclass_addmethod(c, (method) knob_notify,          "notify",           A_CANT, 0);
    eclass_addmethod(c, (method) knob_getdrawparams,   "getdrawparams",    A_CANT, 0);
    eclass_addmethod(c, (method) knob_oksize,          "oksize",           A_CANT, 0);
    eclass_addmethod(c, (method) knob_set,             "set",              A_FLOAT,0);
    eclass_addmethod(c, (method) knob_float,           "float",            A_FLOAT,0);
    eclass_addmethod(c, (method) knob_bang,            "bang",             A_CANT, 0);
    eclass_addmethod(c, (method) knob_mousedown,       "mousedown",        A_CANT, 0);
    eclass_addmethod(c, (method) knob_mousedrag,       "mousedrag",        A_CANT, 0);
    eclass_addmethod(c, (method) knob_preset,          "preset",           A_CANT, 0);
    
    CLASS_ATTR_INVISIBLE            (c, "fontname", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontweight", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontslant", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontsize", 1);
	CLASS_ATTR_DEFAULT              (c, "size", 0, "50. 50.");
    
    CLASS_ATTR_LONG                 (c, "endless", 0, t_knob, f_endless);
	CLASS_ATTR_LABEL                (c, "endless", 0, "Endless Mode");
	CLASS_ATTR_ORDER                (c, "endless", 0, "1");
    CLASS_ATTR_FILTER_CLIP          (c, "endless", 0, 1);
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "endless", 0, "0");
    CLASS_ATTR_STYLE                (c, "endless", 0, "onoff");
    
    CLASS_ATTR_LONG                 (c, "mode", 0, t_knob, f_mode);
	CLASS_ATTR_LABEL                (c, "mode", 0, "Circular Mode");
	CLASS_ATTR_ORDER                (c, "mode", 0, "1");
    CLASS_ATTR_FILTER_CLIP          (c, "mode", 0, 1);
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "mode", 0, "0");
    CLASS_ATTR_STYLE                (c, "mode", 0, "onoff");
    
    CLASS_ATTR_FLOAT                (c, "min", 0, t_knob, f_min);
	CLASS_ATTR_LABEL                (c, "min", 0, "Minimum Value");
	CLASS_ATTR_ORDER                (c, "min", 0, "1");
    CLASS_ATTR_DEFAULT              (c, "min", 0, "0.");
    CLASS_ATTR_SAVE                 (c, "min", 1);
    CLASS_ATTR_STYLE                (c, "min", 0, "number");
    
    CLASS_ATTR_FLOAT                (c, "max", 0, t_knob, f_max);
	CLASS_ATTR_LABEL                (c, "max", 0, "Maximum Value");
	CLASS_ATTR_ORDER                (c, "max", 0, "1");
    CLASS_ATTR_DEFAULT              (c, "max", 0, "1.");
    CLASS_ATTR_SAVE                 (c, "max", 1);
    CLASS_ATTR_STYLE                (c, "max", 0, "number");
    
	CLASS_ATTR_RGBA                 (c, "bgcolor", 0, t_knob, f_color_background);
	CLASS_ATTR_LABEL                (c, "bgcolor", 0, "Background Color");
	CLASS_ATTR_ORDER                (c, "bgcolor", 0, "1");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bgcolor", 0, "0.75 0.75 0.75 1.");
	CLASS_ATTR_STYLE                (c, "bgcolor", 0, "color");
    
	CLASS_ATTR_RGBA                 (c, "bdcolor", 0, t_knob, f_color_border);
	CLASS_ATTR_LABEL                (c, "bdcolor", 0, "Border Color");
	CLASS_ATTR_ORDER                (c, "bdcolor", 0, "2");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bdcolor", 0, "0.5 0.5 0.5 1.");
	CLASS_ATTR_STYLE                (c, "bdcolor", 0, "color");
    
	CLASS_ATTR_RGBA                 (c, "necolor", 0, t_knob, f_color_needle);
	CLASS_ATTR_LABEL                (c, "necolor", 0, "Needle Color");
	CLASS_ATTR_ORDER                (c, "necolor", 0, "3");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "necolor", 0, "0.5 0.5 0.5 1.");
	CLASS_ATTR_STYLE                (c, "necolor", 0, "color");
	
    eclass_register(CLASS_BOX, c);
	knob_class = c;
}

void *knob_new(t_symbol *s, int argc, t_atom *argv)
{
	t_knob *x =  NULL;
	t_binbuf* d;
    long flags;
	if (!(d = binbuf_via_atoms(argc,argv)))
		return NULL;
    
	x = (t_knob *)eobj_new(knob_class);
    flags = 0
    | EBOX_GROWLINK
    ;
	ebox_new((t_ebox *)x, flags);
	
    x->f_out = (t_outlet *)floatout(x);
    x->f_value = 0;
	ebox_attrprocess_viabinbuf(x, d);
	ebox_ready((t_ebox *)x);
	return (x);
}

void knob_getdrawparams(t_knob *x, t_object *patcherview, t_edrawparams *params)
{
	params->d_borderthickness   = 2;
	params->d_cornersize        = 2;
    params->d_bordercolor       = x->f_color_border;
    params->d_boxfillcolor      = x->f_color_background;
}

void knob_oksize(t_knob *x, t_rect *newrect)
{
    newrect->width = pd_clip_min(newrect->width, 25.);
    newrect->height = pd_clip_min(newrect->height, 25.);
}

void knob_float(t_knob *x, float f)
{
    knob_set(x, f);
    knob_output(x);
}

void knob_set(t_knob *x, float f)
{
    if(x->f_endless)
    {
        if(x->f_min < x->f_max)
        {
            x->f_value = fmodf(f + x->f_max - x->f_min, x->f_max - x->f_min);
        }
        else
        {
            x->f_value = fmodf(f + x->f_min - x->f_max, x->f_min - x->f_max);
        }
    }
    else
    {
        if(x->f_min < x->f_max)
            x->f_value = pd_clip_minmax(f, x->f_min, x->f_max);
        else
            x->f_value = pd_clip_minmax(f, x->f_max, x->f_min);
    }
    
    ebox_invalidate_layer((t_ebox *)x, gensym("needle_layer"));
    ebox_redraw((t_ebox *)x);
}

void knob_bang(t_knob *x)
{
    knob_output(x);
}

void knob_output(t_knob *x)
{
    outlet_float((t_outlet*)x->f_out, (float)x->f_value);
    if(ebox_getsender((t_ebox *) x))
        pd_float(ebox_getsender((t_ebox *) x), (float)x->f_value);
}

void knob_free(t_knob *x)
{
	ebox_free((t_ebox *)x);
}

void knob_assist(t_knob *x, void *b, long m, long a, char *s)
{
	;
}

t_pd_err knob_notify(t_knob *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	if (msg == gensym("attr_modified"))
	{
		if(s == gensym("bgcolor") || s == gensym("bdcolor") || s == gensym("necolor") || s == gensym("endless"))
		{
			ebox_invalidate_layer((t_ebox *)x, gensym("needle_layer"));
            ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
		}
        ebox_redraw((t_ebox *)x);
	}
	return 0;
}

void knob_paint(t_knob *x, t_object *view)
{
	t_rect rect;
    if(x->f_endless)
    {
        if(x->f_min < x->f_max)
        {
            x->f_value = fmodf(x->f_value + x->f_max - x->f_min, x->f_max - x->f_min);
        }
        else
        {
            x->f_value = fmodf(x->f_value + x->f_min - x->f_max, x->f_min - x->f_max);
        }
    }
    else
    {
        if(x->f_min < x->f_max)
            x->f_value = pd_clip_minmax(x->f_value, x->f_min, x->f_max);
        else
            x->f_value = pd_clip_minmax(x->f_value, x->f_max, x->f_min);
    }
    
	ebox_get_rect_for_view((t_ebox *)x, &rect);
    draw_background(x, view, &rect);
    draw_needle(x, view, &rect);
}

void draw_background(t_knob *x, t_object *view, t_rect *rect)
{
	t_elayer *g = ebox_start_layer((t_ebox *)x, gensym("background_layer"), rect->width, rect->height);
    
	if (g)
	{
        float size = rect->width * 0.5;
        egraphics_set_color_rgba(g, &x->f_color_border);
        egraphics_rectangle(g, -1, -1, rect->width+1, rect->height+1);
        egraphics_fill(g);
        egraphics_set_color_rgba(g, &x->f_color_background);
        egraphics_circle(g, rect->width * 0.5, rect->height * 0.5, size * 0.9);
        egraphics_fill(g);
        if(!x->f_endless)
        {
            float hsize = pd_clip_min(size * 0.15, 2);
            float alpha = rect->height - hsize;
            egraphics_set_color_rgba(g, &x->f_color_border);
            egraphics_move_to(g, rect->width * 0.5, rect->height * 0.5 + hsize);
            egraphics_line_to(g, rect->height * 0.5 - alpha * 0.5, rect->height);
            egraphics_line_to(g, rect->height * 0.5 + alpha * 0.5, rect->height);
            egraphics_fill(g);
            egraphics_set_color_rgba(g, &x->f_color_background);
            egraphics_circle(g, rect->width * 0.5, rect->height * 0.5, pd_clip_min(size * 0.3, 3));
            egraphics_fill(g);
        }
        egraphics_set_color_rgba(g, &x->f_color_needle);
        egraphics_circle(g, rect->width * 0.5, rect->height * 0.5, pd_clip_min(size * 0.2, 2));
        egraphics_fill(g);
        
        ebox_end_layer((t_ebox*)x, gensym("background_layer"));
	}
	ebox_paint_layer((t_ebox *)x, gensym("background_layer"), 0., 0.);
}

void draw_needle(t_knob *x, t_object *view, t_rect *rect)
{
    float size, abs, ord, pimul, pimin;
    t_elayer *g = ebox_start_layer((t_ebox *)x, gensym("needle_layer"), rect->width, rect->height);
    
    if (g)
	{
        size = rect->width * 0.5;
        if(x->f_endless)
        {
            pimul = EPD_2PI;
            pimin = EPD_PI2;
        }
        else
        {
            pimul = 1.5 * EPD_PI;
            if (x->f_min < x->f_max)
                pimin = 1.5 * EPD_PI2;
            else
                pimin = 0.5 * EPD_PI2;
        }
        if(x->f_min < x->f_max)
        {
            abs =  pd_abscissa(size * 0.9 - 1, (x->f_value - x->f_min) / (x->f_max - x->f_min) * pimul + pimin);
            ord =  pd_ordinate(size * 0.9 - 1, (x->f_value - x->f_min) / (x->f_max - x->f_min) * pimul + pimin);
        }
        else
        {
            abs =  pd_abscissa(size * 0.9 - 1, -(x->f_value - x->f_max) / (x->f_min - x->f_max) * pimul + pimin);
            ord =  pd_ordinate(size * 0.9 - 1, -(x->f_value - x->f_max) / (x->f_min - x->f_max) * pimul + pimin);
            
        }
        egraphics_set_line_width(g, 2);
        egraphics_set_color_rgba(g, &x->f_color_needle);
        egraphics_line_fast(g, rect->width * 0.5, rect->height * 0.5, abs + rect->width * 0.5, ord + rect->height * 0.5);
        ebox_end_layer((t_ebox*)x, gensym("needle_layer"));
    }
   
    ebox_paint_layer((t_ebox *)x, gensym("needle_layer"), 0., 0.);

}

void knob_mousedown(t_knob *x, t_object *patcherview, t_pt pt, long modifiers)
{
    float angle = pd_angle(pt.x - x->j_box.b_rect.width * 0.5, (x->j_box.b_rect.height * 0.5 - pt.y)) / EPD_2PI;
    if(x->f_mode)
    {
        if(x->f_endless)
        {
            if(x->f_min < x->f_max)
            {
                angle = -angle;
                angle -= 0.25;
                while (angle < 0.)
                    angle += 1.;
                while (angle > 1.)
                    angle -= 1.;
                x->f_value = angle * (x->f_max - x->f_min) + x->f_min;
            }
            else
            {
                angle += 0.25;
                while (angle < 0.)
                    angle += 1.;
                while (angle > 1.)
                    angle -= 1.;
                x->f_value = angle * (x->f_min - x->f_max) + x->f_max;
            }
        }
        else
        {
            if(x->f_min < x->f_max)
            {
                angle = -angle;
                angle -= 0.25;
                while (angle < 0.)
                    angle += 1.;
                while (angle > 1.)
                    angle -= 1.;
                angle = pd_clip_minmax(angle, 0.125, 0.875);
                angle -= 0.125;
                angle *= 0.75;
                x->f_value = angle * (x->f_max - x->f_min) + x->f_min;
            }
            else
            {
                angle += 0.25;
                while (angle < 0.)
                    angle += 1.;
                while (angle > 1.)
                    angle -= 1.;
                angle = pd_clip_minmax(angle, 0.125, 0.875);
                angle -= 0.125;
                angle *= 0.75;
                x->f_value = angle * (x->f_min - x->f_max) + x->f_max;
            }
        }
        ebox_invalidate_layer((t_ebox *)x, gensym("needle_layer"));
        ebox_redraw((t_ebox *)x);
        knob_output(x);
    }
    else
    {
        x->f_ref_y = pt.y;
        x->f_ref_value = x->f_value;
    }
}

void knob_mousedrag(t_knob *x, t_object *patcherview, t_pt pt, long modifiers)
{
    float angle = pd_angle(pt.x - x->j_box.b_rect.width * 0.5, (x->j_box.b_rect.height * 0.5 - pt.y)) / EPD_2PI;
    if(x->f_mode)
    {
        if(x->f_endless)
        {
            if(x->f_min < x->f_max)
            {
                angle = -angle;
                angle -= 0.25;
                while (angle < 0.)
                    angle += 1.;
                while (angle > 1.)
                    angle -= 1.;
                x->f_value = angle * (x->f_max - x->f_min) + x->f_min;
            }
            else
            {
                angle += 0.25;
                while (angle < 0.)
                    angle += 1.;
                while (angle > 1.)
                    angle -= 1.;
                x->f_value = angle * (x->f_min - x->f_max) + x->f_max;
            }
        }
        else
        {
            if(x->f_min < x->f_max)
            {
                angle = -angle;
                angle -= 0.25;
                while (angle < 0.)
                    angle += 1.;
                while (angle > 1.)
                    angle -= 1.;
                angle = pd_clip_minmax(angle, 0.125, 0.875);
                angle -= 0.125;
                angle *= 1. / 0.75;
                x->f_value = angle * (x->f_max - x->f_min) + x->f_min;
            }
        
            else
            {
                angle += 0.25;
                while (angle < 0.)
                    angle += 1.;
                while (angle > 1.)
                    angle -= 1.;
                angle = pd_clip_minmax(angle, 0.125, 0.875);
                angle -= 0.125;
                angle *= 1. / 0.75;
                x->f_value = angle * (x->f_min - x->f_max) + x->f_max;
            }
        }
    }
    else
    {
        float diff = x->f_ref_y - pt.y;
        if(diff == 0xffffffff)
            return;
        if(x->f_min < x->f_max)
        {
            if(x->f_endless)
            {
                x->f_value = fmodf(diff / 50. * (x->f_max - x->f_min) + x->f_ref_value + x->f_max - x->f_min, x->f_max - x->f_min);
            }
            else
                x->f_value = pd_clip_minmax(diff / 50. * (x->f_max - x->f_min) + x->f_ref_value, x->f_min, x->f_max);
        }
        else
        {
            if(x->f_endless)
            {
                x->f_value = fmodf(diff / 50. * (x->f_min - x->f_max) + x->f_ref_value + x->f_min - x->f_max, x->f_min - x->f_max);
            }
            else
                x->f_value = pd_clip_minmax(diff / 50. * (x->f_min - x->f_max) + x->f_ref_value, x->f_max, x->f_min);
        }
    }
    ebox_invalidate_layer((t_ebox *)x, gensym("needle_layer"));
    ebox_redraw((t_ebox *)x);
    knob_output(x);
}

void knob_preset(t_knob *x, t_binbuf *b)
{
    binbuf_addv(b, "sf", gensym("float"), (float)x->f_value);
}







