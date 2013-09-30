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

typedef struct  _scope
{
	t_jbox      j_box;
	
	t_clock*	f_clock;
	int			f_startclock;
	long        f_interval;
    float       f_sample_rate;
    long        f_number_of_samples;
    int         f_mode;
    
	float*      f_buffer_x;
    float*      f_buffer_y;
    int         f_index;
    
	t_jrgba		f_color_background;
	t_jrgba		f_color_border;

	t_jrgba		f_color_signal;
    
} t_scope;

t_eclass *scope_class;

extern "C" void scope_tilde_setup(void);
void *scope_new(t_symbol *s, int argc, t_atom *argv);
void scope_free(t_scope *x);
void scope_assist(t_scope *x, void *b, long m, long a, char *s);

void scope_dsp(t_scope *x, t_object *dsp, short *count, double samplerate, long maxvectorsize, long flags);
void scope_performX(t_scope *x, t_object *d, float **ins, long ni, float **outs, long no, long sf, long f,void *up);
void scope_performXY(t_scope *x, t_object *d, float **ins, long ni, float **outs, long no, long sf, long f,void *up);
void scope_tick(t_scope *x);

t_max_err scope_notify(t_scope *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

void scope_getdrawparams(t_scope *x, t_object *patcherview, t_jboxdrawparams *params);
void scope_oksize(t_scope *x, t_rect *newrect);

void scope_paint(t_scope *x, t_object *view);
void draw_background(t_scope *x,  t_object *view, t_rect *rect);
void draw_signals(t_scope *x,  t_object *view, t_rect *rect);

extern "C" void scope_tilde_setup(void)
{
	t_eclass *c;
    
	c = class_new("scope~", (method)scope_new, (method)scope_free, (short)sizeof(t_scope), 0L, A_GIMME, 0);
    
	class_dspinitjbox(c);
	jbox_initclass(c, JBOX_COLOR | JBOX_FIXWIDTH);
	
	class_addmethod(c, (method) scope_dsp,             "dsp",              A_CANT, 0);
	class_addmethod(c, (method) scope_assist,          "assist",           A_CANT, 0);
	class_addmethod(c, (method) scope_paint,           "paint",            A_CANT, 0);
	class_addmethod(c, (method) scope_notify,          "notify",           A_CANT, 0);
    class_addmethod(c, (method) scope_getdrawparams,   "getdrawparams",    A_CANT, 0);
    
	CLASS_ATTR_DEFAULT              (c, "patching_rect", 0, "0 0 125 125");
	CLASS_ATTR_INVISIBLE            (c, "color", 0);

    CLASS_ATTR_LONG                 (c, "interval", 0, t_scope, f_interval);
	CLASS_ATTR_ORDER                (c, "interval", 0, "5");
	CLASS_ATTR_LABEL                (c, "interval", 0, "Refresh Interval in Milliseconds");
	CLASS_ATTR_FILTER_CLIP          (c, "interval", 512, 16384);
	CLASS_ATTR_DEFAULT              (c, "interval", 0, "2048");
	CLASS_ATTR_SAVE                 (c, "interval", 1);
    
    CLASS_ATTR_LONG                 (c, "buffersize", 0, t_scope, f_number_of_samples);
	CLASS_ATTR_ORDER                (c, "buffersize", 0, "5");
	CLASS_ATTR_LABEL                (c, "buffersize", 0, "Buffer size");
	CLASS_ATTR_FILTER_CLIP          (c, "buffersize", 4, 16384);
	CLASS_ATTR_DEFAULT              (c, "buffersize", 0, "128");
	CLASS_ATTR_SAVE                 (c, "buffersize", 1);
	
	CLASS_ATTR_RGBA                 (c, "bgcolor", 0, t_scope, f_color_background);
	CLASS_ATTR_LABEL                (c, "bgcolor", 0, "Background Color");
	CLASS_ATTR_ORDER                (c, "bgcolor", 0, "1");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bgcolor", 0, "0.35 0.23 0.13 1.");
	
	CLASS_ATTR_RGBA                 (c, "bdcolor", 0, t_scope, f_color_border);
	CLASS_ATTR_LABEL                (c, "bdcolor", 0, "Box Border Color");
	CLASS_ATTR_ORDER                (c, "bdcolor", 0, "3");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bdcolor", 0, "0.27 0.21 0. 1");
	
	CLASS_ATTR_RGBA                 (c, "sigcolor", 0, t_scope, f_color_signal);
	CLASS_ATTR_LABEL                (c, "sigcolor", 0, "Signal Color");
	CLASS_ATTR_ORDER                (c, "sigcolor", 0, "4");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "sigcolor", 0, "0. 1. 0. 1.");
    
    class_register(CLASS_NOBOX, c);
	scope_class = c;
    
    post("PdChocolate by Pierre Guillot - CICM | Université Paris 8");
    post("PdChocolate has been elaborated with the PdEnhanced Library");
}

