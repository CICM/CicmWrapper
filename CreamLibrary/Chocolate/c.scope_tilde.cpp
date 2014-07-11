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

typedef struct  _scope
{
	t_edspbox   j_box;

	t_clock*	f_clock;
	int			f_startclock;

	long        f_display_size;
    long        f_signal_size;
    int         f_mode;

	float*      f_buffer_x;
    float*      f_buffer_y;
    int         f_index;

	t_rgba		f_color_background;
	t_rgba		f_color_border;
	t_rgba		f_color_signal;
	float       f_range[2];
} t_scope;

t_eclass *cscope_class;

void *scope_new(t_symbol *s, int argc, t_atom *argv);
void scope_free(t_scope *x);
void scope_assist(t_scope *x, void *b, long m, long a, char *s);

void scope_dsp(t_scope *x, t_object *dsp, short *count, double samplerate, long maxvectorsize, long flags);
void scope_perform(t_scope *x, t_object *d, float **ins, long ni, float **outs, long no, long sf, long f,void *up);

void scope_tick(t_scope *x);

t_pd_err scope_notify(t_scope *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

void scope_getdrawparams(t_scope *x, t_object *patcherview, t_edrawparams *params);
void scope_oksize(t_scope *x, t_rect *newrect);

void scope_paint(t_scope *x, t_object *view);
void draw_background(t_scope *x,  t_object *view, t_rect *rect);
void draw_signal(t_scope *x,  t_object *view, t_rect *rect);
void draw_signalXY(t_scope *x, t_object *view, t_rect *rect);

extern "C" void setup_c0x2escope_tilde(void)
{
	t_eclass *c;

	c = eclass_new("c.scope~", (method)scope_new, (method)scope_free, (short)sizeof(t_scope), 0L, A_GIMME, 0);

	eclass_dspinit(c);
	eclass_init(c, 0);
    cream_initclass(c);

	eclass_addmethod(c, (method) scope_dsp,             "dsp",              A_CANT, 0);
	eclass_addmethod(c, (method) scope_assist,          "assist",           A_CANT, 0);
	eclass_addmethod(c, (method) scope_paint,           "paint",            A_CANT, 0);
	eclass_addmethod(c, (method) scope_notify,          "notify",           A_CANT, 0);
    eclass_addmethod(c, (method) scope_getdrawparams,   "getdrawparams",    A_CANT, 0);
    eclass_addmethod(c, (method) scope_oksize,          "oksize",           A_CANT, 0);

    CLASS_ATTR_INVISIBLE            (c, "fontname", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontweight", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontslant", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontsize", 1);
    CLASS_ATTR_INVISIBLE            (c, "send", 1);
	CLASS_ATTR_DEFAULT              (c, "size", 0, "125 100");

    CLASS_ATTR_LONG                 (c, "bufdis", 0, t_scope, f_display_size);
	CLASS_ATTR_ORDER                (c, "bufdis", 0, "1");
	CLASS_ATTR_LABEL                (c, "bufdis", 0, "Display Buffer Size");
    CLASS_ATTR_FILTER_CLIP          (c, "bufdis", 2, 8092);
	CLASS_ATTR_DEFAULT              (c, "bufdis", 0, "256");
	CLASS_ATTR_SAVE                 (c, "bufdis", 1);
	CLASS_ATTR_STYLE                (c, "bufdis", 0, "number");

    CLASS_ATTR_LONG                 (c, "bufsig", 0, t_scope, f_signal_size);
	CLASS_ATTR_ORDER                (c, "bufsig", 0, "2");
	CLASS_ATTR_LABEL                (c, "bufsig", 0, "Signal Buffer Size");
	CLASS_ATTR_FILTER_CLIP          (c, "bufsig", 2, 8092);
	CLASS_ATTR_DEFAULT              (c, "bufsig", 0, "256");
	CLASS_ATTR_SAVE                 (c, "bufsig", 1);
    CLASS_ATTR_STYLE                (c, "bufsig", 0, "number");

    CLASS_ATTR_FLOAT_ARRAY          (c, "range", 0, t_scope, f_range, 2);
	CLASS_ATTR_ORDER                (c, "range", 0, "2");
	CLASS_ATTR_LABEL                (c, "range", 0, "Range");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "range", 0, "-1 1");

	CLASS_ATTR_RGBA                 (c, "bgcolor", 0, t_scope, f_color_background);
	CLASS_ATTR_LABEL                (c, "bgcolor", 0, "Background Color");
	CLASS_ATTR_ORDER                (c, "bgcolor", 0, "1");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bgcolor", 0, "0.75 0.75 0.75 1.");
	CLASS_ATTR_STYLE                (c, "bgcolor", 0, "color");

	CLASS_ATTR_RGBA                 (c, "bdcolor", 0, t_scope, f_color_border);
	CLASS_ATTR_LABEL                (c, "bdcolor", 0, "Border Color");
	CLASS_ATTR_ORDER                (c, "bdcolor", 0, "2");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bdcolor", 0, "0.5 0.5 0.5 1.");
	CLASS_ATTR_STYLE                (c, "bdcolor", 0, "color");

	CLASS_ATTR_RGBA                 (c, "sicolor", 0, t_scope, f_color_signal);
	CLASS_ATTR_LABEL                (c, "sicolor", 0, "Signal Color");
	CLASS_ATTR_ORDER                (c, "sicolor", 0, "3");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "sicolor", 0, "0. 0.6 0. 0.8");
	CLASS_ATTR_STYLE                (c, "sicolor", 0, "color");

    eclass_register(CLASS_BOX, c);
	cscope_class = c;
}

