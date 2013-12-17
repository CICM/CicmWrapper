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

#define MAXBINBUF 1024

typedef struct _preset
{
	t_ebox      j_box;
    
    t_binbuf**  f_binbuf;
    int         f_nbinbufs;
    int         f_binbuf_selected;
    int         f_binbuf_hover;
    float       f_point_size;
    
	t_rgba		f_color_background;
	t_rgba		f_color_border;
	t_rgba		f_color_button_stored;
    t_rgba		f_color_button_empty;
    t_rgba		f_color_button_selected;
    t_rgba		f_color_text;
    
    t_symbol**  f_preset;
    char        f_init;
} t_preset;

t_eclass *preset_class;

void *preset_new(t_symbol *s, int argc, t_atom *argv);
void preset_free(t_preset *x);
void preset_assist(t_preset *x, void *b, long m, long a, char *s);

void preset_store(t_preset *x, float f);
void preset_float(t_preset *x, float f);
void preset_clear(t_preset *x, float f);
void preset_clearall(t_preset *x);
void preset_output(t_preset *x);
void preset_inc(t_preset *x);
void preset_dec(t_preset *x);
void preset_save(t_preset *x, t_binbuf *d);
void preset_init(t_preset *x, t_binbuf *d);
void preset_read(t_preset *x, t_symbol *s, long argc, t_atom *argv);
void preset_write(t_preset *x, t_symbol *s, long argc, t_atom *argv);

t_pd_err preset_notify(t_preset *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

void preset_getdrawparams(t_preset *x, t_object *patcherview, t_edrawparams *params);
void preset_oksize(t_preset *x, t_rect *newrect);

void preset_paint(t_preset *x, t_object *view);
void draw_background(t_preset *x,  t_object *view, t_rect *rect);

void preset_mousemove(t_preset *x, t_object *patcherview, t_pt pt, long modifiers);
void preset_mousedown(t_preset *x, t_object *patcherview, t_pt pt, long modifiers);
void preset_mouseleave(t_preset *x, t_object *patcherview, t_pt pt, long modifiers);

extern "C" void cpreset_setup(void)
{
	t_eclass *c;
    
	c = eclass_new("cpreset", (method)preset_new, (method)preset_free, (short)sizeof(t_preset), 0L, A_GIMME, 0);
    
	eclass_init(c, 0);
	
	eclass_addmethod(c, (method) preset_assist,          "assist",           A_CANT, 0);
	eclass_addmethod(c, (method) preset_paint,           "paint",            A_CANT, 0);
	eclass_addmethod(c, (method) preset_notify,          "notify",           A_CANT, 0);
    eclass_addmethod(c, (method) preset_getdrawparams,   "getdrawparams",    A_CANT, 0);
    eclass_addmethod(c, (method) preset_oksize,          "oksize",           A_CANT, 0);
    eclass_addmethod(c, (method) preset_store,           "store",            A_FLOAT,0);
    eclass_addmethod(c, (method) preset_clear,           "clear",            A_FLOAT,0);
    eclass_addmethod(c, (method) preset_float,           "float",            A_FLOAT,0);
    eclass_addmethod(c, (method) preset_clearall,        "clear",            A_CANT, 0);
  
    eclass_addmethod(c, (method) preset_mousemove,       "mousemove",        A_CANT, 0);
    eclass_addmethod(c, (method) preset_mousedown,       "mousedown",        A_CANT, 0);
    eclass_addmethod(c, (method) preset_mouseleave,      "mouseleave",       A_CANT, 0);
    
    eclass_addmethod(c, (method) preset_save,            "save",             A_CANT, 0);
    eclass_addmethod(c, (method) preset_read,            "read",             A_GIMME,0);
    eclass_addmethod(c, (method) preset_write,           "write",            A_GIMME,0);
    
	CLASS_ATTR_DEFAULT              (c, "size", 0, "102 34");
    CLASS_ATTR_DEFAULT              (c, "fontsize", 0, "7");
    
	CLASS_ATTR_RGBA                 (c, "bgcolor", 0, t_preset, f_color_background);
	CLASS_ATTR_LABEL                (c, "bgcolor", 0, "Background Color");
	CLASS_ATTR_ORDER                (c, "bgcolor", 0, "1");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bgcolor", 0, "0.75 0.75 0.75 1.");
	
	CLASS_ATTR_RGBA                 (c, "bdcolor", 0, t_preset, f_color_border);
	CLASS_ATTR_LABEL                (c, "bdcolor", 0, "Box Border Color");
	CLASS_ATTR_ORDER                (c, "bdcolor", 0, "2");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bdcolor", 0, "0.5 0.5 0.5 1.");
	
	CLASS_ATTR_RGBA                 (c, "textcolor", 0, t_preset, f_color_text);
	CLASS_ATTR_LABEL                (c, "textcolor", 0, "Text Color");
	CLASS_ATTR_ORDER                (c, "textcolor", 0, "3");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "textcolor", 0, "0. 0. 0. 1.");
    
	CLASS_ATTR_RGBA                 (c, "emcolor", 0, t_preset, f_color_button_empty);
	CLASS_ATTR_LABEL                (c, "emcolor", 0, "Empty Button Color");
	CLASS_ATTR_ORDER                (c, "emcolor", 0, "3");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "emcolor", 0, "0.85 0.85 0.85 1.");
    
    CLASS_ATTR_RGBA                 (c, "stcolor", 0, t_preset, f_color_button_stored);
	CLASS_ATTR_LABEL                (c, "stcolor", 0, "Stored Button Color");
	CLASS_ATTR_ORDER                (c, "stcolor", 0, "3");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "stcolor", 0, "0.5 0.5 0.5 1.");
    
    CLASS_ATTR_RGBA                 (c, "secolor", 0, t_preset, f_color_button_selected);
	CLASS_ATTR_LABEL                (c, "secolor", 0, "Selected Button Color");
	CLASS_ATTR_ORDER                (c, "secolor", 0, "3");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "secolor", 0, "0.15 0.15 0.15 1.");
	
    eclass_register(CLASS_NOBOX, c);
	preset_class = c;
}

