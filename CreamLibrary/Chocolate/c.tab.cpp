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

#define MAXITEMS 100

typedef struct  _tab
{
	t_ebox      j_box;

    t_outlet*   f_out_index;
    t_outlet*   f_out_item;
    t_outlet*   f_out_hover;

    t_symbol*   f_items[MAXITEMS];
    long        f_items_size;
    long        f_item_selected;
    long        f_item_hover;

    long        f_toggle;
    long        f_orientation;
	t_rgba		f_color_background;
	t_rgba		f_color_border;
	t_rgba		f_color_text;
    t_rgba		f_color_hover;
    t_rgba		f_color_select;
} t_tab;

t_eclass *tab_class;

void *tab_new(t_symbol *s, int argc, t_atom *argv);
void tab_free(t_tab *x);
void tab_assist(t_tab *x, void *b, long m, long a, char *s);

t_pd_err tab_notify(t_tab *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

void tab_append(t_tab *x, t_symbol *s, int argc, t_atom *argv);
void tab_insert(t_tab *x, t_symbol *s, int argc, t_atom *argv);
void tab_setitem(t_tab *x, t_symbol *s, int argc, t_atom *argv);
void tab_delete(t_tab *x, t_symbol *s, int argc, t_atom *argv);
void tab_clear(t_tab *x, t_symbol *s, int argc, t_atom *argv);
void tab_clean(t_tab *x);

void tab_float(t_tab *x, t_floatarg f);
void tab_symbol(t_tab *x, t_symbol *s, long argc, t_atom *argv);
void tab_set(t_tab *x, t_symbol *s, int argc, t_atom *argv);
void tab_output(t_tab *x);

void tab_getdrawparams(t_tab *x, t_object *patcherview, t_edrawparams *params);
void tab_oksize(t_tab *x, t_rect *newrect);

void tab_paint(t_tab *x, t_object *view);
void draw_selection(t_tab *x, t_object *view, t_rect *rect);
void draw_background(t_tab *x, t_object *view, t_rect *rect);
void draw_text(t_tab *x, t_object *view, t_rect *rect);

void tab_mousedown(t_tab *x, t_object *patcherview, t_pt pt, long modifiers);
void tab_mouseup(t_tab *x, t_object *patcherview, t_pt pt, long modifiers);
void tab_mouseleave(t_tab *x, t_object *patcherview, t_pt pt, long modifiers);
void tab_mousemove(t_tab *x, t_object *patcherview, t_pt pt, long modifiers);

void tab_preset(t_tab *x, t_binbuf *b);

extern "C" void setup_c0x2etab(void)
{
	t_eclass *c;

	c = eclass_new("c.tab", (method)tab_new, (method)tab_free, (short)sizeof(t_tab), 0L, A_GIMME, 0);

    eclass_init(c, 0);
    cream_initclass(c);

	eclass_addmethod(c, (method) tab_assist,          "assist",           A_CANT, 0);
	eclass_addmethod(c, (method) tab_paint,           "paint",            A_CANT, 0);
	eclass_addmethod(c, (method) tab_notify,          "notify",           A_CANT, 0);
    eclass_addmethod(c, (method) tab_getdrawparams,   "getdrawparams",    A_CANT, 0);
    eclass_addmethod(c, (method) tab_oksize,          "oksize",           A_CANT, 0);

    eclass_addmethod(c, (method) tab_append,          "append",           A_GIMME,0);
    eclass_addmethod(c, (method) tab_insert,          "insert",           A_GIMME,0);
    eclass_addmethod(c, (method) tab_setitem,         "setitem",          A_GIMME,0);
    eclass_addmethod(c, (method) tab_delete,          "delete",           A_GIMME,0);
    eclass_addmethod(c, (method) tab_clear,           "clear",            A_GIMME,0);

    eclass_addmethod(c, (method) tab_float,           "float",            A_FLOAT,0);
    eclass_addmethod(c, (method) tab_symbol,          "anything",         A_GIMME,0);
    eclass_addmethod(c, (method) tab_set,             "set",              A_GIMME,0);
    eclass_addmethod(c, (method) tab_output,          "bang",             A_CANT, 0);

    eclass_addmethod(c, (method) tab_mousedown,        "mousedown",       A_CANT, 0);
    eclass_addmethod(c, (method) tab_mouseup,          "mouseup",         A_CANT, 0);
    eclass_addmethod(c, (method) tab_mousemove,        "mousemove",       A_CANT, 0);
    eclass_addmethod(c, (method) tab_mouseleave,       "mouseleave",      A_CANT, 0);
    eclass_addmethod(c, (method) tab_preset,           "preset",          A_CANT, 0);

	CLASS_ATTR_DEFAULT              (c, "size", 0, "100 13");

    CLASS_ATTR_LONG                 (c, "orientation", 0, t_tab, f_orientation);
	CLASS_ATTR_LABEL                (c, "orientation", 0, "Vertical Orientation");
	CLASS_ATTR_ORDER                (c, "orientation", 0, "1");
    CLASS_ATTR_FILTER_CLIP          (c, "orientation", 0, 1);
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "orientation", 0, "0");
    CLASS_ATTR_STYLE                (c, "orientation", 0, "onoff");
    
    CLASS_ATTR_LONG                 (c, "toggle", 0, t_tab, f_toggle);
	CLASS_ATTR_LABEL                (c, "toggle", 0, "Toggle Mode");
	CLASS_ATTR_ORDER                (c, "toggle", 0, "1");
    CLASS_ATTR_FILTER_CLIP          (c, "toggle", 0, 1);
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "toggle", 0, "0");
    CLASS_ATTR_STYLE                (c, "toggle", 0, "onoff");

    CLASS_ATTR_SYMBOL_VARSIZE       (c, "items", 0, t_tab, f_items, f_items_size, MAXITEMS);
    CLASS_ATTR_LABEL                (c, "items", 0, "Items");
    CLASS_ATTR_ORDER                (c, "items", 0, "1");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "items", 0, "(null)");

	CLASS_ATTR_RGBA                 (c, "bgcolor", 0, t_tab, f_color_background);
	CLASS_ATTR_LABEL                (c, "bgcolor", 0, "Background Color");
	CLASS_ATTR_ORDER                (c, "bgcolor", 0, "1");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bgcolor", 0, "0.75 0.75 0.75 1.");
    CLASS_ATTR_STYLE                (c, "bgcolor", 0, "color");
    
	CLASS_ATTR_RGBA                 (c, "bdcolor", 0, t_tab, f_color_border);
	CLASS_ATTR_LABEL                (c, "bdcolor", 0, "Border Color");
	CLASS_ATTR_ORDER                (c, "bdcolor", 0, "2");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bdcolor", 0, "0.5 0.5 0.5 1.");
    CLASS_ATTR_STYLE                (c, "bdcolor", 0, "color");
    
	CLASS_ATTR_RGBA                 (c, "textcolor", 0, t_tab, f_color_text);
	CLASS_ATTR_LABEL                (c, "textcolor", 0, "Text Color");
	CLASS_ATTR_ORDER                (c, "textcolor", 0, "3");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "textcolor", 0, "0. 0. 0. 1.");
    CLASS_ATTR_STYLE                (c, "textcolor", 0, "color");
    
    CLASS_ATTR_RGBA                 (c, "hocolor", 0, t_tab, f_color_hover);
	CLASS_ATTR_LABEL                (c, "hocolor", 0, "Hover Color");
	CLASS_ATTR_ORDER                (c, "hocolor", 0, "4");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "hocolor", 0, "0.5 0.5 0.5 1.");
    CLASS_ATTR_STYLE                (c, "hocolor", 0, "color");
    
    CLASS_ATTR_RGBA                 (c, "secolor", 0, t_tab, f_color_select);
	CLASS_ATTR_LABEL                (c, "secolor", 0, "Selection Color");
	CLASS_ATTR_ORDER                (c, "secolor", 0, "5");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "secolor", 0, "0.35 0.35 0.35 1.");
    CLASS_ATTR_STYLE                (c, "secolor", 0, "color");
    
    eclass_register(CLASS_BOX, c);
	tab_class = c;
}

