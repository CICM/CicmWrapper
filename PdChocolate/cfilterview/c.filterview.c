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

#include "../../../PdEnhanced/Sources/pd_enhanced.h"

enum
{
    Lowpass = 0,
    Highpass = 1,
    Bandpass = 2,
    Notch = 3,
    Peak = 4,
    Lowshelf = 5,
    Highshelf = 6,
    //Resonante = 7,
    //Allpass = 8
};

typedef struct  _filterview
{
	t_ebox      j_box;
    
    t_outlet*   f_out_coeffs;
    t_outlet*   f_out_config;
    
    long        f_type;
    
    float       f_sample_rate;
    float       f_coeff_a0;
    float       f_coeff_a1;
    float       f_coeff_a2;
    float       f_coeff_b1;
    float       f_coeff_b2;
    
    float       f_frequency;
    float       f_frequency_factor;
    float       f_q_factor;
    float       f_gain_factor;
    
	t_rgba		f_color_background;
	t_rgba		f_color_border;
	t_rgba		f_color_point;
    
} t_filterview;

t_eclass *filterview_class;

void *filterview_new(t_symbol *s, int argc, t_atom *argv);
void filterview_free(t_filterview *x);
void filterview_assist(t_filterview *x, void *b, long m, long a, char *s);

void filterview_set(t_filterview *x, t_symbol *s, long ac, t_atom *av);
void filterview_list(t_filterview *x, t_symbol *s, long ac, t_atom *av);
void filterview_output(t_filterview *x);
void filterview_compute(t_filterview *x);

t_pd_err filterview_notify(t_filterview *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

t_pd_err filterview_type_set(t_filterview *x, t_object *attr, long ac, t_atom *av);

void filterview_getdrawparams(t_filterview *x, t_object *patcherview, t_edrawparams *params);
void filterview_oksize(t_filterview *x, t_rect *newrect);

void filterview_paint(t_filterview *x, t_object *view);
void draw_point(t_filterview *x,  t_object *view, t_rect *rect);

void filterview_mousedrag(t_filterview *x, t_object *patcherview, t_pt pt, long modifiers);

void filterview_preset(t_filterview *x, t_binbuf *b);

void setup_c0x2efilterview(void)
{
	t_eclass *c;
    
	c = eclass_new("c.filterview", (method)filterview_new, (method)filterview_free, (short)sizeof(t_filterview), 0L, A_GIMME, 0);
	eclass_init(c, 0);
	
	eclass_addmethod(c, (method) filterview_assist,          "assist",           A_CANT, 0);
	eclass_addmethod(c, (method) filterview_paint,           "paint",            A_CANT, 0);
	eclass_addmethod(c, (method) filterview_notify,          "notify",           A_CANT, 0);
    eclass_addmethod(c, (method) filterview_getdrawparams,   "getdrawparams",    A_CANT, 0);
    eclass_addmethod(c, (method) filterview_oksize,          "oksize",           A_CANT, 0);
    eclass_addmethod(c, (method) filterview_set,             "set",              A_GIMME,0);
    eclass_addmethod(c, (method) filterview_list,            "list",            A_GIMME,0);
    eclass_addmethod(c, (method) filterview_output,          "bang",             A_CANT, 0);
    
    eclass_addmethod(c, (method) filterview_mousedrag,       "mousedown",        A_CANT, 0);
    eclass_addmethod(c, (method) filterview_mousedrag,       "mousedrag",        A_CANT, 0);
    
    eclass_addmethod(c, (method) filterview_preset,          "preset",           A_CANT, 0);
    
	CLASS_ATTR_DEFAULT              (c, "size", 0, "200 120");
    
    CLASS_ATTR_LONG                 (c, "type", 0, t_filterview, f_type);
	CLASS_ATTR_LABEL                (c, "type", 0, "Filter Type");
    CLASS_ATTR_ACCESSORS			(c, "type", NULL, filterview_type_set);
	CLASS_ATTR_ORDER                (c, "type", 0, "1");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "type", 0, "0.");
    
    CLASS_ATTR_FLOAT                (c, "frequency", 0, t_filterview, f_frequency);
	CLASS_ATTR_LABEL                (c, "frequency", 0, "Boundaries");
	CLASS_ATTR_ORDER                (c, "frequency", 0, "1");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "frequency", 0, "40.");
    
    CLASS_ATTR_FLOAT                (c, "gain", 0, t_filterview, f_gain_factor);
	CLASS_ATTR_LABEL                (c, "gain", 0, "Point size");
	CLASS_ATTR_ORDER                (c, "gain", 0, "3");
    CLASS_ATTR_FILTER_CLIP          (c, "gain", 5., 50.f);
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "gain", 0, "5");
    
    CLASS_ATTR_FLOAT                (c, "factor", 0, t_filterview, f_q_factor);
	CLASS_ATTR_LABEL                (c, "factor", 0, "Point size");
	CLASS_ATTR_ORDER                (c, "factor", 0, "3");
    CLASS_ATTR_FILTER_CLIP          (c, "factor", 5., 50.f);
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "factor", 0, "5");
    
	CLASS_ATTR_RGBA                 (c, "bgcolor", 0, t_filterview, f_color_background);
	CLASS_ATTR_LABEL                (c, "bgcolor", 0, "Background Color");
	CLASS_ATTR_ORDER                (c, "bgcolor", 0, "1");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bgcolor", 0, "0.75 0.75 0.75 1.");
	
	CLASS_ATTR_RGBA                 (c, "bdcolor", 0, t_filterview, f_color_border);
	CLASS_ATTR_LABEL                (c, "bdcolor", 0, "Box Border Color");
	CLASS_ATTR_ORDER                (c, "bdcolor", 0, "2");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bdcolor", 0, "0.5 0.5 0.5 1.");
	
	CLASS_ATTR_RGBA                 (c, "ptcolor", 0, t_filterview, f_color_point);
	CLASS_ATTR_LABEL                (c, "ptcolor", 0, "Point Color");
	CLASS_ATTR_ORDER                (c, "ptcolor", 0, "3");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "ptcolor", 0, "0. 0. 0. 1");
	
	
    eclass_register(CLASS_NOBOX, c);
	filterview_class = c;
}