void *scope_new(t_symbol *s, int argc, t_atom *argv)
{
	t_scope *x =  NULL;
	t_dictionary *d;

	if (!(d = object_dictionaryarg(argc,argv)))
		return NULL;
    
	x = (t_scope *)object_alloc(scope_class);
    
	jbox_new((t_jbox *)x, 0, argc, argv);
	x->j_box.b_firstin = (t_object *)x;
    
	dsp_setupjbox((t_jbox *)x, 2, 0);
    x->f_clock          = clock_new(x,(t_method)scope_tick);
	x->f_startclock     = 0;
    x->f_sample_rate    = sys_getsr();
    
    x->f_buffer_x = new float[16384];
    x->f_buffer_y = new float[16384];
    x->f_mode     = 0;
    x->f_index = 0;
    for(int i = 0; i < 16384; i++)
    {
        x->f_buffer_x[i] = 0.;
        x->f_buffer_y[i] = 0.;
    }
	attr_dictionary_process(x, d);
	jbox_ready((t_jbox *)x);
    
	return (x);
}

void scope_getdrawparams(t_scope *x, t_object *patcherview, t_jboxdrawparams *params)
{
	params->d_borderthickness   = 2;
	params->d_cornersize        = 2;
    params->d_bordercolor       = x->f_color_border;
    params->d_boxfillcolor      = x->f_color_background;
}

void scope_dsp(t_scope *x, t_object *dsp, short *count, double samplerate, long maxvectorsize, long flags)
{
    x->f_sample_rate = samplerate;

    if(!count[1])
    {
        object_method(dsp, gensym("dsp_add"), x, (method)scope_performX, 0, NULL);
        x->f_mode = 0;
    }
    else
    {
        object_method(dsp, gensym("dsp_add"), x, (method)scope_performXY, 0, NULL);
        x->f_mode = 1;
    }
	x->f_startclock = 1;
}

void scope_performX(t_scope *x, t_object *dsp, float **ins, long ni, float **outs, long no, long nsamples, long f,void *up)
{
    for(int i = 0; i < nsamples || x->f_index < x->f_interval; i++)
    {
        x->f_buffer_x[x->f_index] = ins[0][i];
        x->f_index++;
    }
    
	if(x->f_startclock)
	{
		x->f_startclock = 0;
		clock_delay(x->f_clock, 0);
	}
}

void scope_performXY(t_scope *x, t_object *dsp, float **ins, long ni, float **outs, long no, long nsamples, long f,void *up)
{
    for(int i = 0; i < nsamples || x->f_index < x->f_interval; i++)
    {
        x->f_buffer_x[x->f_index] = ins[0][i];
        x->f_buffer_y[x->f_index] = ins[1][i];
        x->f_index++;
    }
    
	if(x->f_startclock)
	{
		x->f_startclock = 0;
		clock_delay(x->f_clock, 0);
	}
}