void *tab_new(t_symbol *s, int argc, t_atom *argv)
{
	t_tab *x =  NULL;
	t_binbuf* d;
    long flags;

	if (!(d = binbuf_via_atoms(argc,argv)))
		return NULL;

	x = (t_tab *)eobj_new(tab_class);

    flags = 0
    | EBOX_GROWINDI
    ;

	ebox_new((t_ebox *)x, flags);

    x->f_out_index  = floatout(x);
    x->f_out_item   = listout(x);
    x->f_out_hover  = floatout(x);

    x->f_item_selected = -1;
    x->f_item_hover    = -1;
    x->f_items_size = 0;
    x->j_box.b_rect.width = 100;
	ebox_attrprocess_viabinbuf(x, d);
	ebox_ready((t_ebox *)x);

	return (x);
}

void tab_free(t_tab *x)
{
	ebox_free((t_ebox *)x);
}

void tab_assist(t_tab *x, void *b, long m, long a, char *s)
{
	;
}

// MENU GESTION //

void tab_sizemustchange(t_tab *x)
{
    t_rect rect;
    ebox_get_rect_for_view((t_ebox *)x, &rect);
    if(x->f_orientation)
    {
        if(rect.width < sys_fontwidth(x->j_box.b_font.c_size) * 3 || rect.height < (sys_fontheight(x->j_box.b_font.c_size) + 4) * pd_clip_min(x->f_items_size, 1))
            object_attr_setvalueof((t_object *)x, gensym("size"), 0, NULL);
        else
        {
            ebox_invalidate_layer((t_ebox *)x, gensym("text_layer"));
            ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
            ebox_invalidate_layer((t_ebox *)x, gensym("selection_layer"));
            ebox_redraw((t_ebox *)x);
        }
    }
    else
    {
        if(rect.width < sys_fontwidth(x->j_box.b_font.c_size) * 3 * pd_clip_min(x->f_items_size, 1) || rect.height < (sys_fontheight(x->j_box.b_font.c_size) + 4) * sys_fontheight(x->j_box.b_font.c_size) + 4)
            object_attr_setvalueof((t_object *)x, gensym("size"), 0, NULL);
        else
        {
            ebox_invalidate_layer((t_ebox *)x, gensym("text_layer"));
            ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
            ebox_invalidate_layer((t_ebox *)x, gensym("selection_layer"));
            ebox_redraw((t_ebox *)x);
        }
    }
}

