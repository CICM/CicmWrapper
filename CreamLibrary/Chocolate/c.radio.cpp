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

typedef struct _radio
{
	t_ebox      j_box;
    
    t_outlet*   f_out;
	t_rgba		f_color_background;
	t_rgba		f_color_border;
	t_rgba		f_color_item;
    char        f_direction;
    long        f_items[256];
    long        f_nitems;
    long        f_mode;
} t_radio;

t_eclass *radio_class;

void *radio_new(t_symbol *s, int argc, t_atom *argv);
void radio_free(t_radio *x);
void radio_assist(t_radio *x, void *b, long m, long a, char *s);

void radio_float(t_radio *x, float f);
void radio_set(t_radio *x, t_symbol* s, long argc, t_atom* argv);
void radio_list(t_radio *x, t_symbol* s, long argc, t_atom* argv);
void radio_bang(t_radio *x);
void radio_output(t_radio *x);

t_pd_err radio_notify(t_radio *x, t_symbol *s, t_symbol *msg, void *sender, void *data);
t_pd_err radio_nitems_set(t_radio *x, t_object *attr, long ac, t_atom *av);
t_pd_err radio_mode_set(t_radio *x, t_object *attr, long ac, t_atom *av);

void radio_preset(t_radio *x, t_binbuf *b);

void radio_getdrawparams(t_radio *x, t_object *patcherview, t_edrawparams *params);
void radio_oksize(t_radio *x, t_rect *newrect);

void radio_paint(t_radio *x, t_object *view);
void draw_background(t_radio *x,  t_object *view, t_rect *rect);
void draw_items(t_radio *x, t_object *view, t_rect *rect);

void radio_mousedown(t_radio *x, t_object *patcherview, t_pt pt, long modifiers);

extern "C" void setup_c0x2eradio(void)
{
	t_eclass *c;
    
	c = eclass_new("c.radio", (method)radio_new, (method)radio_free, (short)sizeof(t_radio), 0L, A_GIMME, 0);
    
	eclass_init(c, 0);
    cream_initclass(c);
	
	eclass_addmethod(c, (method) radio_assist,          "assist",           A_CANT, 0);
	eclass_addmethod(c, (method) radio_paint,           "paint",            A_CANT, 0);
	eclass_addmethod(c, (method) radio_notify,          "notify",           A_CANT, 0);
    eclass_addmethod(c, (method) radio_getdrawparams,   "getdrawparams",    A_CANT, 0);
    eclass_addmethod(c, (method) radio_oksize,          "oksize",           A_CANT, 0);
    eclass_addmethod(c, (method) radio_set,             "set",              A_GIMME,0);
    eclass_addmethod(c, (method) radio_list,            "list",             A_GIMME,0);
    eclass_addmethod(c, (method) radio_float,           "float",            A_FLOAT,0);
    eclass_addmethod(c, (method) radio_bang,            "bang",             A_CANT, 0);
    eclass_addmethod(c, (method) radio_mousedown,       "mousedown",        A_CANT, 0);
    eclass_addmethod(c, (method) radio_preset,          "preset",           A_CANT, 0);
    
    CLASS_ATTR_INVISIBLE            (c, "fontname", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontweight", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontslant", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontsize", 1);
	CLASS_ATTR_DEFAULT              (c, "size", 0, "15. 120.");
    
    CLASS_ATTR_LONG                 (c, "nitems", 0, t_radio, f_nitems);
	CLASS_ATTR_LABEL                (c, "nitems", 0, "Number of Items");
    CLASS_ATTR_ACCESSORS			(c, "nitems", NULL, radio_nitems_set);
	CLASS_ATTR_ORDER                (c, "nitems", 0, "1");
    CLASS_ATTR_FILTER_CLIP          (c, "nitems", 1, 256);
    CLASS_ATTR_DEFAULT              (c, "nitems", 0, "8");
    CLASS_ATTR_SAVE                 (c, "nitems", 1);
    CLASS_ATTR_STYLE                (c, "nitems", 0, "number");
    
    CLASS_ATTR_LONG                 (c, "mode", 0, t_radio, f_mode);
	CLASS_ATTR_LABEL                (c, "mode", 0, "Check List Mode");
    CLASS_ATTR_ACCESSORS			(c, "mode", NULL, radio_mode_set);
	CLASS_ATTR_ORDER                (c, "mode", 0, "1");
    CLASS_ATTR_FILTER_CLIP          (c, "mode", 0, 1);
    CLASS_ATTR_DEFAULT              (c, "mode", 0, "0");
    CLASS_ATTR_SAVE                 (c, "mode", 1);
    CLASS_ATTR_STYLE                (c, "mode", 0, "onoff");
    
	CLASS_ATTR_RGBA                 (c, "bgcolor", 0, t_radio, f_color_background);
	CLASS_ATTR_LABEL                (c, "bgcolor", 0, "Background Color");
	CLASS_ATTR_ORDER                (c, "bgcolor", 0, "1");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bgcolor", 0, "0.75 0.75 0.75 1.");
	CLASS_ATTR_STYLE                (c, "bgcolor", 0, "color");
    
	CLASS_ATTR_RGBA                 (c, "bdcolor", 0, t_radio, f_color_border);
	CLASS_ATTR_LABEL                (c, "bdcolor", 0, "Border Color");
	CLASS_ATTR_ORDER                (c, "bdcolor", 0, "2");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bdcolor", 0, "0.5 0.5 0.5 1.");
	CLASS_ATTR_STYLE                (c, "bdcolor", 0, "color");
    
	CLASS_ATTR_RGBA                 (c, "itcolor", 0, t_radio, f_color_item);
	CLASS_ATTR_LABEL                (c, "itcolor", 0, "Item Color");
	CLASS_ATTR_ORDER                (c, "itcolor", 0, "3");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "itcolor", 0, "0.5 0.5 0.5 1.");
	CLASS_ATTR_STYLE                (c, "itcolor", 0, "color");
    
    eclass_register(CLASS_BOX, c);
	radio_class = c;
}