void scope_tick(t_scope *x)
{
    if(x->f_index >= x->f_interval)
    {
        jbox_invalidate_layer((t_object *)x, NULL, gensym("signal_layer"));
        jbox_redraw((t_jbox *)x);
    }
	if(sys_getdspstate())
		clock_delay(x->f_clock, x->f_interval / x->f_sample_rate * 1000.);
}

void scope_free(t_scope *x)
{
	dsp_freejbox((t_jbox *)x);
    clock_free(x->f_clock);
    free(x->f_buffer_x);
    free(x->f_buffer_y);
}

void scope_assist(t_scope *x, void *b, long m, long a, char *s)
{
	;
}

t_max_err scope_notify(t_scope *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	if (msg == gensym("attr_modified"))
	{
		if(s == gensym("bgcolor") || s == gensym("bdcolor") || s == gensym("sigcolor"))
		{
			jbox_invalidate_layer((t_object *)x, NULL, gensym("background_layer"));
            jbox_invalidate_layer((t_object *)x, NULL, gensym("signal_layer"));
		}
		jbox_redraw((t_jbox *)x);
	}
	return 0;
}

void scope_paint(t_scope *x, t_object *view)
{
	t_rect rect;
	ebox_get_rect_for_view((t_object *)x, view, &rect);
	draw_background(x, view, &rect);
    draw_signals(x, view, &rect);
}

void draw_background(t_scope *x,  t_object *view, t_rect *rect)
{
	t_egraphics *g = ebox_start_layer((t_object *)x, view, gensym("background_layer"), rect->width, rect->height);
 
	if (g)
	{
        for(int i = 0; i < 3; i++)
        {
            egraphics_move_to(g, 0., (i + 1) * rect->height / 4.);
            egraphics_line_to(g, rect->width, (i + 1) * rect->height / 4.);
            egraphics_stroke(g);
        }
        
        for(int i = 0; i < 6; i++)
        {
            egraphics_move_to(g, (i + 1) * rect->width / 7, 0);
            egraphics_line_to(g, (i + 1) * rect->width / 7, rect->height);
            egraphics_stroke(g);
        }
        
		ebox_end_layer((t_object*)x, view, gensym("background_layer"));
	}
	ebox_paint_layer((t_object *)x, view, gensym("background_layer"), 0., 0.);
}


void draw_signals(t_scope *x, t_object *view, t_rect *rect)
{
	t_egraphics *g = ebox_start_layer((t_object *)x, view, gensym("signal_layer"), rect->width, rect->height);
	
	if (g)
	{
        float center_x = rect->width * 0.5;
        float center_y = rect->height * 0.5;
        float increment = x->f_interval / (float)x->f_number_of_samples;
        egraphics_set_source_jrgba(g, &x->f_color_signal);
        egraphics_set_line_width(g, 2);
        
        if(!x->f_mode)
        {
            egraphics_move_to(g, 1, x->f_buffer_x[0] * (center_y - 1.) + center_y);
            for (float i = 1; i < x->f_interval; i += increment)
            {
                egraphics_line_to(g, 1 + (i / (float)x->f_interval) * (rect->width - 2),
                                  x->f_buffer_x[(int)i] * (center_y - 1.) + center_y);
            }
        }
        else
        {
            egraphics_move_to(g, 1 + x->f_buffer_y[0] * (center_x - 2) + center_x, x->f_buffer_x[0] * (center_y - 1.) + center_y);
            for (float i = 1; i < x->f_interval; i += increment)
            {
                egraphics_line_to(g, 1 + x->f_buffer_y[(int)i] * (center_x - 2) + center_x, x->f_buffer_x[(int)i] * (center_y - 1.) + center_y);
            }
        }
        x->f_index = 0;
        egraphics_stroke(g);
        ebox_end_layer((t_object*)x, view, gensym("signal_layer"));
	}
	ebox_paint_layer((t_object *)x, view, gensym("signal_layer"), 0., 0.);
    
}