t_symbol* tab_atoms_to_sym(t_atom* argv, long argc)
{
    int i;
    char temp[MAXPDSTRING];
    char text[MAXPDSTRING];
    atom_string(argv, text, MAXPDSTRING);
    for(i = 1; i < argc; i++)
    {
        atom_string(argv+i, temp, MAXPDSTRING);
        strcat(text, " ");
        strcat(text, temp);
    }
    return gensym(text);
}

long tab_symbol_exist(t_tab *x, t_symbol* s)
{
    long i;
    long j = -1;
    for(i = 0; i < x->f_items_size; i++)
    {
        if(!strcmp(s->s_name, x->f_items[i]->s_name))
        {
            j = i;
            break;
        }
    }
    return j;
}

void tab_append(t_tab *x, t_symbol *s, int argc, t_atom *argv)
{
    t_symbol* item = tab_atoms_to_sym(argv, argc);

    if(argc && argv && tab_symbol_exist(x, item) == -1)
    {
        x->f_items[x->f_items_size] = item;
        x->f_items_size++;
        tab_sizemustchange(x);
    }
}

void tab_insert(t_tab *x, t_symbol *s, int argc, t_atom *argv)
{
    int i;
    t_symbol* item = tab_atoms_to_sym(argv+1, argc-1);

    if(argc > 1 && argv && atom_gettype(argv) == A_FLOAT && atom_getfloat(argv) >= 0 && tab_symbol_exist(x, item) == -1)
    {
        if(atom_getfloat(argv) >= x->f_items_size)
            x->f_items[x->f_items_size] = item;
        else
        {
            for(i = x->f_items_size - 1; i >= atom_getfloat(argv); i--)
                x->f_items[i+1] = x->f_items[i];
            x->f_items[atom_getint(argv)] = item;
        }
        x->f_items_size++;

        tab_sizemustchange(x);
    }
}

void tab_setitem(t_tab *x, t_symbol *s, int argc, t_atom *argv)
{
    t_symbol* item = tab_atoms_to_sym(argv+1, argc-1);
    if(argc > 1 && argv && atom_gettype(argv) == A_FLOAT && tab_symbol_exist(x, item) == -1)
    {
        if(atom_getfloat(argv) >= 0 && atom_getfloat(argv) < x->f_items_size)
            x->f_items[atom_getint(argv)] = item;

        ebox_invalidate_layer((t_ebox *)x, gensym("text_layer"));
        ebox_invalidate_layer((t_ebox *)x, gensym("selection_layer"));
        ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
        ebox_redraw((t_ebox *)x);
    }
}

