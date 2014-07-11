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

typedef struct  _meter
{
	t_edspbox   j_box;
	
	t_clock*	f_clock;
	int			f_startclock;
	long        f_interval;
    void*       f_peaks_outlet;
    float       f_peak_value;
	char        f_direction;
    
    long		f_over_led_preserved;
	
	t_rgba		f_color_background;
	t_rgba		f_color_border;

	t_rgba		f_color_signal_cold;
	t_rgba		f_color_signal_tepid;
	t_rgba		f_color_signal_warm;
	t_rgba		f_color_signal_hot;
	t_rgba		f_color_signal_over;
	
} t_meter;

t_eclass *meter_class;

void *meter_new(t_symbol *s, int argc, t_atom *argv);
void meter_free(t_meter *x);
void meter_assist(t_meter *x, void *b, long m, long a, char *s);

void meter_dsp(t_meter *x, t_object *dsp, short *count, double samplerate, long maxvectorsize, long flags);
void meter_perform(t_meter *x, t_object *d, float **ins, long ni, float **outs, long no, long sf, long f,void *up);
void meter_tick(t_meter *x);
void meter_output(t_meter *x);

t_pd_err meter_notify(t_meter *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

void meter_getdrawparams(t_meter *x, t_object *patcherview, t_edrawparams *params);
void meter_oksize(t_meter *x, t_rect *newrect);

void meter_paint(t_meter *x, t_object *view);
void draw_background(t_meter *x,  t_object *view, t_rect *rect);
void draw_leds(t_meter *x,  t_object *view, t_rect *rect);

extern "C" void setup_c0x2emeter_tilde(void)
{
	t_eclass *c;
    
	c = eclass_new("c.meter~", (method)meter_new, (method)meter_free, (short)sizeof(t_meter), 0L, A_GIMME, 0);

	eclass_dspinit(c);
	eclass_init(c, 0);
    cream_initclass(c);
	
	eclass_addmethod(c, (method) meter_dsp,             "dsp",              A_CANT, 0);
	eclass_addmethod(c, (method) meter_assist,          "assist",           A_CANT, 0);
	eclass_addmethod(c, (method) meter_paint,           "paint",            A_CANT, 0);
	eclass_addmethod(c, (method) meter_notify,          "notify",           A_CANT, 0);
    eclass_addmethod(c, (method) meter_getdrawparams,   "getdrawparams",    A_CANT, 0);
    eclass_addmethod(c, (method) meter_oksize,          "oksize",           A_CANT, 0);
    
    CLASS_ATTR_INVISIBLE            (c, "fontname", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontweight", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontslant", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontsize", 1);
	CLASS_ATTR_DEFAULT              (c, "size", 0, "13 85");
    
    CLASS_ATTR_LONG                 (c, "interval", 0, t_meter, f_interval);
	CLASS_ATTR_ORDER                (c, "interval", 0, "1");
	CLASS_ATTR_LABEL                (c, "interval", 0, "Refresh Interval in Milliseconds");
	CLASS_ATTR_FILTER_MIN           (c, "interval", 20);
	CLASS_ATTR_DEFAULT              (c, "interval", 0, "50");
	CLASS_ATTR_SAVE                 (c, "interval", 1);
	CLASS_ATTR_STYLE                (c, "interval", 0, "number");
    
	CLASS_ATTR_RGBA                 (c, "bgcolor", 0, t_meter, f_color_background);
	CLASS_ATTR_LABEL                (c, "bgcolor", 0, "Background Color");
	CLASS_ATTR_ORDER                (c, "bgcolor", 0, "1");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bgcolor", 0, "0.75 0.75 0.75 1.");
	CLASS_ATTR_STYLE                (c, "bgcolor", 0, "color");
    
	CLASS_ATTR_RGBA                 (c, "bdcolor", 0, t_meter, f_color_border);
	CLASS_ATTR_LABEL                (c, "bdcolor", 0, "Border Color");
	CLASS_ATTR_ORDER                (c, "bdcolor", 0, "2");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bdcolor", 0, "0.5 0.5 0.5 1.");
	CLASS_ATTR_STYLE                (c, "bdcolor", 0, "color");
    
	CLASS_ATTR_RGBA                 (c, "coldcolor", 0, t_meter, f_color_signal_cold);
	CLASS_ATTR_LABEL                (c, "coldcolor", 0, "Cold Signal Color");
	CLASS_ATTR_ORDER                (c, "coldcolor", 0, "3");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "coldcolor", 0, "0. 0.6 0. 0.8");
	CLASS_ATTR_STYLE                (c, "coldcolor", 0, "color");
    
	CLASS_ATTR_RGBA                 (c, "tepidcolor", 0, t_meter, f_color_signal_tepid);
	CLASS_ATTR_LABEL                (c, "tepidcolor", 0, "Tepid Signal Color");
	CLASS_ATTR_ORDER                (c, "tepidcolor", 0, "4");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "tepidcolor", 0, "0.6 0.73 0. 0.8");
	CLASS_ATTR_STYLE                (c, "tepidcolor", 0, "color");
    
	CLASS_ATTR_RGBA                 (c, "warmcolor", 0, t_meter, f_color_signal_warm);
	CLASS_ATTR_LABEL                (c, "warmcolor", 0, "Warm Signal Color");
	CLASS_ATTR_ORDER                (c, "warmcolor", 0, "5");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "warmcolor", 0, ".85 .85 0. 0.8");
	CLASS_ATTR_STYLE                (c, "warmcolor", 0, "color");
    
	CLASS_ATTR_RGBA                 (c, "hotcolor", 0, t_meter, f_color_signal_hot);
	CLASS_ATTR_LABEL                (c, "hotcolor", 0, "Hot Signal Color");
	CLASS_ATTR_ORDER                (c, "hotcolor", 0, "6");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "hotcolor", 0, "1. 0.6 0. 0.8");
	CLASS_ATTR_STYLE                (c, "hotcolor", 0, "color");
    
	CLASS_ATTR_RGBA                 (c, "overcolor", 0, t_meter, f_color_signal_over);
	CLASS_ATTR_LABEL                (c, "overcolor", 0, "Overload Signal Color");
	CLASS_ATTR_ORDER                (c, "overcolor", 0, "7");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "overcolor", 0, "1. 0. 0. 0.8");
	CLASS_ATTR_STYLE                (c, "overcolor", 0, "color");
    
    eclass_register(CLASS_BOX, c);
	meter_class = c;
}

