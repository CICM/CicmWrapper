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
#define MAXBINBUF 2048

typedef struct _preset
{
	t_ebox      j_box;

    t_binbuf*   f_binbuf[2048];
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
void preset_interpolate(t_preset *x, float f);
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

t_symbol* s_preset;
t_symbol* s_interpolate;
t_symbol* s_null;
t_symbol* s_nothing;

extern "C" void setup_c0x2epreset(void)
{
	t_eclass *c;
    s_preset = gensym("preset");
    s_null   = gensym("(null)");
    s_nothing = gensym("''");
    s_interpolate = gensym("interpolate");

	c = eclass_new("c.preset", (method)preset_new, (method)preset_free, (short)sizeof(t_preset), 0L, A_GIMME, 0);

	eclass_init(c, 0);
    cream_initclass(c);

	eclass_addmethod(c, (method) preset_assist,          "assist",           A_CANT, 0);
	eclass_addmethod(c, (method) preset_paint,           "paint",            A_CANT, 0);
	eclass_addmethod(c, (method) preset_notify,          "notify",           A_CANT, 0);
    eclass_addmethod(c, (method) preset_getdrawparams,   "getdrawparams",    A_CANT, 0);
    eclass_addmethod(c, (method) preset_oksize,          "oksize",           A_CANT, 0);
    eclass_addmethod(c, (method) preset_store,           "store",            A_FLOAT,0);
    eclass_addmethod(c, (method) preset_clear,           "clear",            A_FLOAT,0);
    eclass_addmethod(c, (method) preset_float,           "float",            A_FLOAT,0);
    eclass_addmethod(c, (method) preset_interpolate,     "inter",            A_FLOAT,0);
    eclass_addmethod(c, (method) preset_clearall,        "clearall",         A_CANT,0);

    eclass_addmethod(c, (method) preset_mousemove,       "mousemove",        A_CANT, 0);
    eclass_addmethod(c, (method) preset_mousedown,       "mousedown",        A_CANT, 0);
    eclass_addmethod(c, (method) preset_mouseleave,      "mouseleave",       A_CANT, 0);

    eclass_addmethod(c, (method) preset_save,            "save",             A_CANT, 0);
    eclass_addmethod(c, (method) preset_read,            "read",             A_GIMME,0);
    eclass_addmethod(c, (method) preset_write,           "write",            A_GIMME,0);

    CLASS_ATTR_INVISIBLE            (c, "send", 1);
	CLASS_ATTR_DEFAULT              (c, "size", 0, "102 34");
    CLASS_ATTR_DEFAULT              (c, "fontsize", 0, "10");

	CLASS_ATTR_RGBA                 (c, "bgcolor", 0, t_preset, f_color_background);
	CLASS_ATTR_LABEL                (c, "bgcolor", 0, "Background Color");
	CLASS_ATTR_ORDER                (c, "bgcolor", 0, "1");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bgcolor", 0, "0.75 0.75 0.75 1.");
    CLASS_ATTR_STYLE                (c, "bgcolor", 0, "color");

	CLASS_ATTR_RGBA                 (c, "bdcolor", 0, t_preset, f_color_border);
	CLASS_ATTR_LABEL                (c, "bdcolor", 0, "Border Color");
	CLASS_ATTR_ORDER                (c, "bdcolor", 0, "2");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bdcolor", 0, "0.5 0.5 0.5 1.");
    CLASS_ATTR_STYLE                (c, "bdcolor", 0, "color");

	CLASS_ATTR_RGBA                 (c, "textcolor", 0, t_preset, f_color_text);
	CLASS_ATTR_LABEL                (c, "textcolor", 0, "Text Color");
	CLASS_ATTR_ORDER                (c, "textcolor", 0, "3");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "textcolor", 0, "0. 0. 0. 1.");
    CLASS_ATTR_STYLE                (c, "textcolor", 0, "color");

	CLASS_ATTR_RGBA                 (c, "emcolor", 0, t_preset, f_color_button_empty);
	CLASS_ATTR_LABEL                (c, "emcolor", 0, "Empty Button Color");
	CLASS_ATTR_ORDER                (c, "emcolor", 0, "3");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "emcolor", 0, "0.85 0.85 0.85 1.");
    CLASS_ATTR_STYLE                (c, "emcolor", 0, "color");

    CLASS_ATTR_RGBA                 (c, "stcolor", 0, t_preset, f_color_button_stored);
	CLASS_ATTR_LABEL                (c, "stcolor", 0, "Stored Button Color");
	CLASS_ATTR_ORDER                (c, "stcolor", 0, "3");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "stcolor", 0, "0.5 0.5 0.5 1.");
    CLASS_ATTR_STYLE                (c, "stcolor", 0, "color");

    CLASS_ATTR_RGBA                 (c, "secolor", 0, t_preset, f_color_button_selected);
	CLASS_ATTR_LABEL                (c, "secolor", 0, "Selected Button Color");
	CLASS_ATTR_ORDER                (c, "secolor", 0, "3");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "secolor", 0, "0.15 0.15 0.15 1.");
    CLASS_ATTR_STYLE                (c, "secolor", 0, "color");

    eclass_register(CLASS_BOX, c);
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

	x = (t_preset *)eobj_new(preset_class);
    x->f_init = 0;
    flags = 0
    | EBOX_GROWINDI
    ;
	ebox_new((t_ebox *)x, flags);

    for(i = 0; i < MAXBINBUF; i++)
    {
        x->f_binbuf[i]  = binbuf_new();
    }


    x->f_nbinbufs = 0;
    x->f_binbuf_selected = 0;
    x->f_binbuf_hover    = 0;

    preset_init(x, d);
	ebox_attrprocess_viabinbuf(x, d);
	ebox_ready((t_ebox *)x);
    x->f_init = 1;
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
    t_binbuf *b;
    char id[MAXPDSTRING];
    t_gobj *y = NULL;
    t_ebox *z = NULL;
    t_gotfn mpreset = NULL;
    t_atom av[2];

    int index = (int)(f - 1);

    if(index >= 0 && index < MAXBINBUF)
    {
        b = x->f_binbuf[index];
        if(binbuf_getnatom(b))
            binbuf_clear(b);

        for(y = eobj_getcanvas(x)->gl_list; y; y = y->g_next)
        {
            z = (t_ebox *)y;
            mpreset = zgetfn(&y->g_pd, s_preset);
            if(mpreset && z->b_objpreset_id)
            {
                if(z->b_objpreset_id != s_null && z->b_objpreset_id != s_nothing)
                {
                    sprintf(id, "@%s", z->b_objpreset_id->s_name);
                    atom_setsym(av, gensym(id));
                    atom_setsym(av+1, eobj_getclassname(z));
                    binbuf_add(b, 2, av);
                    mpreset(z, b);
                }
            }
            mpreset = NULL;
        }
    }
}