void tab_delete(t_tab *x, t_symbol *s, int argc, t_atom *argv)
{
    int i;
    if(argc > 0 && argv && atom_gettype(argv) == A_FLOAT)
    {
        if(atom_getfloat(argv) >= 0 && atom_getfloat(argv) < x->f_items_size)
        {
            for(i = atom_getfloat(argv); i < x->f_items_size - 1; i++)
                x->f_items[i] = x->f_items[i+1];
            x->f_items_size--;
            for(i = x->f_items_size; i < MAXITEMS; i++)
                x->f_items[i] = gensym("(null)");
        }
        tab_sizemustchange(x);
    }
}

void tab_clear(t_tab *x, t_symbol *s, int argc, t_atom *argv)
{
    int i;
    for(i = 0; i < MAXITEMS; i++)
    {
        x->f_items[i] = gensym("(null)");
    }
    x->f_items_size = 0;
    tab_sizemustchange(x);
}

void tab_clean(t_tab *x)
{
    int i, j;
    for(i = 0; i < x->f_items_size; i++)
    {
        if(x->f_items[i] == gensym("(null)"))
        {
            for(j = i; j < x->f_items_size; j++)
                x->f_items[j] = x->f_items[j+1];
            x->f_items_size--;
            i--;
        }
    }
}

// MENU SELECTION AND OUTPUT

void tab_setfloat(t_tab *x, t_floatarg f)
{
    if(!x->f_toggle)
        return;
    if(f >= 0 && f < x->f_items_size)
    {
        x->f_item_selected = f;
        ebox_invalidate_layer((t_ebox *)x, gensym("text_layer"));
        ebox_invalidate_layer((t_ebox *)x, gensym("selection_layer"));
        ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
        ebox_redraw((t_ebox *)x);
    }
}

void tab_setsymbol(t_tab *x, t_symbol* s)
{
    long i;
    if(!x->f_toggle)
        return;
    i = tab_symbol_exist(x, s);
    if(i != -1)
        x->f_item_selected = i;
    ebox_invalidate_layer((t_ebox *)x, gensym("text_layer"));
    ebox_invalidate_layer((t_ebox *)x, gensym("selection_layer"));
    ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
    ebox_redraw((t_ebox *)x);
}

void tab_float(t_tab *x, t_floatarg f)
{
    if(x->f_toggle)
    {
        tab_setfloat(x, f);
        tab_output(x);
    }
    else if(f >= 0 && f < x->f_items_size)
    {
        x->f_item_selected = f;
        tab_output(x);
        x->f_item_selected = -1;
    }
}

void tab_symbol(t_tab *x, t_symbol *s, long argc, t_atom *argv)
{
    int i;
    i = tab_symbol_exist(x, s);
    if(x->f_toggle)
    {
        if(i != -1)
            x->f_item_selected = i;
        ebox_invalidate_layer((t_ebox *)x, gensym("text_layer"));
        ebox_invalidate_layer((t_ebox *)x, gensym("selection_layer"));
        ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
        ebox_redraw((t_ebox *)x);
        tab_output(x);
    }
    else if(i != -1)
    {
        x->f_item_selected = i;
        tab_output(x);
        x->f_item_selected = -1;
    }
}

void tab_set(t_tab *x, t_symbol *s, int argc, t_atom *argv)
{
    if(argc && argv)
    {
        if(atom_gettype(argv) == A_FLOAT)
            tab_setfloat(x, atom_getfloat(argv));
        else if (atom_gettype(argv) == A_SYMBOL)
            tab_setsymbol(x, tab_atoms_to_sym(argv, argc));
    }
}

void tab_output(t_tab *x)
{
    if(x->f_items_size > 0)
    {
        outlet_float(x->f_out_index, x->f_item_selected);
        outlet_symbol(x->f_out_item, x->f_items[x->f_item_selected]);
        outlet_float(x->f_out_hover, x->f_item_hover);
        
        if(ebox_getsender((t_ebox *) x))
            pd_float(ebox_getsender((t_ebox *) x), (float)x->f_item_selected);
    }
}

// MENU DRAW

void tab_getdrawparams(t_tab *x, t_object *patcherview, t_edrawparams *params)
{
	params->d_borderthickness   = 2;
	params->d_cornersize        = 2;
    params->d_bordercolor       = x->f_color_border;
    params->d_boxfillcolor      = x->f_color_background;
}

void tab_oksize(t_tab *x, t_rect *newrect)
{
    if(x->f_orientation)
    {
        newrect->width = pd_clip_min(newrect->width, sys_fontwidth(x->j_box.b_font.c_size) * 3);
        newrect->height = pd_clip_min(newrect->height, (sys_fontheight(x->j_box.b_font.c_size) + 4) * pd_clip_min(x->f_items_size, 1));
    }
    else
    {
        newrect->width = pd_clip_min(newrect->width, sys_fontwidth(x->j_box.b_font.c_size) * 3 * pd_clip_min(x->f_items_size, 1));
        newrect->height = pd_clip_min(newrect->height, sys_fontheight(x->j_box.b_font.c_size) + 4);
    }
}

