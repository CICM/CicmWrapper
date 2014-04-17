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

typedef struct _orpheus
{
	t_ebox      j_box;
    
    t_outlet*   f_out;
    t_symbol*   f_name;
	t_rgba		f_color_background;
	t_rgba		f_color_border;
    t_rgba		f_color_text;
} t_orpheus;

t_eclass *orpheus_class;

void *orpheus_new(t_symbol *s, int argc, t_atom *argv);
void orpheus_free(t_orpheus *x);
void orpheus_assist(t_orpheus *x, void *b, long m, long a, char *s);

void orpheus_output(t_orpheus *x, t_symbol* s, long argc, t_atom* argv);

t_pd_err orpheus_notify(t_orpheus *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

void orpheus_getdrawparams(t_orpheus *x, t_object *patcherview, t_edrawparams *params);
void orpheus_oksize(t_orpheus *x, t_rect *newrect);

void orpheus_paint(t_orpheus *x, t_object *view);
void draw_background(t_orpheus *x,  t_object *view, t_rect *rect);

void orpheus_mousedown(t_orpheus *x, t_object *patcherview, t_pt pt, long modifiers);
void orpheus_mouseup(t_orpheus *x, t_object *patcherview, t_pt pt, long modifiers);

extern "C" void setup_c0x2eorpheus(void)
{
	t_eclass *c;
    
	c = eclass_new("c.orpheus", (method)orpheus_new, (method)orpheus_free, (short)sizeof(t_orpheus), 0L, A_GIMME, 0);
    
	eclass_init(c, 0);
	
	eclass_addmethod(c, (method) orpheus_assist,          "assist",           A_CANT, 0);
	eclass_addmethod(c, (method) orpheus_paint,           "paint",            A_CANT, 0);
	eclass_addmethod(c, (method) orpheus_notify,          "notify",           A_CANT, 0);
    eclass_addmethod(c, (method) orpheus_getdrawparams,   "getdrawparams",    A_CANT, 0);
    eclass_addmethod(c, (method) orpheus_oksize,          "oksize",           A_CANT, 0);
    eclass_addmethod(c, (method) orpheus_output,          "float",            A_FLOAT,0);
    eclass_addmethod(c, (method) orpheus_output,          "bang",             A_CANT, 0);
    eclass_addmethod(c, (method) orpheus_output,          "list",             A_GIMME,0);
    eclass_addmethod(c, (method) orpheus_output,          "anything",         A_GIMME,0);
    
    eclass_addmethod(c, (method) orpheus_mousedown,       "mousedown",        A_CANT, 0);
    eclass_addmethod(c, (method) orpheus_mouseup,         "mouseup",          A_CANT, 0);
    
	CLASS_ATTR_DEFAULT              (c, "size", 0, "100. 15.");
    
    CLASS_ATTR_SYMBOL               (c, "cname", 0, t_orpheus, f_name);
	CLASS_ATTR_LABEL                (c, "cname", 0, "Chronos Name");
	CLASS_ATTR_ORDER                (c, "cname", 0, "1");
    
	CLASS_ATTR_RGBA                 (c, "bgcolor", 0, t_orpheus, f_color_background);
	CLASS_ATTR_LABEL                (c, "bgcolor", 0, "Background Color");
	CLASS_ATTR_ORDER                (c, "bgcolor", 0, "1");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bgcolor", 0, "0.75 0.75 0.75 1.");
	
	CLASS_ATTR_RGBA                 (c, "bdcolor", 0, t_orpheus, f_color_border);
	CLASS_ATTR_LABEL                (c, "bdcolor", 0, "Box Border Color");
	CLASS_ATTR_ORDER                (c, "bdcolor", 0, "2");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bdcolor", 0, "0.5 0.5 0.5 1.");
    
    CLASS_ATTR_RGBA                 (c, "textcolor", 0, t_orpheus, f_color_text);
	CLASS_ATTR_LABEL                (c, "textcolor", 0, "Text Color");
	CLASS_ATTR_ORDER                (c, "textcolor", 0, "3");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "textcolor", 0, "0. 0. 0. 1.");
	
	
    eclass_register(CLASS_NOBOX, c);
	orpheus_class = c;
}

