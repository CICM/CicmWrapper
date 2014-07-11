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

#define MAXPOINTS 256

typedef struct _breakpoints
{
	t_ebox      j_box;

    t_outlet*   f_out_float;
    t_outlet*   f_out_list;
    t_outlet*   f_out_function;
    t_clock*    f_clock;

    t_pt        f_size;
    t_pt        f_mouse;
    long        f_outline_mode;
    t_symbol*   f_outline;
    float       f_point_abscissa[MAXPOINTS];
    float       f_point_ordinate[MAXPOINTS];
    int         f_number_of_points;
    long        f_point_hover;
    long        f_point_selected;
    long        f_point_last_created;
    long        f_output_inc;
    char        f_output_nextprev;

    float       f_range_abscissa[2];
    float       f_range_ordinate[2];

	t_rgba		f_color_background;
	t_rgba		f_color_border;
	t_rgba		f_color_point;
    t_rgba		f_color_line;
    t_rgba		f_color_text;

} t_breakpoints;

t_eclass *breakpoints_class;

void *breakpoints_new(t_symbol *s, int argc, t_atom *argv);
void breakpoints_free(t_breakpoints *x);
void breakpoints_assist(t_breakpoints *x, void *b, long m, long a, char *s);

void breakpoints_float(t_breakpoints *x, float f);
void breakpoints_bang(t_breakpoints *x);
void breakpoints_inc(t_breakpoints *x);
void breakpoints_next(t_breakpoints *x);
void breakpoints_prev(t_breakpoints *x);
void breakpoints_getlist(t_breakpoints *x);

void breakpoints_erase(t_breakpoints *x);
void breakpoints_add(t_breakpoints *x, t_symbol* s, int argc, t_atom* argv);
void breakpoints_move(t_breakpoints *x, t_symbol* s, int argc, t_atom* argv);
void breakpoints_remove(t_breakpoints *x, t_symbol* s, int argc, t_atom* argv);

void breakpoints_scaleabs(t_breakpoints *x, t_symbol* s, int argc, t_atom* argv);
void breakpoints_scaleord(t_breakpoints *x, t_symbol* s, int argc, t_atom* argv);

t_pd_err breakpoints_notify(t_breakpoints *x, t_symbol *s, t_symbol *msg, void *sender, void *data);
void breakpoints_preset(t_breakpoints *x, t_binbuf *b);
void breakpoints_save(t_breakpoints *x, t_binbuf *d);
void breakpoints_init(t_breakpoints *x, t_binbuf *d);
void breakpoints_function(t_breakpoints *x, t_symbol* s, int argc, t_atom* argv);
void breakpoints_write(t_breakpoints *x, t_symbol *s, int argc, t_atom *argv);
void breakpoints_read(t_breakpoints *x, t_symbol *s, int argc, t_atom *argv);

void breakpoints_getdrawparams(t_breakpoints *x, t_object *patcherview, t_edrawparams *params);
void breakpoints_oksize(t_breakpoints *x, t_rect *newrect);

void breakpoints_paint(t_breakpoints *x, t_object *view);
void draw_text(t_breakpoints *x, t_object *view, t_rect *rect);
void draw_points(t_breakpoints *x,  t_object *view, t_rect *rect);

void breakpoints_mousedown(t_breakpoints *x, t_object *patcherview, t_pt pt, long modifiers);
void breakpoints_mousemove(t_breakpoints *x, t_object *patcherview, t_pt pt, long modifiers);
void breakpoints_mousedrag(t_breakpoints *x, t_object *patcherview, t_pt pt, long modifiers);
void breakpoints_mouseleave(t_breakpoints *x, t_object *patcherview, t_pt pt, long modifiers);
void breakpoints_mouseup(t_breakpoints *x, t_object *patcherview, t_pt pt, long modifiers);