void preset_float(t_preset *x, float f)
{
    long ac = 0;
    t_atom* av = NULL;
    t_gobj *y = NULL;
    t_ebox *z = NULL;
    t_gotfn mpreset = NULL;
    char id[MAXPDSTRING];
    int index = (int)f;
    t_binbuf* b = NULL;

    if(!x->f_init)
        return;

    if(index < 1)
        index = 1;
    else if(index > MAXBINBUF)
        index = MAXBINBUF;

    b = x->f_binbuf[index-1];
    if(binbuf_getnatom(b) == 0 || binbuf_getvec(b) == NULL)
        return;

    x->f_binbuf_selected = index;

    for (y = eobj_getcanvas(x)->gl_list; y; y = y->g_next)
    {
        z = (t_ebox *)y;
        mpreset = zgetfn(&y->g_pd, s_preset);
        if(mpreset && z->b_objpreset_id != s_null && z->b_objpreset_id != s_nothing)
        {
            sprintf(id, "@%s", z->b_objpreset_id->s_name);
            binbuf_get_attribute(b, gensym(id), &ac, &av);
            if(ac > 1 && av && atom_gettype(av) == A_SYM && atom_gettype(av+1) == A_SYM)
            {
                if(eobj_getclassname(z) == atom_getsym(av))
                    pd_typedmess((t_pd *)z, atom_getsym(av+1), ac-2, av+2);
                free(av);
                av = NULL;
                ac = 0;
            }
        }
        mpreset = NULL;
    }
    
    ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
    ebox_redraw((t_ebox *)x);
}