void *radio_new(t_symbol *s, int argc, t_atom *argv)
{
	t_radio *x =  NULL;
	t_binbuf* d;
    long flags;
	if (!(d = binbuf_via_atoms(argc,argv)))
		return NULL;
    
	x = (t_radio *)eobj_new(radio_class);
    flags = 0
    | EBOX_GROWINDI
    ;
	ebox_new((t_ebox *)x, flags);
    x->f_out = (t_outlet *)outlet_new((t_object *)x, &s_anything);
    
	ebox_attrprocess_viabinbuf(x, d);//To ensure the number of items has been setted before we compute the box size (not clean)
    ebox_attrprocess_viabinbuf(x, d);
	ebox_ready((t_ebox *)x);
    
	return (x);
}

void radio_getdrawparams(t_radio *x, t_object *patcherview, t_edrawparams *params)
{
	params->d_borderthickness   = 2;
	params->d_cornersize        = 2;
    params->d_bordercolor       = x->f_color_border;
    params->d_boxfillcolor      = x->f_color_background;
}

void radio_oksize(t_radio *x, t_rect *newrect)
{
    newrect->width = pd_clip_min(newrect->width, 8.);
    newrect->height = pd_clip_min(newrect->height, 8.);
    
    if(newrect->width > newrect->height)
        x->f_direction = 1;
    else
        x->f_direction = 0;
    
    if(x->f_direction)
    {
        newrect->width = pd_clip_min(newrect->width, x->f_nitems * newrect->height);
    }
    else
    {
        newrect->height = pd_clip_min(newrect->height, x->f_nitems * newrect->width);
    }
    if((int)newrect->width % 2 == 1)
        newrect->width++;
    if((int)newrect->height % 2 == 1)
        newrect->height++;
}

void radio_set(t_radio *x, t_symbol* s, long argc, t_atom* argv)
{
    int i;
    if(argc && argv)
    {
        if(x->f_mode)
        {
            for(i = 0; i < argc && i < x->f_nitems; i++)
            {
                if(atom_gettype(argv+i) == A_FLOAT && atom_getfloat(argv+i) == 0)
                    x->f_items[i] = 0;
                else
                    x->f_items[i] = 1;
            }
        }
        else if(atom_gettype(argv) == A_FLOAT && atom_getfloat(argv) >= 0 && atom_getfloat(argv) < x->f_nitems)
        {
            for(i = 0; i < x->f_nitems; i++)
                x->f_items[(i)] = 0;
            x->f_items[(int)atom_getfloat(argv)] = 1;
        }
        
        ebox_invalidate_layer((t_ebox *)x, gensym("items_layer"));
        ebox_redraw((t_ebox *)x);
    }
}

void radio_float(t_radio *x, float f)
{
    int i;
    if(x->f_mode || f < 0 || f > x->f_nitems)
        return;
    
    for(i = 0; i < x->f_nitems; i++)
        x->f_items[i] = 0;
    x->f_items[(int)f] = 1;
    
    ebox_invalidate_layer((t_ebox *)x, gensym("items_layer"));
    ebox_redraw((t_ebox *)x);
    radio_output(x);
}