extern "C" void setup_c0x2ebreakpoints(void)
{
	t_eclass *c;

	c = eclass_new("c.breakpoints", (method)breakpoints_new, (method)breakpoints_free, (short)sizeof(t_breakpoints), 0L, A_GIMME, 0);

	eclass_init(c, 0);
    cream_initclass(c);
    
	eclass_addmethod(c, (method) breakpoints_assist,          "assist",           A_CANT, 0);
	eclass_addmethod(c, (method) breakpoints_paint,           "paint",            A_CANT, 0);
	eclass_addmethod(c, (method) breakpoints_notify,          "notify",           A_CANT, 0);
    eclass_addmethod(c, (method) breakpoints_getdrawparams,   "getdrawparams",    A_CANT, 0);
    eclass_addmethod(c, (method) breakpoints_oksize,          "oksize",           A_CANT, 0);

    eclass_addmethod(c, (method) breakpoints_float,           "float",            A_FLOAT,0);
    eclass_addmethod(c, (method) breakpoints_bang,            "bang",             A_CANT, 0);
    eclass_addmethod(c, (method) breakpoints_next,            "next",             A_CANT, 0);
    eclass_addmethod(c, (method) breakpoints_prev,            "prev",             A_CANT, 0);
    eclass_addmethod(c, (method) breakpoints_getlist,         "getlist",          A_CANT, 0);

    eclass_addmethod(c, (method) breakpoints_add,             "add",              A_GIMME,0);
    eclass_addmethod(c, (method) breakpoints_move,            "move",             A_GIMME,0);
    eclass_addmethod(c, (method) breakpoints_remove,          "remove",           A_GIMME,0);
    eclass_addmethod(c, (method) breakpoints_erase,           "erase",            A_CANT,0);
    eclass_addmethod(c, (method) breakpoints_function,        "function",         A_GIMME,0);

    eclass_addmethod(c, (method) breakpoints_scaleabs,        "scaleabs",         A_GIMME,0);
    eclass_addmethod(c, (method) breakpoints_scaleord,        "scaleord",         A_GIMME,0);

    eclass_addmethod(c, (method) breakpoints_mousedown,       "mousedown",        A_CANT, 0);
    eclass_addmethod(c, (method) breakpoints_mousemove,       "mousemove",        A_CANT, 0);
    eclass_addmethod(c, (method) breakpoints_mousedrag,       "mousedrag",        A_CANT, 0);
    eclass_addmethod(c, (method) breakpoints_mouseleave,      "mouseleave",       A_CANT, 0);
    eclass_addmethod(c, (method) breakpoints_mouseup,         "mouseup",          A_CANT, 0);

    eclass_addmethod(c, (method) breakpoints_preset,          "preset",           A_CANT, 0);
    eclass_addmethod(c, (method) breakpoints_read,            "read",             A_GIMME,0);
    eclass_addmethod(c, (method) breakpoints_write,           "write",            A_GIMME,0);
    eclass_addmethod(c, (method) breakpoints_save,            "save",             A_CANT, 0);

    CLASS_ATTR_INVISIBLE            (c, "send", 1);
	CLASS_ATTR_DEFAULT              (c, "size", 0, "150. 100.");

    CLASS_ATTR_FLOAT_ARRAY          (c, "absrange", 0, t_breakpoints, f_range_abscissa, 2);
	CLASS_ATTR_ORDER                (c, "absrange", 0, "2");
	CLASS_ATTR_LABEL                (c, "absrange", 0, "Abscissa Range");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "absrange", 0, "0. 1000.");

    CLASS_ATTR_FLOAT_ARRAY          (c, "ordrange", 0, t_breakpoints, f_range_ordinate, 2);
	CLASS_ATTR_ORDER                (c, "ordrange", 0, "2");
	CLASS_ATTR_LABEL                (c, "ordrange", 0, "Ordinate Range");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "ordrange", 0, "0. 1.");
    
    CLASS_ATTR_SYMBOL               (c, "outline", 0, t_breakpoints, f_outline);
	CLASS_ATTR_ORDER                (c, "outline", 0, "2");
	CLASS_ATTR_LABEL                (c, "outline", 0, "Outline");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "outline", 0, "Linear");
    CLASS_ATTR_ITEMS                (c, "outline", 0, "Linear Cosine Cubic");
    CLASS_ATTR_STYLE                (c, "outline", 0, "menu");

	CLASS_ATTR_RGBA                 (c, "bgcolor", 0, t_breakpoints, f_color_background);
	CLASS_ATTR_LABEL                (c, "bgcolor", 0, "Background Color");
	CLASS_ATTR_ORDER                (c, "bgcolor", 0, "1");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bgcolor", 0, "0.75 0.75 0.75 1.");
    CLASS_ATTR_STYLE                (c, "bgcolor", 0, "color");
    
	CLASS_ATTR_RGBA                 (c, "bdcolor", 0, t_breakpoints, f_color_border);
	CLASS_ATTR_LABEL                (c, "bdcolor", 0, "Border Color");
	CLASS_ATTR_ORDER                (c, "bdcolor", 0, "2");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bdcolor", 0, "0.5 0.5 0.5 1.");
    CLASS_ATTR_STYLE                (c, "bdcolor", 0, "color");
    
	CLASS_ATTR_RGBA                 (c, "ptcolor", 0, t_breakpoints, f_color_point);
	CLASS_ATTR_LABEL                (c, "ptcolor", 0, "Point Color");
	CLASS_ATTR_ORDER                (c, "ptcolor", 0, "3");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "ptcolor", 0, "0.5 0.5 0.5 1.");
    CLASS_ATTR_STYLE                (c, "ptcolor", 0, "color");
    
    CLASS_ATTR_RGBA                 (c, "licolor", 0, t_breakpoints, f_color_line);
	CLASS_ATTR_LABEL                (c, "licolor", 0, "Line Color");
	CLASS_ATTR_ORDER                (c, "licolor", 0, "4");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "licolor", 0, "0. 0. 0. 1.");
    CLASS_ATTR_STYLE                (c, "licolor", 0, "color");
    
    CLASS_ATTR_RGBA                 (c, "textcolor", 0, t_breakpoints, f_color_text);
	CLASS_ATTR_LABEL                (c, "textcolor", 0, "Text Color");
	CLASS_ATTR_ORDER                (c, "textcolor", 0, "3");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "textcolor", 0, "0. 0. 0. 1.");
    CLASS_ATTR_STYLE                (c, "textcolor", 0, "color");
    
    eclass_register(CLASS_BOX, c);
	breakpoints_class = c;
}

