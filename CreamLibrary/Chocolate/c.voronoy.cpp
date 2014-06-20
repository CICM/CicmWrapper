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

#include "Voronoy.h"
#include "../c.library.h"

#define MAXPOINTS 256
using namespace Cicm;

typedef struct _voronoy
{
	t_ebox      j_box;

    t_outlet*   f_out_float;
    t_outlet*   f_out_list;

    t_pt        f_size;

	t_rgba		f_color_background;
	t_rgba		f_color_border;
	t_rgba		f_color_point;
    t_rgba		f_color_voronoy;
	Cicm::Voronoy* f_voronoy;
} t_voronoy;

t_eclass *voronoy_class;

void *voronoy_new(t_symbol *s, int argc, t_atom *argv);
void voronoy_free(t_voronoy *x);
void voronoy_assist(t_voronoy *x, void *b, long m, long a, char *s);

t_pd_err voronoy_notify(t_voronoy *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

void voronoy_getdrawparams(t_voronoy *x, t_object *patcherview, t_edrawparams *params);
void voronoy_oksize(t_voronoy *x, t_rect *newrect);

void voronoy_list(t_voronoy *x, t_symbol *s, int argc, t_atom *argv);

void voronoy_paint(t_voronoy *x, t_object *view);
void draw_points(t_voronoy *x,  t_object *view, t_rect *rect);

extern "C" void setup_c0x2evoronoy(void)
{
	t_eclass *c;

	c = eclass_new("c.voronoy", (method)voronoy_new, (method)voronoy_free, (short)sizeof(t_voronoy), 0L, A_GIMME, 0);

	eclass_init(c, 0);

	eclass_addmethod(c, (method) voronoy_assist,          "assist",           A_CANT, 0);
	eclass_addmethod(c, (method) voronoy_paint,           "paint",            A_CANT, 0);
	eclass_addmethod(c, (method) voronoy_notify,          "notify",           A_CANT, 0);
    eclass_addmethod(c, (method) voronoy_getdrawparams,   "getdrawparams",    A_CANT, 0);
    eclass_addmethod(c, (method) voronoy_oksize,          "oksize",           A_CANT, 0);
	eclass_addmethod(c, (method) voronoy_list,			  "list",			  A_GIMME,0);

    CLASS_ATTR_INVISIBLE            (c, "send", 1);
	CLASS_ATTR_DEFAULT              (c, "size", 0, "150. 150.");

	CLASS_ATTR_RGBA                 (c, "bgcolor", 0, t_voronoy, f_color_background);
	CLASS_ATTR_LABEL                (c, "bgcolor", 0, "Background Color");
	CLASS_ATTR_ORDER                (c, "bgcolor", 0, "1");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bgcolor", 0, "0.75 0.75 0.75 1.");
    CLASS_ATTR_STYLE                (c, "bgcolor", 0, "color");
    
	CLASS_ATTR_RGBA                 (c, "bdcolor", 0, t_voronoy, f_color_border);
	CLASS_ATTR_LABEL                (c, "bdcolor", 0, "Border Color");
	CLASS_ATTR_ORDER                (c, "bdcolor", 0, "2");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bdcolor", 0, "0.5 0.5 0.5 1.");
    CLASS_ATTR_STYLE                (c, "bdcolor", 0, "color");
    
	CLASS_ATTR_RGBA                 (c, "ptcolor", 0, t_voronoy, f_color_point);
	CLASS_ATTR_LABEL                (c, "ptcolor", 0, "Point Color");
	CLASS_ATTR_ORDER                (c, "ptcolor", 0, "3");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "ptcolor", 0, "0.5 0.5 0.5 1.");
    CLASS_ATTR_STYLE                (c, "ptcolor", 0, "color");
    
    CLASS_ATTR_RGBA                 (c, "decolor", 0, t_voronoy, f_color_voronoy);
	CLASS_ATTR_LABEL                (c, "decolor", 0, "Voronoy Color");
	CLASS_ATTR_ORDER                (c, "decolor", 0, "4");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "decolor", 0, "0. 0. 0. 1.");
    CLASS_ATTR_STYLE                (c, "decolor", 0, "color");
    
    eclass_register(CLASS_BOX, c);
	voronoy_class = c;
}

void *voronoy_new(t_symbol *s, int argc, t_atom *argv)
{
	t_voronoy *x =  NULL;
	t_binbuf* d;
    long flags;
	if (!(d = binbuf_via_atoms(argc,argv)))
		return NULL;

	x = (t_voronoy *)eobj_new(voronoy_class);

	flags = 0
    | EBOX_GROWLINK
    ;
	ebox_new((t_ebox *)x, flags);

    x->f_out_float = (t_outlet *)floatout(x);
    x->f_out_list = (t_outlet *)listout(x);
    x->f_voronoy = new Cicm::Voronoy(Cicm::Voronoy::Plane);

    ebox_attrprocess_viabinbuf(x, d);
	ebox_ready((t_ebox *)x);
    return (x);
}