void radio_list(t_radio *x, t_symbol* s, long argc, t_atom* argv)
{
    int i;
    if(argc && argv && x->f_mode)
    {
        for(i = 0; i < argc && i < x->f_nitems; i++)
        {
            if(atom_gettype(argv+i) == A_FLOAT && atom_getfloat(argv+i) == 0)
                x->f_items[i] = 0;
            else
                x->f_items[i] = 1;
        }
        ebox_invalidate_layer((t_ebox *)x, gensym("items_layer"));
        ebox_redraw((t_ebox *)x);
        radio_output(x);
    }
}

void radio_bang(t_radio *x)
{
    radio_output(x);
}

void radio_output(t_radio *x)
{
    int i;
    t_atom av[256];
    if(x->f_mode)
    {
        for(i = 0; i < x->f_nitems; i++)
            atom_setfloat(av+i, (float)x->f_items[i]);
        outlet_list(x->f_out, &s_list, x->f_nitems, av);
        if(ebox_getsender((t_ebox *) x))
            pd_list(ebox_getsender((t_ebox *) x), &s_list, x->f_nitems, av);
    }
    else
    {
        for(i = 0; i < x->f_nitems; i++)
        {
            if(x->f_items[i] != 0)
            {
                outlet_float(x->f_out, (float)i);
                if(ebox_getsender((t_ebox *) x))
                    pd_float(ebox_getsender((t_ebox *) x), (float)i);
            }
        }
    }
}

void radio_free(t_radio *x)
{
	ebox_free((t_ebox *)x);
}

void radio_assist(t_radio *x, void *b, long m, long a, char *s)
{
	;
}

t_pd_err radio_notify(t_radio *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	if (msg == gensym("attr_modified"))
	{
		if(s == gensym("bgcolor") || s == gensym("bdcolor") || s == gensym("itcolor"))
		{
			ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
            ebox_invalidate_layer((t_ebox *)x, gensym("items_layer"));
		}
        
        ebox_redraw((t_ebox *)x);
	}
	return 0;
}

void radio_paint(t_radio *x, t_object *view)
{
	t_rect rect;
	ebox_get_rect_for_view((t_ebox *)x, &rect);
    draw_background(x, view, &rect);
    draw_items(x, view, &rect);
}

void draw_background(t_radio *x, t_object *view, t_rect *rect)
{
	int i;
	t_elayer *g = ebox_start_layer((t_ebox *)x, gensym("background_layer"), rect->width, rect->height);
    
	if (g)
	{
        egraphics_set_color_rgba(g, &x->f_color_border);
        egraphics_set_line_width(g, 1);
        egraphics_rectangle(g, -1, -1, rect->width+1, rect->height+1);
        egraphics_fill(g);
        egraphics_set_color_rgba(g, &x->f_color_background);
        if(x->f_mode)
        {
            if(x->f_direction)
            {
                float ratio = rect->width / x->f_nitems;
                float offset = rect->height * 0.45;
                for(i = 0; i < x->f_nitems; i++)
                {
                    egraphics_rectangle(g, (i + 0.5) * ratio - offset, rect->height * 0.05, offset * 2., offset * 2.);
                    egraphics_fill(g);
                }
            }
            else
            {
                float ratio = rect->height / x->f_nitems;
                float offset = rect->width * 0.45;
                for(i = 0; i < x->f_nitems; i++)
                {
                    egraphics_rectangle(g, rect->width * 0.05, (i + 0.5) * ratio - offset, offset * 2., offset * 2.);
                    egraphics_fill(g);
                }
            }
        }
        else
        {
            if(x->f_direction)
            {
                float ratio = rect->width / x->f_nitems;
                for(i = 0; i < x->f_nitems; i++)
                {
                    egraphics_circle(g, (i + 0.5) * ratio, rect->height * 0.5, rect->height * 0.45);
                    egraphics_fill(g);
                }
            }
            else
            {
                float ratio = rect->height / x->f_nitems;
                for(i = 0; i < x->f_nitems; i++)
                {
                    egraphics_circle(g, rect->width * 0.5, (i + 0.5) * ratio, rect->width * 0.45);
                    egraphics_fill(g);
                }
            }
        }
        
        ebox_end_layer((t_ebox*)x, gensym("background_layer"));
	}
	ebox_paint_layer((t_ebox *)x, gensym("background_layer"), 0., 0.);
}

