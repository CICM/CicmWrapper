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

extern "C"  {
#include "../../../PdEnhanced/Sources/cicm_wrapper.h"
}

typedef struct  _blacboard
{
	t_ebox      j_box;
    t_rect      f_box;
    
    long        f_pen_mode;
    t_pt        f_pen_new;
    t_pt        f_pen_old;
    char        f_pen_down;
    
    long        f_width;
    t_symbol*   f_color;
    char        f_fill;
    
    t_outlet*   f_out_move;
    t_outlet*   f_out_drag;
    t_outlet*   f_out_down;
    
    
    t_rgba		f_color_background;
	t_rgba		f_color_border;
} t_blacboard;

t_eclass *blackboard_class;

void *blackboard_new(t_symbol *s, int argc, t_atom *argv);
void blackboard_free(t_blacboard *x);
void blackboard_assist(t_blacboard *x, void *b, long m, long a, char *s);
void blackboard_output(t_blacboard *x);

void blackboard_clear(t_blacboard *x);
void blackboard_reset(t_blacboard *x);
void blackboard_width(t_blacboard *x, float f);
void blackboard_color(t_blacboard *x, t_symbol *s, int argc, t_atom *argv);
void blackboard_fill(t_blacboard *x, float f);

void blackboard_line(t_blacboard *x, t_symbol *s, int argc, t_atom *argv);
void blackboard_path(t_blacboard *x, t_symbol *s, int argc, t_atom *argv);
void blackboard_rect(t_blacboard *x, t_symbol *s, int argc, t_atom *argv);
void blackboard_oval(t_blacboard *x, t_symbol *s, int argc, t_atom *argv);
void blackboard_arc(t_blacboard *x, t_symbol *s, int argc, t_atom *argv);
void blackboard_image(t_blacboard *x, t_symbol *s, int argc, t_atom *argv);
void blackboard_text(t_blacboard *x, t_symbol *s, int argc, t_atom *argv);