void *scope_new(t_symbol *s, int argc, t_atom *argv)
{
	int i;
	t_scope *x =  NULL;
	t_binbuf* d;
    long flags;
	if (!(d = binbuf_via_atoms(argc,argv)))
		return NULL;

	x = (t_scope *)eobj_new(cscope_class);
    if(x)
    {
        flags = 0
        | EBOX_GROWINDI
        | EBOX_IGNORELOCKCLICK
        ;

        ebox_new((t_ebox *)x, flags);
        eobj_dspsetup((t_ebox *)x, 2, 0);

        x->f_buffer_x = (float *)calloc(80192, sizeof(float));
        x->f_buffer_y = (float *)calloc(80192, sizeof(float));
        x->f_mode     = 0;
        x->f_index    = 0;

        for(i = 0; i < 80192; i++)
        {
            x->f_buffer_x[i] = 0.;
            x->f_buffer_y[i] = 0.;
        }

        x->f_clock          = clock_new(x,(t_method)scope_tick);
        x->f_startclock     = 0;

        ebox_attrprocess_viabinbuf(x, d);
        ebox_ready((t_ebox *)x);
    }
	return (x);
}

void scope_getdrawparams(t_scope *x, t_object *patcherview, t_edrawparams *params)
{
	params->d_borderthickness   = 2.;
	params->d_cornersize        = 2.;
    params->d_bordercolor       = x->f_color_border;
    params->d_boxfillcolor      = x->f_color_background;
}

void scope_oksize(t_scope *x, t_rect *newrect)
{
    newrect->width = pd_clip_min(newrect->width, 8.);
    newrect->height = pd_clip_min(newrect->height, 8.);
}

void scope_dsp(t_scope *x, t_object *dsp, short *count, double samplerate, long maxvectorsize, long flags)
{
    if(count[1])
        x->f_mode = 1;
    else
        x->f_mode = 0;

    object_method(dsp, gensym("dsp_add"), x, (method)scope_perform, 0, NULL);
	x->f_startclock = 1;
}

void scope_perform(t_scope *x, t_object *dsp, float **ins, long ni, float **outs, long no,long nsamples,long f,void *up)
{
    int i;
    for(i = 0; i < nsamples; i++)
    {
        x->f_buffer_x[x->f_index] = ins[0][i];
        x->f_buffer_y[x->f_index] = ins[1][i];
        x->f_index++;
        if(x->f_index >= x->f_signal_size && x->f_startclock)
        {
            x->f_index = 0;
            clock_delay(x->f_clock, 0);
        }
    }
}

void scope_tick(t_scope *x)
{
	ebox_invalidate_layer((t_ebox *)x, gensym("signal_layer"));
	ebox_redraw((t_ebox *)x);

	if(!sys_getdspstate())
    {
        x->f_startclock = 0;
        clock_unset(x->f_clock);
    }
}

void scope_free(t_scope *x)
{
	ebox_free((t_ebox *)x);
    clock_free(x->f_clock);
    free(x->f_buffer_x);
    free(x->f_buffer_y);
}

void scope_assist(t_scope *x, void *b, long m, long a, char *s)
{
	;
}

