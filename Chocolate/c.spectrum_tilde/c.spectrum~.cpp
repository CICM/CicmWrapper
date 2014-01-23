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
#include "../../../PdEnhanced/Sources/efft.h"
}

#define FFT_SIZE 1024.f
#define ARR_SIZE 512.f

typedef struct  _spectrum
{
	t_edspbox   j_box;
	
	t_clock*	f_clock;
	int			f_startclock;
	long        f_interval;
    void*       f_peaks_outlet;
	
    long        f_inc;
    long        f_logfreq;
    t_sample*   f_samples;
    t_sample*   f_real;
    t_sample*   f_imag;
    t_sample*   f_spec;
    t_sample*   f_winbuf;
    
    t_symbol*   f_amplitude;
    int         f_amp;
    
    t_symbol*   f_window;

	t_rgba		f_color_background;
	t_rgba		f_color_border;
    t_rgba		f_color_spectrum;
	
} t_spectrum;

t_eclass *spectrum_class;

void *spectrum_new(t_symbol *s, int argc, t_atom *argv);
void spectrum_free(t_spectrum *x);
void spectrum_assist(t_spectrum *x, void *b, long m, long a, char *s);

void spectrum_dsp(t_spectrum *x, t_object *dsp, short *count, double samplerate, long maxvectorsize, long flags);
void spectrum_perform(t_spectrum *x, t_object *d, float **ins, long ni, float **outs, long no, long sf, long f,void *up);
void spectrum_tick(t_spectrum *x);
void spectrum_output(t_spectrum *x);

t_pd_err spectrum_notify(t_spectrum *x, t_symbol *s, t_symbol *msg, void *sender, void *data);
t_pd_err spectrum_amplitude_set(t_spectrum *x, t_object *attr, long ac, t_atom *av);
t_pd_err spectrum_window_set(t_spectrum *x, t_object *attr, long ac, t_atom *av);
t_pd_err spectrum_fftsize_set(t_spectrum *x, t_object *attr, long ac, t_atom *av);

void spectrum_getdrawparams(t_spectrum *x, t_object *patcherview, t_edrawparams *params);
void spectrum_oksize(t_spectrum *x, t_rect *newrect);

void spectrum_paint(t_spectrum *x, t_object *view);
void draw_background(t_spectrum *x,  t_object *view, t_rect *rect);
void draw_leds(t_spectrum *x,  t_object *view, t_rect *rect);

