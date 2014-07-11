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

typedef struct  _plane
{
	t_ebox      j_box;
    
    t_outlet*   f_out_x;
    t_outlet*   f_out_y;
    
    float       f_size;
    t_pt        f_position;
    t_pt        f_ratio;
    t_rect      f_boundaries;
	t_rgba		f_color_background;
	t_rgba		f_color_border;
	t_rgba		f_color_point;
    
} t_plane;

t_eclass *plane_class;

void *plane_new(t_symbol *s, int argc, t_atom *argv);
void plane_free(t_plane *x);
void plane_assist(t_plane *x, void *b, long m, long a, char *s);

void plane_set(t_plane *x, t_symbol *s, long ac, t_atom *av);
void plane_list(t_plane *x, t_symbol *s, long ac, t_atom *av);
void plane_output(t_plane *x);

t_pd_err plane_notify(t_plane *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

t_pd_err plane_bound_set(t_plane *x, t_object *attr, long ac, t_atom *av);

void plane_getdrawparams(t_plane *x, t_object *patcherview, t_edrawparams *params);
void plane_oksize(t_plane *x, t_rect *newrect);

void plane_paint(t_plane *x, t_object *view);
void draw_point(t_plane *x,  t_object *view, t_rect *rect);

void plane_mousedrag(t_plane *x, t_object *patcherview, t_pt pt, long modifiers);

void plane_preset(t_plane *x, t_binbuf *b);

extern "C" void setup_c0x2eplane(void)
{
	t_eclass *c;
    
	c = eclass_new("c.plane", (method)plane_new, (method)plane_free, (short)sizeof(t_plane), 0L, A_GIMME, 0);
	eclass_init(c, 0);
    cream_initclass(c);
	
	eclass_addmethod(c, (method) plane_assist,          "assist",           A_CANT, 0);
	eclass_addmethod(c, (method) plane_paint,           "paint",            A_CANT, 0);
	eclass_addmethod(c, (method) plane_notify,          "notify",           A_CANT, 0);
    eclass_addmethod(c, (method) plane_getdrawparams,   "getdrawparams",    A_CANT, 0);
    eclass_addmethod(c, (method) plane_oksize,          "oksize",           A_CANT, 0);
    eclass_addmethod(c, (method) plane_set,             "set",              A_GIMME,0);
    eclass_addmethod(c, (method) plane_list,            "list",            A_GIMME,0);
    eclass_addmethod(c, (method) plane_output,          "bang",             A_CANT, 0);
    
    eclass_addmethod(c, (method) plane_mousedrag,       "mousedown",        A_CANT, 0);
    eclass_addmethod(c, (method) plane_mousedrag,       "mousedrag",        A_CANT, 0);
    
    eclass_addmethod(c, (method) plane_preset,          "preset",           A_CANT, 0);
    
    CLASS_ATTR_INVISIBLE            (c, "fontname", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontweight", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontslant", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontsize", 1);
	CLASS_ATTR_DEFAULT              (c, "size", 0, "120 120");
    
    CLASS_ATTR_FLOAT_ARRAY          (c, "bound", 0, t_plane, f_boundaries, 4);
	CLASS_ATTR_LABEL                (c, "bound", 0, "Boundaries");
    CLASS_ATTR_ACCESSORS			(c, "bound", NULL, plane_bound_set);
	CLASS_ATTR_ORDER                (c, "bound", 0, "2");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bound", 0, "-1. -1. 1. 1.");
    
    CLASS_ATTR_FLOAT                (c, "ptsize", 0, t_plane, f_size);
	CLASS_ATTR_LABEL                (c, "ptsize", 0, "Point size");
	CLASS_ATTR_ORDER                (c, "ptsize", 0, "3");
    CLASS_ATTR_FILTER_CLIP          (c, "ptsize", 5., 50.f);
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "ptsize", 0, "5");
    CLASS_ATTR_STYLE                (c, "ptsize", 0, "number");
    CLASS_ATTR_STEP                 (c, "ptsize", 0.5);
    
	CLASS_ATTR_RGBA                 (c, "bgcolor", 0, t_plane, f_color_background);
	CLASS_ATTR_LABEL                (c, "bgcolor", 0, "Background Color");
	CLASS_ATTR_ORDER                (c, "bgcolor", 0, "1");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bgcolor", 0, "0.75 0.75 0.75 1.");
	CLASS_ATTR_STYLE                (c, "bgcolor", 0, "color");
    
	CLASS_ATTR_RGBA                 (c, "bdcolor", 0, t_plane, f_color_border);
	CLASS_ATTR_LABEL                (c, "bdcolor", 0, "Border Color");
	CLASS_ATTR_ORDER                (c, "bdcolor", 0, "2");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bdcolor", 0, "0.5 0.5 0.5 1.");
	CLASS_ATTR_STYLE                (c, "bdcolor", 0, "color");
    
	CLASS_ATTR_RGBA                 (c, "ptcolor", 0, t_plane, f_color_point);
	CLASS_ATTR_LABEL                (c, "ptcolor", 0, "Point Color");
	CLASS_ATTR_ORDER                (c, "ptcolor", 0, "3");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "ptcolor", 0, "0. 0. 0. 1");
	CLASS_ATTR_STYLE                (c, "ptcolor", 0, "color");
	
    eclass_register(CLASS_BOX, c);
	plane_class = c;
}