void *preset_new(t_symbol *s, int argc, t_atom *argv)
{
    int i;
	t_preset *x =  NULL;
	t_binbuf* d;
    long flags;
	if (!(d = binbuf_via_atoms(argc,argv)))
		return NULL;
    
	x = (t_preset *)ebox_alloc(preset_class);
    x->f_init = 0;
    flags = 0
    | EBOX_GROWINDI
    ;
	ebox_new((t_ebox *)x, flags);
	x->j_box.b_firstin = (t_object *)x;
    
    x->f_binbuf = (t_binbuf **)calloc(MAXBINBUF, sizeof(t_binbuf *));
    
    for(i = 0; i < MAXBINBUF; i++)
        x->f_binbuf[i]  = binbuf_new();
    
    x->f_nbinbufs = 0;
    x->f_binbuf_selected = 0;
    x->f_binbuf_hover    = 0;
    
    preset_init(x, d);
	binbuf_attr_process(x, d);
	ebox_ready((t_ebox *)x);

	return (x);
}

void preset_getdrawparams(t_preset *x, t_object *patcherview, t_edrawparams *params)
{
	params->d_borderthickness   = 2;
	params->d_cornersize        = 2;
    params->d_bordercolor       = x->f_color_border;
    params->d_boxfillcolor      = x->f_color_background;
}

void preset_oksize(t_preset *x, t_rect *newrect)
{
    newrect->width = pd_clip_min(newrect->width, 15.);
    newrect->height = pd_clip_min(newrect->height, 15.);
}

void preset_store(t_preset *x, float f)
{
    t_gobj *y;
    t_ebox *z;
    t_canvas* canvas = x->j_box.e_canvas;
    char id[MAXPDSTRING];
    int index = (int)f;
    
    if (index < 1 || index > MAXBINBUF)
        return;
    if(!x->f_init)
        return;
    
    binbuf_clear(x->f_binbuf[index-1]);
    for (y = canvas->gl_list; y; y = y->g_next)
    {
        for(int i = 0; i < y->g_pd->c_nmethod; i++)
        {
            if(y->g_pd->c_methods[i].me_name == gensym("preset"))
            {
                z = (t_ebox *)y;
                if(z->e_objpreset_id != gensym("(null)") && z->e_objpreset_id != gensym("''"))
                {
                    sprintf(id, "@%s", z->e_objpreset_id->s_name);
                    binbuf_addv(x->f_binbuf[index-1], "ss", gensym(id), z->e_obj.te_g.g_pd->c_name);
                    y->g_pd->c_methods[i].me_fun((t_object *)y, x->f_binbuf[index-1]);
                }
            }
        }
    }
}