t_pd_err blackboard_notify(t_blacboard *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

void blackboard_getdrawparams(t_blacboard *x, t_object *patcherview, t_edrawparams *params);
void blackboard_oksize(t_blacboard *x, t_rect *newrect);

void blackboard_paint(t_blacboard *x, t_object *view);
void blackboard_pen(t_blacboard *x);

void blackboard_mousemove(t_blacboard *x, t_object *patcherview, t_pt pt, long modifiers);
void blackboard_mousedrag(t_blacboard *x, t_object *patcherview, t_pt pt, long modifiers);
void blackboard_mousedown(t_blacboard *x, t_object *patcherview, t_pt pt, long modifiers);
void blackboard_mouseup(t_blacboard *x, t_object *patcherview, t_pt pt, long modifiers);
void blackboard_mouseleave(t_blacboard *x, t_object *patcherview, t_pt pt, long modifiers);


extern "C" void setup_c0x2eblackboard(void)
{
	t_eclass *c;
    
	c = eclass_new("c.blackboard", (method)blackboard_new, (method)blackboard_free, (short)sizeof(t_blacboard), 0L, A_GIMME, 0);
	eclass_init(c, 0);
	
	eclass_addmethod(c, (method) blackboard_assist,          "assist",           A_CANT, 0);
	eclass_addmethod(c, (method) blackboard_paint,           "paint",            A_CANT, 0);
	eclass_addmethod(c, (method) blackboard_notify,          "notify",           A_CANT, 0);
    eclass_addmethod(c, (method) blackboard_getdrawparams,   "getdrawparams",    A_CANT, 0);
    eclass_addmethod(c, (method) blackboard_oksize,          "oksize",           A_CANT, 0);
    
    eclass_addmethod(c, (method) blackboard_width,           "width",            A_FLOAT,0);
    eclass_addmethod(c, (method) blackboard_color,           "color",            A_GIMME,0);
    eclass_addmethod(c, (method) blackboard_fill,            "fill",             A_FLOAT,0);
    
    eclass_addmethod(c, (method) blackboard_line,            "line",             A_GIMME,0);
    eclass_addmethod(c, (method) blackboard_path,            "path",             A_GIMME,0);
    eclass_addmethod(c, (method) blackboard_rect,            "rect",             A_GIMME,0);
    eclass_addmethod(c, (method) blackboard_oval,            "oval",             A_GIMME,0);
    eclass_addmethod(c, (method) blackboard_arc,             "arc",              A_GIMME,0);
    eclass_addmethod(c, (method) blackboard_image,           "image",            A_GIMME,0);
    eclass_addmethod(c, (method) blackboard_text,            "text",             A_GIMME,0);
    
    eclass_addmethod(c, (method) blackboard_clear,           "clear",            A_CANT, 0);
    eclass_addmethod(c, (method) blackboard_reset,           "reset",            A_CANT, 0);
    
    eclass_addmethod(c, (method) blackboard_mousemove,       "mousemove",        A_CANT, 0);
    eclass_addmethod(c, (method) blackboard_mousedrag,       "mousedrag",        A_CANT, 0);
    eclass_addmethod(c, (method) blackboard_mousedown,       "mousedown",        A_CANT, 0);
    eclass_addmethod(c, (method) blackboard_mouseup,         "mouseup",          A_CANT, 0);
    eclass_addmethod(c, (method) blackboard_mouseleave,      "mouseleave",       A_CANT, 0);
    
	CLASS_ATTR_DEFAULT              (c, "size", 0, "200 200");
    CLASS_ATTR_INVISIBLE            (c, "send", 1);
    
    CLASS_ATTR_LONG                 (c, "chalkmode", 0, t_blacboard, f_pen_mode);
	CLASS_ATTR_LABEL                (c, "chalkmode", 0, "Chalk Mode");
    CLASS_ATTR_FILTER_CLIP          (c, "chalkmode", 0, 1);
	CLASS_ATTR_ORDER                (c, "chalkmode", 0, "1");
	CLASS_ATTR_DEFAULT              (c, "chalkmode", 0, "1");
    CLASS_ATTR_SAVE                 (c, "chalkmode", 0);
    CLASS_ATTR_STYLE                (c, "chalkmode", 0, "onoff");
    
	CLASS_ATTR_RGBA                 (c, "bgcolor", 0, t_blacboard, f_color_background);
	CLASS_ATTR_LABEL                (c, "bgcolor", 0, "Background Color");
	CLASS_ATTR_ORDER                (c, "bgcolor", 0, "1");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bgcolor", 0, "0.75 0.75 0.75 1.");
	CLASS_ATTR_STYLE                (c, "bgcolor", 0, "color");
    
	CLASS_ATTR_RGBA                 (c, "bdcolor", 0, t_blacboard, f_color_border);
	CLASS_ATTR_LABEL                (c, "bdcolor", 0, "Border Color");
	CLASS_ATTR_ORDER                (c, "bdcolor", 0, "2");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bdcolor", 0, "0.5 0.5 0.5 1.");
	CLASS_ATTR_STYLE                (c, "bdcolor", 0, "color");
	
    eclass_register(CLASS_NOBOX, c);
    erouter_add_libary(gensym("chocolate"), "Chocolate and Coffee Libraries by Pierre Guillot", "© 2013 - 2014  CICM | Paris 8 University", "Version Beta 0.1");
	blackboard_class = c;
}

void *blackboard_new(t_symbol *s, int argc, t_atom *argv)
{
	t_blacboard *x =  NULL;
	t_binbuf* d;
    long flags;
	if (!(d = binbuf_via_atoms(argc,argv)))
		return NULL;
    
	x = (t_blacboard *)eobj_new(blackboard_class);
    flags = 0
    | EBOX_GROWINDI
    ;
	ebox_new((t_ebox *)x, flags);
    
    x->f_out_drag   = (t_outlet *)listout(x);
    x->f_out_move   = (t_outlet *)listout(x);
    x->f_out_down   = (t_outlet *)floatout(x);
    x->f_pen_new.x  = 0.;
    x->f_pen_new.y  = 0.;
    x->f_pen_old.x  = 0.;
    x->f_pen_old.y  = 0.;
    x->f_pen_down   = 0;
    
    x->f_width      = 1;
    x->f_color      = gensym("#000000");
    x->f_fill       = 0;
    
	ebox_attrprocess_viabinbuf(x, d);
	ebox_ready((t_ebox *)x);
	return (x);
}

void blackboard_getdrawparams(t_blacboard *x, t_object *patcherview, t_edrawparams *params)
{
	params->d_borderthickness   = 2;
	params->d_cornersize        = 2;
    params->d_bordercolor       = x->f_color_border;
    params->d_boxfillcolor      = x->f_color_background;
}

void blackboard_oksize(t_blacboard *x, t_rect *newrect)
{
    newrect->width = pd_clip_min(newrect->width, 15.);
    newrect->height = pd_clip_min(newrect->height, 15.);
}

void blackboard_output(t_blacboard *x)
{
    t_atom argv[2];
    if(ebox_isdrawable((t_ebox *)x))
    {
        atom_setfloat(argv, x->f_pen_new.x);
        atom_setfloat(argv+1, x->f_pen_new.y);
        if(x->f_pen_down)
            outlet_list(x->f_out_drag, &s_list, 2, argv);
        else
            outlet_list(x->f_out_move, &s_list, 2, argv);
    }
}

void blackboard_free(t_blacboard *x)
{
	ebox_free((t_ebox *)x);
}

void blackboard_assist(t_blacboard *x, void *b, long m, long a, char *s)
{
	;
}

t_pd_err blackboard_notify(t_blacboard *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	if (msg == gensym("attr_modified"))
	{
		if(s == gensym("bgcolor") || s == gensym("bdcolor"))
		{
			ebox_invalidate_layer((t_ebox *)x, gensym("point_layer"));
		}
        ebox_redraw((t_ebox *)x);
        
	}
	return 0;
}

void blackboard_clear(t_blacboard *x)
{
    sys_vgui("%s delete %s\n", x->j_box.b_drawing_id->s_name, x->j_box.b_all_id->s_name);
    ebox_redraw((t_ebox *)x);
}

void blackboard_reset(t_blacboard *x)
{
    x->f_pen_new.x    = 0.;
    x->f_pen_new.y    = 0.;
    x->f_pen_old.x    = 0.;
    x->f_pen_old.y    = 0.;
    x->f_width        = 1;
    x->f_fill         = 0;
    x->f_color        = gensym("#000000");
    
    blackboard_clear(x);
}

void blackboard_width(t_blacboard *x, float f)
{
    x->f_width = pd_clip_min(f, 1);
}

void blackboard_color(t_blacboard *x, t_symbol *s, int argc, t_atom *argv)
{
    t_rgb color_rgb = {0., 0., 0.};
    t_hsl color_hsl = {0., 0., 0.};
    if(argc > 1 && atom_gettype(argv) == A_SYM)
    {
        if(atom_getsym(argv) == gensym("rgba") || atom_getsym(argv) == gensym("rgb"))
        {
            if(atom_gettype(argv+1) == A_FLOAT)
                color_rgb.red = atom_getfloat(argv+1);
            if(argc > 2 && atom_gettype(argv+2) == A_FLOAT)
                color_rgb.green = atom_getfloat(argv+2);
            if(argc > 3 && atom_gettype(argv+3) == A_FLOAT)
                color_rgb.blue = atom_getfloat(argv+3);
            x->f_color = gensym(rgb_to_hex(color_rgb));
        }
        else if(atom_getsym(argv) == gensym("hsla") || atom_getsym(argv) == gensym("hsl"))
        {
            if(atom_gettype(argv+1) == A_FLOAT)
                color_hsl.hue = atom_getfloat(argv+1);
            if(argc > 2 && atom_gettype(argv+2) == A_FLOAT)
                color_hsl.saturation = atom_getfloat(argv+2);
            if(argc > 3 && atom_gettype(argv+3) == A_FLOAT)
                color_hsl.lightness = atom_getfloat(argv+3);
            x->f_color = gensym(hsl_to_hex(color_hsl));
        }
        else if(atom_getsym(argv) == gensym("hex") || atom_getsym(argv) == gensym("hexa"))
        {
            if(atom_gettype(argv+1) == A_SYM)
                x->f_color = atom_getsym(argv+1);
        }
    }
}

void blackboard_fill(t_blacboard *x, float f)
{
    x->f_fill = pd_clip_minmax(f, 0, 1);
}

void blackboard_line(t_blacboard *x, t_symbol *s, int argc, t_atom *argv)
{
    if(!ebox_isdrawable((t_ebox *)x) || x->j_box.b_editor_id == NULL)
        return;
    if(argc > 3 && argv)
    {
        if (atom_gettype(argv) == A_FLOAT && atom_gettype(argv+1) == A_FLOAT && atom_gettype(argv+2) == A_FLOAT && atom_gettype(argv+3) == A_FLOAT)
        {
            sys_vgui("%s create line %d %d %d %d ", x->j_box.b_drawing_id->s_name, (int)atom_getfloat(argv), (int)atom_getfloat(argv+1), (int)atom_getfloat(argv+2), (int)atom_getfloat(argv+3));
            sys_vgui("-fill %s -width %d -tags %s\n", x->f_color->s_name, (int)x->f_width,  x->j_box.b_all_id->s_name);
        }
        ebox_redraw((t_ebox *)x);
    }
}

void blackboard_path(t_blacboard *x, t_symbol *s, int argc, t_atom *argv)
{
    int i;
    if(!ebox_isdrawable((t_ebox *)x) || x->j_box.b_editor_id == NULL)
        return;
    if(argc > 3 && argv)
    {
        if (atom_gettype(argv) == A_FLOAT && atom_gettype(argv+1) == A_FLOAT && atom_gettype(argv+2) == A_FLOAT && atom_gettype(argv+3) == A_FLOAT)
        {
            if(x->f_fill)
                sys_vgui("%s create polygon %d %d %d %d ", x->j_box.b_drawing_id->s_name, (int)atom_getfloat(argv), (int)atom_getfloat(argv+1), (int)atom_getfloat(argv+2), (int)atom_getfloat(argv+3));
            else
                sys_vgui("%s create line  %d %d %d %d", x->j_box.b_drawing_id->s_name, (int)atom_getfloat(argv), (int)atom_getfloat(argv+1), (int)atom_getfloat(argv+2), (int)atom_getfloat(argv+3));
            
            for(i = 4; i < argc; i += 2)
            {
                if(argc > i+1 && atom_gettype(argv+i) == A_FLOAT && atom_gettype(argv+i+1) == A_FLOAT)
                    sys_vgui("%d %d ", (int)atom_getfloat(argv+i), (int)atom_getfloat(argv+i+1));
            }
            
            if(x->f_fill)
                sys_vgui("-fill %s -width 0 -tags %s\n", x->f_color->s_name,  x->j_box.b_all_id->s_name);
            else
                sys_vgui("-fill %s -width %d -tags %s\n", x->f_color->s_name, (int)x->f_width, x->j_box.b_all_id->s_name);
            ebox_redraw((t_ebox *)x);
        }
    }
}

void blackboard_rect(t_blacboard *x, t_symbol *s, int argc, t_atom *argv)
{
    if(!ebox_isdrawable((t_ebox *)x) || x->j_box.b_editor_id == NULL)
        return;
    if(argc > 3 && argv)
    {
        if (atom_gettype(argv) == A_FLOAT && atom_gettype(argv+1) == A_FLOAT && atom_gettype(argv+2) == A_FLOAT && atom_gettype(argv+3) == A_FLOAT)
        {
            sys_vgui("%s create rectangle %d %d %d %d ", x->j_box.b_drawing_id->s_name, (int)atom_getfloat(argv), (int)atom_getfloat(argv+1), (int)atom_getfloat(argv+2), (int)atom_getfloat(argv+3));
            
            if(x->f_fill)
                sys_vgui("-fill %s -width 0 -tags %s\n", x->f_color->s_name,  x->j_box.b_all_id->s_name);
            else
                sys_vgui("-outline %s -width %d -tags %s\n", x->f_color->s_name, (int)x->f_width, x->j_box.b_all_id->s_name);
            ebox_redraw((t_ebox *)x);
        }
    }
}

void blackboard_oval(t_blacboard *x, t_symbol *s, int argc, t_atom *argv)
{
    if(!ebox_isdrawable((t_ebox *)x)|| x->j_box.b_editor_id == NULL)
        return;
    if(argc > 3 && argv)
    {
        if (atom_gettype(argv) == A_FLOAT && atom_gettype(argv+1) == A_FLOAT && atom_gettype(argv+2) == A_FLOAT && atom_gettype(argv+3) == A_FLOAT)
        {
            sys_vgui("%s create oval %d %d %d %d ", x->j_box.b_drawing_id->s_name, (int)atom_getfloat(argv), (int)atom_getfloat(argv+1), (int)atom_getfloat(argv+2), (int)atom_getfloat(argv+3));
            
            if(x->f_fill)
                sys_vgui("-fill %s -width 0 -tags %s\n", x->f_color->s_name,  x->j_box.b_all_id->s_name);
            else
                sys_vgui("-outline %s -width %d -tags %s\n", x->f_color->s_name, (int)x->f_width, x->j_box.b_all_id->s_name);
            ebox_redraw((t_ebox *)x);
        }
    }
}

void blackboard_arc(t_blacboard *x, t_symbol *s, int argc, t_atom *argv)
{
    if(!ebox_isdrawable((t_ebox *)x) || x->j_box.b_editor_id == NULL)
        return;
    if(argc > 5 && argv)
    {
        if (atom_gettype(argv) == A_FLOAT && atom_gettype(argv+1) == A_FLOAT && atom_gettype(argv+2) == A_FLOAT && atom_gettype(argv+3) == A_FLOAT && atom_gettype(argv+4) == A_FLOAT && atom_gettype(argv+5) == A_FLOAT)
        {
            sys_vgui("%s create arc %d %d %d %d -start %d -extent %d ", x->j_box.b_drawing_id->s_name, (int)atom_getfloat(argv), (int)atom_getfloat(argv+1), (int)atom_getfloat(argv+2), (int)atom_getfloat(argv+3), (int)atom_getfloat(argv+4), (int)atom_getfloat(argv+5));
        
            if(x->f_fill)
                sys_vgui("-style pieslice -fill %s -width 0 -tags %s\n", x->f_color->s_name,  x->j_box.b_all_id->s_name);
            else
                sys_vgui("-style arc -outline %s -width %d -tags %s\n", x->f_color->s_name, (int)x->f_width, x->j_box.b_all_id->s_name);
            ebox_redraw((t_ebox *)x);
        }
    }
}

void blackboard_image(t_blacboard *x, t_symbol *s, int argc, t_atom *argv)
{
    if(!ebox_isdrawable((t_ebox *)x) || x->j_box.b_editor_id == NULL)
        return;
    if(argc > 2 && argv)
    {
        if (atom_gettype(argv) == A_FLOAT && atom_gettype(argv+1) == A_FLOAT && atom_gettype(argv+2) == A_SYM)
        {
            sys_vgui("%s create image %d %d -anchor nw -image [image create photo -file %s/%s] -tags %s\n", x->j_box.b_drawing_id->s_name,  (int)atom_getfloat(argv), (int)atom_getfloat(argv+1), canvas_getdir(x->j_box.b_obj.o_canvas)->s_name, atom_getsym(argv+2)->s_name, x->j_box.b_all_id->s_name);
            ebox_redraw((t_ebox *)x);
        }
    }
    
}

void blackboard_text(t_blacboard *x, t_symbol *s, int argc, t_atom *argv)
{
    int i;
    char buffer[MAXPDSTRING];
    if(!ebox_isdrawable((t_ebox *)x) || x->j_box.b_editor_id == NULL)
        return;
    if(argc > 2 && argv)
    {
        if (atom_gettype(argv) == A_FLOAT && atom_gettype(argv+1) == A_FLOAT)
        {
            sys_vgui("%s create text %d %d -anchor nw -text {", x->j_box.b_drawing_id->s_name, (int)atom_getfloat(argv), (int)atom_getfloat(argv+1));
            for(i = 2; i < argc; i++)
            {
                atom_string(argv+i, buffer, MAXPDSTRING);
                sys_vgui("%s ", buffer);
            }
            sys_vgui("} -font {%s %d %s} -fill %s -tags %s\n", x->j_box.b_font.c_family->s_name, (int)x->j_box.b_font.c_size, x->j_box.b_font.c_weight->s_name, x->f_color->s_name, x->j_box.b_all_id->s_name);
        }
        ebox_redraw((t_ebox *)x);
    }
}

void blackboard_pen(t_blacboard *x)
{
    sys_vgui("%s create line %d %d %d %d ", x->j_box.b_drawing_id->s_name, (int)(x->f_pen_old.x), (int)(x->f_pen_old.y), (int)(x->f_pen_new.x), (int)(x->f_pen_new.y));
    sys_vgui("-fill %s -width %d -tags %s\n", x->f_color->s_name, (int)x->f_width,  x->j_box.b_all_id->s_name);
    ebox_redraw((t_ebox *)x);
}

void blackboard_paint(t_blacboard *x, t_object *view)
{
	t_rect rect;
	ebox_get_rect_for_view((t_ebox *)x, &rect);
    x->f_box = rect;
}

void blackboard_mousemove(t_blacboard *x, t_object *patcherview, t_pt pt, long modifiers)
{
    x->f_pen_new = pt;
    x->f_pen_down = 0;
    blackboard_output(x);
}

void blackboard_mousedrag(t_blacboard *x, t_object *patcherview, t_pt pt, long modifiers)
{
    x->f_pen_old = x->f_pen_new;
    x->f_pen_new = pt;
    x->f_pen_down = 1;
    if(x->f_pen_mode)
    {
        blackboard_pen(x);
    }
    blackboard_output(x);
    outlet_float(x->f_out_down, 1);
}

void blackboard_mousedown(t_blacboard *x, t_object *patcherview, t_pt pt, long modifiers)
{
    x->f_pen_new = pt;
    x->f_pen_old = pt;
    x->f_pen_down = 1;
    if(x->f_pen_mode)
    {
        blackboard_pen(x);
    }
    blackboard_output(x);
}

void blackboard_mouseup(t_blacboard *x, t_object *patcherview, t_pt pt, long modifiers)
{
    x->f_pen_new = pt;
    x->f_pen_down = 0;
    blackboard_output(x);
    outlet_float(x->f_out_down, 0);
}

void blackboard_mouseleave(t_blacboard *x, t_object *patcherview, t_pt pt, long modifiers)
{
    ;
}






