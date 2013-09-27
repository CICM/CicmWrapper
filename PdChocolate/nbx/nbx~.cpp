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

typedef struct  _nbx
{
	t_jbox      j_box;
	
	t_clock*	f_clock;
	int			f_startclock;
	long        f_interval;
    void*       f_peaks_outlet;
    float       f_peak_value;
	
	t_jrgba		f_color_background;
	t_jrgba		f_color_border;
	t_jrgba		f_color_text;
	
} t_nbx;

t_eclass *nbx_class;

extern "C" void nbx_tilde_setup(void);
void *nbx_new(t_symbol *s, int argc, t_atom *argv);
void nbx_free(t_nbx *x);
void nbx_assist(t_nbx *x, void *b, long m, long a, char *s);

void nbx_dsp(t_nbx *x, t_object *dsp, short *count, double samplerate, long maxvectorsize, long flags);
void nbx_perform(t_nbx *x, t_object *d, float **ins, long ni, float **outs, long no, long sf, long f,void *up);
void nbx_tick(t_nbx *x);
void nbx_output(t_nbx *x);

t_max_err nbx_notify(t_nbx *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

void nbx_getdrawparams(t_nbx *x, t_object *patcherview, t_jboxdrawparams *params);
void nbx_paint(t_nbx *x, t_object *view);
void draw_value(t_nbx *x,  t_object *view, t_rect *rect);

extern "C" void nbx_tilde_setup(void)
{
	t_eclass *c;
    
	c = class_new("nbx~", (method)nbx_new, (method)nbx_free, (short)sizeof(t_nbx), 0L, A_GIMME, 0);
    
	class_dspinitjbox(c);
	jbox_initclass(c, JBOX_COLOR | JBOX_FIXWIDTH);
	
	class_addmethod(c, (method) nbx_dsp,             "dsp",              A_CANT, 0);
	class_addmethod(c, (method) nbx_assist,          "assist",           A_CANT, 0);
	class_addmethod(c, (method) nbx_paint,           "paint",            A_CANT, 0);
	class_addmethod(c, (method) nbx_notify,          "notify",           A_CANT, 0);
    class_addmethod(c, (method) nbx_getdrawparams,   "getdrawparams",    A_CANT, 0);
    
	CLASS_ATTR_DEFAULT			(c, "patching_rect", 0, "0 0 30 15");
	CLASS_ATTR_INVISIBLE		(c, "color", 0);
        
    CLASS_ATTR_LONG				(c, "interval", 0, t_nbx, f_interval);
	CLASS_ATTR_ORDER			(c, "interval", 0, "5");
	CLASS_ATTR_LABEL			(c, "interval", 0, "Refresh Interval in Milliseconds");
	CLASS_ATTR_FILTER_MIN		(c, "interval", 20);
	CLASS_ATTR_DEFAULT			(c, "interval", 0, "50");
	CLASS_ATTR_SAVE				(c, "interval", 1);
	
	CLASS_ATTR_RGBA				(c, "bgcolor", 0, t_nbx, f_color_background);
	CLASS_ATTR_LABEL			(c, "bgcolor", 0, "Background Color");
	CLASS_ATTR_ORDER			(c, "bgcolor", 0, "1");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "bgcolor", 0, "1. 1 1 1.");
	
	CLASS_ATTR_RGBA				(c, "bordercolor", 0, t_nbx, f_color_border);
	CLASS_ATTR_LABEL			(c, "bordercolor", 0, "Box Border Color");
	CLASS_ATTR_ORDER			(c, "bordercolor", 0, "3");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "bordercolor", 0, "0.25 0.25 0.25 1.");
	
	CLASS_ATTR_RGBA				(c, "textcolor", 0, t_nbx, f_color_text);
	CLASS_ATTR_LABEL			(c, "textcolor", 0, "Text Color");
	CLASS_ATTR_ORDER			(c, "textcolor", 0, "4");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "textcolor", 0, "0. 0. 0. 1.");
	
	
    class_register(CLASS_NOBOX, c);
	nbx_class = c;
    
    post("PdChocolate by Pierre Guillot - CICM | Université Paris 8");
    post("PdChocolate has been elaborated with the PdEnhanced Library");
}

