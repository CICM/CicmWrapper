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
}
typedef struct _subcanvas
{
	t_ebox      j_box;
    
    t_outlet*   f_out;
	t_rgba		f_color_background;
	t_rgba		f_color_border;
	
    t_symbol*   f_name;
    t_canvas*   f_subcanvas;
    t_canvas*   f_owncanvas;
} t_subcanvas;

t_eclass *subcanvas_class;

void *subcanvas_new(t_symbol *s, int argc, t_atom *argv);
void subcanvas_free(t_subcanvas *x);
void subcanvas_assist(t_subcanvas *x, void *b, long m, long a, char *s);

void subcanvas_output(t_subcanvas *x, t_symbol* s, long argc, t_atom* argv);

t_pd_err subcanvas_notify(t_subcanvas *x, t_symbol *s, t_symbol *msg, void *sender, void *data);
t_pd_err subcanvas_canvas_set(t_subcanvas *x, t_eattr *attr, long argc, t_atom* argv);

void subcanvas_getdrawparams(t_subcanvas *x, t_object *patcherview, t_edrawparams *params);
void subcanvas_oksize(t_subcanvas *x, t_rect *newrect);

void subcanvas_paint(t_subcanvas *x, t_object *view);
void draw_background(t_subcanvas *x,  t_object *view, t_rect *rect);

void subcanvas_mousedown(t_subcanvas *x, t_object *patcherview, t_pt pt, long modifiers);
void subcanvas_mouseup(t_subcanvas *x, t_object *patcherview, t_pt pt, long modifiers);

extern "C" void setup_c0x2esubcanvas(void)
{
	t_eclass *c;
    
	c = eclass_new("c.subcanvas", (method)subcanvas_new, (method)subcanvas_free, (short)sizeof(t_subcanvas), 0L, A_GIMME, 0);
    
	eclass_init(c, 0);
	
	eclass_addmethod(c, (method) subcanvas_assist,          "assist",           A_CANT, 0);
	eclass_addmethod(c, (method) subcanvas_paint,           "paint",            A_CANT, 0);
	eclass_addmethod(c, (method) subcanvas_notify,          "notify",           A_CANT, 0);
    eclass_addmethod(c, (method) subcanvas_getdrawparams,   "getdrawparams",    A_CANT, 0);
    eclass_addmethod(c, (method) subcanvas_oksize,          "oksize",           A_CANT, 0);
    eclass_addmethod(c, (method) subcanvas_output,          "float",            A_FLOAT,0);
    eclass_addmethod(c, (method) subcanvas_output,          "bang",             A_CANT, 0);
    eclass_addmethod(c, (method) subcanvas_output,          "list",             A_GIMME,0);
    eclass_addmethod(c, (method) subcanvas_output,          "anything",         A_GIMME,0);
    
    eclass_addmethod(c, (method) subcanvas_mousedown,       "mousedown",        A_CANT, 0);
    eclass_addmethod(c, (method) subcanvas_mouseup,         "mouseup",          A_CANT, 0);
    
    CLASS_ATTR_INVISIBLE            (c, "fontname", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontweight", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontslant", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontsize", 1);
	CLASS_ATTR_DEFAULT              (c, "size", 0, "16. 16.");
    
    CLASS_ATTR_SYMBOL               (c, "canvas", 0, t_subcanvas, f_name);
    CLASS_ATTR_ACCESSORS            (c, "canvas", NULL, subcanvas_canvas_set);
	CLASS_ATTR_LABEL                (c, "canvas", 0, "Array Name");
	CLASS_ATTR_ORDER                (c, "canvas", 0, "1");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "canvas", 0, "(null)");
    
	CLASS_ATTR_RGBA                 (c, "bgcolor", 0, t_subcanvas, f_color_background);
	CLASS_ATTR_LABEL                (c, "bgcolor", 0, "Background Color");
	CLASS_ATTR_ORDER                (c, "bgcolor", 0, "1");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bgcolor", 0, "0.75 0.75 0.75 1.");
	CLASS_ATTR_STYLE                (c, "bgcolor", 0, "color");
    
	CLASS_ATTR_RGBA                 (c, "bdcolor", 0, t_subcanvas, f_color_border);
	CLASS_ATTR_LABEL                (c, "bdcolor", 0, "Border Color");
	CLASS_ATTR_ORDER                (c, "bdcolor", 0, "2");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bdcolor", 0, "0.5 0.5 0.5 1.");
	CLASS_ATTR_STYLE                (c, "bdcolor", 0, "color");
	
    eclass_register(CLASS_BOX, c);
    cicm_post();
	subcanvas_class = c;
}