void *breakpoints_new(t_symbol *s, int argc, t_atom *argv)
{
	t_breakpoints *x =  NULL;
	t_binbuf* d;
    long flags;
	if (!(d = binbuf_via_atoms(argc,argv)))
		return NULL;

	x = (t_breakpoints *)eobj_new(breakpoints_class);
    x->f_outline_mode = 0;
    flags = 0
    | EBOX_GROWINDI
    ;
	ebox_new((t_ebox *)x, flags);

    x->f_out_float = (t_outlet *)floatout(x);
    x->f_out_list = (t_outlet *)listout(x);
    x->f_out_function = (t_outlet *)listout(x);

    x->f_number_of_points = 0;
    x->f_point_hover    = -1;
    x->f_point_selected = -1;
    x->f_output_inc     = -1;
    x->f_output_nextprev = 0;
    x->f_point_last_created = -1;
    x->f_mouse.x = -666666;
    x->f_mouse.y = -666666;
    
    x->f_clock = clock_new(x, (t_method)breakpoints_inc);

    ebox_attrprocess_viabinbuf(x, d);
    breakpoints_init(x, d);
	ebox_ready((t_ebox *)x);
    return (x);
}

void breakpoints_free(t_breakpoints *x)
{
	ebox_free((t_ebox *)x);
    clock_free(x->f_clock);
}

void breakpoints_getdrawparams(t_breakpoints *x, t_object *patcherview, t_edrawparams *params)
{
	params->d_borderthickness   = 2;
	params->d_cornersize        = 2;
    params->d_bordercolor       = x->f_color_border;
    params->d_boxfillcolor      = x->f_color_background;
}

void breakpoints_oksize(t_breakpoints *x, t_rect *newrect)
{
    newrect->width = pd_clip_min(newrect->width, 15.);
    newrect->height = pd_clip_min(newrect->height, 15.);
}

float breakpoints_linear(t_breakpoints *x, float f)
{
    int i;
    float ratio;
    for(i = 0; i < x->f_number_of_points - 1; i++)
    {
        if(f >= x->f_point_abscissa[i] && f <= x->f_point_abscissa[i+1])
        {
            ratio = (f - x->f_point_abscissa[i]) / (x->f_point_abscissa[i+1] - x->f_point_abscissa[i]);
            return x->f_point_ordinate[i] * (1. - ratio) + x->f_point_ordinate[i+1] * (ratio);
        }
    }
    return 0;
}

float breakpoints_cosine(t_breakpoints *x, float f)
{
    int i;
    float ratio1, ratio2;
    for(i = 0; i < x->f_number_of_points - 1; i++)
    {
        if(f >= x->f_point_abscissa[i] && f <= x->f_point_abscissa[i+1])
        {
            ratio1 = (f - x->f_point_abscissa[i]) / (x->f_point_abscissa[i+1] - x->f_point_abscissa[i]);
            ratio2 = (1.f - cosf(ratio1 * EPD_PI)) * 0.5f;
            return x->f_point_ordinate[i] * (1.f - ratio2) + x->f_point_ordinate[i+1] * ratio2;
        }
    }
    return 0;
}

float breakpoints_cubic(t_breakpoints *x, float f)
{
    int i;
    float ratio1, ratio2;
    float y0, y1, y2, y3;
    float a0, a1, a2, a3;
    
    for(i = 0; i < x->f_number_of_points - 1; i++)
    {
        if(f >= x->f_point_abscissa[i] && f <= x->f_point_abscissa[i+1])
        {
            ratio1 = (f - x->f_point_abscissa[i]) / (x->f_point_abscissa[i+1] - x->f_point_abscissa[i]);
            ratio2 = ratio1*ratio1;
            if(i == 0)
            {
                y1 = y0 = x->f_point_ordinate[i];
                y2 = x->f_point_ordinate[i+1];
                y3 = x->f_point_ordinate[i+2];
            }
            else if(i == x->f_number_of_points - 2)
            {
                y0 = x->f_point_ordinate[i-1];
                y1 = x->f_point_ordinate[i];
                y3 = y2 = x->f_point_ordinate[i+1];
            }
            else
            {
                y0 = x->f_point_ordinate[i-1];
                y1 = x->f_point_ordinate[i];
                y2 = x->f_point_ordinate[i+1];
                y3 = x->f_point_ordinate[i+2];
            }
            
            a0 = y3 - y2 - y0 + y1;
            a1 = y0 - y1 - a0;
            a2 = y2 - y0;
            a3 = y1;
            
            return a0 * ratio1 * ratio2 + a1 * ratio2 + a2 * ratio1 + a3;
        }
    }
    return 0;
}

float breakpoints_interpolation(t_breakpoints *x, float f)
{
    f = pd_clip_minmax(f, x->f_range_abscissa[0], x->f_range_abscissa[1]);
    if(x->f_outline_mode == 0)
        return breakpoints_linear(x, f);
    else if(x->f_outline_mode == 1 || x->f_number_of_points == 2)
        return breakpoints_cosine(x, f);
    else
        return breakpoints_cubic(x, f);
}

void breakpoints_float(t_breakpoints *x, float f)
{
    float result;
    if(x->f_number_of_points == 0)
        return;
    if(x->f_number_of_points == 1)
    {
        outlet_float(x->f_out_float, x->f_point_ordinate[0]);
        return;
    }
    result = breakpoints_interpolation(x, f);
    outlet_float(x->f_out_float, result);
}

void breakpoints_getlist(t_breakpoints *x)
{
    int i, j;
    t_atom *av;
    if(x->f_number_of_points)
    {
        av = (t_atom *)calloc(x->f_number_of_points * 2, sizeof(t_atom));
        for(i = 0, j = 0; i < x->f_number_of_points; j += 2, i++)
        {
            atom_setfloat(av+j, x->f_point_ordinate[i]);
            atom_setfloat(av+j+1, x->f_point_abscissa[i]);
        }
        outlet_list(x->f_out_function, &s_list, x->f_number_of_points * 2, av);
		free(av);
    }
}