void *filterview_new(t_symbol *s, int argc, t_atom *argv)
{
	t_filterview *x =  NULL;
	t_binbuf* d;
    long flags;
	if (!(d = binbuf_via_atoms(argc,argv)))
		return NULL;
    
	x = (t_filterview *)ebox_alloc(filterview_class);
    flags = 0
    | EBOX_GROWINDI
    ;
	ebox_new((t_ebox *)x, flags);
	x->j_box.b_firstin = (t_object *)x;
    
    x->f_out_coeffs = (t_outlet *)listout(x);
    x->f_out_config = (t_outlet *)anythingout(x);
    x->f_sample_rate = sys_getsr();
    x->f_frequency_factor = 0.5;
	binbuf_attr_process(x, d);
	ebox_ready((t_ebox *)x);
	return (x);
}

void filterview_getdrawparams(t_filterview *x, t_object *patcherview, t_edrawparams *params)
{
	params->d_borderthickness   = 2;
	params->d_cornersize        = 2;
    params->d_bordercolor       = x->f_color_border;
    params->d_boxfillcolor      = x->f_color_background;
}

void filterview_oksize(t_filterview *x, t_rect *newrect)
{
    newrect->width = pd_clip_min(newrect->width, 15.);
    newrect->height = pd_clip_min(newrect->height, 15.);
}

void filterview_set(t_filterview *x, t_symbol *s, long ac, t_atom *av)
{
    if(ac && av)
    {
        ebox_invalidate_layer((t_ebox *)x, gensym("point_layer"));
        ebox_redraw((t_ebox *)x);
    }
}

void filterview_list(t_filterview *x, t_symbol *s, long ac, t_atom *av)
{
    filterview_set(x, NULL, ac, av);
    filterview_output(x);
}