void *subcanvas_new(t_symbol *s, int argc, t_atom *argv)
{
    t_canvas* realcnv;
    t_atom av[1];
	t_subcanvas *x =  NULL;
	t_binbuf* d;
    long flags;
	if (!(d = binbuf_via_atoms(argc,argv)))
		return NULL;
    
    atom_setfloat(av, 0);
	x = (t_subcanvas *)eobj_new(subcanvas_class);
    realcnv = x->j_box.b_obj.o_canvas;
    x->f_owncanvas = canvas_new(0, 0, 0, NULL);
    canvas_vis(x->f_owncanvas, 0);
    pd_typedmess((t_pd *)x->f_owncanvas, gensym("pop"), 1, av);
    canvas_setcurrent(x->j_box.b_obj.o_canvas);
    x->j_box.b_obj.o_canvas = x->f_owncanvas;
    x->f_subcanvas = NULL;
    x->f_name   = gensym("(null)");
    flags = 0
    | EBOX_GROWINDI
    ;
	ebox_new((t_ebox *)x, flags);
    x->f_out = (t_outlet *)bangout((t_object *)x);
	ebox_attrprocess_viabinbuf(x, d);
	ebox_ready((t_ebox *)x);
    canvas_vis(x->f_owncanvas, 0);
    //pd_typedmess((t_pd *)x->f_owncanvas, gensym("pop"), 1, av);
    canvas_setcurrent(realcnv);
	return (x);
}

void subcanvas_getdrawparams(t_subcanvas *x, t_object *patcherview, t_edrawparams *params)
{
	params->d_borderthickness   = 2;
	params->d_cornersize        = 2;
    params->d_bordercolor       = x->f_color_border;
    params->d_boxfillcolor      = x->f_color_background;
}

void subcanvas_oksize(t_subcanvas *x, t_rect *newrect)
{
    newrect->width = pd_clip_min(newrect->width, 16.);
    newrect->height = pd_clip_min(newrect->height, 16.);
    if((int)newrect->width % 2 == 0)
        newrect->width++;
    if((int)newrect->height % 2 == 0)
        newrect->height++;
}

void subcanvas_output(t_subcanvas *x, t_symbol* s, long argc, t_atom* argv)
{
    ebox_invalidate_layer((t_ebox*)x, gensym("buffer_layer"));
    ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
    ebox_redraw((t_ebox *)x);
}

void subcanvas_free(t_subcanvas *x)
{
	ebox_free((t_ebox *)x);
}

void subcanvas_assist(t_subcanvas *x, void *b, long m, long a, char *s)
{
	;
}

t_pd_err subcanvas_notify(t_subcanvas *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	if (msg == gensym("attr_modified"))
	{
		if(s == gensym("bgcolor") || s == gensym("bdcolor") || s == gensym("bacolor"))
		{
			ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
		}
        ebox_redraw((t_ebox *)x);
	}
	return 0;
}

void subcanvas_paint(t_subcanvas *x, t_object *view)
{
	t_rect rect;
	ebox_get_rect_for_view((t_ebox *)x, &rect);
    draw_background(x, view, &rect);
}

void draw_background(t_subcanvas *x, t_object *view, t_rect *rect)
{
	t_elayer *g = ebox_start_layer((t_ebox *)x, gensym("background_layer"), rect->width, rect->height);
    
	if (g)
	{
        ebox_end_layer((t_ebox*)x, gensym("background_layer"));
	}
	ebox_paint_layer((t_ebox *)x, gensym("background_layer"), 0., 0.);
}

void subcanvas_mousedown(t_subcanvas *x, t_object *patcherview, t_pt pt, long modifiers)
{
    ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
    ebox_redraw((t_ebox *)x);
}

void subcanvas_mouseup(t_subcanvas *x, t_object *patcherview, t_pt pt, long modifiers)
{
    ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
    ebox_redraw((t_ebox *)x);
}


t_pd_err subcanvas_canvas_set(t_subcanvas *x, t_eattr *attr, long argc, t_atom* argv)
{
    t_atom a[6];
    t_canvas *owner = x->f_owncanvas;
    t_gobj *y = NULL;
    t_gobj *last = NULL;
    t_atom av[3];
    
    if(argc && argv && atom_gettype(argv) == A_SYM && atom_getsym(argv) != gensym("(null)"))
    {
        for(y = x->f_owncanvas->gl_list; y; )
        {
            last = y;
            y = y->g_next;
            x->f_owncanvas->gl_list = y;
            y = x->f_owncanvas->gl_list;
            pd_free((t_pd *)last);
        }

        atom_setfloat(av, 0);
        atom_setfloat(av+1, 127);
        atom_setfloat(av+2, 0);

        x->f_name = atom_getsym(argv);
        atom_setfloat(a, 0);
        atom_setfloat(a+1, 0);
        atom_setsym(a+2, x->f_name);

        pd_typedmess((t_pd *)owner, gensym("obj"), 3, a);
        canvas_loadbang(owner);
        
        for(y = x->f_owncanvas->gl_list; y; y = y->g_next)
        {
            if(eobj_getclassname(y) == gensym("canvas"))
                x->f_subcanvas = (t_canvas *)y;
        }
        
        if(x->f_subcanvas)
        {
            for(y = x->f_subcanvas->gl_list; y; y = y->g_next)
            {
                post(eobj_getclassname(y)->s_name);
                if(strncmp(eobj_getclassname(y)->s_name, "c.", 2) == 0 && eobj_isbox(y))
                {
                    gobj_vis(y, x->f_owncanvas, 1);
                }
            }
        }
    }

    return 0;
}






