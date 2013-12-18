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
#include "../../../PdEnhanced/Sources/pd_enhanced.h"
}

#ifndef _WINDOWS
#define sprintf_s sprintf
#endif


typedef struct  _number
{
	t_ebox      j_box;

	t_clock*	f_clock;
	int			f_startclock;
    
	long        f_interval;
    long        f_ndecimal;
    void*       f_peaks_outlet;
    
    float       f_peak_value;
    int         f_max_decimal;
    
	t_rgba		f_color_background;
	t_rgba		f_color_border;
	t_rgba		f_color_text;
    
} t_number;

t_eclass *number_class;

void *number_new(t_symbol *s, int argc, t_atom *argv);
void number_free(t_number *x);
void number_assist(t_number *x, void *b, long m, long a, char *s);

void number_dsp(t_number *x, t_object *dsp, short *count, double samplerate, long maxvectorsize, long flags);
void number_perform(t_number *x, t_object *d, float **ins, long ni, float **outs, long no, long sf, long f,void *up);
void number_tick(t_number *x);
void number_output(t_number *x);
t_pd_err number_notify(t_number *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

void number_getdrawparams(t_number *x, t_object *patcherview, t_edrawparams *params);
void number_oksize(t_number *x, t_rect *newrect);

void number_paint(t_number *x, t_object *view);
void draw_background(t_number *x, t_object *view, t_rect *rect);
void draw_value(t_number *x,  t_object *view, t_rect *rect);

void number_preset(t_number *x, t_binbuf *b);

extern "C" void setup_c0x2enumber_tilde(void)
{
	t_eclass *c;
    
	c = eclass_new("c.number~", (method)number_new, (method)number_free, (short)sizeof(t_number), 0L, A_GIMME, 0);
    
	eclass_dspinit(c);
	eclass_init(c, 0);
	
	eclass_addmethod(c, (method) number_dsp,             "dsp",              A_CANT, 0);
	eclass_addmethod(c, (method) number_assist,          "assist",           A_CANT, 0);
	eclass_addmethod(c, (method) number_paint,           "paint",            A_CANT, 0);
	eclass_addmethod(c, (method) number_notify,          "notify",           A_CANT, 0);
    eclass_addmethod(c, (method) number_getdrawparams,   "getdrawparams",    A_CANT, 0);
    eclass_addmethod(c, (method) number_oksize,          "oksize",           A_CANT, 0);
    eclass_addmethod(c, (method) number_output,          "bang",             A_CANT, 0);
    
	CLASS_ATTR_DEFAULT			(c, "size", 0, "53 13");
    
    CLASS_ATTR_LONG				(c, "interval", 0, t_number, f_interval);
	CLASS_ATTR_ORDER			(c, "interval", 0, "2");
	CLASS_ATTR_LABEL			(c, "interval", 0, "Refresh Interval in Milliseconds");
	CLASS_ATTR_FILTER_MIN		(c, "interval", 20);
	CLASS_ATTR_DEFAULT			(c, "interval", 0, "50");
	CLASS_ATTR_SAVE				(c, "interval", 1);
	
    CLASS_ATTR_LONG				(c, "decimal", 0, t_number, f_ndecimal);
	CLASS_ATTR_ORDER			(c, "decimal", 0, "3");
	CLASS_ATTR_LABEL			(c, "decimal", 0, "Number of decimal");
    CLASS_ATTR_DEFAULT          (c, "decimal", 0, "6");
	CLASS_ATTR_FILTER_MIN		(c, "decimal", 0);
    CLASS_ATTR_FILTER_MAX		(c, "decimal", 6);
	CLASS_ATTR_SAVE				(c, "decimal", 1);
    
	CLASS_ATTR_RGBA                 (c, "bgcolor", 0, t_number, f_color_background);
	CLASS_ATTR_LABEL                (c, "bgcolor", 0, "Background Color");
	CLASS_ATTR_ORDER                (c, "bgcolor", 0, "1");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bgcolor", 0, "0.75 0.75 0.75 1.");
	
	CLASS_ATTR_RGBA                 (c, "bdcolor", 0, t_number, f_color_border);
	CLASS_ATTR_LABEL                (c, "bdcolor", 0, "Box Border Color");
	CLASS_ATTR_ORDER                (c, "bdcolor", 0, "2");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bdcolor", 0, "0.5 0.5 0.5 1.");
	
	CLASS_ATTR_RGBA                 (c, "textcolor", 0, t_number, f_color_text);
	CLASS_ATTR_LABEL                (c, "textcolor", 0, "Text Color");
	CLASS_ATTR_ORDER                (c, "textcolor", 0, "3");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "textcolor", 0, "0. 0. 0. 1.");
	
    eclass_register(CLASS_NOBOX, c);
	number_class = c;
}