void breakpoints_bang(t_breakpoints *x)
{
    t_atom av[2];
    x->f_output_inc = 0;
    if(x->f_number_of_points)
    {
        atom_setfloat(av, x->f_point_ordinate[0]);
        atom_setfloat(av+1, x->f_point_abscissa[0]);
        outlet_list(x->f_out_list, &s_list, 2, av);

        if(x->f_number_of_points > 1)
            clock_delay(x->f_clock, x->f_point_abscissa[1] - x->f_point_abscissa[0]);
    }
}

void breakpoints_inc(t_breakpoints *x)
{
    t_atom av[2];
    x->f_output_inc++;
    if(x->f_output_inc < x->f_number_of_points)
    {
        atom_setfloat(av, x->f_point_ordinate[x->f_output_inc]);
        atom_setfloat(av+1, x->f_point_abscissa[x->f_output_inc]);
        outlet_list(x->f_out_list, &s_list, 2, av);
        if(x->f_number_of_points > x->f_output_inc )
        {
            clock_delay(x->f_clock, x->f_point_abscissa[x->f_output_inc+1] - x->f_point_abscissa[x->f_output_inc]);
            return;
        }
    }
}

void breakpoints_next(t_breakpoints *x)
{
    t_atom av[2];
    x->f_output_nextprev++;
    if(x->f_output_nextprev >= x->f_number_of_points)
        x->f_output_nextprev = 0;
    atom_setfloat(av, (float)x->f_point_ordinate[(int)x->f_output_nextprev]);
    atom_setfloat(av+1, (float)x->f_point_abscissa[(int)x->f_output_nextprev]);
    outlet_list(x->f_out_list, &s_list, 2, av);
}

void breakpoints_prev(t_breakpoints *x)
{
    t_atom av[2];
    x->f_output_nextprev--;
    if(x->f_output_nextprev < 0)
        x->f_output_nextprev = x->f_number_of_points - 1;
    atom_setfloat(av, (float)x->f_point_ordinate[(int)x->f_output_nextprev]);
    atom_setfloat(av+1, (float)x->f_point_abscissa[(int)x->f_output_nextprev]);
    outlet_list(x->f_out_list, &s_list, 2, av);
}

void breakpoints_erase(t_breakpoints *x)
{
    x->f_number_of_points = 0;
    ebox_invalidate_layer((t_ebox *)x, gensym("points_layer"));
    ebox_redraw((t_ebox *)x);
}

void breakpoints_add(t_breakpoints *x, t_symbol* s, int argc, t_atom* argv)
{
    int index, i;
    float abs, ord;
    if(x->f_number_of_points == MAXPOINTS)
        return;

    if(argc && argv)
    {
        if(argc == 2 && atom_gettype(argv) == A_FLOAT && atom_gettype(argv+1) == A_FLOAT)
        {
            abs = atom_getfloat(argv);
            ord = pd_clip_minmax(atom_getfloat(argv+1), x->f_range_ordinate[0], x->f_range_ordinate[1]);

            if(abs < x->f_range_abscissa[0] || abs > x->f_range_abscissa[1])
                return;

            if(x->f_number_of_points == 0)
            {
                index  = 0;
            }
            else if(abs >= x->f_point_abscissa[x->f_number_of_points-1])
            {
                index = x->f_number_of_points;
            }
            else
            {
                index = 0;
                for(i = 0; i < x->f_number_of_points-1; i++)
                {
                    if(abs > x->f_point_abscissa[i])
                        index = i+1;
                }

                for(i = x->f_number_of_points-1; i >= index; i--)
                {
                    x->f_point_abscissa[i+1] = x->f_point_abscissa[i];
                    x->f_point_ordinate[i+1] = x->f_point_ordinate[i];
                }
            }
            x->f_point_abscissa[index] = abs;
            x->f_point_ordinate[index] = ord;
            x->f_number_of_points++;
            x->f_point_last_created = index;

            ebox_invalidate_layer((t_ebox *)x, gensym("points_layer"));
            ebox_redraw((t_ebox *)x);

        }
    }
}

void breakpoints_remove(t_breakpoints *x, t_symbol* s, int argc, t_atom* argv)
{
    int index, i;
    if(x->f_number_of_points == MAXPOINTS)
        return;

    if(argc && argv)
    {
        if(argc == 1 && atom_gettype(argv) == A_FLOAT)
        {
            index = atom_getfloat(argv);
            if(index >= x->f_number_of_points || index < 0)
                return;

            for(i = index; i < x->f_number_of_points; i++)
            {
                x->f_point_abscissa[i] = x->f_point_abscissa[i+1];
                x->f_point_ordinate[i] = x->f_point_ordinate[i+1];
            }
            x->f_number_of_points--;
            ebox_invalidate_layer((t_ebox *)x, gensym("points_layer"));
            ebox_redraw((t_ebox *)x);
        }
    }
}