extern "C" void setup_c0x2espectrum_tilde(void)
{
	t_eclass *c;
    
	c = eclass_new("c.spectrum~", (method)spectrum_new, (method)spectrum_free, (short)sizeof(t_spectrum), 0L, A_GIMME, 0);

	eclass_dspinit(c);
	eclass_init(c, 0);
	
	eclass_addmethod(c, (method) spectrum_dsp,             "dsp",              A_CANT, 0);
	eclass_addmethod(c, (method) spectrum_assist,          "assist",           A_CANT, 0);
	eclass_addmethod(c, (method) spectrum_paint,           "paint",            A_CANT, 0);
	eclass_addmethod(c, (method) spectrum_notify,          "notify",           A_CANT, 0);
    eclass_addmethod(c, (method) spectrum_getdrawparams,   "getdrawparams",    A_CANT, 0);
    eclass_addmethod(c, (method) spectrum_oksize,          "oksize",           A_CANT, 0);
    
    CLASS_ATTR_INVISIBLE            (c, "fontname", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontweight", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontslant", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontsize", 1);
	CLASS_ATTR_DEFAULT              (c, "size", 0, "250 100");
    
    CLASS_ATTR_SYMBOL               (c, "window", 0, t_spectrum, f_window);
    CLASS_ATTR_ACCESSORS			(c, "window", NULL, spectrum_window_set);
	CLASS_ATTR_ORDER                (c, "window", 0, "2");
	CLASS_ATTR_LABEL                (c, "window", 0, "Window");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "window", 0, "Hanning");
    CLASS_ATTR_ITEMS                (c, "window", 0, "Square Hanning Hamming Blackman");
    CLASS_ATTR_STYLE                (c, "window", 0, "menu");
    
    CLASS_ATTR_SYMBOL               (c, "amp", 0, t_spectrum, f_amplitude);
    CLASS_ATTR_ACCESSORS			(c, "amp", NULL, spectrum_amplitude_set);
	CLASS_ATTR_ORDER                (c, "amp", 0, "2");
	CLASS_ATTR_LABEL                (c, "amp", 0, "Amplitude");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "amp", 0, "Decibel");
    CLASS_ATTR_ITEMS                (c, "amp", 0, "Magnitude Power Decibel");
    CLASS_ATTR_STYLE                (c, "amp", 0, "menu");
    
    CLASS_ATTR_LONG                 (c, "logfreq", 0, t_spectrum, f_logfreq);
	CLASS_ATTR_ORDER                (c, "logfreq", 0, "2");
	CLASS_ATTR_LABEL                (c, "logfreq", 0, "Logarithmic Frequency Scale");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "logfreq", 0, "1");
    CLASS_ATTR_STYLE                (c, "logfreq", 0, "onoff");
    
    CLASS_ATTR_LONG                 (c, "interval", 0, t_spectrum, f_interval);
	CLASS_ATTR_ORDER                (c, "interval", 0, "1");
	CLASS_ATTR_LABEL                (c, "interval", 0, "Refresh Interval in Milliseconds");
	CLASS_ATTR_FILTER_MIN           (c, "interval", 20);
	CLASS_ATTR_DEFAULT              (c, "interval", 0, "50");
	CLASS_ATTR_SAVE                 (c, "interval", 1);
	CLASS_ATTR_STYLE                (c, "interval", 0, "number");
    
	CLASS_ATTR_RGBA                 (c, "bgcolor", 0, t_spectrum, f_color_background);
	CLASS_ATTR_LABEL                (c, "bgcolor", 0, "Background Color");
	CLASS_ATTR_ORDER                (c, "bgcolor", 0, "1");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bgcolor", 0, "0.75 0.75 0.75 1.");
	CLASS_ATTR_STYLE                (c, "bgcolor", 0, "color");
    
	CLASS_ATTR_RGBA                 (c, "bdcolor", 0, t_spectrum, f_color_border);
	CLASS_ATTR_LABEL                (c, "bdcolor", 0, "Border Color");
	CLASS_ATTR_ORDER                (c, "bdcolor", 0, "2");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bdcolor", 0, "0.5 0.5 0.5 1.");
	CLASS_ATTR_STYLE                (c, "bdcolor", 0, "color");
    
    CLASS_ATTR_RGBA                 (c, "spcolor", 0, t_spectrum, f_color_spectrum);
	CLASS_ATTR_LABEL                (c, "spcolor", 0, "Spectrum Color");
	CLASS_ATTR_ORDER                (c, "spcolor", 0, "2");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "spcolor", 0, "0.16995 0.355709 0.501945 1.");
	CLASS_ATTR_STYLE                (c, "spcolor", 0, "color");
    
    eclass_register(CLASS_BOX, c);
    cicm_post();
	spectrum_class = c;
}

void *spectrum_new(t_symbol *s, int argc, t_atom *argv)
{
	t_spectrum *x =  NULL;
	t_binbuf* d;
    long flags;
	if (!(d = binbuf_via_atoms(argc,argv)))
		return NULL;
    
	x = (t_spectrum *)eobj_new(spectrum_class);

    x->f_real       = (t_sample *)calloc(4096, sizeof(t_sample));
    x->f_imag       = (t_sample *)calloc(4096, sizeof(t_sample));
    x->f_spec       = (t_sample *)calloc(4096, sizeof(t_sample));
    x->f_samples    = (t_sample *)calloc(4096, sizeof(t_sample));
    x->f_winbuf     = (t_sample *)calloc(4096, sizeof(t_sample));
    ewindow_hanning(x->f_winbuf, FFT_SIZE);
    flags = 0
    | EBOX_GROWINDI
    | EBOX_IGNORELOCKCLICK
    ;
    
	ebox_new((t_ebox *)x, flags);
    eobj_dspsetup((t_ebox *)x, 1, 0);
    
    x->f_peaks_outlet   = floatout(x);
    x->f_clock          = clock_new(x,(t_method)spectrum_tick);
	x->f_startclock     = 0;
    ebox_attrprocess_viabinbuf(x, d);
	ebox_ready((t_ebox *)x);
    x->f_inc = 0;
	return (x);
}

void spectrum_getdrawparams(t_spectrum *x, t_object *patcherview, t_edrawparams *params)
{
	params->d_borderthickness   = 2.;
	params->d_cornersize        = 2.;
    params->d_bordercolor       = x->f_color_border;
    params->d_boxfillcolor      = x->f_color_background;
}

void spectrum_oksize(t_spectrum *x, t_rect *newrect)
{
    newrect->width = pd_clip_min(newrect->width, 8.);
    newrect->height = pd_clip_min(newrect->height, 8.);
}