void *number_new(t_symbol *s, int argc, t_atom *argv)
{
	t_number *x =  NULL;
	t_binbuf* d;
    long flags;
    
	if (!(d = binbuf_via_atoms(argc,argv)))
		return NULL;
    
	x = (t_number *)ebox_alloc(number_class);
    
    flags = 0
    | EBOX_GROWINDI
    | EBOX_IGNORELOCKCLICK
    ;
    
	ebox_new((t_ebox *)x, flags);
	x->j_box.b_firstin = (t_object *)x;
    
	ebox_dspsetup((t_ebox *)x, 1, 1);
    
    x->f_peaks_outlet   = floatout(x);
    x->f_peak_value     = 0.;
    x->f_clock          = clock_new(x,(t_method)number_tick);
	x->f_startclock     = 0;
    
	binbuf_attr_process(x, d);
	ebox_ready((t_ebox *)x);
    
	return (x);
}

void number_getdrawparams(t_number *x, t_object *patcherview, t_edrawparams *params)
{
	params->d_borderthickness   = 2;
	params->d_cornersize        = 2;
    params->d_bordercolor       = x->f_color_border;
    params->d_boxfillcolor      = x->f_color_background;
}

void number_oksize(t_number *x, t_rect *newrect)
{
    newrect->width = pd_clip_min(newrect->width, sys_fontwidth(x->j_box.e_font.c_size) * 3 + 8);
    newrect->height = sys_fontheight(x->j_box.e_font.c_size) + 4;
}

void number_dsp(t_number *x, t_object *dsp, short *count, double samplerate, long maxvectorsize, long flags)
{
    object_method(dsp, gensym("dsp_add"), x, (method)number_perform, 0, NULL);
	x->f_startclock = 1;
}

void number_perform(t_number *x, t_object *dsp, float **ins, long ni, float **outs, long no, long nsamples, long f,void *up)
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

void number_tick(t_number *x)
{    
    number_output(x);
	ebox_invalidate_layer((t_ebox *)x, gensym("value_layer"));
	ebox_redraw((t_ebox *)x);
    
	if(canvas_dspstate)
		clock_delay(x->f_clock, x->f_interval);
}

void number_output(t_number *x)
{
    outlet_float((t_outlet*)x->f_peaks_outlet, (float)x->f_peak_value);
}

void number_free(t_number *x)
{
	ebox_dspfree((t_ebox *)x);
    clock_free(x->f_clock);
}

void number_assist(t_number *x, void *b, long m, long a, char *s)
{
	;
}

t_pd_err number_notify(t_number *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	if (msg == gensym("attr_modified"))
	{
		if(s == gensym("bgcolor") || s == gensym("bdcolor") || s == gensym("textcolor"))
		{
			ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
			ebox_invalidate_layer((t_ebox *)x, gensym("value_layer"));
		}
        if(s == gensym("fontsize"))
        {
            object_attr_setvalueof((t_object *)x, gensym("size"), 0, NULL);
        }
        ebox_redraw((t_ebox *)x);
	}
	return 0;
}