void breakpoints_move(t_breakpoints *x, t_symbol* s, int argc, t_atom* argv)
{
    int index;
    float abs, ord;
    if(x->f_number_of_points == MAXPOINTS)
        return;

    if(argc && argv)
    {
        if(argc == 3 && atom_gettype(argv) == A_FLOAT && atom_gettype(argv+1) == A_FLOAT && atom_gettype(argv+2) == A_FLOAT)
        {
            index = atom_getfloat(argv);
            abs = atom_getfloat(argv+1);
            ord = pd_clip_minmax(atom_getfloat(argv+2), x->f_range_ordinate[0], x->f_range_ordinate[1]);
            if(index >= x->f_number_of_points || index < 0)
            {
                return;
            }
            else if(index == 0)
            {
                if(x->f_number_of_points > 1)
                    x->f_point_abscissa[0] = pd_clip_minmax(abs, x->f_range_abscissa[0], x->f_point_abscissa[1]);
                else
                    x->f_point_abscissa[0] = pd_clip_minmax(abs, x->f_range_abscissa[0], x->f_range_abscissa[1]);
                x->f_point_ordinate[0] = ord;
            }
            else if(index == x->f_number_of_points-1)
            {
                x->f_point_abscissa[index] = pd_clip_minmax(abs, x->f_point_abscissa[index-1], x->f_range_abscissa[1]);
                x->f_point_ordinate[index] = ord;
            }
            else
            {
                x->f_point_abscissa[index] = pd_clip_minmax(abs, x->f_point_abscissa[index-1], x->f_point_abscissa[index+1]);
                x->f_point_ordinate[index] = ord;
            }

            ebox_invalidate_layer((t_ebox *)x, gensym("points_layer"));
            ebox_redraw((t_ebox *)x);
        }
    }
}

void breakpoints_scaleabs(t_breakpoints *x, t_symbol* s, int argc, t_atom* argv)
{
    int i;
    float min, max, ratio;
    if(argc >= 2 && argv && atom_gettype(argv) == A_FLOAT && atom_gettype(argv+1) == A_FLOAT)
    {
        min = atom_getfloat(argv);
        max = atom_getfloat(argv+1);
        if(min > max)
        {
            min = max;
            max = atom_getfloat(argv);
        }

        ratio = x->f_range_abscissa[1] - x->f_range_abscissa[0];
        for(i = 0; i < x->f_number_of_points; i++)
        {
            x->f_point_abscissa[i] = ((x->f_point_abscissa[i] - x->f_range_abscissa[0]) / ratio) * (max - min) + min;
        }
        object_attr_setvalueof((t_object *)x, gensym("absrange"), argc, argv);
    }

}

void breakpoints_scaleord(t_breakpoints *x, t_symbol* s, int argc, t_atom* argv)
{
    int i;
    float min, max, ratio;
    if(argc >= 2 && argv && atom_gettype(argv) == A_FLOAT && atom_gettype(argv+1) == A_FLOAT)
    {
        min = atom_getfloat(argv);
        max = atom_getfloat(argv+1);
        if(min > max)
        {
            min = max;
            max = atom_getfloat(argv);
        }

        ratio = x->f_range_ordinate[1] - x->f_range_ordinate[0];
        for(i = 0; i < x->f_number_of_points; i++)
        {
            x->f_point_ordinate[i] = ((x->f_point_ordinate[i] - x->f_range_ordinate[0]) / ratio) * (max - min) + min;
        }
        object_attr_setvalueof((t_object *)x, gensym("ordrange"), argc, argv);
    }
}

void breakpoints_assist(t_breakpoints *x, void *b, long m, long a, char *s)
{
	;
}

t_pd_err breakpoints_notify(t_breakpoints *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
    int i, j;
    float max;
	if (msg == gensym("attr_modified"))
	{
		if(s == gensym("bgcolor") || s == gensym("bdcolor") || s == gensym("ptcolor") || s == gensym("licolor") || s == gensym("textcolor") || s == gensym("fontsize") || s == gensym("fontname") || s == gensym("fontweight") || s == gensym("fontslant"))
		{
			ebox_invalidate_layer((t_ebox *)x, gensym("points_layer"));
            ebox_invalidate_layer((t_ebox *)x, gensym("text_layer"));
		}
        if(s == gensym("absrange") || s == gensym("ordrange"))
        {
            if(x->f_range_abscissa[0] > x->f_range_abscissa[1])
            {
                max = x->f_range_abscissa[0];
                x->f_range_abscissa[0] = x->f_range_abscissa[1];
                x->f_range_abscissa[1] = max;
            }
            if(x->f_range_ordinate[0] > x->f_range_ordinate[1])
            {
                max = x->f_range_ordinate[0];
                x->f_range_ordinate[0] = x->f_range_ordinate[1];
                x->f_range_ordinate[1] = max;
            }

            for(i = 0; i < x->f_number_of_points; i++)
            {
                if(x->f_point_abscissa[i] < x->f_range_abscissa[0] || x->f_point_abscissa[i] > x->f_range_abscissa[1])
                {
                    for(j = i; j < x->f_number_of_points; j++)
                    {
                        x->f_point_abscissa[j] = x->f_point_abscissa[j+1];
                        x->f_point_ordinate[j] = x->f_point_ordinate[j+1];
                    }
                    x->f_number_of_points--;
                    i--;
                }
                x->f_point_ordinate[i] = pd_clip_minmax(x->f_point_ordinate[i], x->f_range_ordinate[0], x->f_range_ordinate[1]);
            }
            ebox_invalidate_layer((t_ebox *)x, gensym("points_layer"));
        }
        if(s == gensym("outline"))
        {
            if(x->f_outline == gensym("Cosine"))
            {
                x->f_outline_mode = 1;
            }
            else if(x->f_outline == gensym("Cubic"))
            {
                x->f_outline_mode = 2;
            }
            else
            {
                x->f_outline = gensym("Linear");
                x->f_outline_mode = 0;
            }
            ebox_invalidate_layer((t_ebox *)x, gensym("points_layer"));
        }
        ebox_redraw((t_ebox *)x);
	}
	return 0;
}