void preset_interpolate(t_preset *x, float f)
{
    t_gobj *y;
    t_ebox *z;
    t_gotfn mpreset = NULL;
    t_gotfn minterp = NULL;

    long acdo, acup, ac, max;
    t_atom *avdo, *avup, *av;
    char id[MAXPDSTRING];
    int i, j, indexdo, indexup, realdo, realup;
    float ratio;

    if(!x->f_init)
        return;

    max = -1;
    for(i = MAXBINBUF-1; i >= 0; i--)
    {
        if(binbuf_getnatom(x->f_binbuf[i]))
        {
            max = i + 1;
            break;
        }
    }
    if(max < 1)
        return;

    indexdo = pd_clip_minmax(floorf(f)-1, 0, max-1);
    indexup = pd_clip_minmax(ceilf(f)-1, 0, max-1);
    if(indexdo == indexup || f <= 1 || f >= max)
    {
        preset_float(x, f);
        return;
    }

    x->f_binbuf_selected = indexup;

    // Look for all the objects in a canvas //
    for (y = eobj_getcanvas(x)->gl_list; y; y = y->g_next)
    {
        z = (t_ebox *)y;
        mpreset = zgetfn(&y->g_pd, s_preset);
        // We find a preset method so we can send preset //
        if(mpreset && z->b_objpreset_id != s_null && z->b_objpreset_id != s_nothing)
        {
            sprintf(id, "@%s", z->b_objpreset_id->s_name);
            realdo = -1;
            realup = -1;
            acdo = 0;
            acup = 0;

            // Look for all the preset from the smallest index to zero //
            for(j = indexdo; j >= 0 && realdo == -1; j--)
            {
                // We find a recorded preset //
                if(binbuf_getnatom(x->f_binbuf[j]))
                {
                    // We get the preset //
                    binbuf_get_attribute(x->f_binbuf[j], gensym(id), &acdo, &avdo);
                    if(acdo >= 2 && avdo && atom_gettype(avdo) == A_SYM && atom_gettype(avdo+1) == A_SYM)
                    {
                        // If the object is in the preset we record the preset else we skip this preset //
                        if(eobj_getclassname(z) == atom_getsym(avdo))
                        {
                            realdo = j;
                            break;
                        }
                        else
                        {
                            avdo = NULL;
                            acdo = 0;
                        }
                    }
                }
            }

            // Look for all the preset from the biggest index to the top //
            for(j = indexup; j <= max && realup == -1; j++)
            {
                // We find a recorded preset //
                if(binbuf_getnatom(x->f_binbuf[j]))
                {
                    // We get the preset //
                    binbuf_get_attribute(x->f_binbuf[j], gensym(id), &acup, &avup);
                    if(acup >= 2 && avup && atom_gettype(avup) == A_SYM && atom_gettype(avup+1) == A_SYM)
                    {
                        // If the object is in the preset we record the preset else we skip this preset //
                        if(eobj_getclassname(z)== atom_getsym(avup))
                        {
                            realup = j;
                            break;
                        }
                        else
                        {
                            avup = NULL;
                            acup = 0;
                        }
                    }
                }
            }

            // If we have the 2 presets with the same selector for this object then we make an interpolation //
            if(acdo && acup && atom_getsym(avdo+1) == atom_getsym(avup+1))
            {
                minterp = zgetfn(&y->g_pd, s_interpolate);
                if(minterp)
                {
                    t_atom theta;
                    ratio = (float)(f - (realdo + 1)) / (float)(realup - realdo);
                    atom_setfloat(&theta, ratio);
                    minterp((t_pd *)z, (short)acdo-2, avdo+2, (short)acup-2, avup+2, theta);
                }
                else
                {
                    ratio = (float)(f - (realdo + 1)) / (float)(realup - realdo);
                    ac = acdo;
                    av = (t_atom *)calloc(ac, sizeof(t_atom));
                    atom_setsym(av+1, atom_getsym(avdo+1));
                    for(j = 2; j < ac; j++)
                    {
                        if(j < acup)
                        {
                            if(atom_gettype(avdo+j) == A_FLOAT && atom_gettype(avup+j) == A_FLOAT )
                            {
                                atom_setfloat(av+j, atom_getfloat(avdo+j) * (1. - ratio) + atom_getfloat(avup+j) * ratio);
                            }
                            else
                            {
                                av[j] = avdo[j];
                            }
                        }
                        else
                        {
                            av[j] = avdo[j];
                        }
                    }

                    pd_typedmess((t_pd *)z, atom_getsym(av+1), ac-2, av+2);
                    free(av);
                }
            }
            // If we have only the smallest preset for this object //
            else if(acdo)
            {
                pd_typedmess((t_pd *)z, atom_getsym(avdo+1), acdo-2, avdo+2);
            }
            // If we have only the highest preset for this object //
            else if(acup)
            {
                pd_typedmess((t_pd *)z, atom_getsym(avup+1), acup-2, avup+2);
            }
        }
        mpreset = NULL;
    }
    ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
    ebox_redraw((t_ebox *)x);
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
	int i;
    for(i = 0; i < MAXBINBUF; i++)
        binbuf_clear(x->f_binbuf[i]);
    if(!x->f_init)
        return;
    x->f_binbuf_selected = 0;
    ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
    ebox_redraw((t_ebox *)x);
}