void number_paint(t_number *x, t_object *view)
{
	t_rect rect;
    float fontwidth = sys_fontwidth(x->j_box.e_font.c_size);
    ebox_get_rect_for_view((t_ebox *)x, &rect);
    x->f_max_decimal = (rect.width - fontwidth - 8) / fontwidth - 2;
    draw_background(x, view, &rect);
    draw_value(x, view, &rect);
}

void draw_background(t_number *x, t_object *view, t_rect *rect)
{
	t_elayer *g = ebox_start_layer((t_ebox *)x, gensym("background_layer"), rect->width, rect->height);
	t_etext *jtl = etext_layout_create();
    
	if (g && jtl)
	{
        etext_layout_set(jtl, "~", &x->j_box.e_font, 1, rect->height / 2., rect->width, 0, ETEXT_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP);
        etext_layout_settextcolor(jtl, &x->f_color_text);
        etext_layout_draw(jtl, g);
        
        egraphics_set_line_width(g, 2);
        egraphics_set_color_rgba(g, &x->f_color_border);
        egraphics_move_to(g, 0, 0);
        egraphics_line_to(g, sys_fontwidth(x->j_box.e_font.c_size) + 6,  rect->height / 2.);
        egraphics_line_to(g, 0, rect->height);
        egraphics_stroke(g);
        
		ebox_end_layer((t_ebox*)x, gensym("background_layer"));
	}
	ebox_paint_layer((t_ebox *)x, gensym("background_layer"), 0., 0.);
    etext_layout_destroy(jtl);
}

void draw_value(t_number *x, t_object *view, t_rect *rect)
{
	t_elayer *g = ebox_start_layer((t_ebox *)x, gensym("value_layer"), rect->width, rect->height);
	t_etext *jtl = etext_layout_create();
    
	if (g && jtl)
	{
        int size, inc = 0;
        float peak;
        char number[256];
        sprintf_s(number, "%i", (int)x->f_peak_value);
        size = strlen(number);
        // TRONQUER LE NOMBRE ENTIER
        if(size > x->f_max_decimal+1)
        {
            sprintf_s(number, "%i...", (int)(x->f_peak_value / pow(10, size - (x->f_max_decimal+1))));
        }
        // TRONQUER LES DECIMALS
        else
        {
            // ENLEVER LES ZERO
            peak = x->f_peak_value;
            while(peak - (int)peak != 0)
            {
                peak *= 10;
                inc++;
            }
            // TRONQUER SELON LE NOMBRE MAXIMUM DE DECIMAL
            if(inc > x->f_max_decimal - size)
                inc = x->f_max_decimal - size;
            if(inc > x->f_ndecimal)
                inc = x->f_ndecimal;
            
            if(inc == 0)
                sprintf_s(number, "%i.", (int)x->f_peak_value);
            else if(inc == 1)
                sprintf_s(number, "%.1f", x->f_peak_value);
            else if(inc == 2)
                sprintf_s(number, "%.2f", x->f_peak_value);
            else if(inc == 3)
                sprintf_s(number, "%.3f", x->f_peak_value);
            else if(inc == 4)
                sprintf_s(number, "%.4f", x->f_peak_value);
            else if(inc == 5)
                sprintf_s(number, "%.5f", x->f_peak_value);
            else
                sprintf_s(number, "%.6f", x->f_peak_value);
        }
        etext_layout_settextcolor(jtl, &x->f_color_text);
        
        etext_layout_set(jtl, number, &x->j_box.e_font, sys_fontwidth(x->j_box.e_font.c_size) + 8, rect->height / 2., rect->width - 3, 0, ETEXT_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP);
        
        etext_layout_draw(jtl, g);
		ebox_end_layer((t_ebox*)x, gensym("value_layer"));
	}
	ebox_paint_layer((t_ebox *)x, gensym("value_layer"), 0., 0.);
    etext_layout_destroy(jtl);
}