void *orpheus_new(t_symbol *s, int argc, t_atom *argv)
{
	t_orpheus *x =  NULL;
	t_binbuf* d;
    long flags;
	if (!(d = binbuf_via_atoms(argc,argv)))
		return NULL;
    
	x = (t_orpheus *)ebox_alloc(orpheus_class);
    flags = 0
    | EBOX_GROWINDI
    ;
	ebox_new((t_ebox *)x, flags);
	x->j_box.b_firstin = (t_object *)x;
   
    x->f_out = (t_outlet *)bangout((t_object *)x);
	binbuf_attr_process(x, d);
	ebox_ready((t_ebox *)x);
	return (x);
}

void orpheus_getdrawparams(t_orpheus *x, t_object *patcherview, t_edrawparams *params)
{
	params->d_borderthickness   = 2;
	params->d_cornersize        = 2;
    params->d_bordercolor       = x->f_color_border;
    params->d_boxfillcolor      = x->f_color_background;
}

void orpheus_oksize(t_orpheus *x, t_rect *newrect)
{
    newrect->width = pd_clip_min(newrect->width, sys_fontwidth(ebox_getfontsize((t_ebox *)x)) * 10. + 4);
    newrect->height = pd_clip_min(newrect->height, sys_fontheight(ebox_getfontsize((t_ebox *)x)) + 4);
    
}

void orpheus_output(t_orpheus *x, t_symbol* s, long argc, t_atom* argv)
{
    ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
    ebox_redraw((t_ebox *)x);
}

void orpheus_free(t_orpheus *x)
{
	ebox_free((t_ebox *)x);
}

void orpheus_assist(t_orpheus *x, void *b, long m, long a, char *s)
{
	;
}

t_pd_err orpheus_notify(t_orpheus *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	if (msg == gensym("attr_modified"))
	{
		if(s == gensym("bgcolor") || s == gensym("bdcolor") || s == gensym("textcolor"))
		{
			ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
		}
        ebox_redraw((t_ebox *)x);
	}
	return 0;
}

void orpheus_paint(t_orpheus *x, t_object *view)
{
	t_rect rect;
	ebox_get_rect_for_view((t_ebox *)x, &rect);
    draw_background(x, view, &rect);
}

void draw_background(t_orpheus *x, t_object *view, t_rect *rect)
{
    float height    = sys_fontheight(ebox_getfontsize((t_ebox *)x)) + 4;
	t_elayer *g     = ebox_start_layer((t_ebox *)x, gensym("background_layer"), rect->width, rect->height);
    t_etext *jtl    = etext_layout_create();
	if (g && jtl)
	{
        egraphics_set_line_width(g, 2.);
        egraphics_set_color_rgba(g, &x->f_color_border);
        egraphics_line_fast(g, 0., height, rect->width, height);
        
        if(x->f_name)
        {
            etext_layout_set(jtl, x->f_name->s_name, &x->j_box.e_font, 5, height * 0.5, rect->width, 0, ETEXT_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP);
            etext_layout_settextcolor(jtl, &x->f_color_text);
            etext_layout_draw(jtl, g);
        }
        
        ebox_end_layer((t_ebox*)x, gensym("background_layer"));
	}
    if(jtl)
    {
        etext_layout_destroy(jtl);
    }
    
	ebox_paint_layer((t_ebox *)x, gensym("background_layer"), 0., 0.);
}

void orpheus_mousedown(t_orpheus *x, t_object *patcherview, t_pt pt, long modifiers)
{
    ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
    ebox_redraw((t_ebox *)x);
}

void orpheus_mouseup(t_orpheus *x, t_object *patcherview, t_pt pt, long modifiers)
{
    ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
    ebox_redraw((t_ebox *)x);
}







