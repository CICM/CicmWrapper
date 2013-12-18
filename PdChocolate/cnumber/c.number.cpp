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

#ifndef _WINDOWS
#define sprintf_s sprintf
#endif


typedef struct  _number
{
	t_ebox      j_box;
    
    void*       f_outlet;
    bool        f_mode;
    
    float       f_value;
    float       f_refvalue;
    float       f_deriv;
    float       f_inc;
    
	char        f_textvalue[256];
    
    long        f_max_decimal;
    long        f_ndecimal;
    float       f_min;
    char        f_minbound;
    float       f_max;
    char        f_maxbound;
    
	t_rgba		f_color_background;
	t_rgba		f_color_border;
	t_rgba		f_color_text;
} t_number;

t_eclass *number_class;

void *number_new(t_symbol *s, int argc, t_atom *argv);
void number_free(t_number *x);
void number_assist(t_number *x, void *b, long m, long a, char *s);

void number_float(t_number *x, t_floatarg f);
void number_set(t_number *x, t_floatarg f);
void number_output(t_number *x);
t_pd_err number_notify(t_number *x, t_symbol *s, t_symbol *msg, void *sender, void *data);
t_pd_err number_min_set(t_number *x, t_object *attr, long ac, t_atom *av);
t_pd_err number_max_set(t_number *x, t_object *attr, long ac, t_atom *av);

void number_getdrawparams(t_number *x, t_object *patcherview, t_edrawparams *params);
void number_oksize(t_number *x, t_rect *newrect);

void number_paint(t_number *x, t_object *view);
void draw_background(t_number *x, t_object *view, t_rect *rect);
void draw_peak_value(t_number *x, t_object *view, t_rect *rect);
void draw_value_drag(t_number *x,  t_object *view, t_rect *rect);
void draw_value_text(t_number *x,  t_object *view, t_rect *rect);

void number_mousedown(t_number *x, t_object *patcherview, t_pt pt, long modifiers);
void number_dblclick(t_number *x, t_object *patcherview, t_pt pt, long modifiers);
void number_mousedrag(t_number *x, t_object *patcherview, t_pt pt, long modifiers);

void number_key(t_number *x, t_object *patcherview, char textcharacter, long modifiers);
void number_keyfilter(t_number *x, t_object *patcherview, char textcharacter, long modifiers);
void number_deserted(t_number *x);

void number_preset(t_number *x, t_binbuf *b);