void *meter_new(t_symbol *s, int argc, t_atom *argv)
{
	t_meter *x =  NULL;
	t_binbuf* d;
    long flags;
	if (!(d = binbuf_via_atoms(argc,argv)))
		return NULL;
    
	x = (t_meter *)eobj_new(meter_class);
    
    flags = 0
    | EBOX_GROWINDI
    | EBOX_IGNORELOCKCLICK
    ;
    
	ebox_new((t_ebox *)x, flags);
    eobj_dspsetup((t_ebox *)x, 1, 0);
    
    x->f_direction      = 0;
    x->f_peaks_outlet   = floatout(x);
    x->f_peak_value     = -90.;
    x->f_clock          = clock_new(x,(t_method)meter_tick);
	x->f_startclock     = 0;
    x->f_over_led_preserved = 0;
    ebox_attrprocess_viabinbuf(x, d);
	ebox_ready((t_ebox *)x);
    
	return (x);
}

void meter_getdrawparams(t_meter *x, t_object *patcherview, t_edrawparams *params)
{
	params->d_borderthickness   = 2.;
	params->d_cornersize        = 2.;
    params->d_bordercolor       = x->f_color_border;
    params->d_boxfillcolor      = x->f_color_background;
}

void meter_oksize(t_meter *x, t_rect *newrect)
{
    newrect->width = pd_clip_min(newrect->width, 8.);
    newrect->height = pd_clip_min(newrect->height, 8.);
    
    if(newrect->width > newrect->height)
        x->f_direction = 1;
    else
        x->f_direction = 0;
    
    if(x->f_direction)
    {
        newrect->width = pd_clip_min(newrect->width, 50.);
    }
    else
    {
        newrect->height = pd_clip_min(newrect->height, 50.);
    }
}

void meter_dsp(t_meter *x, t_object *dsp, short *count, double samplerate, long maxvectorsize, long flags)
{
    object_method(dsp, gensym("dsp_add"), x, (method)meter_perform, 0, NULL);
	x->f_startclock = 1;
}

void meter_perform(t_meter *x, t_object *dsp, float **ins, long ni, float **outs, long no, long nsamples, long f,void *up)
{
    int i;
    float peak = fabs(ins[0][0]);
    for(i = 1; i < nsamples; i++)
    {
        if(fabs(ins[0][i]) > peak)
            peak = fabs(ins[0][i]);
    }
    
    if(peak > 0.)
        x->f_peak_value = 20. * log10(peak);
    else
        x->f_peak_value = -90.;

    if (x->f_startclock)
    {
        x->f_startclock = 0;
        clock_delay(x->f_clock, 0);
    }
}

void meter_tick(t_meter *x)
{
    if(x->f_peak_value >= 0)
    {
        x->f_over_led_preserved = 1;
    }
    else if(x->f_over_led_preserved > 0)
    {
        x->f_over_led_preserved++;
    }
    if(x->f_over_led_preserved >= 1000. / x->f_interval)
    {
        x->f_over_led_preserved = 0;
    }
    
    meter_output(x);
    
	ebox_invalidate_layer((t_ebox *)x, gensym("leds_layer"));
	ebox_redraw((t_ebox *)x);
    
	if(sys_getdspstate())
		clock_delay(x->f_clock, x->f_interval);
}