void preset_float(t_preset *x, float f)
{
    t_gobj *y;
    t_ebox *z;
    long ac;
    t_atom* av;
    char id[MAXPDSTRING];
    t_canvas* canvas = x->j_box.e_canvas;
    int index = (int)f;
    
    if (index < 1 || index > MAXBINBUF)
        return;
    if(!x->f_init)
        return;
    if(binbuf_getnatom(x->f_binbuf[index-1]))
    {
        x->f_binbuf_selected = index;
        for (y = canvas->gl_list; y; y = y->g_next)
        {
            for(int i = 0; i < y->g_pd->c_nmethod; i++)
            {
                if(y->g_pd->c_methods[i].me_name == gensym("preset"))
                {
                    z = (t_ebox *)y;
                    sprintf(id, "@%s", z->e_objpreset_id->s_name);
                    binbuf_get_attribute(x->f_binbuf[index-1], gensym(id), &ac, &av);
                    
                    if(ac >= 2 && av && atom_gettype(av) == A_SYM && atom_gettype(av+1) == A_SYM)
                    {
                        if(z->e_obj.te_g.g_pd->c_name == atom_getsym(av))
                            pd_typedmess((t_pd *)z, atom_getsym(av+1), ac-2, av+2);
                        free(av);
                    }
                }
            }
        }
        ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
        ebox_redraw((t_ebox *)x);
    }
}

void preset_clear(t_preset *x, float f)
{
    int index = (int)f;
    
    if (index < 1 || index > MAXBINBUF)
        return;
    if(!x->f_init)
        return;
    
    if (x->f_binbuf_selected == index)
    {
        x->f_binbuf_selected = 0;
    }
    binbuf_clear(x->f_binbuf[index-1]);
    ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
    ebox_redraw((t_ebox *)x);
}

void preset_clearall(t_preset *x)
{
    for(int i = 0; i < MAXBINBUF; i++)
        binbuf_clear(x->f_binbuf[i]);
    if(!x->f_init)
        return;
    ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
    ebox_redraw((t_ebox *)x);
}

void preset_output(t_preset *x)
{
    ;
}

void preset_free(t_preset *x)
{
	ebox_free((t_ebox *)x);
    for(int i = 0; i < MAXBINBUF; i++)
        binbuf_free(x->f_binbuf[i]);
    free(x->f_binbuf);
}

void preset_assist(t_preset *x, void *b, long m, long a, char *s)
{
	;
}

t_pd_err preset_notify(t_preset *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	if (msg == gensym("attr_modified"))
	{
		if(s == gensym("bgcolor") || s == gensym("bdcolor") || s == gensym("btcolor"))
		{
			ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
		}
        ebox_redraw((t_ebox *)x);
	}
	return 0;
}

void preset_paint(t_preset *x, t_object *view)
{
	t_rect rect;
	ebox_get_rect_for_view((t_ebox *)x, &rect);
    x->f_point_size = ebox_font_size((t_ebox *)x);
    draw_background(x, view, &rect);
    x->f_init = 1;    
}

void draw_background(t_preset *x, t_object *view, t_rect *rect)
{
    char number[256];
    t_rgba color;
	t_elayer *g = ebox_start_layer((t_ebox *)x, gensym("background_layer"), rect->width, rect->height);
    t_etext *jtl = etext_layout_create();
    
	if (g && jtl)
	{
        for (int xc = x->f_point_size * 1.25, yc = x->f_point_size * 1.25, i = 1;  yc + x->f_point_size / 2. < rect->height; )
        {
            if(x->f_binbuf_selected == i)
                color = rgba_addContrast(x->f_color_button_selected, 0.1);
            else if(!binbuf_getnatom(x->f_binbuf[i-1]))
                color = rgba_addContrast(x->f_color_button_empty, 0.1);
            else if(binbuf_getnatom(x->f_binbuf[i-1]))
                color = rgba_addContrast(x->f_color_button_stored, -0.1);
            
            egraphics_set_color_rgba(g, &color);
            egraphics_circle(g, xc, yc, x->f_point_size);
            egraphics_fill(g);
            
            if(x->f_binbuf_hover == i)
            {
                egraphics_set_line_width(g, 2);
                egraphics_stroke(g);
                egraphics_set_line_width(g, 1);
            }
            
            sprintf(number, "%i", i);
            etext_layout_set(jtl, number, &x->j_box.e_font, xc, yc, rect->width, 0, ETEXT_CENTER, ETEXT_NOWRAP);
            etext_layout_settextcolor(jtl, &x->f_color_text);
            etext_layout_draw(jtl, g);
            
            xc += x->f_point_size * 2.5;
            if(xc + x->f_point_size / 2. > rect->width)
            {
                xc = x->f_point_size * 1.25;
                yc += x->f_point_size * 2.5;
            }
            i++;
        }
        
        ebox_end_layer((t_ebox*)x, gensym("background_layer"));
	}
	ebox_paint_layer((t_ebox *)x, gensym("background_layer"), 0., 0.);
}