extern "C" void setup_c0x2enumber(void)
{
	t_eclass *c;
    
	c = eclass_new("c.number", (method)number_new, (method)number_free, (short)sizeof(t_number), 0L, A_GIMME, 0);
    
	eclass_init(c, 0);
	
	eclass_addmethod(c, (method) number_assist,          "assist",           A_CANT, 0);
	eclass_addmethod(c, (method) number_paint,           "paint",            A_CANT, 0);
	eclass_addmethod(c, (method) number_notify,          "notify",           A_CANT, 0);
    eclass_addmethod(c, (method) number_getdrawparams,   "getdrawparams",    A_CANT, 0);
    eclass_addmethod(c, (method) number_oksize,          "oksize",           A_CANT, 0);
    eclass_addmethod(c, (method) number_float,           "float",            A_FLOAT,0);
    eclass_addmethod(c, (method) number_set,             "set",              A_FLOAT,0);
    eclass_addmethod(c, (method) number_output,          "bang",             A_CANT, 0);
    
    eclass_addmethod(c, (method) number_mousedown,        "mousedown",       A_CANT, 0);
    eclass_addmethod(c, (method) number_mousedrag,        "mousedrag",       A_CANT, 0);
    eclass_addmethod(c, (method) number_dblclick,         "dblclick",        A_CANT, 0);
    eclass_addmethod(c, (method) number_key,              "key",             A_CANT, 0);
    eclass_addmethod(c, (method) number_keyfilter,        "keyfilter",       A_CANT, 0);
    eclass_addmethod(c, (method) number_deserted,         "deserted",        A_CANT, 0);
    
    eclass_addmethod(c, (method) number_preset,           "preset",          A_CANT, 0);
    
	CLASS_ATTR_DEFAULT			(c, "size", 0, "53 13");
	
    CLASS_ATTR_FLOAT			(c, "min", 0, t_number, f_min);
	CLASS_ATTR_ORDER			(c, "min", 0, "3");
	CLASS_ATTR_LABEL			(c, "min", 0, "Minimum Value");
    CLASS_ATTR_DEFAULT          (c, "min", 0, "nan");
    CLASS_ATTR_ACCESSORS        (c, "min", NULL, number_min_set);
	CLASS_ATTR_SAVE				(c, "min", 1);
    
    CLASS_ATTR_FLOAT			(c, "max", 0, t_number, f_max);
	CLASS_ATTR_ORDER			(c, "max", 0, "3");
	CLASS_ATTR_LABEL			(c, "max", 0, "Maximum Value");
    CLASS_ATTR_DEFAULT          (c, "max", 0, "nan");
    CLASS_ATTR_ACCESSORS        (c, "max", NULL, number_max_set);
	CLASS_ATTR_SAVE				(c, "max", 1);
    
    CLASS_ATTR_LONG				(c, "decimal", 0, t_number, f_ndecimal);
	CLASS_ATTR_ORDER			(c, "decimal", 0, "3");
	CLASS_ATTR_LABEL			(c, "decimal", 0, "Number of decimal");
    CLASS_ATTR_DEFAULT          (c, "decimal", 0, "6");
	CLASS_ATTR_FILTER_MIN		(c, "decimal", 0);
    CLASS_ATTR_FILTER_MAX		(c, "decimal", 6);
	CLASS_ATTR_SAVE				(c, "decimal", 1);
    
	CLASS_ATTR_RGBA                 (c, "bgcolor", 0, t_number, f_color_background);
	CLASS_ATTR_LABEL                (c, "bgcolor", 0, "Background Color");
	CLASS_ATTR_ORDER                (c, "bgcolor", 0, "1");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bgcolor", 0, "0.75 0.75 0.75 1.");
	
	CLASS_ATTR_RGBA                 (c, "bdcolor", 0, t_number, f_color_border);
	CLASS_ATTR_LABEL                (c, "bdcolor", 0, "Box Border Color");
	CLASS_ATTR_ORDER                (c, "bdcolor", 0, "2");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bdcolor", 0, "0.5 0.5 0.5 1.");
	
	CLASS_ATTR_RGBA                 (c, "textcolor", 0, t_number, f_color_text);
	CLASS_ATTR_LABEL                (c, "textcolor", 0, "Text Color");
	CLASS_ATTR_ORDER                (c, "textcolor", 0, "3");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "textcolor", 0, "0. 0. 0. 1.");
	
    eclass_register(CLASS_NOBOX, c);
	number_class = c;
}

void *number_new(t_symbol *s, int argc, t_atom *argv)
{
	t_number *x =  NULL;
	t_binbuf* d;
    long flags;
    
	if (!(d = binbuf_via_atoms(argc,argv)))
		return NULL;
    
	x = (t_number *)ebox_alloc(number_class);
    
    flags = 0
    | EBOX_GROWINDI
    ;
    
	ebox_new((t_ebox *)x, flags);
	x->j_box.b_firstin = (t_object *)x;
    
    x->f_outlet   = floatout(x);
    x->f_mode     = 0.f;
    sprintf(x->f_textvalue, "0.");
    x->f_value    = 0;
    
	binbuf_attr_process(x, d);
	ebox_ready((t_ebox *)x);
    
	return (x);
}

void number_getdrawparams(t_number *x, t_object *patcherview, t_edrawparams *params)
{
	params->d_borderthickness   = 2;
	params->d_cornersize        = 2;
    params->d_bordercolor       = x->f_color_border;
    params->d_boxfillcolor      = x->f_color_background;
}

void number_oksize(t_number *x, t_rect *newrect)
{
    newrect->width = pd_clip_min(newrect->width, sys_fontwidth(x->j_box.e_font.c_size) * 3 + 8);
    newrect->height = sys_fontheight(x->j_box.e_font.c_size) + 4;
}

void number_float(t_number *x, t_floatarg f)
{
    x->f_value = f;
    number_output(x);
    ebox_invalidate_layer((t_ebox *)x, gensym("value_layer"));
    ebox_redraw((t_ebox *)x);
}

void number_set(t_number *x, t_floatarg f)
{
    x->f_value = f;
    ebox_invalidate_layer((t_ebox *)x, gensym("value_layer"));
    ebox_redraw((t_ebox *)x);
}

