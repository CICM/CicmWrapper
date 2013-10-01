/**
 HoaLibrary : A High Order Ambisonics Library
 Copyright (c) 2012-2013 Julien Colafrancesco, Pierre Guillot, Eliott Paris, CICM, Universite Paris-8.
 All rights reserved.
 
 Website  : http://www.mshparisnord.fr/hoalibrary/
 Contacts : cicm.mshparisnord@gmail.com

 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 
 - Redistributions may not be sold, nor may they be used in a commercial product or activity.
 - Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 - Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 - Neither the name of the CICM nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIALL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include "../../../PdEnhanced/Sources/pd_enhanced.h"

typedef struct  _vu
{
	t_ebox      j_box;
	
	t_clock*	f_clock;
	int			f_startclock;
	long        f_interval;
    void*       f_peaks_outlet;
    float       f_peak_value;
	bool        f_direction;
    
    long		f_over_led_preserved;
	
	t_jrgba		f_color_background;
	t_jrgba		f_color_border;

	t_jrgba		f_color_signal_cold;
	t_jrgba		f_color_signal_tepid;
	t_jrgba		f_color_signal_warm;
	t_jrgba		f_color_signal_hot;
	t_jrgba		f_color_signal_over;
	
} t_vu;

t_eclass *vu_class;

extern "C" void vu_tilde_setup(void);
void *vu_new(t_symbol *s, int argc, t_atom *argv);
void vu_free(t_vu *x);
void vu_assist(t_vu *x, void *b, long m, long a, char *s);

void vu_dsp(t_vu *x, t_object *dsp, short *count, double samplerate, long maxvectorsize, long flags);
void vu_perform(t_vu *x, t_object *d, float **ins, long ni, float **outs, long no, long sf, long f,void *up);
void vu_tick(t_vu *x);
void vu_output(t_vu *x);

t_max_err vu_notify(t_vu *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

void vu_getdrawparams(t_vu *x, t_object *patcherview, t_jboxdrawparams *params);
void vu_oksize(t_vu *x, t_rect *newrect);

void vu_paint(t_vu *x, t_object *view);
void draw_background(t_vu *x,  t_object *view, t_rect *rect);
void draw_leds(t_vu *x,  t_object *view, t_rect *rect);

extern "C" void vu_tilde_setup(void)
{
	t_eclass *c;
    
	c = class_new("vu~", (method)vu_new, (method)vu_free, (short)sizeof(t_vu), 0L, A_GIMME, 0);
    class_addcreator((t_newmethod)vu_new, gensym("meter~"), A_GIMME, 0);

	class_dspinitjbox(c);
	jbox_initclass(c, JBOX_COLOR | JBOX_FIXWIDTH);
	
	class_addmethod(c, (method) vu_dsp,             "dsp",              A_CANT, 0);
	class_addmethod(c, (method) vu_assist,          "assist",           A_CANT, 0);
	class_addmethod(c, (method) vu_paint,           "paint",            A_CANT, 0);
	class_addmethod(c, (method) vu_notify,          "notify",           A_CANT, 0);
    class_addmethod(c, (method) vu_getdrawparams,   "getdrawparams",    A_CANT, 0);
    class_addmethod(c, (method) vu_oksize,          "oksize",           A_CANT, 0);
    
	CLASS_ATTR_DEFAULT			(c, "size", 0, "13 85");
	CLASS_ATTR_INVISIBLE		(c, "color", 0);
    
    CLASS_ATTR_LONG				(c, "interval", 0, t_vu, f_interval);
	CLASS_ATTR_ORDER			(c, "interval", 0, "5");
	CLASS_ATTR_LABEL			(c, "interval", 0, "Refresh Interval in Milliseconds");
	CLASS_ATTR_FILTER_MIN		(c, "interval", 20);
	CLASS_ATTR_DEFAULT			(c, "interval", 0, "50");
	CLASS_ATTR_SAVE				(c, "interval", 1);
	
	CLASS_ATTR_RGBA				(c, "bgcolor", 0, t_vu, f_color_background);
	CLASS_ATTR_LABEL			(c, "bgcolor", 0, "Background Color");
	CLASS_ATTR_ORDER			(c, "bgcolor", 0, "1");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "bgcolor", 0, "0.35 0.23 0.13 1.");
	
	CLASS_ATTR_RGBA				(c, "bdcolor", 0, t_vu, f_color_border);
	CLASS_ATTR_LABEL			(c, "bdcolor", 0, "Box Border Color");
	CLASS_ATTR_ORDER			(c, "bdcolor", 0, "3");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "bdcolor", 0, "0.27 0.21 0. 1");
	
	CLASS_ATTR_RGBA				(c, "coldcolor", 0, t_vu, f_color_signal_cold);
	CLASS_ATTR_LABEL			(c, "coldcolor", 0, "Cold Signal Color");
	CLASS_ATTR_ORDER			(c, "coldcolor", 0, "4");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "coldcolor", 0, "0. 0.6 0. 0.8");
	
	CLASS_ATTR_RGBA				(c, "tepidcolor", 0, t_vu, f_color_signal_tepid);
	CLASS_ATTR_LABEL			(c, "tepidcolor", 0, "Tepid Signal Color");
	CLASS_ATTR_ORDER			(c, "tepidcolor", 0, "5");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "tepidcolor", 0, "0.6 0.73 0. 0.8");
	
	CLASS_ATTR_RGBA				(c, "warmcolor", 0, t_vu, f_color_signal_warm);
	CLASS_ATTR_LABEL			(c, "warmcolor", 0, "Warm Signal Color");
	CLASS_ATTR_ORDER			(c, "warmcolor", 0, "6");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "warmcolor", 0, ".85 .85 0. 0.8");
	
	CLASS_ATTR_RGBA				(c, "hotcolor", 0, t_vu, f_color_signal_hot);
	CLASS_ATTR_LABEL			(c, "hotcolor", 0, "Hot Signal Color");
	CLASS_ATTR_ORDER			(c, "hotcolor", 0, "7");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "hotcolor", 0, "1. 0.6 0. 0.8");
	
	CLASS_ATTR_RGBA				(c, "overcolor", 0, t_vu, f_color_signal_over);
	CLASS_ATTR_LABEL			(c, "overcolor", 0, "Overload Signal Color");
	CLASS_ATTR_ORDER			(c, "overcolor", 0, "8");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "overcolor", 0, "1. 0. 0. 0.8");
	
    class_register(CLASS_NOBOX, c);
	vu_class = c;
    
    post("PdChocolate by Pierre Guillot - CICM | Université Paris 8");
    post("PdChocolate has been elaborated with the PdEnhanced Library");
}

void *vu_new(t_symbol *s, int argc, t_atom *argv)
{
	t_vu *x =  NULL;
	t_dictionary *d;
	long flags;
	if (!(d = object_dictionaryarg(argc,argv)))
		return NULL;
    
	x = (t_vu *)ebox_alloc(vu_class);
    
	flags = 0
    | JBOX_DRAWFIRSTIN
    | JBOX_DRAWINLAST
    | JBOX_TRANSPARENT
    | JBOX_DRAWBACKGROUND
    | JBOX_GROWY
    ;
    
	ebox_new((t_ebox *)x, 0, argc, argv);
	x->j_box.b_firstin = (t_object *)x;
    
    ebox_dspsetup((t_ebox *)x, 1, 0);
    
    x->f_direction      = 0;
    x->f_peaks_outlet   = floatout(x);
    x->f_peak_value     = -90.;
    x->f_clock          = clock_new(x,(t_method)vu_tick);
	x->f_startclock     = 0;
    x->f_over_led_preserved = 0;
	attr_dictionary_process(x, d);	
	ebox_ready((t_ebox *)x);
    
	return (x);
}

void vu_getdrawparams(t_vu *x, t_object *patcherview, t_edrawparams *params)
{
	params->d_borderthickness   = 2.;
	params->d_cornersize        = 2.;
    params->d_bordercolor       = x->f_color_border;
    params->d_boxfillcolor      = x->f_color_background;
}

void vu_oksize(t_vu *x, t_rect *newrect)
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

void vu_dsp(t_vu *x, t_object *dsp, short *count, double samplerate, long maxvectorsize, long flags)
{
    object_method(dsp, gensym("dsp_add"), x, (method)vu_perform, 0, NULL);
	x->f_startclock = 1;
}

void vu_perform(t_vu *x, t_object *dsp, float **ins, long ni, float **outs, long no, long nsamples, long f,void *up)
{
    float peak = fabs(ins[0][0]);
    for(int i = 1; i < nsamples; i++)
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

void vu_tick(t_vu *x)
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
    
    vu_output(x);
    
	ebox_invalidate_layer((t_object *)x, NULL, gensym("leds_layer"));
	ebox_redraw((t_jbox *)x);
    
	if(sys_getdspstate())
		clock_delay(x->f_clock, x->f_interval);
}

void vu_output(t_vu *x)
{
    outlet_float((t_outlet*)x->f_peaks_outlet, (float)x->f_peak_value);
}

void vu_free(t_vu *x)
{
	dsp_freejbox((t_jbox *)x);
    clock_free(x->f_clock);
}

void vu_assist(t_vu *x, void *b, long m, long a, char *s)
{
	;
}

t_pd_err vu_notify(t_vu *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	if (msg == gensym("attr_modified"))
	{
		if(s == gensym("bgcolor") || s == gensym("bdcolor"))
		{
			ebox_invalidate_layer((t_object *)x, NULL, gensym("background_layer"));
		}
		else if( s == gensym("coldcolor") || s == gensym("tepidcolor") || s == gensym("warmcolor") || s == gensym("hotcolor") || s == gensym("overcolor"))
		{
			ebox_invalidate_layer((t_object *)x, NULL, gensym("background_layer"));
			ebox_invalidate_layer((t_object *)x, NULL, gensym("leds_layer"));
		}
		ebox_redraw((t_jbox *)x);
	}
	return 0;
}

void vu_paint(t_vu *x, t_object *view)
{
	t_rect rect;
	ebox_get_rect_for_view((t_object *)x, view, &rect);
	draw_background(x, view, &rect);
    draw_leds(x, view, &rect);
}

void draw_background(t_vu *x,  t_object *view, t_rect *rect)
{
	t_elayer *g = ebox_start_layer((t_object *)x, view, gensym("background_layer"), rect->width, rect->height);
 
	if (g)
	{
        egraphics_set_source_jrgba(g, &x->f_color_border);
        if(!x->f_direction)
        {
            for(int i = 1; i < 13; i++)
            {
                egraphics_move_to(g, 0., i * rect->height / 13.f);
                egraphics_line_to(g, rect->width, i * rect->height / 13.f);
                egraphics_stroke(g);
            }
        }
        else
        {
            for(int i = 1; i < 13; i++)
            {
                egraphics_move_to(g, i * rect->width / 13.f, 0.f);
                egraphics_line_to(g, i * rect->width / 13.f, rect->height);
                egraphics_stroke(g);
            }
        }
		ebox_end_layer((t_object*)x, view, gensym("background_layer"));
	}
	ebox_paint_layer((t_object *)x, view, gensym("background_layer"), 0.f, 0.f);
}


void draw_leds(t_vu *x, t_object *view, t_rect *rect)
{
	t_elayer *g = ebox_start_layer((t_object *)x, view, gensym("leds_layer"), rect->width, rect->height);
	
	if (g)
	{
        float led_height = rect->height / 13.f;
        float led_width = rect->width / 13.f;
        for(float i = 12, dB = -39; i > 0; i--, dB += 3.f)
        {
            if(x->f_peak_value >= dB)
            {
                if(i > 9)
                    egraphics_set_source_jrgba(g, &x->f_color_signal_cold);
                else if(i > 6)
                    egraphics_set_source_jrgba(g, &x->f_color_signal_tepid);
                else if(i > 3)
                    egraphics_set_source_jrgba(g, &x->f_color_signal_warm);
                else if(i > 0)
                    egraphics_set_source_jrgba(g, &x->f_color_signal_hot);
                if(!x->f_direction)
                {
                    egraphics_rectangle_rounded(g, 1, i * led_height + 1, rect->width - 2, led_height - 1, 1.f);
                }
                else
                {
                    egraphics_rectangle_rounded(g, (12 - i) * led_width + 1, 1, led_width - 1, rect->height - 2, 1.);
                }
                egraphics_fill(g);
            }
        }
        if(x->f_over_led_preserved)
        {
            egraphics_set_source_jrgba(g, &x->f_color_signal_over);
            if(!x->f_direction)
            {
                egraphics_rectangle_rounded(g, 1, 1, rect->width - 1, led_height - 1, 1.f);
            }
            else
            {
                egraphics_rectangle_rounded(g, 12 * led_width + 1, 1, led_width - 1,  rect->height - 1, 1.);

            }
            
            egraphics_fill(g);
        }
		ebox_end_layer((t_object*)x, view, gensym("leds_layer"));
	}
	ebox_paint_layer((t_object *)x, view, gensym("leds_layer"), 0., 0.);
}