void preset_mousemove(t_preset *x, t_object *patcherview, t_pt pt, long modifiers)
{
    int index;
    int n_row_button = (x->j_box.e_rect.width - x->f_point_size * 1.24) / (x->f_point_size * 2.5) + 1;
    
    index = (int)((pt.y) / (x->f_point_size * 2.5)) * n_row_button;
    index += pd_clip_max((pt.x) / (x->f_point_size * 2.5) + 1, n_row_button);
    x->f_binbuf_hover = index;
    ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
    ebox_redraw((t_ebox *)x);
}

void preset_mousedown(t_preset *x, t_object *patcherview, t_pt pt, long modifiers)
{
    int index;
    int n_row_button = (x->j_box.e_rect.width - x->f_point_size * 1.24) / (x->f_point_size * 2.5) + 1;
    index = (int)((pt.y) / (x->f_point_size * 2.5)) * n_row_button;
    index += pd_clip_max((pt.x) / (x->f_point_size * 2.5) + 1, n_row_button);
    x->f_binbuf_hover = index;
    
    if(modifiers == EMOD_ALT)
        preset_clear(x, index);
    if(modifiers == EMOD_SHIFT)
        preset_store(x, index);
    preset_float(x, index);
    
}

void preset_mouseleave(t_preset *x, t_object *patcherview, t_pt pt, long modifiers)
{
    x->f_binbuf_hover = 0;
    ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
    ebox_redraw((t_ebox *)x);
}

void preset_save(t_preset *x, t_binbuf *d)
{
    binbuf_addv(d, "ss", gensym("@preset"), gensym("["));
    for(int i = 0; i < MAXBINBUF; i++)
    {
        if(binbuf_getnatom(x->f_binbuf[i]))
        {
            binbuf_addv(d, "sf", gensym("@index"), (float)i);
            binbuf_addbinbuf(d, x->f_binbuf[i]);
        }
    }
    binbuf_addv(d, "s", gensym("]"));
}

void preset_init(t_preset *x, t_binbuf *d)
{
    long index;
    long ac = binbuf_getnatom(d);
    t_atom* av = binbuf_getvec(d);
    
    for(int i = 0; i < ac; i++)
    {
        if(atom_gettype(av+i) == A_SYM && atom_getsym(av+i) == gensym("@preset"))
        {
            for(;i < ac; i++)
            {
                if(atom_gettype(av+i) == A_SYM && atom_getsym(av+i) == gensym("@index"))
                {
                    i++;
                    if(i+1 < ac && atom_gettype(av+i) == A_LONG)
                    {
                        index = atom_getlong(av+i);
                        binbuf_clear(x->f_binbuf[index]);
                        i++;
                        int check = 1;
                        for(; i < ac && check; i++)
                        {
                            if(atom_gettype(av+i) == A_SYM && atom_getsym(av+i) == gensym("@index"))
                            {
                                i -=  2;
                                check = 0;
                            }
                            else if(atom_gettype(av+i) == A_SYM && atom_getsym(av+i) == gensym("]"))
                            {
                                return;
                            }
                            else
                            {
                                binbuf_add(x->f_binbuf[index], 1, av+i);
                            }
                        }
                        
                    }
                }
            }
        }
    }
    
}

void preset_read(t_preset *x, t_symbol *s, long argc, t_atom *argv)
{
    t_binbuf *d = binbuf_new();
    if(d && argv && argc && atom_gettype(argv) == A_SYM)
    {
        if(binbuf_read(d, atom_getsym(argv)->s_name, "", 0))
        {
            object_error(x, "preset : %s read failed", atom_getsym(argv)->s_name);
        }
        else
        {
            preset_init(x, d);
            ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
            ebox_redraw((t_ebox *)x);
        }
    }
    if(d)
        binbuf_free(d);
}

void preset_write(t_preset *x, t_symbol *s, long argc, t_atom *argv)
{
    t_binbuf *d = binbuf_new();
    if(d && argv && argc && atom_gettype(argv) == A_SYM)
    {
        preset_save(x, d);
        if(binbuf_write(d, atom_getsym(argv)->s_name, "", 0))
        {
            object_error(x, "preset : %s write failed", atom_getsym(argv)->s_name);
        }
    }
    if(d)
        binbuf_free(d);
    
}