void breakpoints_paint(t_breakpoints *x, t_object *view)
{
	t_rect rect;
	ebox_get_rect_for_view((t_ebox *)x, &rect);
    x->f_size.x = rect.width;
    x->f_size.y = rect.height;
    draw_text(x, view, &rect);
    draw_points(x, view, &rect);
}

void draw_text(t_breakpoints *x, t_object *view, t_rect *rect)
{
    char number[512];
    t_elayer *g = ebox_start_layer((t_ebox *)x, gensym("text_layer"), rect->width, rect->height);
    t_etext *jtl = etext_layout_create();
	if(g && jtl)
	{
        float height = sys_fontheight(ebox_getfontsize((t_ebox *)x)) + 1;
        if (x->f_point_selected != -1)
        {
            sprintf(number, "x : %.2f y : %.2f", x->f_point_abscissa[x->f_point_selected], x->f_point_ordinate[x->f_point_selected]);
        }
        else if(x->f_mouse.x != -666666 && x->f_mouse.y != -666666)
        {
            x->f_mouse.x = pd_clip_minmax(x->f_mouse.x, x->f_range_abscissa[0], x->f_range_abscissa[1]);
            x->f_mouse.y = pd_clip_minmax(x->f_mouse.y, x->f_range_ordinate[0], x->f_range_ordinate[1]);
            sprintf(number, "x : %.2f y : %.2f", x->f_mouse.x, x->f_mouse.y);
        }
        else
        {
            ebox_end_layer((t_ebox*)x, gensym("text_layer"));
            ebox_paint_layer((t_ebox *)x, gensym("text_layer"), 0., 0.);
            return;
        }
        etext_layout_set(jtl, number, &x->j_box.b_font, 5, height * 0.5, rect->width, 0, ETEXT_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP);
        etext_layout_settextcolor(jtl, &x->f_color_text);
        etext_layout_draw(jtl, g);

        ebox_end_layer((t_ebox*)x, gensym("text_layer"));
	}
    if(jtl)
    {
       etext_layout_destroy(jtl);
    }
	ebox_paint_layer((t_ebox *)x, gensym("text_layer"), 0., 0.);
}

void draw_points(t_breakpoints *x, t_object *view, t_rect *rect)
{
    int i;
    float abs, ord;
    float max, inc;
    float abs2;
    float ratiox, ratioy;
    float height = sys_fontheight(ebox_getfontsize((t_ebox *)x)) + 2;
	t_elayer *g = ebox_start_layer((t_ebox *)x, gensym("points_layer"), rect->width, rect->height);
    
	if (g && x->f_number_of_points)
	{
        ratiox = (rect->width - 4.) / (x->f_range_abscissa[1] - x->f_range_abscissa[0]);
        ratioy = (rect->height - height - 4.) / (x->f_range_ordinate[1] - x->f_range_ordinate[0]);

        egraphics_set_line_width(g, 2);
        egraphics_set_color_rgba(g, &x->f_color_line);
        
        if(x->f_outline_mode == 0)
        {
            abs = (x->f_point_abscissa[0] - x->f_range_abscissa[0]) * ratiox + 2.;
            ord = rect->height - (x->f_point_ordinate[0] - x->f_range_ordinate[0]) * ratioy - 2.;
            egraphics_move_to(g, abs, ord);
            for(i = 0; i < x->f_number_of_points; i++)
            {
                abs = (x->f_point_abscissa[i] - x->f_range_abscissa[0]) * ratiox + 2.;
                ord = rect->height - (x->f_point_ordinate[i] - x->f_range_ordinate[0]) * ratioy - 2.;
                egraphics_line_to(g, abs, ord);
            }
            egraphics_stroke(g);
        }
        else if (x->f_outline_mode == 1 || x->f_outline_mode == 2)
        {
            abs = (x->f_point_abscissa[0] - x->f_range_abscissa[0]) * ratiox + 2.;
            ord = rect->height - (x->f_point_ordinate[0] - x->f_range_ordinate[0]) * ratioy - 2.;
            egraphics_move_to(g, abs, ord);
            
            max = (x->f_point_abscissa[x->f_number_of_points-1] - x->f_range_ordinate[0]) * ratiox + 2.;
            inc = (x->f_point_abscissa[x->f_number_of_points-1] - x->f_point_abscissa[0]) / (float)(max - abs);
            abs2 = x->f_point_abscissa[0]+inc;
            if(x->f_point_abscissa[x->f_number_of_points-1] == x->f_point_abscissa[x->f_number_of_points-2])
            {
                for(i = abs; i <= max && abs2 <= x->f_point_abscissa[x->f_number_of_points-1]+inc; i++, abs2 += inc)
                {
                    ord = rect->height - (breakpoints_interpolation(x, abs2) - x->f_range_ordinate[0]) * ratioy - 2.;
                    egraphics_line_to(g, i, ord);
                }
            }
            else
            {
                for(i = abs; i <= max && abs2 <= x->f_point_abscissa[x->f_number_of_points-1]; i++, abs2 += inc)
                {
                    ord = rect->height - (breakpoints_interpolation(x, abs2) - x->f_range_ordinate[0]) * ratioy - 2.;
                    egraphics_line_to(g, i, ord);
                }
            }
            
            egraphics_stroke(g);
        }

        egraphics_set_color_rgba(g, &x->f_color_point);
        for(i = 0; i < x->f_number_of_points; i++)
        {
            abs = (x->f_point_abscissa[i] - x->f_range_abscissa[0]) * ratiox + 2;
            ord = rect->height - (x->f_point_ordinate[i] - x->f_range_ordinate[0]) * ratioy - 2;
            if(i == x->f_point_hover || i == x->f_point_selected)
            {
                egraphics_circle(g, abs, ord, 4.);
            }
            else
                egraphics_circle(g, abs, ord, 3.);
            egraphics_fill(g);
        }
	}
    if(g)
    {
        egraphics_set_color_rgba(g, &x->f_color_border);
        egraphics_set_line_width(g, 2.);
        egraphics_line_fast(g, -2, height - 1, rect->width+4, height - 1);
        ebox_end_layer((t_ebox*)x, gensym("points_layer"));
    }
	ebox_paint_layer((t_ebox *)x, gensym("points_layer"), 0., 0.);
}