t_pd_err scope_notify(t_scope *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
    int max;
	if (msg == gensym("attr_modified"))
	{
		if(s == gensym("bgcolor") || s == gensym("bdcolor") || s == gensym("sicolor") || s == gensym("range") || s == gensym("xymode"))
		{
            if(x->f_range[0] > x->f_range[1])
            {
                max  = x->f_range[0];
                x->f_range[0] = x->f_range[1];
                x->f_range[1] = max;
            }
			ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
			ebox_invalidate_layer((t_ebox *)x, gensym("signal_layer"));
		}
		ebox_redraw((t_ebox *)x);
	}
	return 0;
}

void scope_paint(t_scope *x, t_object *view)
{
	t_rect rect;
	ebox_get_rect_for_view((t_ebox *)x, &rect);
	draw_background(x, view, &rect);
    if(x->f_mode)
        draw_signalXY(x, view, &rect);
    else
        draw_signal(x, view, &rect);
}

void draw_background(t_scope *x,  t_object *view, t_rect *rect)
{
	int i;
	t_elayer *g = ebox_start_layer((t_ebox *)x, gensym("background_layer"), rect->width, rect->height);

	if (g)
	{
        egraphics_set_color_rgba(g, &x->f_color_border);
        for(i = 1; i < 4; i++)
        {
            egraphics_line_fast(g, -2, rect->height * 0.25 * (float)i, rect->width + 4, rect->height * 0.25 * (float)i);
        }
        for(i = 1; i < 6; i++)
        {
            egraphics_line_fast(g, rect->width * (1. / 6.) * (float)i, -2, rect->width * (1. / 6.) * (float)i, rect->height + 4);
        }
		ebox_end_layer((t_ebox*)x, gensym("background_layer"));
	}
	ebox_paint_layer((t_ebox *)x, gensym("background_layer"), 0.f, 0.f);
}


void draw_signal(t_scope *x, t_object *view, t_rect *rect)
{
	t_elayer *g = ebox_start_layer((t_ebox *)x, gensym("signal_layer"), rect->width, rect->height);

	if (g)
	{
        float i;
        float ratio = -rect->height / pd_clip_min((x->f_range[1] - x->f_range[0]), 0.001);
        int increment = pd_clip_min((float)x->f_signal_size / (float)x->f_display_size, 1);

        egraphics_set_color_rgba(g, &x->f_color_signal);
        egraphics_set_line_width(g, 2);

        egraphics_move_to(g, 0, (x->f_buffer_x[0] - x->f_range[0]) * ratio + rect->height);
        for(i = increment; i < x->f_signal_size; i += increment)
        {
            egraphics_line_to(g, i / (float)x->f_signal_size * rect->width, (x->f_buffer_x[(int)i] - x->f_range[0]) * ratio + rect->height);
        }

        egraphics_stroke(g);
        x->f_index = 0;
		ebox_end_layer((t_ebox *)x, gensym("signal_layer"));
	}
	ebox_paint_layer((t_ebox *)x, gensym("signal_layer"), 0., 0.);
}

void draw_signalXY(t_scope *x, t_object *view, t_rect *rect)
{
	t_elayer *g = ebox_start_layer((t_ebox *)x, gensym("signal_layer"), rect->width, rect->height);

	if (g)
	{
        float i;
        float ratioy = -rect->height / pd_clip_min((x->f_range[1] - x->f_range[0]), 0.001);
        float ratiox = rect->width / pd_clip_min((x->f_range[1] - x->f_range[0]), 0.001);
        int increment = pd_clip_min((float)x->f_signal_size / (float)x->f_display_size, 1);

        egraphics_set_color_rgba(g, &x->f_color_signal);
        egraphics_set_line_width(g, 2);

        egraphics_move_to(g, (x->f_buffer_x[0] - x->f_range[0]) * ratiox, (x->f_buffer_y[0] - x->f_range[0]) * ratioy + rect->height);
        for(i = increment; i < x->f_signal_size; i += increment)
        {
            egraphics_line_to(g, (x->f_buffer_x[(int)i] - x->f_range[0]) * ratiox, (x->f_buffer_y[(int)i] - x->f_range[0]) * ratioy + rect->height);
        }

        egraphics_stroke(g);
        x->f_index = 0;
		ebox_end_layer((t_ebox *)x, gensym("signal_layer"));
	}
	ebox_paint_layer((t_ebox *)x, gensym("signal_layer"), 0., 0.);
}