void filterview_output(t_filterview *x)
{
    t_atom argv[5];
    if(x->j_box.e_ready_to_draw)
    {
        atom_setfloat(argv, x->f_coeff_a0);
        atom_setfloat(argv+1, x->f_coeff_a1);
        atom_setfloat(argv+2, x->f_coeff_a2);
        atom_setfloat(argv+3, x->f_coeff_b1);
        atom_setfloat(argv+4, x->f_coeff_b2);
        outlet_list(x->f_out_coeffs, &s_list, 5, argv);
        
        if(x->f_type == Lowpass)
            atom_setsym(argv, gensym("lowpass"));
        else if(x->f_type == Highpass)
            atom_setsym(argv, gensym("highpass"));
        else if(x->f_type == Bandpass)
            atom_setsym(argv, gensym("bandpass"));
        else if(x->f_type == Notch)
            atom_setsym(argv, gensym("notch"));
        else if(x->f_type == Peak)
            atom_setsym(argv, gensym("peak"));
        else if(x->f_type == Lowshelf)
            atom_setsym(argv, gensym("lowshelf"));
        else if(x->f_type == Highshelf)
            atom_setsym(argv, gensym("highshelf"));
        else
            atom_setsym(argv, gensym("nothing"));
        
        atom_setfloat(argv+1, x->f_frequency);
        atom_setfloat(argv+2, x->f_gain_factor);
        atom_setfloat(argv+3, x->f_q_factor);
        outlet_anything(x->f_out_config, &s_list, 4, argv);
    }
}

void filterview_free(t_filterview *x)
{
	ebox_free((t_ebox *)x);
}

void filterview_assist(t_filterview *x, void *b, long m, long a, char *s)
{
	;
}

t_pd_err filterview_notify(t_filterview *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	if (msg == gensym("attr_modified"))
	{
		if(s == gensym("bgcolor") || s == gensym("bdcolor") || s == gensym("ptcolor") || s == gensym("ptsize"))
		{
			ebox_invalidate_layer((t_ebox *)x, gensym("point_layer"));
		}
        ebox_redraw((t_ebox *)x);
        
	}
	return 0;
}

t_pd_err filterview_type_set(t_filterview *x, t_object *attr, long ac, t_atom *av)
{
    if(ac && av)
    {
        if(atom_gettype(av) == A_FLOAT)
        {
            x->f_type = pd_clip_minmax(atom_getfloat(av), 0, 8);
        }
        else if(atom_gettype(av) == A_SYM)
        {
            if(atom_getsym(av) == gensym("lowpass"))
                x->f_type = Lowpass;
            else if(atom_getsym(av) == gensym("highpass"))
                x->f_type = Highpass;
            else if(atom_getsym(av) == gensym("bandpass"))
                x->f_type = Bandpass;
            else if(atom_getsym(av) == gensym("notch"))
                x->f_type = Notch;
            else if(atom_getsym(av) == gensym("peak"))
                x->f_type = Peak;
            else if(atom_getsym(av) == gensym("lowshelf"))
                x->f_type = Lowshelf;
            else if(atom_getsym(av) == gensym("highshelf"))
                x->f_type = Highshelf;/*
            else if(atom_getsym(av) == gensym("allpass"))
                x->f_type = Allpass;
            else if(atom_getsym(av) == gensym("resonante"))
                x->f_type = Resonante;*/
        }
        filterview_compute(x);
    }
    return 0;
}

void filterview_paint(t_filterview *x, t_object *view)
{
	t_rect rect;
	ebox_get_rect_for_view((t_ebox *)x, &rect);
    draw_point(x, view, &rect);
    
}

void draw_point(t_filterview *x, t_object *view, t_rect *rect)
{
	t_elayer *g = ebox_start_layer((t_ebox *)x, gensym("point_layer"), rect->width, rect->height);
    
	if (g)
	{
        ;
	}
	ebox_paint_layer((t_ebox *)x, gensym("point_layer"), 0, 0);
}

void filterview_mousedrag(t_filterview *x, t_object *patcherview, t_pt pt, long modifiers)
{
    t_atom argv[2];
    filterview_set(x, NULL, 2, argv);
    filterview_output(x);
}

void filterview_preset(t_filterview *x, t_binbuf *b)
{
   // binbuf_addv(b, "sff", gensym("list"), x->f_position.x, x->f_position.y);
}