void preset_output(t_preset *x)
{
    ;
}

void preset_free(t_preset *x)
{
	int i;
	ebox_free((t_ebox *)x);
    for(i = 0; i < MAXBINBUF; i++)
        binbuf_free(x->f_binbuf[i]);
}

void preset_assist(t_preset *x, void *b, long m, long a, char *s)
{
	;
}

t_pd_err preset_notify(t_preset *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	if (msg == gensym("attr_modified"))
	{
		if(s == gensym("bgcolor") || s == gensym("bdcolor") || s == gensym("btcolor") || s == gensym("fontsize") || s == gensym("fontname") || s == gensym("fontweight") || s == gensym("fontslant"))
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
#ifdef __APPLE__
    x->j_box.b_font.c_size -= 3;
#elif _WINDOWS
    x->j_box.b_font.c_size -= 2;
#endif
	ebox_get_rect_for_view((t_ebox *)x, &rect);
    x->f_point_size = ebox_getfontsize((t_ebox *)x);
    draw_background(x, view, &rect);
    x->f_init = 1;

#ifdef __APPLE__
    x->j_box.b_font.c_size += 3;
#elif _WINDOWS
    x->j_box.b_font.c_size += 2;
#endif
}

void draw_background(t_preset *x, t_object *view, t_rect *rect)
{
	int i, xc, yc;
    char number[256];
    t_rgba color;
	t_elayer *g = ebox_start_layer((t_ebox *)x, gensym("background_layer"), rect->width, rect->height);
    t_etext *jtl = etext_layout_create();

	if (g && jtl)
	{
        for (xc = x->f_point_size * 1.25, yc = x->f_point_size * 1.25, i = 1;  yc + x->f_point_size / 2. < rect->height; )
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
            etext_layout_set(jtl, number, &x->j_box.b_font, xc, yc, rect->width, 0, ETEXT_CENTER, ETEXT_JCENTER, ETEXT_NOWRAP);
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
    int n_row_button = (x->j_box.b_rect.width - x->f_point_size * 1.24) / (x->f_point_size * 2.5) + 1;

    index = (int)((pt.y) / (x->f_point_size * 2.5)) * n_row_button;
    index += pd_clip_max((pt.x) / (x->f_point_size * 2.5) + 1, n_row_button);
    x->f_binbuf_hover = index;
    ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
    ebox_redraw((t_ebox *)x);
}

void preset_mousedown(t_preset *x, t_object *patcherview, t_pt pt, long modifiers)
{
    int index;
    int n_row_button = (x->j_box.b_rect.width - x->f_point_size * 1.24) / (x->f_point_size * 2.5) + 1;
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
	int i;
    binbuf_addv(d, "ss", gensym("@preset"), gensym("["));
    for(i = 0; i < MAXBINBUF; i++)
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
	int i, check;
    long index;
    long ac = binbuf_getnatom(d);
    t_atom* av = binbuf_getvec(d);

    for(i = 0; i < ac; i++)
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
                        check = 1;
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
            object_error(x, "preset : %s read failed", atom_getsym(argv)->s_name);
        else
        {
            preset_init(x, d);
            ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
            ebox_redraw((t_ebox *)x);
            post("preset : read %s.", atom_getsym(argv)->s_name);
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
            object_error(x, "preset : %s write failed", atom_getsym(argv)->s_name);
        else
            post("preset : write %s.", atom_getsym(argv)->s_name);
            
    }
    if(d)
        binbuf_free(d);

}





