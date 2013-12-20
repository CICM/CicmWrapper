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
    Allpass = 7,
    Resonante = 8
};

typedef struct  _filterview
{
	t_ebox      j_box;
    
    t_outlet*   f_out_x;
    t_outlet*   f_out_y;
    
    long        f_type;
    
    float       f_coeff_a0;
    float       f_coeff_a1;
    float       f_coeff_a2;
    float       f_coeff_b1;
    float       f_coeff_b2;
    
    float       f_cutoff;
    float       f_q;
    float       f_gain;
    
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

t_pd_err filterview_notify(t_filterview *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

t_pd_err filterview_bound_set(t_filterview *x, t_object *attr, long ac, t_atom *av);

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
    
	CLASS_ATTR_DEFAULT              (c, "size", 0, "120 120");
    
    CLASS_ATTR_FLOAT                (c, "frequency", 0, t_filterview, f_boundaries, 4);
	CLASS_ATTR_LABEL                (c, "frequency", 0, "Boundaries");
    CLASS_ATTR_ACCESSORS			(c, "frequency", NULL, filterview_bound_set);
	CLASS_ATTR_ORDER                (c, "frequency", 0, "1");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "frequency", 0, "10025");
    
    CLASS_ATTR_FLOAT                (c, "ptsize", 0, t_filterview, f_size);
	CLASS_ATTR_LABEL                (c, "ptsize", 0, "Point size");
	CLASS_ATTR_ORDER                (c, "ptsize", 0, "3");
    CLASS_ATTR_FILTER_CLIP          (c, "ptsize", 5., 50.f);
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "ptsize", 0, "5");
    
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
    | EBOX_GROWLINK
    ;
	ebox_new((t_ebox *)x, flags);
	x->j_box.b_firstin = (t_object *)x;
    
    x->f_out_x = (t_outlet *)floatout(x);
    x->f_out_y = (t_outlet *)floatout(x);
    
    x->f_position.x = 0.;
    x->f_position.y = 0.;
    
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
        if(ac >= 1)
        {
            if(x->f_boundaries.x < x->f_boundaries.width)
                x->f_position.x = pd_clip_minmax(atom_getfloat(av), x->f_boundaries.x, x->f_boundaries.width);
            else
                x->f_position.x = pd_clip_minmax(atom_getfloat(av), x->f_boundaries.width, x->f_boundaries.x);
        }
        if(ac >= 2)
        {
            if(x->f_boundaries.y < x->f_boundaries.height)
                x->f_position.y = pd_clip_minmax(atom_getfloat(av+1), x->f_boundaries.y, x->f_boundaries.height);
            else
                x->f_position.y = pd_clip_minmax(atom_getfloat(av+1), x->f_boundaries.height, x->f_boundaries.y);
        }
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
    if(x->j_box.e_ready_to_draw)
    {
        outlet_float(x->f_out_x, x->f_position.x);
        outlet_float(x->f_out_y, x->f_position.y);
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

t_pd_err filterview_bound_set(t_filterview *x, t_object *attr, long ac, t_atom *av)
{
	t_atom argv[2];
    
    if(ac && av)
    {
        if(ac >= 1)
            x->f_boundaries.x = atom_getfloat(av);
        if(ac >= 2)
            x->f_boundaries.y = atom_getfloat(av+1);
        if(ac >= 3)
            x->f_boundaries.width = atom_getfloat(av+2);
        if(ac >= 4)
            x->f_boundaries.height = atom_getfloat(av+3);
        
        // Boundaries changed -> recompute the point position !
        atom_setfloat(argv, x->f_position.x);
        atom_setfloat(argv+1, x->f_position.y);
        filterview_set(x, NULL, 2, argv);
        filterview_output(x);
    }
    return 0;
}

void filterview_paint(t_filterview *x, t_object *view)
{
	t_rect rect;
	ebox_get_rect_for_view((t_ebox *)x, &rect);
    x->f_ratio.x = (rect.width - 2 * x->f_size - 2) / (x->f_boundaries.width - x->f_boundaries.x);
    x->f_ratio.y = (rect.height - 2 * x->f_size - 2) / (x->f_boundaries.height - x->f_boundaries.y);
    draw_point(x, view, &rect);
    
}

void draw_point(t_filterview *x, t_object *view, t_rect *rect)
{
	t_elayer *g = ebox_start_layer((t_ebox *)x, gensym("point_layer"), rect->width, rect->height);
    
	if (g)
	{
        t_matrix matrix;
        // Here, an example matrix
        egraphics_matrix_init(&matrix, x->f_ratio.x, 0.f, 0.f, -x->f_ratio.y, x->f_boundaries.x * -x->f_ratio.x, rect->height + x->f_boundaries.y * x->f_ratio.y);
        egraphics_set_matrix(g, &matrix);
        
        egraphics_set_color_rgba(g, &x->f_color_point);
        // We use oval to keep a perfect circle with the matrix stranformation
        egraphics_oval(g, x->f_position.x, x->f_position.y, x->f_size / x->f_ratio.x, x->f_size / x->f_ratio.y);
        egraphics_set_line_width(g, 2);
        egraphics_fill(g);
        ebox_end_layer((t_ebox*)x, gensym("point_layer"));
	}
	ebox_paint_layer((t_ebox *)x, gensym("point_layer"), x->f_size + 1, -x->f_size - 1);
}

void filterview_mousedrag(t_filterview *x, t_object *patcherview, t_pt pt, long modifiers)
{
    t_atom argv[2];
    pt.x -= (x->f_size + 2);
    pt.y += (x->f_size - 2);
    atom_setfloat(argv, (pt.x + x->f_boundaries.x * x->f_ratio.x) / x->f_ratio.x);
    atom_setfloat(argv+1, (pt.y - (x->j_box.e_rect.height + x->f_boundaries.y * x->f_ratio.y)) / -x->f_ratio.y);
    filterview_set(x, NULL, 2, argv);
    filterview_output(x);
}

void filterview_preset(t_filterview *x, t_binbuf *b)
{
    binbuf_addv(b, "sff", gensym("list"), x->f_position.x, x->f_position.y);
}