void voronoy_free(t_voronoy *x)
{
	ebox_free((t_ebox *)x);
	delete x->f_voronoy;
}

void voronoy_list(t_voronoy *x, t_symbol *s, int argc, t_atom *argv)
{
	x->f_voronoy->clear();
	for(int i = 0; i < argc - 1; i+= 2)
	{
		if(atom_gettype(argv+i) == atom_gettype(argv+i+1) == A_FLOAT)
			x->f_voronoy->addPointCartesian(atom_getfloat(argv+i), atom_getfloat(argv+i+1));
	}
	x->f_voronoy->perform();

	ebox_invalidate_layer((t_ebox *)x, gensym("points_layer"));
    ebox_redraw((t_ebox *)x);
}

void voronoy_getdrawparams(t_voronoy *x, t_object *patcherview, t_edrawparams *params)
{
	params->d_borderthickness   = 2;
	params->d_cornersize        = 2;
    params->d_bordercolor       = x->f_color_border;
    params->d_boxfillcolor      = x->f_color_background;
}

void voronoy_oksize(t_voronoy *x, t_rect *newrect)
{
    newrect->width = pd_clip_min(newrect->width, 15.);
    newrect->height = pd_clip_min(newrect->height, 15.);
}

void voronoy_assist(t_voronoy *x, void *b, long m, long a, char *s)
{
	;
}

t_pd_err voronoy_notify(t_voronoy *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	if (msg == gensym("attr_modified"))
	{
		if(s == gensym("bgcolor") || s == gensym("bdcolor") || s == gensym("ptcolor") || s == gensym("licolor"))
		{
			ebox_invalidate_layer((t_ebox *)x, gensym("points_layer"));
		}
        ebox_redraw((t_ebox *)x);
	}
	return 0;
}

void voronoy_paint(t_voronoy *x, t_object *view)
{
	t_rect rect;
	ebox_get_rect_for_view((t_ebox *)x, &rect);
    x->f_size.x = rect.width;
    x->f_size.y = rect.height;
    draw_points(x, view, &rect);
}

void draw_points(t_voronoy *x, t_object *view, t_rect *rect)
{
	t_elayer *g = ebox_start_layer((t_ebox *)x, gensym("points_layer"), rect->width, rect->height);
    t_matrix transform;
	float factor = rect->width * 0.25;
	if (g)
	{
		egraphics_matrix_init(&transform, 1, 0, 0, -1, rect->width * .5, rect->width * .5);
        egraphics_set_matrix(g, &transform);
		egraphics_set_color_rgba(g, &x->f_color_voronoy);

		for(int i = 0; i < x->f_voronoy->getNumberOfPoints(); i++)
		{
			egraphics_circle(g, x->f_voronoy->getPointAbscissa(i) * factor, x->f_voronoy->getPointOrdinate(i) * factor, 3);
			egraphics_fill(g);

			if(x->f_voronoy->getPointVoronoyLenght(i))
			{
				//post(" %i %f %f", i, x->f_voronoy->getPointVoronoyAbscissa(i, 0) * factor, x->f_voronoy->getPointCircleOrdinate(i, 0) * factor);
				egraphics_move_to(g, x->f_voronoy->getPointVoronoyAbscissa(i, 0) * factor, x->f_voronoy->getPointVoronoyOrdinate(i, 0) * factor);
				for(int j = 1; j < x->f_voronoy->getPointVoronoyLenght(i); j++)
				{
					egraphics_line_to(g, x->f_voronoy->getPointVoronoyAbscissa(i, j) * factor, x->f_voronoy->getPointVoronoyOrdinate(i, j) * factor);
					//post(" %i %f %f", i, x->f_voronoy->getPointCircleAbscissa(i, j) * factor, x->f_voronoy->getPointCircleOrdinate(i, j) * factor);
				}
				egraphics_close_path(g);
				egraphics_stroke(g);
			}
		}
		
		egraphics_set_color_rgba(g, &x->f_color_point);

		ebox_end_layer((t_ebox*)x,  gensym("points_layer"));
	}

	ebox_paint_layer((t_ebox *)x, gensym("points_layer"), 0., 0.);
}

void voronoy_mousemove(t_voronoy *x, t_object *patcherview, t_pt pt, long modifiers)
{
    ebox_invalidate_layer((t_ebox *)x, gensym("points_layer"));
    ebox_redraw((t_ebox *)x);
}