void *nbx_new(t_symbol *s, int argc, t_atom *argv)
{
	t_nbx *x =  NULL;
	t_dictionary *d;
	long flags;
	if (!(d = object_dictionaryarg(argc,argv)))
		return NULL;
    
	x = (t_nbx *)object_alloc(nbx_class);
    
	flags = 0
    | JBOX_DRAWFIRSTIN
    | JBOX_DRAWINLAST
    | JBOX_TRANSPARENT
    | JBOX_DRAWBACKGROUND
    | JBOX_GROWY
    ;
    
	jbox_new((t_jbox *)x, 0, argc, argv);
	x->j_box.b_firstin = (t_object *)x;
    
	dsp_setupjbox((t_jbox *)x, 1, 1);
    
    x->f_peaks_outlet   = floatout(x);
    x->f_peak_value     = 0.;
    x->f_clock          = clock_new(x,(t_method)nbx_tick);
	x->f_startclock     = 0;
    
	attr_dictionary_process(x, d);	
	jbox_ready((t_jbox *)x);
    
	return (x);
}

void nbx_getdrawparams(t_nbx *x, t_object *patcherview, t_jboxdrawparams *params)
{
	params->d_borderthickness   = 1;
	params->d_cornersize        = 8;
    params->d_bordercolor       = x->f_color_border;
    params->d_boxfillcolor      = x->f_color_background;
}

void nbx_dsp(t_nbx *x, t_object *dsp, short *count, double samplerate, long maxvectorsize, long flags)
{
    object_method(dsp, gensym("dsp_add"), x, (method)nbx_perform, 0, NULL);
	x->f_startclock = 1;
}

void nbx_perform(t_nbx *x, t_object *dsp, float **ins, long ni, float **outs, long no, long nsamples, long f,void *up)
{
    for(int i = 0; i < nsamples; i++)
    {
        x->f_peak_value = ins[0][i];
        outs[0][i] = ins[0][i];
    }

	if (x->f_startclock)
	{
		x->f_startclock = 0;
		clock_delay(x->f_clock, 0);
	}
}

void nbx_tick(t_nbx *x)
{    
    nbx_output(x);
    
	jbox_invalidate_layer((t_object *)x, NULL, gensym("value_layer"));
	jbox_redraw((t_jbox *)x);
    
	if(sys_getdspstate())
		clock_delay(x->f_clock, x->f_interval);
}

void nbx_output(t_nbx *x)
{
    outlet_float((t_outlet*)x->f_peaks_outlet, (float)x->f_peak_value);
}

void nbx_free(t_nbx *x)
{
	dsp_freejbox((t_jbox *)x);
    clock_free(x->f_clock);
}

void nbx_assist(t_nbx *x, void *b, long m, long a, char *s)
{
	;
}

t_max_err nbx_notify(t_nbx *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	if (msg == gensym("attr_modified"))
	{
		if(s == gensym("mbgcolor") || s == gensym("leds_bg") || s == gensym("drawmborder"))
		{
			jbox_invalidate_layer((t_object *)x, NULL, gensym("background_layer"));
		}
		else if(s == gensym("cicolor") || s == gensym("coldcolor") || s == gensym("tepidcolor") || s == gensym("warmcolor") || s == gensym("hotcolor") || s == gensym("overcolor") || s == gensym("numleds"))
		{
			jbox_invalidate_layer((t_object *)x, NULL, gensym("background_layer"));
			jbox_invalidate_layer((t_object *)x, NULL, gensym("value_layer"));
		}
		else if(s == gensym("dbperled") || s == gensym("nhotleds") || s == gensym("ntepidleds") || s == gensym("nwarmleds"))
		{
			jbox_invalidate_layer((t_object *)x, NULL, gensym("value_layer"));
		}
		jbox_redraw((t_jbox *)x);
	}
	return 0;
}

void nbx_paint(t_nbx *x, t_object *view)
{
	t_rect rect;
	jbox_get_rect_for_view((t_object *)x, view, &rect);
    draw_value(x, view, &rect);
}

void draw_value(t_nbx *x, t_object *view, t_rect *rect)
{
	t_jgraphics *g = jbox_start_layer((t_object *)x, view, gensym("value_layer"), rect->width, rect->height);
	t_etextlayout *jtl = jtextlayout_create();
	if (g && jtl)
	{
        char number[256];
        sprintf(number, "%f", x->f_peak_value);
        jtextlayout_set(jtl, number, &x->j_box.e_font, 5, 8., 0, 0, ETEXT_LEFT, ETEXT_NOWRAP);
        jtextlayout_draw(jtl, g);
		jbox_end_layer((t_object*)x, view, gensym("value_layer"));
	}
	jbox_paint_layer((t_object *)x, view, gensym("value_layer"), 0., 0.);
}