void breakpoints_mousemove(t_breakpoints *x, t_object *patcherview, t_pt pt, long modifiers)
{
    int i;
    float abs, ord;
    float height = sys_fontheight(ebox_getfontsize((t_ebox *)x)) + 2;
    float distx = (3. / (x->f_size.x - 4.)) * (x->f_range_abscissa[1] - x->f_range_abscissa[0]);
    float disty = (3. / (x->f_size.y - 4. - height)) * (x->f_range_ordinate[1] - x->f_range_ordinate[0]);

    abs = ((pt.x - 3.) / (x->f_size.x - 4.)) * (x->f_range_abscissa[1] - x->f_range_abscissa[0]) + x->f_range_abscissa[0];
    ord = ((x->f_size.y - (pt.y - 4.) - 4.) / (x->f_size.y - 4. - height)) * (x->f_range_ordinate[1] - x->f_range_ordinate[0]) + x->f_range_ordinate[0];
    x->f_point_hover = -1;
    x->f_mouse.x = abs;
    x->f_mouse.y = ord;

    for(i = 0; i < x->f_number_of_points; i++)
    {
        if(abs > x->f_point_abscissa[i] - distx && abs < x->f_point_abscissa[i] + distx && ord > x->f_point_ordinate[i] - disty && ord < x->f_point_ordinate[i] + disty)
        {
            x->f_point_hover = i;
        }
    }
    ebox_invalidate_layer((t_ebox *)x, gensym("points_layer"));
    ebox_invalidate_layer((t_ebox *)x, gensym("text_layer"));
    ebox_redraw((t_ebox *)x);
}

void breakpoints_mousedown(t_breakpoints *x, t_object *patcherview, t_pt pt, long modifiers)
{
    int i;
    t_atom av[2];
    float abs, ord;
    float height = sys_fontheight(ebox_getfontsize((t_ebox *)x)) + 2;
    float distx = (3. / (x->f_size.x - 4.)) * (x->f_range_abscissa[1] - x->f_range_abscissa[0]);
    float disty = (3. / (x->f_size.y - 4. - height)) * (x->f_range_ordinate[1] - x->f_range_ordinate[0]);

    abs = ((pt.x - 3.) / (x->f_size.x - 4.)) * (x->f_range_abscissa[1] - x->f_range_abscissa[0]) + x->f_range_abscissa[0];
    ord = ((x->f_size.y - (pt.y - 4.) - 4.) / (x->f_size.y - 4. - height)) * (x->f_range_ordinate[1] - x->f_range_ordinate[0]) + x->f_range_ordinate[0];

    x->f_point_selected = -1;
    x->f_point_hover    = -1;
    x->f_point_last_created = -1;
    x->f_mouse.x = abs;
    x->f_mouse.y = ord;

    ebox_invalidate_layer((t_ebox *)x, gensym("text_layer"));
    if(modifiers == EMOD_SHIFT)
    {
        atom_setfloat(av, abs);
        atom_setfloat(av+1, ord);
        breakpoints_add(x, NULL, 2, av);
        x->f_point_selected = x->f_point_last_created;
        return;
    }

    for(i = 0; i < x->f_number_of_points; i++)
    {
        if(abs > x->f_point_abscissa[i] - distx && abs < x->f_point_abscissa[i] + distx && ord > x->f_point_ordinate[i] - disty && ord < x->f_point_ordinate[i] + disty)
        {
            x->f_point_selected = i;
            x->f_point_hover    = i;

        }
    }
    if(modifiers == EMOD_CTRL)
    {
        atom_setfloat(av, x->f_point_selected);
        breakpoints_remove(x, NULL, 1, av);
        x->f_point_selected = -1;
        x->f_point_hover    = -1;
        return;
    }
    else if(modifiers == EMOD_CTRL)
    {
        // Sustain ?
    }
}