void meter_output(t_meter *x)
{
    outlet_float((t_outlet*)x->f_peaks_outlet, (float)x->f_peak_value);
    if(ebox_getsender((t_ebox *) x))
        pd_float(ebox_getsender((t_ebox *) x), (float)x->f_peak_value);
}

void meter_free(t_meter *x)
{
	ebox_free((t_ebox *)x);
    clock_free(x->f_clock);
}

void meter_assist(t_meter *x, void *b, long m, long a, char *s)
{
	;
}

t_pd_err meter_notify(t_meter *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	if (msg == gensym("attr_modified"))
	{
		if(s == gensym("bgcolor") || s == gensym("bdcolor"))
		{
			ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
		}
		else if( s == gensym("coldcolor") || s == gensym("tepidcolor") || s == gensym("warmcolor") || s == gensym("hotcolor") || s == gensym("overcolor"))
		{
			ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
			ebox_invalidate_layer((t_ebox *)x, gensym("leds_layer"));
		}
		ebox_redraw((t_ebox *)x);
	}
	return 0;
}

void meter_paint(t_meter *x, t_object *view)
{
	t_rect rect;
	ebox_get_rect_for_view((t_ebox *)x, &rect);
	draw_background(x, view, &rect);
    draw_leds(x, view, &rect);
}

void draw_background(t_meter *x,  t_object *view, t_rect *rect)
{
	int i;
	t_elayer *g = ebox_start_layer((t_ebox *)x, gensym("background_layer"), rect->width, rect->height);
 
	if (g)
	{
        egraphics_set_color_rgba(g, &x->f_color_border);
        if(!x->f_direction)
        {
            for(i = 1; i < 13; i++)
            {
                egraphics_move_to(g, 0., i * rect->height / 13.f);
                egraphics_line_to(g, rect->width, i * rect->height / 13.f);
                egraphics_stroke(g);
            }
        }
        else
        {
            for(i = 1; i < 13; i++)
            {
                egraphics_move_to(g, i * rect->width / 13.f, 0.f);
                egraphics_line_to(g, i * rect->width / 13.f, rect->height);
                egraphics_stroke(g);
            }
        }
		ebox_end_layer((t_ebox*)x, gensym("background_layer"));
	}
	ebox_paint_layer((t_ebox *)x, gensym("background_layer"), 0.f, 0.f);
}


void draw_leds(t_meter *x, t_object *view, t_rect *rect)
{
	float i;
	float dB;
	t_elayer *g = ebox_start_layer((t_ebox *)x, gensym("leds_layer"), rect->width, rect->height);
	
	if (g)
	{
        float led_height = rect->height / 13.f;
        float led_width = rect->width / 13.f;
        for(i = 12, dB = -39; i > 0; i--, dB += 3.f)
        {
            if(x->f_peak_value >= dB)
            {
                if(i > 9)
                    egraphics_set_color_rgba(g, &x->f_color_signal_cold);
                else if(i > 6)
                    egraphics_set_color_rgba(g, &x->f_color_signal_tepid);
                else if(i > 3)
                    egraphics_set_color_rgba(g, &x->f_color_signal_warm);
                else if(i > 0)
                    egraphics_set_color_rgba(g, &x->f_color_signal_hot);
                if(!x->f_direction)
                {
                    if(i > 11)
                        egraphics_rectangle_rounded(g, 0, i * led_height + 1, rect->width, led_height, 1.f);
                    else
                        egraphics_rectangle_rounded(g, 0, i * led_height + 1, rect->width, led_height - 1, 1.f);
                }
                else
                {
                    if(i > 11)
                        egraphics_rectangle_rounded(g, 0, 0, led_width, rect->height, 1.);
                    else
                        egraphics_rectangle_rounded(g, (12 - i) * led_width + 1, 0, led_width - 1, rect->height, 1.);
                }
                egraphics_fill(g);
            }
        }
        if(x->f_over_led_preserved)
        {
            egraphics_set_color_rgba(g, &x->f_color_signal_over);
            if(!x->f_direction)
            {
                egraphics_rectangle_rounded(g, 0, 0, rect->width, led_height, 1.f);
            }
            else
            {
                egraphics_rectangle_rounded(g, 12 * led_width + 1, 0, led_width,  rect->height, 1.);

            }
            
            egraphics_fill(g);
        }
		ebox_end_layer((t_ebox *)x, gensym("leds_layer"));
	}
	ebox_paint_layer((t_ebox *)x, gensym("leds_layer"), 0., 0.);
}