void number_output(t_number *x)
{
    if(x->f_maxbound)
        x->f_value = pd_clip_max(x->f_value, x->f_max);
    if(x->f_minbound)
        x->f_value = pd_clip_min(x->f_value, x->f_min);
    
    outlet_float((t_outlet*)x->f_outlet, (float)x->f_value);
}

void number_free(t_number *x)
{
	ebox_free((t_ebox *)x);
}

void number_assist(t_number *x, void *b, long m, long a, char *s)
{
	;
}

t_pd_err number_notify(t_number *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	if (msg == gensym("attr_modified"))
	{
		if(s == gensym("bgcolor") || s == gensym("bdcolor") || s == gensym("textcolor"))
		{
			ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
			ebox_invalidate_layer((t_ebox *)x, gensym("value_layer"));
		}
        if(s == gensym("fontsize"))
        {
            object_attr_setvalueof((t_object *)x, gensym("size"), 0, NULL);
        }
        ebox_redraw((t_ebox *)x);
	}
	return 0;
}

void number_paint(t_number *x, t_object *view)
{
	t_rect rect;
    float fontwidth = sys_fontwidth(x->j_box.e_font.c_size);
    ebox_get_rect_for_view((t_ebox *)x, &rect);
    
    x->f_max_decimal = (rect.width - fontwidth - 8) / fontwidth - 2;
    draw_background(x, view, &rect);

    if(!x->f_mode)
        draw_value_drag(x, view, &rect);
    else
        draw_value_text(x, view, &rect);
}

void draw_background(t_number *x, t_object *view, t_rect *rect)
{
	t_elayer *g = ebox_start_layer((t_ebox *)x, gensym("background_layer"), rect->width, rect->height);
	t_etext *jtl = etext_layout_create();
    
	if (g && jtl)
	{
        etext_layout_set(jtl, "©", &x->j_box.e_font, 1.5, rect->height / 2., rect->width, 0, ETEXT_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP);
        
        etext_layout_settextcolor(jtl, &x->f_color_text);
        etext_layout_draw(jtl, g);
        
        egraphics_set_line_width(g, 2);
        egraphics_set_color_rgba(g, &x->f_color_border);
        egraphics_move_to(g, sys_fontwidth(x->j_box.e_font.c_size) + 6, 0);
        egraphics_line_to(g, sys_fontwidth(x->j_box.e_font.c_size) + 6,  rect->height );
        egraphics_stroke(g);
        
		ebox_end_layer((t_ebox*)x, gensym("background_layer"));
	}
	ebox_paint_layer((t_ebox *)x, gensym("background_layer"), 0., 0.);
    etext_layout_destroy(jtl);
}

void draw_value_drag(t_number *x, t_object *view, t_rect *rect)
{
	t_elayer *g = ebox_start_layer((t_ebox *)x, gensym("value_layer"), rect->width, rect->height);
	t_etext *jtl = etext_layout_create();
    
	if (g && jtl)
	{
        int size, inc = 7;
        float peak;
        char number[256];
        sprintf_s(number, "%i", (int)x->f_value);
        size = strlen(number);
        // TRONQUER LE NOMBRE ENTIER
        if(size > x->f_max_decimal+1)
        {
            sprintf_s(number, "%i...", (int)(x->f_value / pow(10, size - (x->f_max_decimal+1))));
        }
        // TRONQUER LES DECIMALS
        else
        {
            // ENLEVER LES ZERO
            if(x->f_value == 0.f)
                inc = 0;
            else
            {
                peak = (int)(x->f_value * 1000000);
                while((peak - (long)peak) == 0.f)
                {
                    peak /= 10.;
                    inc--;
                }
            }
            // TRONQUER SELON LE NOMBRE MAXIMUM DE DECIMAL
            if(inc > x->f_max_decimal - size)
                inc = x->f_max_decimal - size;
            if(inc > x->f_ndecimal)
                inc = x->f_ndecimal;
            
            if(inc == 0 || x->f_value == (int)x->f_value)
                sprintf_s(number, "%i.", (int)x->f_value);
            else if(inc == 1)
                sprintf_s(number, "%.1f", x->f_value);
            else if(inc == 2)
                sprintf_s(number, "%.2f", x->f_value);
            else if(inc == 3)
                sprintf_s(number, "%.3f", x->f_value);
            else if(inc == 4)
                sprintf_s(number, "%.4f", x->f_value);
            else if(inc == 5)
                sprintf_s(number, "%.5f", x->f_value);
            else
                sprintf_s(number, "%.6f", x->f_value);
        }
        etext_layout_settextcolor(jtl, &x->f_color_text);
        
        etext_layout_set(jtl, number, &x->j_box.e_font, sys_fontwidth(x->j_box.e_font.c_size) + 8, rect->height / 2., rect->width - 3, 0, ETEXT_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP);
        
        etext_layout_draw(jtl, g);
		ebox_end_layer((t_ebox*)x, gensym("value_layer"));
        etext_layout_destroy(jtl);
	}
	ebox_paint_layer((t_ebox *)x, gensym("value_layer"), 0., 0.);
}