void breakpoints_mousedrag(t_breakpoints *x, t_object *patcherview, t_pt pt, long modifiers)
{
    t_atom av[3];
    float abs, ord;
    float height = sys_fontheight(ebox_getfontsize((t_ebox *)x)) + 2;
    abs = ((pt.x - 3.) / (x->f_size.x - 4.)) * (x->f_range_abscissa[1] - x->f_range_abscissa[0]) + x->f_range_abscissa[0];
    ord = ((x->f_size.y - (pt.y - 4.) - 4.) / (x->f_size.y - 4. - height)) * (x->f_range_ordinate[1] - x->f_range_ordinate[0]) + x->f_range_ordinate[0];


    x->f_mouse.x = abs;
    x->f_mouse.y = ord;
    ebox_invalidate_layer((t_ebox *)x, gensym("text_layer"));
    if(x->f_point_selected != -1)
    {
        atom_setfloat(av, x->f_point_selected);
        atom_setfloat(av+1, abs);
        atom_setfloat(av+2, ord);
        breakpoints_move(x, NULL, 3, av);
        return;
    }
}

void breakpoints_mouseleave(t_breakpoints *x, t_object *patcherview, t_pt pt, long modifiers)
{
    x->f_point_selected = -1;
    x->f_point_hover    = -1;
    x->f_mouse.x = -666666;
    x->f_mouse.y = -666666;

    ebox_invalidate_layer((t_ebox *)x, gensym("text_layer"));
    ebox_invalidate_layer((t_ebox *)x, gensym("points_layer"));
    ebox_redraw((t_ebox *)x);
}

void breakpoints_mouseup(t_breakpoints *x, t_object *patcherview, t_pt pt, long modifiers)
{
    float abs, ord;
    float height = sys_fontheight(ebox_getfontsize((t_ebox *)x)) + 2;
    abs = ((pt.x - 3.) / (x->f_size.x - 4.)) * (x->f_range_abscissa[1] - x->f_range_abscissa[0]) + x->f_range_abscissa[0];
    ord = ((x->f_size.y - (pt.y - 4.) - 4.) / (x->f_size.y - 4. - height)) * (x->f_range_ordinate[1] - x->f_range_ordinate[0]) + x->f_range_ordinate[0];


    x->f_mouse.x = abs;
    x->f_mouse.y = ord;
    x->f_point_selected    = -1;

    ebox_invalidate_layer((t_ebox *)x, gensym("text_layer"));
    ebox_invalidate_layer((t_ebox *)x, gensym("points_layer"));
    ebox_redraw((t_ebox *)x);
}

void breakpoints_preset(t_breakpoints *x, t_binbuf *b)
{
    int i;

    binbuf_addv(b, "s", gensym("function"));
    for(i = 0; i < x->f_number_of_points; i++)
    {
        binbuf_addv(b, "ff", (float)x->f_point_abscissa[i], (float)x->f_point_ordinate[i]);
    }
}

void breakpoints_save(t_breakpoints *x, t_binbuf *d)
{
    int i;
    if(x->f_number_of_points)
    {
        binbuf_addv(d, "s", gensym("@points"));
        for(i = 0; i < x->f_number_of_points; i++)
        {
            binbuf_addv(d, "ff", x->f_point_abscissa[i], x->f_point_ordinate[i]);
        }
    }
}

void breakpoints_read(t_breakpoints *x, t_symbol *s, int argc, t_atom *argv)
{
    t_binbuf *d = binbuf_new();
    if(d && argv && argc && atom_gettype(argv) == A_SYM)
    {
        if(binbuf_read(d, atom_getsym(argv)->s_name, "", 0))
        {
            object_error(x, "breakpoints : %s read failed", atom_getsym(argv)->s_name);
        }
        else
        {
            breakpoints_init(x, d);
        }
    }
    if(d)
        binbuf_free(d);
}

void breakpoints_write(t_breakpoints *x, t_symbol *s, int argc, t_atom *argv)
{
    t_binbuf *d = binbuf_new();
    if(d && argv && argc && atom_gettype(argv) == A_SYM)
    {
        breakpoints_save(x, d);
        if(binbuf_write(d, atom_getsym(argv)->s_name, "", 0))
        {
            object_error(x, "breakpoints : %s write failed", atom_getsym(argv)->s_name);
        }
    }
    if(d)
        binbuf_free(d);

}

void breakpoints_function(t_breakpoints *x, t_symbol* s, int argc, t_atom* argv)
{
    t_binbuf* b = binbuf_new();

    if(argc && argv)
    {
        binbuf_addv(b, "s", gensym("@points"));
        binbuf_add(b, argc, argv);
        breakpoints_init(x, b);
        binbuf_free(b);

    }
}

void breakpoints_init(t_breakpoints *x, t_binbuf *d)
{
    int i;
    long ac = binbuf_getnatom(d);
    t_atom* av = binbuf_getvec(d);
    breakpoints_erase(x);
    for(i = 0; i < ac; i++)
    {
        if(atom_gettype(av+i) == A_SYM && atom_getsym(av+i) == gensym("@points"))
        {
            i++;
            for(;i < ac-1; i += 2)
            {
                if(atom_gettype(av+i) == A_FLOAT && atom_gettype(av+i+1) == A_FLOAT)
                {
                    breakpoints_add(x, NULL, 2, av+i);
                }
                else if(atom_gettype(av+i) == A_SYM || atom_gettype(av+i+1) == A_SYM)
                {
                    //return;
                }
            }
        }
    }
}