void spectrum_dsp(t_spectrum *x, t_object *dsp, short *count, double samplerate, long maxvectorsize, long flags)
{
    x->f_inc = 0;
    object_method(dsp, gensym("dsp_add"), x, (method)spectrum_perform, 0, NULL);
	x->f_startclock = 1;
}

void spectrum_perform(t_spectrum *x, t_object *dsp, float **ins, long ni, float **outs, long no, long nsamples, long f,void *up)
{
	int i;
    for(i = 0; i < nsamples; i++)
    {
        x->f_samples[x->f_inc] = ins[0][i];
        if(x->f_inc++ >= FFT_SIZE*3)
            x->f_inc = 0;
    }

	if (x->f_startclock)
	{
		x->f_startclock = 0;
		clock_delay(x->f_clock, 0);
	}
}

void spectrum_tick(t_spectrum *x)
{
    spectrum_output(x);
    if(x->f_inc > FFT_SIZE)
    {
        memcpy(x->f_real, x->f_samples, FFT_SIZE * sizeof(t_sample));
    }
    else
    {
        memcpy(x->f_real, x->f_samples+(int)FFT_SIZE, FFT_SIZE * sizeof(t_sample));
    }
    for(int i = 0; i < FFT_SIZE; i++)
        x->f_real[i] = (x->f_real[i] * x->f_winbuf[i]) / (float)ARR_SIZE;
    memset(x->f_imag, 0, FFT_SIZE * sizeof(t_sample));
    mayer_fft(FFT_SIZE, x->f_real, x->f_imag);
    
    if(x->f_amp == 0)
        espectrum_magnitude(x->f_spec, x->f_real, x->f_imag, ARR_SIZE);
    else if(x->f_amp == 1)
        espectrum_power(x->f_spec, x->f_real, x->f_imag,  ARR_SIZE);
    else
    {
        espectrum_decibel(x->f_spec, x->f_real, x->f_imag,  ARR_SIZE);
        for(int i = 0; i < ARR_SIZE; i++)
            x->f_spec[i] = (90.f + x->f_spec[i]) / 90.f;
    }

   	ebox_invalidate_layer((t_ebox *)x, gensym("spectrum_layer"));
	ebox_redraw((t_ebox *)x);
    
	if(sys_getdspstate())
		clock_delay(x->f_clock, x->f_interval);
}

void spectrum_output(t_spectrum *x)
{
    ;
}

void spectrum_free(t_spectrum *x)
{
	ebox_free((t_ebox *)x);
    clock_free(x->f_clock);
    free(x->f_samples);
    free(x->f_real);
    free(x->f_imag);
    free(x->f_spec);
    free(x->f_winbuf);
}

void spectrum_assist(t_spectrum *x, void *b, long m, long a, char *s)
{
	;
}

t_pd_err spectrum_notify(t_spectrum *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	if (msg == gensym("attr_modified"))
	{
		if(s == gensym("bgcolor") || s == gensym("bdcolor") || s == gensym("logfreq"))
		{
			ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
		}
        if(s == gensym("amp") || s == gensym("window") || s == gensym("logfreq"))
		{
			ebox_invalidate_layer((t_ebox *)x, gensym("spectrum_layer"));
		}
		ebox_redraw((t_ebox *)x);
	}
	return 0;
}

void spectrum_paint(t_spectrum *x, t_object *view)
{
	t_rect rect;
	ebox_get_rect_for_view((t_ebox *)x, &rect);
	draw_background(x, view, &rect);
    draw_leds(x, view, &rect);
}

void draw_background(t_spectrum *x,  t_object *view, t_rect *rect)
{
    int i;
	t_elayer *g = ebox_start_layer((t_ebox *)x, gensym("background_layer"), rect->width, rect->height);
 
	if (g)
	{
        egraphics_set_color_rgba(g, &x->f_color_border);
        egraphics_set_line_width(g, 2.);
        for(i = 1; i < 4; i++)
        {
            if(!x->f_logfreq)
                egraphics_line_fast(g, rect->width * (i / 4.f), 0., rect->width * (i / 4.f), rect->height);
            else
                egraphics_line_fast(g, log10((float)(i / 4.f) * 39.f + 1.f) / log10(40.0f) * rect->width, 0.,  log10((float)(i / 4.f) * 39.f + 1.f) / log10(40.0f) * rect->width, rect->height);
        }
		ebox_end_layer((t_ebox*)x, gensym("background_layer"));
	}
	ebox_paint_layer((t_ebox *)x, gensym("background_layer"), 0.f, 0.f);
}