void draw_value_text(t_number *x,  t_object *view, t_rect *rect)
{
    t_elayer *g = ebox_start_layer((t_ebox *)x, gensym("value_layer"), rect->width, rect->height);
	t_etext *jtl = etext_layout_create();

	if (g && jtl)
	{
        char number[256];
        sprintf_s(number, "%s|", x->f_textvalue);
        etext_layout_settextcolor(jtl, &x->f_color_text);
        
        etext_layout_set(jtl, number, &x->j_box.e_font, sys_fontwidth(x->j_box.e_font.c_size) + 8, rect->height / 2., rect->width - 3, 0, ETEXT_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP);
        
        etext_layout_draw(jtl, g);
		ebox_end_layer((t_ebox*)x, gensym("value_layer"));
        etext_layout_destroy(jtl);
	}
	ebox_paint_layer((t_ebox *)x, gensym("value_layer"), 0., 0.);
}

void number_mousedown(t_number *x, t_object *patcherview, t_pt pt, long modifiers)
{
    x->f_mode = 0;
    float text_width = sys_fontwidth(x->j_box.e_font.c_size);
    if( pt.x >= text_width + 6)
    {
        int i = 1;
        int n_integer = 1;
        float pos = pt.x - text_width + 8 / text_width;
        x->f_deriv = pt.y;
        x->f_refvalue = x->f_value;
        while(fabs(x->f_refvalue) >= powf(10, n_integer))
            n_integer++;
        
        while(text_width + 6 + i * text_width < pos)
            i++;
        
        if(x->f_refvalue < 0) // due to "-" offset
        {
            if(i < n_integer)
                x->f_inc = -powf(10, (n_integer - i));
            else
                x->f_inc = -1. / powf(10, (i - n_integer - 1));
        }
        else
        {
            if(i < n_integer + 2)
                x->f_inc = -powf(10, (n_integer - i));
            else
                x->f_inc = -1. / powf(10, (i - n_integer - 1));
        }
        x->f_inc = pd_clip_minmax(x->f_inc, -100., -0.000001);
    }
}

void number_mousedrag(t_number *x, t_object *patcherview, t_pt pt, long modifiers)
{
    x->f_mode = 0;
    ebox_set_cursor((t_ebox *)x, 2);
    
    if(modifiers == EMOD_SHIFT)
        x->f_value = x->f_refvalue + (pt.y - x->f_deriv) * x->f_inc * 0.01;
    else
        x->f_value = x->f_refvalue + (pt.y - x->f_deriv) * x->f_inc * 0.5;
    
    number_output(x);
    ebox_invalidate_layer((t_ebox *)x, gensym("value_layer"));
    ebox_redraw((t_ebox *)x);
}

void number_dblclick(t_number *x, t_object *patcherview, t_pt pt, long modifiers)
{
    if(x->f_mode == 0)
    {
        x->f_mode = 1;
        sprintf(x->f_textvalue, "");
        ebox_invalidate_layer((t_ebox *)x, gensym("value_layer"));
        ebox_redraw((t_ebox *)x);
    }
}