void filterview_compute(t_filterview *x)
{
    double norm;
    double gain = x->f_gain_factor;
    
    if(x->f_type == Lowpass)
    {
        norm = 1. / (double)(1. + x->f_frequency_factor / x->f_q_factor + x->f_frequency_factor * x->f_frequency_factor);
        x->f_coeff_a0 = x->f_frequency_factor * x->f_frequency_factor * norm;
        x->f_coeff_a1 = 2 * x->f_coeff_a0;
        x->f_coeff_a2 = x->f_coeff_a0;
        x->f_coeff_b1 = 2 * (x->f_frequency_factor * x->f_frequency_factor - 1) * norm;
        x->f_coeff_b2 = (1 - x->f_frequency_factor / x->f_q_factor + x->f_frequency_factor * x->f_frequency_factor) * norm;
    }
    else if(x->f_type == Highpass)
    {
        norm = 1. / (double)(1. + x->f_frequency_factor / x->f_q_factor + x->f_frequency_factor * x->f_frequency_factor);
        x->f_coeff_a0 = 1 * norm;
        x->f_coeff_a1 = -2 * x->f_coeff_a0;
        x->f_coeff_a2 = x->f_coeff_a0;
        x->f_coeff_b1 = 2 * (x->f_frequency_factor * x->f_frequency_factor - 1) * norm;
        x->f_coeff_b2 = (1 - x->f_frequency_factor / x->f_q_factor + x->f_frequency_factor * x->f_frequency_factor) * norm;
    }
    else if(x->f_type == Bandpass)
    {
        norm = 1. / (double)(1. + x->f_frequency_factor / x->f_q_factor + x->f_frequency_factor * x->f_frequency_factor);
        x->f_coeff_a0 = x->f_frequency_factor / x->f_q_factor * norm;
        x->f_coeff_a1 = 0.;
        x->f_coeff_a2 = -(x->f_frequency_factor / x->f_q_factor * norm);
        x->f_coeff_b1 = 2 * (x->f_frequency_factor * x->f_frequency_factor - 1) * norm;
        x->f_coeff_b2 = (1 - x->f_frequency_factor / x->f_q_factor + x->f_frequency_factor * x->f_frequency_factor) * norm;
    }
    else if(x->f_type == Notch)
    {
        norm = 1 / (1 + x->f_frequency_factor / x->f_q_factor + x->f_frequency_factor * x->f_frequency_factor);
        x->f_coeff_a0 = (1 + x->f_frequency_factor * x->f_frequency_factor) * norm;
        x->f_coeff_a1 = 2 * (x->f_frequency_factor * x->f_frequency_factor - 1) * norm;
        x->f_coeff_a2 = x->f_coeff_a0;
        x->f_coeff_b1 = x->f_coeff_a1;
        x->f_coeff_b2 = (1 - x->f_frequency_factor / x->f_q_factor + x->f_frequency_factor * x->f_frequency_factor) * norm;
    }
    else if(x->f_type == Peak)
    {
        if (x->f_gain_factor >= 0.)
        {
            norm = 1 / (1 + 1. / x->f_q_factor * x->f_frequency_factor + x->f_frequency_factor * x->f_frequency_factor);
            x->f_coeff_a0 = (1 + gain / x->f_q_factor * x->f_frequency_factor + x->f_frequency_factor * x->f_frequency_factor) * norm;
            x->f_coeff_a1 = 2 * (x->f_frequency_factor * x->f_frequency_factor - 1) * norm;
            x->f_coeff_a2 = (1 - gain / x->f_q_factor * x->f_frequency_factor + x->f_frequency_factor * x->f_frequency_factor) * norm;
            x->f_coeff_b1 = x->f_coeff_a1;
            x->f_coeff_b2 = (1 - 1 / x->f_q_factor * x->f_frequency_factor + x->f_frequency_factor * x->f_frequency_factor) * norm;
        }
        else
        {
            norm = 1 / (1 + gain / x->f_q_factor * x->f_frequency_factor + x->f_frequency_factor * x->f_frequency_factor);
            x->f_coeff_a0 = (1 + 1. / x->f_q_factor * x->f_frequency_factor + x->f_frequency_factor * x->f_frequency_factor) * norm;
            x->f_coeff_a1 = 2 * (x->f_frequency_factor * x->f_frequency_factor - 1) * norm;
            x->f_coeff_a2 = (1 - 1. / x->f_q_factor * x->f_frequency_factor + x->f_frequency_factor * x->f_frequency_factor) * norm;
            x->f_coeff_b1 = x->f_coeff_a1;
            x->f_coeff_b2 = (1 - gain / x->f_q_factor * x->f_frequency_factor + x->f_frequency_factor * x->f_frequency_factor) * norm;
        }
    }
    else if(x->f_type == Lowshelf)
    {
        if (x->f_gain_factor >= 0.)
        {
            norm = 1 / (1 + sqrt(2.) * x->f_frequency_factor + x->f_frequency_factor * x->f_frequency_factor);
            x->f_coeff_a0 = (1 + sqrt(2.*gain) * x->f_frequency_factor + gain * x->f_frequency_factor * x->f_frequency_factor) * norm;
            x->f_coeff_a1 = 2 * (gain * x->f_frequency_factor * x->f_frequency_factor - 1) * norm;
            x->f_coeff_a2 = (1 - sqrt(2*gain) * x->f_frequency_factor + gain * x->f_frequency_factor * x->f_frequency_factor) * norm;
            x->f_coeff_b1 = 2 * (x->f_frequency_factor * x->f_frequency_factor - 1) * norm;
            x->f_coeff_b2 = (1 - sqrt(2.) * x->f_frequency_factor + x->f_frequency_factor * x->f_frequency_factor) * norm;
        }
        else
        {
            norm = 1 / (1 + sqrt(2.*gain) * x->f_frequency_factor + gain * x->f_frequency_factor * x->f_frequency_factor);
            x->f_coeff_a0 = (1 + sqrt(2.) * x->f_frequency_factor + x->f_frequency_factor * x->f_frequency_factor) * norm;
            x->f_coeff_a1 = 2 * (x->f_frequency_factor * x->f_frequency_factor - 1) * norm;
            x->f_coeff_a2 = (1 - sqrt(2.) * x->f_frequency_factor + x->f_frequency_factor * x->f_frequency_factor) * norm;
            x->f_coeff_b1 = 2 * (gain * x->f_frequency_factor * x->f_frequency_factor - 1) * norm;
            x->f_coeff_b2 = (1 - sqrt(2.*gain) * x->f_frequency_factor + gain * x->f_frequency_factor * x->f_frequency_factor) * norm;
        }
    }
    else if(x->f_type == Highshelf)
    {
        if (x->f_gain_factor >= 0.)
        {
            norm = 1 / (1 + sqrt(2.) * x->f_frequency_factor + x->f_frequency_factor * x->f_frequency_factor);
            x->f_coeff_a0 = (gain + sqrt(2*gain) * x->f_frequency_factor + x->f_frequency_factor * x->f_frequency_factor) * norm;
            x->f_coeff_a1 = 2 * (x->f_frequency_factor * x->f_frequency_factor - gain) * norm;
            x->f_coeff_a2 = (gain - sqrt(2*gain) * x->f_frequency_factor + x->f_frequency_factor * x->f_frequency_factor) * norm;
            x->f_coeff_b1 = 2 * (x->f_frequency_factor * x->f_frequency_factor - 1) * norm;
            x->f_coeff_b2 = (1 - sqrt(2.) * x->f_frequency_factor + x->f_frequency_factor * x->f_frequency_factor) * norm;
        }
        else
        {
            norm = 1 / (gain + sqrt(2*gain) * x->f_frequency_factor + x->f_frequency_factor * x->f_frequency_factor);
            x->f_coeff_a0 = (1 + sqrt(2.) * x->f_frequency_factor + x->f_frequency_factor * x->f_frequency_factor) * norm;
            x->f_coeff_a1 = 2 * (x->f_frequency_factor * x->f_frequency_factor - 1) * norm;
            x->f_coeff_a2 = (1 - sqrt(2.) * x->f_frequency_factor + x->f_frequency_factor * x->f_frequency_factor) * norm;
            x->f_coeff_b1 = 2 * (x->f_frequency_factor * x->f_frequency_factor - gain) * norm;
            x->f_coeff_b2 = (gain - sqrt(2*gain) * x->f_frequency_factor + x->f_frequency_factor * x->f_frequency_factor) * norm;
        }
    }
}