void draw_items(t_radio *x, t_object *view, t_rect *rect)
{
	int i;
	t_elayer *g = ebox_start_layer((t_ebox *)x, gensym("items_layer"), rect->width, rect->height);
    
	if (g)
	{
        egraphics_set_color_rgba(g, &x->f_color_item);
        egraphics_set_line_width(g, 2);
        if(x->f_mode)
        {
            if(x->f_direction)
            {
                float ratio = rect->width / x->f_nitems;
                float offset = rect->height * 0.35;
                for(i = 0; i < x->f_nitems; i++)
                {
                    if(x->f_items[i])
                    {
                        egraphics_rectangle(g, (i + 0.5) * ratio - offset, rect->height *0.15, offset * 2., offset * 2.);
                        egraphics_fill(g);
                    }
                }
            }
            else
            {
                float ratio = rect->height / x->f_nitems;
                float offset = rect->width * 0.35;
                for(i = 0; i < x->f_nitems; i++)
                {
                    if(x->f_items[i])
                    {
                        egraphics_rectangle(g, rect->width * 0.15, (i + 0.5) * ratio - offset, offset * 2., offset * 2.);
                        egraphics_fill(g);
                    }
                }
            }
        }
        else
        {
            if(x->f_direction)
            {
                float ratio = rect->width / x->f_nitems;
                for(i = 0; i < x->f_nitems; i++)
                {
                    if(x->f_items[i])
                    {
                        egraphics_circle(g, (i + 0.5) * ratio, rect->height * 0.5, rect->height * 0.35);
                        egraphics_fill(g);
                    }
                }
            }
            else
            {
                float ratio = rect->height / x->f_nitems;
                for(i = 0; i < x->f_nitems; i++)
                {
                    if(x->f_items[i])
                    {
                        egraphics_circle(g, rect->width * 0.5, (i + 0.5) * ratio, rect->width * 0.35);
                        egraphics_fill(g);
                    }
                }
            }
        }
        
        ebox_end_layer((t_ebox*)x, gensym("items_layer"));
	}
	ebox_paint_layer((t_ebox *)x, gensym("items_layer"), 0., 0.);
}

void radio_mousedown(t_radio *x, t_object *patcherview, t_pt pt, long modifiers)
{
    int i, index;
    if(x->f_direction)
    {
        index = (pt.x - 2) / x->j_box.b_rect.width * x->f_nitems;
    }
    else
    {
        index = (pt.y - 2) / x->j_box.b_rect.height * x->f_nitems;
    }
    
    if(index >= 0 && index < x->f_nitems)
    {
        if(x->f_mode)
        {
            if(x->f_items[index] == 0)
                x->f_items[index] = 1;
            else
                x->f_items[index] = 0;
        }
        else
        {
            for(i = 0; i < x->f_nitems; i++)
                x->f_items[i] = 0;
            x->f_items[index] = 1;
        }
    }
    
    ebox_invalidate_layer((t_ebox *)x, gensym("items_layer"));
    ebox_redraw((t_ebox *)x);
    radio_output(x);
}

void radio_preset(t_radio *x, t_binbuf *b)
{
    int i;
    
    if(x->f_mode)
    {
        binbuf_addv(b, "s", gensym("list"));
        for(i = 0; i < x->f_nitems; i++)
            binbuf_addv(b, "f", (float)x->f_items[i]);

    }
    else
    {
        for(i = 0; i < x->f_nitems; i++)
        {
            if(x->f_items[i])
                binbuf_addv(b, "sf", gensym("float"), (float)i);
        }
    }
}

t_pd_err radio_nitems_set(t_radio *x, t_object *attr, long ac, t_atom *av)
{
	int i;
    t_atom argv[2];
    if(ac && av && atom_gettype(av) == A_FLOAT)
    {
        if(pd_clip_minmax(atom_getfloat(av), 1, 256) != x->f_nitems)
        {
            x->f_nitems = pd_clip_minmax(atom_getfloat(av), 1, 256);
            for(i = 0; i < x->f_nitems; i++)
                x->f_items[i] = 0;
            
            ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
            ebox_invalidate_layer((t_ebox *)x, gensym("items_layer"));
            atom_setfloat(argv, x->j_box.b_rect.width);
            atom_setfloat(argv+1, x->j_box.b_rect.height);
            object_attr_setvalueof((t_object *)x, gensym("size"), 2, argv);
        }
    }
    return 0;
}

t_pd_err radio_mode_set(t_radio *x, t_object *attr, long ac, t_atom *av)
{
	int i;
    if(ac && av && atom_gettype(av) == A_FLOAT)
    {
        if(pd_clip_minmax(atom_getfloat(av), 0, 1) != x->f_mode)
        {
            x->f_mode = pd_clip_minmax(atom_getfloat(av), 0, 1);
            for(i = 0; i < x->f_nitems; i++)
                x->f_items[i] = 0;
            ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
            ebox_invalidate_layer((t_ebox *)x, gensym("items_layer"));
            ebox_redraw((t_ebox *)x);
        }
    }
    return 0;
}




