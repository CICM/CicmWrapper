/**
 HoaLibrary : A High Order Ambisonics Library
 Copyright (c) 2012-2013 Julien Colafrancesco, Pierre Guillot, Eliott Paris, CICM, Universite Paris-8.
 All rights reserved.
 
 Website  : http://www.mshparisnord.fr/hoalibrary/
 Contacts : cicm.mshparisnord@gmail.com

 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 
 - Redistributions may not be sold, nor may they be used in a commercial product or activity.
 - Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 - Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 - Neither the name of the CICM nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIALL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "../../../PdEnhanced/Sources/pd_enhanced.h"

#ifndef _WINDOWS
#define sprintf_s sprintf
#endif

#define MAXITEMS 100

typedef struct  _menu
{
	t_ebox      j_box;
	
    t_outlet*   f_out_index;
    t_outlet*   f_out_item;
    t_symbol*   f_items[MAXITEMS];
    long        f_items_size;
    long        f_item_selected;
    
	t_rgba		f_color_background;
	t_rgba		f_color_border;
	t_rgba		f_color_text;
} t_menu;

t_eclass *menu_class;

void menu_setup(void);
void *menu_new(t_symbol *s, int argc, t_atom *argv);
void menu_free(t_menu *x);
void menu_assist(t_menu *x, void *b, long m, long a, char *s);


t_pd_err menu_notify(t_menu *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

void menu_append(t_menu *x, t_symbol *s, int argc, t_atom *argv);
void menu_insert(t_menu *x, t_symbol *s, int argc, t_atom *argv);
void menu_setitem(t_menu *x, t_symbol *s, int argc, t_atom *argv);
void menu_delete(t_menu *x, t_symbol *s, int argc, t_atom *argv);
void menu_clear(t_menu *x, t_symbol *s, int argc, t_atom *argv);

void menu_float(t_menu *x, t_floatarg f);
void menu_symbol(t_menu *x, t_symbol *s, int argc, t_atom *argv);
void menu_set(t_menu *x, t_symbol *s, int argc, t_atom *argv);
void menu_output(t_menu *x);

void menu_getdrawparams(t_menu *x, t_object *patcherview, t_edrawparams *params);
void menu_oksize(t_menu *x, t_rect *newrect);

void menu_paint(t_menu *x, t_object *view);
void draw_background(t_menu *x, t_object *view, t_rect *rect);
void draw_item(t_menu *x,  t_object *view, t_rect *rect);

void menu_mousedown(t_menu *x, t_object *patcherview, t_pt pt, long modifiers);
void menu_mousedrag(t_menu *x, t_object *patcherview, t_pt pt, long modifiers);
void menu_mousemove(t_menu *x, t_object *patcherview, t_pt pt, long modifiers);

void menu_setup(void)
{
	t_eclass *c;
    
	c = eclass_new("menu", (method)menu_new, (method)menu_free, (short)sizeof(t_menu), 0L, A_GIMME, 0);
    class_addcreator((t_newmethod)menu_new, gensym("umenu"), A_GIMME, 0);
    
    eclass_init(c, 0);

	eclass_addmethod(c, (method) menu_assist,          "assist",           A_CANT, 0);
	eclass_addmethod(c, (method) menu_paint,           "paint",            A_CANT, 0);
	eclass_addmethod(c, (method) menu_notify,          "notify",           A_CANT, 0);
    eclass_addmethod(c, (method) menu_getdrawparams,   "getdrawparams",    A_CANT, 0);
    eclass_addmethod(c, (method) menu_oksize,          "oksize",           A_CANT, 0);
    
    eclass_addmethod(c, (method) menu_append,          "append",           A_GIMME,0);
    eclass_addmethod(c, (method) menu_insert,          "insert",           A_GIMME,0);
    eclass_addmethod(c, (method) menu_setitem,         "setitem",          A_GIMME,0);
    eclass_addmethod(c, (method) menu_delete,          "delete",           A_GIMME,0);
    eclass_addmethod(c, (method) menu_clear,           "clear",            A_GIMME,0);
    
    eclass_addmethod(c, (method) menu_float,           "float",            A_FLOAT,0);
    eclass_addmethod(c, (method) menu_symbol,          "anything",         A_GIMME,0);
    eclass_addmethod(c, (method) menu_set,             "set",              A_GIMME,0);
    eclass_addmethod(c, (method) menu_output,          "bang",             A_CANT, 0);
    
    eclass_addmethod(c, (method) menu_mousedown,        "mousedown",       A_CANT, 0);
    eclass_addmethod(c, (method) menu_mousemove,        "mousemove",       A_CANT, 0);
    
	CLASS_ATTR_DEFAULT			(c, "size", 0, "100 13");
    
    CLASS_ATTR_SYMBOL_VARSIZE   (c, "items", 0, t_menu, f_items, f_items_size, MAXITEMS);
    CLASS_ATTR_LABEL			(c, "items", 0, "Items");
    CLASS_ATTR_ORDER			(c, "items", 0, "1");
    CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "items", 0, "");
    
	CLASS_ATTR_RGBA				(c, "bgcolor", 0, t_menu, f_color_background);
	CLASS_ATTR_LABEL			(c, "bgcolor", 0, "Background Color");
	CLASS_ATTR_ORDER			(c, "bgcolor", 0, "1");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "bgcolor", 0, "0.35 0.23 0.13 1.");
	
	CLASS_ATTR_RGBA				(c, "bdcolor", 0, t_menu, f_color_border);
	CLASS_ATTR_LABEL			(c, "bdcolor", 0, "Box Border Color");
	CLASS_ATTR_ORDER			(c, "bdcolor", 0, "2");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "bdcolor", 0, "0.27 0.21 0. 1");
	
	CLASS_ATTR_RGBA				(c, "textcolor", 0, t_menu, f_color_text);
	CLASS_ATTR_LABEL			(c, "textcolor", 0, "Text Color");
	CLASS_ATTR_ORDER			(c, "textcolor", 0, "3");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "textcolor", 0, "1. 1. 1. 1.");
	
    eclass_register(CLASS_NOBOX, c);
	menu_class = c;
}

void *menu_new(t_symbol *s, int argc, t_atom *argv)
{
	t_menu *x =  NULL;
	t_binbuf* d;
    long flags;
    
	if (!(d = binbuf_via_atoms(argc,argv)))
		return NULL;
    
	x = (t_menu *)ebox_alloc(menu_class);
    
    flags = 0
    | EBOX_GROWINDI
    ;
    
	ebox_new((t_ebox *)x, flags, argc, argv);
	x->j_box.b_firstin = (t_object *)x;
    
    x->f_out_index  = floatout(x);
    x->f_out_item   = listout(x);
    x->f_item_selected = 0;
	binbuf_attr_process(x, d);
	ebox_ready((t_ebox *)x);
    
	return (x);
}

void menu_free(t_menu *x)
{
	ebox_dspfree((t_ebox *)x);
}

void menu_assist(t_menu *x, void *b, long m, long a, char *s)
{
	;
}

// MENU GESTION //

t_symbol* atoms_to_sym(t_atom* argv, long argc)
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


long menu_symbol_exist(t_menu *x, t_symbol* s)
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

void menu_append(t_menu *x, t_symbol *s, int argc, t_atom *argv)
{
    t_symbol* item = atoms_to_sym(argv, argc);
    
    if(argc && argv && menu_symbol_exist(x, item) == -1)
    {
        x->f_items[x->f_items_size] = item;
        x->f_items_size++;
        ebox_invalidate_layer((t_object *)x, NULL, gensym("selection"));
        ebox_redraw((t_ebox *)x);
    }
}

void menu_insert(t_menu *x, t_symbol *s, int argc, t_atom *argv)
{
    int i;
    t_symbol* item = atoms_to_sym(argv+1, argc-1);
    
    if(argc > 1 && argv && atom_gettype(argv) == A_FLOAT && atom_getfloat(argv) >= 0 && menu_symbol_exist(x, item) == -1)
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
        ebox_invalidate_layer((t_object *)x, NULL, gensym("selection"));
        ebox_redraw((t_ebox *)x);
    }
}

void menu_setitem(t_menu *x, t_symbol *s, int argc, t_atom *argv)
{
    t_symbol* item = atoms_to_sym(argv+1, argc-1);
    if(argc > 1 && argv && atom_gettype(argv) == A_FLOAT && menu_symbol_exist(x, item) == -1)
    {
        if(atom_getfloat(argv) >= 0 && atom_getfloat(argv) < x->f_items_size)
            x->f_items[atom_getint(argv)] = item;
        ebox_invalidate_layer((t_object *)x, NULL, gensym("selection"));
        ebox_redraw((t_ebox *)x);
    }
}

void menu_delete(t_menu *x, t_symbol *s, int argc, t_atom *argv)
{
    int i;
    if(argc > 0 && argv && atom_gettype(argv) == A_FLOAT)
    {
        if(atom_getfloat(argv) >= 0 && atom_getfloat(argv) < x->f_items_size)
        {
            for(i = atom_getfloat(argv); i < x->f_items_size - 1; i++)
                x->f_items[i] = x->f_items[i+1];
            x->f_items_size--;
            for(i = x->f_items_size - 1; i < MAXITEMS; i++)
                x->f_items[i] = NULL;
            
            ebox_invalidate_layer((t_object *)x, NULL, gensym("selection"));
            ebox_redraw((t_ebox *)x);
        }
    }
}

void menu_clear(t_menu *x, t_symbol *s, int argc, t_atom *argv)
{
    int i;
    for(i = 0; i < MAXITEMS; i++)
    {
        x->f_items[i] = NULL;
    }
}

// MENU SELECTION AND OUTPUT

void menu_setfloat(t_menu *x, t_floatarg f)
{
    if(f >= 0 && f < x->f_items_size)
    {
        x->f_item_selected = f;
        ebox_invalidate_layer((t_object *)x, NULL, gensym("value_layer"));
        ebox_redraw((t_ebox *)x);
    }
}

void menu_setsymbol(t_menu *x, t_symbol* s)
{
    long i = menu_symbol_exist(x, s);
    if(i != -1)
        x->f_item_selected = i;
    ebox_invalidate_layer((t_object *)x, NULL, gensym("value_layer"));
    ebox_redraw((t_ebox *)x);
}

void menu_float(t_menu *x, t_floatarg f)
{
    menu_setfloat(x, f);
    menu_output(x);
}

void menu_symbol(t_menu *x, t_symbol *s, int argc, t_atom *argv)
{
    int i;
    t_atom* av = malloc(argc + 1);
    atom_setsym(av, s);
    for(i = 0; i < argc; i++)
        av[i+1] = argv[i];

    menu_setsymbol(x, atoms_to_sym(av, argc+1));
    menu_output(x);
}

void menu_set(t_menu *x, t_symbol *s, int argc, t_atom *argv)
{
    if(argc && argv)
    {
        if(atom_gettype(argv) == A_FLOAT)
            menu_setfloat(x, atom_getfloat(argv));
        else if (atom_gettype(argv) == A_SYMBOL)
            menu_setsymbol(x, atoms_to_sym(argv, argc));
    }
}

void menu_output(t_menu *x)
{
    if(x->f_items_size > 0)
    {
        outlet_float(x->f_out_index, x->f_item_selected);
        outlet_symbol(x->f_out_item, x->f_items[x->f_item_selected]);
    }
}

// MENU DRAW

void menu_getdrawparams(t_menu *x, t_object *patcherview, t_edrawparams *params)
{
	params->d_borderthickness   = 2;
	params->d_cornersize        = 2;
    params->d_bordercolor       = x->f_color_border;
    params->d_boxfillcolor      = x->f_color_background;
}

void menu_oksize(t_menu *x, t_rect *newrect)
{
    newrect->width = pd_clip_min(newrect->width, sys_fontwidth(x->j_box.e_font.c_size) * 3 + 8);
    newrect->height = sys_fontheight(x->j_box.e_font.c_size) + 4;
    if(newrect->width < newrect->height * 2)
        newrect->width = newrect->height * 2;
}

t_pd_err menu_notify(t_menu *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	if (msg == gensym("attr_modified"))
	{
		if(s == gensym("bgcolor") || s == gensym("bdcolor") || s == gensym("textcolor") || s == gensym("mode"))
		{
			ebox_invalidate_layer((t_object *)x, NULL, gensym("background_layer"));
			ebox_invalidate_layer((t_object *)x, NULL, gensym("value_layer"));
		}
        if(s == gensym("fontsize"))
        {
            object_attr_setvalueof((t_object *)x, gensym("size"), 0, NULL);
        }
        ebox_redraw((t_ebox *)x);
	}
	return 0;
}

void menu_paint(t_menu *x, t_object *view)
{
	t_rect rect;
	ebox_get_rect_for_view((t_object *)x, view, &rect);
    draw_background(x, view, &rect);
    draw_item(x, view, &rect);
}

void draw_background(t_menu *x, t_object *view, t_rect *rect)
{
	t_elayer *g = ebox_start_layer((t_object *)x, view, gensym("background_layer"), rect->width, rect->height);
    
	if (g)
	{
        // Separation
        egraphics_set_color_rgba(g, &x->f_color_border);
        egraphics_set_line_width(g, 2);
        egraphics_line_fast(g, 0., 0., 0., rect->height);
        
        // Arraw Up
        egraphics_move_to(g, rect->height * 0.3 + 1, rect->height * 0.4);
        egraphics_line_to(g, rect->height * 0.7 + 1, rect->height * 0.4);
        egraphics_line_to(g, rect->height * 0.5 + 1, rect->height * 0.1);
        egraphics_fill(g);
        
        // Arraw Down
        egraphics_move_to(g, rect->height * 0.3 + 1, rect->height * 0.6);
        egraphics_line_to(g, rect->height * 0.7 + 1, rect->height * 0.6);
        egraphics_line_to(g, rect->height * 0.5 + 1, rect->height * 0.9);
        egraphics_fill(g);
        
		ebox_end_layer((t_object*)x, view, gensym("background_layer"));
	}
	ebox_paint_layer((t_object *)x, view, gensym("background_layer"), rect->width - rect->height, 0.);
}

void draw_item(t_menu *x, t_object *view, t_rect *rect)
{
    ;
}

void menu_mousedown(t_menu *x, t_object *patcherview, t_pt pt, long modifiers)
{
    ;
}

void menu_mousedrag(t_menu *x, t_object *patcherview, t_pt pt, long modifiers)
{
    ;
}

void menu_mousemove(t_menu *x, t_object *patcherview, t_pt pt, long modifiers)
{
    ;
}