t_pd_err tab_notify(t_tab *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	if (msg == gensym("attr_modified"))
	{
		if(s == gensym("bgcolor") || s == gensym("bdcolor") || s == gensym("textcolor") || s == gensym("orientation") || s == gensym("hocolor") || s == gensym("secolor") || s == gensym("fontsize") || s == gensym("fontname") || s == gensym("fontweight") || s == gensym("fontslant"))
		{
            ebox_invalidate_layer((t_ebox *)x, gensym("selection_layer"));
            ebox_invalidate_layer((t_ebox *)x, gensym("text_layer"));
			ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
		}
        if(s == gensym("fontsize") || s == gensym("orientation") || s == gensym("items"))
        {
            object_attr_setvalueof((t_object *)x, gensym("size"), 0, NULL);
        }
        ebox_redraw((t_ebox *)x);
	}
	return 0;
}

void tab_paint(t_tab *x, t_object *view)
{
	t_rect rect;
	ebox_get_rect_for_view((t_ebox *)x, &rect);
    tab_clean(x);

    draw_selection(x, view, &rect);
    draw_background(x, view, &rect);
    draw_text(x, view, &rect);
}

void draw_selection(t_tab *x, t_object *view, t_rect *rect)
{
    float ratio;
	t_elayer *g = ebox_start_layer((t_ebox *)x, gensym("selection_layer"), rect->width, rect->height);
    t_etext *jtl = etext_layout_create();
	if(g && jtl)
	{
        if(x->f_orientation)
            ratio = rect->height / (float)x->f_items_size;
        else
            ratio = rect->width / (float)x->f_items_size;

        egraphics_set_line_width(g, 2);
        if(x->f_item_hover != -1)
        {
            egraphics_set_color_rgba(g, &x->f_color_hover);
            if(x->f_orientation)
                egraphics_rectangle(g, -2, ratio * x->f_item_hover - 1, rect->width+4, ratio + 1);
            else
                egraphics_rectangle(g, ratio * x->f_item_hover - 1, -2, ratio+2, rect->height+4);
            egraphics_fill(g);
        }

        if(x->f_item_selected != -1)
        {
            egraphics_set_color_rgba(g, &x->f_color_select);
            if(x->f_orientation)
                egraphics_rectangle(g, -4, ratio * x->f_item_selected - 1, rect->width+4, ratio + 1);
            else
                egraphics_rectangle(g, ratio * x->f_item_selected - 1, -2, ratio+2, rect->height+4);
            egraphics_fill(g);
        }
		ebox_end_layer((t_ebox*)x, gensym("selection_layer"));
	}
    etext_layout_destroy(jtl);
	ebox_paint_layer((t_ebox *)x, gensym("selection_layer"),  0., 0.);
}

void draw_background(t_tab *x, t_object *view, t_rect *rect)
{
    int i;
    float ratio;
	t_elayer *g = ebox_start_layer((t_ebox *)x, gensym("background_layer"), rect->width, rect->height);
    t_etext *jtl = etext_layout_create();
	if(g && jtl)
	{
        if(x->f_orientation)
            ratio = rect->height / (float)x->f_items_size;
        else
            ratio = rect->width / (float)x->f_items_size;

        egraphics_set_color_rgba(g, &x->f_color_border);
        egraphics_set_line_width(g, 2);
        for(i = 1; i < x->f_items_size; i++)
        {
            if(x->f_orientation)
                egraphics_line_fast(g, -2, ratio * i, rect->width + 4, ratio * i);
            else
                egraphics_line_fast(g, ratio * i, -2, ratio * i, rect->height + 4);
        }
		ebox_end_layer((t_ebox*)x, gensym("background_layer"));
	}
    etext_layout_destroy(jtl);
	ebox_paint_layer((t_ebox *)x, gensym("background_layer"),  0., 0.);
}