void number_key(t_number *x, t_object *patcherview, char textcharacter, long modifiers)
{
    if(!x->f_mode || strlen(x->f_textvalue) >= 256)
    {
        if(textcharacter == 'R')
        {
            x->f_value++;
            number_output(x);
        }
        else if(textcharacter == 'T')
        {
            x->f_value--;
            number_output(x);
        }
        
    }
    else
    {
        if(textcharacter == '-' && strlen(x->f_textvalue) == 0)
        {
            strncat(x->f_textvalue, &textcharacter, 1);
        }
        else if(textcharacter == '.')
        {
            if(atof(x->f_textvalue) - atoi(x->f_textvalue) == 0 && x->f_textvalue[strlen(x->f_textvalue)-1] != '.')
            {
                strncat(x->f_textvalue, &textcharacter, 1);
            }
        }
        else if(isdigit(textcharacter))
        {
            strncat(x->f_textvalue, &textcharacter, 1);
            
        }
    }
    
    ebox_invalidate_layer((t_ebox *)x, gensym("value_layer"));
    ebox_redraw((t_ebox *)x);
}

void number_keyfilter(t_number *x, t_object *patcherview, char textcharacter, long modifiers)
{
    if(!x->f_mode)
        return;

    if(textcharacter == EKEY_DEL)
    {
        if(strlen(x->f_textvalue) > 1)
        {
            sprintf(x->f_textvalue+strlen(x->f_textvalue)-1, "");
        }
        else
            sprintf_s(x->f_textvalue, "");
        
        ebox_invalidate_layer((t_ebox *)x, gensym("value_layer"));
        ebox_redraw((t_ebox *)x);            
    }
    else if (textcharacter == EKEY_TAB || textcharacter == EKEY_ENTER)
    {
        x->f_mode = 0;
        x->f_value = atof(x->f_textvalue);
        
        number_output(x);
        ebox_invalidate_layer((t_ebox *)x, gensym("value_layer"));
        ebox_redraw((t_ebox *)x);
    }
    else if (textcharacter == EKEY_ESC)
    {
        x->f_mode = 0;
        sprintf(x->f_textvalue, "");
        
        ebox_invalidate_layer((t_ebox *)x, gensym("value_layer"));
        ebox_redraw((t_ebox *)x);
    }
}

void number_deserted(t_number *x)
{
    x->f_mode = 0;
    sprintf(x->f_textvalue, "");
    ebox_invalidate_layer((t_ebox *)x, gensym("value_layer"));
    ebox_redraw((t_ebox *)x);
}

void number_preset(t_number *x, t_binbuf *b)
{
    binbuf_addv(b, "sf", gensym("float"), x->f_value);
}

t_pd_err number_min_set(t_number *x, t_object *attr, long ac, t_atom *av)
{
    if(ac && av)
    {
        if(atom_gettype(av) == A_FLOAT && atom_getfloat(av) != NAN)
        {
            x->f_min = atom_getfloat(av);
            x->f_minbound = 1;
            
            if(x->f_maxbound && x->f_max < x->f_min)
            {
                x->f_min = x->f_max;
                x->f_max = atom_getfloat(av);
            }
            
            if(x->f_maxbound)
                x->f_value = pd_clip_max(x->f_value, x->f_max);
            x->f_value = pd_clip_min(x->f_value, x->f_min);
            
            ebox_invalidate_layer((t_ebox *)x, gensym("value_layer"));
            ebox_redraw((t_ebox *)x);
            return 0;
        }
    }
    x->f_min = NAN;
    x->f_minbound = 0;
    ebox_invalidate_layer((t_ebox *)x, gensym("value_layer"));
    ebox_redraw((t_ebox *)x);
    return 0;
}

t_pd_err number_max_set(t_number *x, t_object *attr, long ac, t_atom *av)
{
    if(ac && av)
    {
        if(atom_gettype(av) == A_FLOAT && atom_getfloat(av) != NAN)
        {
            x->f_max = atom_getfloat(av);
            x->f_maxbound = 1;
            
            if(x->f_minbound && x->f_max < x->f_min)
            {
                x->f_max = x->f_min;
                x->f_min = atom_getfloat(av);
            }
            
            x->f_value = pd_clip_max(x->f_value, x->f_max);
            if(x->f_minbound)
                x->f_value = pd_clip_min(x->f_value, x->f_min);
            
            ebox_invalidate_layer((t_ebox *)x, gensym("value_layer"));
            ebox_redraw((t_ebox *)x);
            return 0;
        }
    }
    x->f_max = NAN;
    x->f_maxbound = 0;
    ebox_invalidate_layer((t_ebox *)x, gensym("value_layer"));
    ebox_redraw((t_ebox *)x);
    return 0;
}