void draw_leds(t_spectrum *x, t_object *view, t_rect *rect)
{
    float x0, y0;
	t_elayer *g = ebox_start_layer((t_ebox *)x, gensym("spectrum_layer"), rect->width, rect->height);
	
	if (g)
	{
        egraphics_set_color_rgba(g, &x->f_color_spectrum);
        egraphics_move_to(g, 0, rect->height - (x->f_spec[0] * rect->height));
        for(int i = 2; i < ARR_SIZE; i++)
        {
            if(!x->f_logfreq)
                x0 = ((float)(i - 1) / (float)ARR_SIZE) * rect->width;
            else
                x0 = log10(((float)(i - 1) / (float)ARR_SIZE) * 39.f + 1.f) / log10(40.0f) * rect->width;
            y0 = rect->height - (x->f_spec[i] * rect->height) + 2;
            egraphics_line_to(g, x0, y0);
        }
        egraphics_line_to(g, rect->width, rect->height - (x->f_spec[(int)ARR_SIZE-1] * rect->height) + 2);
        egraphics_line_to(g, rect->width, rect->height + 2);
        egraphics_line_to(g, 0, rect->height + 2);
        egraphics_close_path(g);
        egraphics_fill(g);
		ebox_end_layer((t_ebox *)x, gensym("spectrum_layer"));
	}
	ebox_paint_layer((t_ebox *)x, gensym("spectrum_layer"), 0., 0.);
}

t_pd_err spectrum_amplitude_set(t_spectrum *x, t_object *attr, long ac, t_atom *av)
{
    if(ac && av)
    {
        if(atom_gettype(av) == A_SYM)
        {
            if(atom_getsym(av) == gensym("Magnitude") || atom_getsym(av) == gensym("magnitude"))
            {
                x->f_amplitude = gensym("Magnitude");
                x->f_amp = 0;
            }
            else if(atom_getsym(av) == gensym("Power") || atom_getsym(av) == gensym("power"))
            {
                x->f_amplitude = gensym("Power");
                x->f_amp = 1;
            }
            else
            {
                x->f_amplitude = gensym("Decibel");
                x->f_amp = 2;
            }
        }
        else if(atom_gettype(av) == A_FLOAT)
        {
            if(atom_getfloat(av) < 1)
            {
                x->f_amplitude = gensym("Magnitude");
                x->f_amp = 0;
            }
            else if(atom_getfloat(av) < 2)
            {
                x->f_amplitude = gensym("Power");
                x->f_amp = 1;
            }
            else
            {
                x->f_amplitude = gensym("Decibel");
                x->f_amp = 2;
            }
        }
    }
    return 0;
}

t_pd_err spectrum_window_set(t_spectrum *x, t_object *attr, long ac, t_atom *av)
{
    if(ac && av)
    {
        if(atom_gettype(av) == A_SYM)
        {
            if(atom_getsym(av) == gensym("Square") || atom_getsym(av) == gensym("square"))
            {
                x->f_window = gensym("Square");
                ewindow_square(x->f_winbuf, FFT_SIZE);
            }
            else if(atom_getsym(av) == gensym("Hanning") || atom_getsym(av) == gensym("hanning"))
            {
                x->f_window = gensym("Hanning");
                ewindow_hanning(x->f_winbuf, FFT_SIZE);
            }
            else if(atom_getsym(av) == gensym("Hamming") || atom_getsym(av) == gensym("hamming"))
            {
                x->f_window = gensym("Hamming");
                ewindow_hamming(x->f_winbuf, FFT_SIZE);
            }
            else if(atom_getsym(av) == gensym("Blackman") || atom_getsym(av) == gensym("blackman"))
            {
                x->f_window = gensym("Blackman");
                ewindow_blackman(x->f_winbuf, FFT_SIZE);
            }
            else
            {
                x->f_window = gensym("Hanning");
                ewindow_hanning(x->f_winbuf, FFT_SIZE);
            }
        }
        else if(atom_gettype(av) == A_FLOAT)
        {
            if(atom_getfloat(av) < 1)
            {
                x->f_window = gensym("Square");
                ewindow_square(x->f_winbuf, FFT_SIZE);
            }
            else if(atom_getfloat(av) < 2)
            {
                x->f_window = gensym("Hanning");
                ewindow_hanning(x->f_winbuf, FFT_SIZE);
            }
            else if(atom_getfloat(av) < 3)
            {
                x->f_window = gensym("Hanning");
                ewindow_hamming(x->f_winbuf, FFT_SIZE);
            }
            else
            {
                x->f_window = gensym("Blackman");
                ewindow_blackman(x->f_winbuf, FFT_SIZE);
            }
        }
        
    }
    return 0;
}