void draw_text(t_tab *x, t_object *view, t_rect *rect)
{
    int i;
    float ratio;
    t_elayer *g = ebox_start_layer((t_ebox *)x, gensym("text_layer"), rect->width, rect->height);
    t_etext *jtl = etext_layout_create();
	if (g && jtl)
	{
        if(x->f_orientation)
            ratio = rect->height / (float)x->f_items_size;
        else
            ratio = rect->width / (float)x->f_items_size;
        for(i = 0; i < x->f_items_size; i++)
        {
            if(x->f_items[i] != gensym("(null)"))
            {
                if(x->f_orientation)
                {

                    etext_layout_settextcolor(jtl, &x->f_color_text);
#ifdef __APPLE__
                    etext_layout_set(jtl, x->f_items[i]->s_name, &x->j_box.b_font, rect->width * 0.5, ratio * (i + 1.) - sys_fontheight(x->j_box.b_font.c_size) * 0.5, rect->width, 0, ETEXT_CENTER, ETEXT_JCENTER, ETEXT_NOWRAP);
#elif _WINDOWS
                    etext_layout_set(jtl, x->f_items[i]->s_name, &x->j_box.b_font, rect->width * 0.5, ratio * (i + 1.) - sys_fontheight(x->j_box.b_font.c_size) * 0.5, rect->width, 0, ETEXT_CENTER, ETEXT_JCENTER, ETEXT_NOWRAP);
#else
                    etext_layout_set(jtl, x->f_items[i]->s_name, &x->j_box.b_font, rect->width * 0.5, ratio * (i + 0.5), rect->width, 0, ETEXT_CENTER, ETEXT_JCENTER, ETEXT_NOWRAP);
#endif
                    etext_layout_draw(jtl, g);
                }
                else
                {
                    etext_layout_settextcolor(jtl, &x->f_color_text);
                    etext_layout_set(jtl, x->f_items[i]->s_name, &x->j_box.b_font, ratio * (i + 0.5), rect->height * 0.5, ratio - 3, 0, ETEXT_CENTER, ETEXT_JCENTER, ETEXT_WRAP);
                    etext_layout_draw(jtl, g);
                }
            }
        }

        ebox_end_layer((t_ebox*)x, gensym("text_layer"));
	}
	ebox_paint_layer((t_ebox *)x, gensym("text_layer"), 0., 0.);
}

void tab_mousedown(t_tab *x, t_object *patcherview, t_pt pt, long modifiers)
{
    int index;
    if(x->f_orientation)
    {
        index = (pt.y - 2.) / (x->j_box.b_rect.height / (float)x->f_items_size);
    }
    else
    {
        index = (pt.x) / (x->j_box.b_rect.width / (float)x->f_items_size);
    }
    x->f_item_selected = pd_clip_minmax(index, 0, x->f_items_size-1);
    ebox_invalidate_layer((t_ebox *)x, gensym("selection_layer"));
    ebox_invalidate_layer((t_ebox *)x, gensym("text_layer"));
    ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
    ebox_redraw((t_ebox *)x);
    tab_output(x);
}

void tab_mouseup(t_tab *x, t_object *patcherview, t_pt pt, long modifiers)
{
    if(!x->f_toggle)
    {
        x->f_item_selected = -1;
        ebox_invalidate_layer((t_ebox *)x, gensym("selection_layer"));
        ebox_invalidate_layer((t_ebox *)x, gensym("text_layer"));
        ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
        ebox_redraw((t_ebox *)x);
    }
}

void tab_mouseleave(t_tab *x, t_object *patcherview, t_pt pt, long modifiers)
{
    x->f_item_hover = -1;
    ebox_invalidate_layer((t_ebox *)x, gensym("selection_layer"));
    ebox_invalidate_layer((t_ebox *)x, gensym("text_layer"));
    ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
    ebox_redraw((t_ebox *)x);
    outlet_float(x->f_out_hover, x->f_item_hover);
}

void tab_mousemove(t_tab *x, t_object *patcherview, t_pt pt, long modifiers)
{
    int index;
    if(x->f_orientation)
    {
        index = (pt.y - 2.) / (x->j_box.b_rect.height / (float)x->f_items_size);
    }
    else
    {
        index = (pt.x) / (x->j_box.b_rect.width / (float)x->f_items_size);
    }
    x->f_item_hover = pd_clip_minmax(index, 0, x->f_items_size-1);
    ebox_invalidate_layer((t_ebox *)x, gensym("selection_layer"));
    ebox_invalidate_layer((t_ebox *)x, gensym("text_layer"));
    ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
    ebox_redraw((t_ebox *)x);
    outlet_float(x->f_out_hover, x->f_item_hover);
}

void tab_preset(t_tab *x, t_binbuf *b)
{
    binbuf_addv(b, "sf", gensym("float"), (float)x->f_item_selected);
}