void *plane_new(t_symbol *s, int argc, t_atom *argv)
{
	t_plane *x =  NULL;
	t_binbuf* d;
    long flags;
	if (!(d = binbuf_via_atoms(argc,argv)))
		return NULL;
    
	x = (t_plane *)eobj_new(plane_class);
    flags = 0
    | EBOX_GROWLINK
    ;
	ebox_new((t_ebox *)x, flags);
    x->f_out_x = (t_outlet *)floatout(x);
    x->f_out_y = (t_outlet *)floatout(x);
    
    x->f_position.x = 0.;
    x->f_position.y = 0.;
    
	ebox_attrprocess_viabinbuf(x, d);
	ebox_ready((t_ebox *)x);
	return (x);
}

void plane_getdrawparams(t_plane *x, t_object *patcherview, t_edrawparams *params)
{
	params->d_borderthickness   = 2;
	params->d_cornersize        = 2;
    params->d_bordercolor       = x->f_color_border;
    params->d_boxfillcolor      = x->f_color_background;
}

void plane_oksize(t_plane *x, t_rect *newrect)
{
    newrect->width = pd_clip_min(newrect->width, 15.);
    newrect->height = pd_clip_min(newrect->height, 15.);
}

void plane_set(t_plane *x, t_symbol *s, long ac, t_atom *av)
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

void plane_list(t_plane *x, t_symbol *s, long ac, t_atom *av)
{
    plane_set(x, NULL, ac, av);
    plane_output(x);
}

void plane_output(t_plane *x)
{
    t_atom argv[2];
    if(ebox_isdrawable((t_ebox *)x))
    {
        outlet_float(x->f_out_x, x->f_position.x);
        outlet_float(x->f_out_y, x->f_position.y);
        if(ebox_getsender((t_ebox *) x))
        {
            atom_setfloat(argv, x->f_position.x);
            atom_setfloat(argv+1, x->f_position.y);
            pd_list(ebox_getsender((t_ebox *) x), &s_list, 2, argv);
        }
    }
}

void plane_free(t_plane *x)
{
	ebox_free((t_ebox *)x);
}

void plane_assist(t_plane *x, void *b, long m, long a, char *s)
{
	;
}

t_pd_err plane_notify(t_plane *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
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

t_pd_err plane_bound_set(t_plane *x, t_object *attr, long ac, t_atom *av)
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
        plane_set(x, NULL, 2, argv);
        plane_output(x);
    }
    return 0;
}

void plane_paint(t_plane *x, t_object *view)
{
	t_rect rect;
	ebox_get_rect_for_view((t_ebox *)x, &rect);
    x->f_ratio.x = (rect.width - 2 * x->f_size - 2) / (x->f_boundaries.width - x->f_boundaries.x);
    x->f_ratio.y = (rect.height - 2 * x->f_size - 2) / (x->f_boundaries.height - x->f_boundaries.y);
    draw_point(x, view, &rect);
    
}

void draw_point(t_plane *x, t_object *view, t_rect *rect)
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

void plane_mousedrag(t_plane *x, t_object *patcherview, t_pt pt, long modifiers)
{
    t_atom argv[2];
    pt.x -= (x->f_size + 2);
    pt.y += (x->f_size - 2);
    atom_setfloat(argv, (pt.x + x->f_boundaries.x * x->f_ratio.x) / x->f_ratio.x);
    atom_setfloat(argv+1, (pt.y - (x->j_box.b_rect.height + x->f_boundaries.y * x->f_ratio.y)) / -x->f_ratio.y);
    plane_set(x, NULL, 2, argv);
    plane_output(x);
}

void plane_preset(t_plane *x, t_binbuf *b)
{
    binbuf_addv(b, "sff", gensym("list"), x->f_position.x, x->f_position.y);
}






