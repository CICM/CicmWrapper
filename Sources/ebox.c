/*
 * CicmWrapper
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "eobj.h"
#include "ebox.h"
#include "egraphics.h"
#include "eobj.h"
#include "eclass.h"
#include "epopup.h"
#include "float.h"

static char *my_cursorlist[] =
{
    "left_ptr",
    "center_ptr",
    "sb_v_double_arrow",
    "plus",
    "hand2",
    "circle",
    "X_cursor",
    "bottom_side",
    "bottom_right_corner",
    "right_side",
    "double_arrow",
    "exchange",
    "xterm"
};

static void ebox_invalidate_all(t_ebox *x);
static void ebox_draw_border(t_ebox* x);
static void ebox_draw_iolets(t_ebox* x);
static void ebox_update(t_ebox *x);
static void ebox_erase(t_ebox* x);
static void ebox_select(t_ebox* x);
static void ebox_move(t_ebox* x);



static void ebox_paint(t_ebox *x)
{
    /*
    t_eclass* c = eobj_getclass(x);
    ebox_update(x);
    sys_vgui("%s configure -bg %s\n", x->b_drawing_id->s_name, rgba_to_hex(&(x->b_boxparameters.d_boxfillcolor)));
    if(x->b_pinned)
    {
        sys_vgui((char *)"lower %s\n", x->b_drawing_id->s_name);
    }
    if(c->c_widget.w_paint)
    {
        c->c_widget.w_paint(x, (t_object *)eobj_getcanvas(x));
    }
    ebox_draw_border(x);
    ebox_draw_iolets(x);
     */
}

//! Widget

void ebox_set_cursor(t_ebox* x, t_object* view, int cursor)
{
    /*
    if(x->b_drawing_id)
    {
        cursor = (int)pd_clip(cursor, 0, 12);
        sys_vgui("%s configure -cursor %s\n", x->b_drawing_id->s_name, my_cursorlist[cursor]);
    }
    else
    {
        int todo;
    }
     */
}



void ebox_texteditor_keypress(t_ebox *x, t_symbol *s, float f)
{
    /*
    t_etexteditor* editor;
    const t_eclass* c = eobj_getclass(x);
    if(c && c->c_widget.w_texteditor_keypress)
    {
        editor = etexteditor_getfromsymbol(s);
        if(editor)
        {
            c->c_widget.w_texteditor_keypress(x, editor, (int)f);
        }
    }
     */
}

void ebox_texteditor_keyfilter(t_ebox *x, t_symbol *s, float f)
{
    /*
    t_etexteditor* editor;
    const t_eclass* c = eobj_getclass(x);
    if(c && c->c_widget.w_texteditor_keyfilter)
    {
        editor = etexteditor_getfromsymbol(s);
        if(editor)
        {
            c->c_widget.w_texteditor_keyfilter(x, editor, (int)f);
        }
    }
     */
}

void ebox_texteditor_focus(t_ebox *x, t_symbol *s, float f)
{
    /*
    t_etexteditor* editor;
    const t_eclass* c = eobj_getclass(x);
    if(c && c->c_widget.w_texteditor_focus)
    {
        editor = etexteditor_getfromsymbol(s);
        if(editor)
        {
            c->c_widget.w_texteditor_focus(x, editor, (int)f);
        }
    }
     */
}

t_pd_err ebox_notify(t_ebox *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
    t_eclass* c = eobj_getclass(x);
    /*
    if(s == s_cream_size)
    {
        if(c->c_widget.w_oksize != NULL)
            c->c_widget.w_oksize(x, &x->b_rect);
        ebox_invalidate_all(x);
        if(ebox_isdrawable(x))
        {
            sys_vgui("%s itemconfigure %s -width %d -height %d\n", x->b_canvas_id->s_name, x->b_window_id->s_name, (int)x->b_rect.width, (int)x->b_rect.height);
            canvas_fixlinesfor(x->b_obj.o_canvas, (t_text *)x);
        }
        ebox_redraw(x);
    }
    else if(s == s_pinned && ebox_isdrawable(x))
    {
        if(x->b_pinned)
        {
            sys_vgui((char *)"lower %s\n", x->b_drawing_id->s_name);
        }
        else
        {
            sys_vgui((char *)"raise %s\n", x->b_drawing_id->s_name);
        }
    }
    */
    return 0;
}



void ebox_redraw(t_ebox *x)
{
    /*
    if(ebox_isdrawable(x) && x->b_have_window)
    {
        ebox_paint(x);
    }
    if(x->b_obj.o_camo_id->s_thing)
    {
        pd_symbol(x->b_obj.o_camo_id->s_thing, s_cream_repaint);
    }
     */
}

void ebox_get_rect_for_view(t_ebox *x, t_rect *rect)
{
    /*
    rect->x = x->b_rect.x + x->b_boxparameters.d_borderthickness;
    rect->y = x->b_rect.y + x->b_boxparameters.d_borderthickness;
    rect->width = x->b_rect.width - x->b_boxparameters.d_borderthickness * 2.f;
    rect->height = x->b_rect.height - x->b_boxparameters.d_borderthickness * 2.f;
     */
}

t_elayer* ebox_start_layer(t_ebox *x, t_object* view, t_symbol *name, float width, float height)
{
    return NULL;
}

t_pd_err ebox_end_layer(t_ebox *x, t_symbol *name)
{
    int i;
    /*
    for(i = 0; i < x->b_number_of_layers; i++)
    {
        if(x->b_layers[i].e_name == name)
        {
            x->b_layers[i].e_state = EGRAPHICS_TODRAW;
            return 0;
        }
    }
     */
    return -1;
}


t_pd_err ebox_invalidate_layer(t_ebox *x, t_symbol *name)
{
    int i;
    /*
    for(i = 0; i < x->b_number_of_layers; i++)
    {
        if(x->b_layers[i].e_name == name)
        {
            x->b_layers[i].e_state = EGRAPHICS_INVALID;
            return 0;
        }
    }
     */
    return -1;
}

static void stringifyJustification(int justification, char* text)
{
    if(justification & ETEXT_TOP)
        sprintf(text, "-anchor n");
    else if(justification & ETEXT_BOTTOM)
        sprintf(text, "-anchor s");
    else
        sprintf(text, "-anchor ");
    
    if(justification & ETEXT_LEFT)
        strncat(text, "w", 1);
    else if(justification & ETEXT_RIGHT)
        strncat(text, "e", 1);
    else if(justification & ETEXT_CENTRED)
        strncat(text, "center", 6);
}

static t_pt recomputeRectangle(int justification, t_pt const* p1, t_pt const* p2)
{
    t_pt pt;
    if(justification & ETEXT_TOP)
        pt.y = p1->y;
    else if(justification & ETEXT_BOTTOM)
        pt.y = p1->y + p2->y;
    else
        pt.y = p1->y + p2->y * 0.5f;
    
    if(justification & ETEXT_LEFT)
        pt.x = p1->x;
    else if(justification & ETEXT_RIGHT)
        pt.x = p1->x + p2->x;
    else
        pt.x = p1->x + p2->x * 0.5f;
    
    return pt;
}

t_pd_err ebox_paint_layer(t_ebox *x, t_symbol *name, float x_p, float y_p)
{
    if(1)//!x->b_drawing_id)
    {
        return 0;
    }
    /*
    int i, j;
    float start, extent, radius;
    t_elayer* g = NULL;
    for(i = 0; i < x->b_number_of_layers; i++)
    {
        if(x->b_layers[i].e_name == name)
        {
            g = &x->b_layers[i];
            if(g->e_state == EGRAPHICS_CLOSE)
            {
                sys_vgui("%s raise %s\n", x->b_drawing_id->s_name, g->e_id->s_name);
                return -1;
            }
            else if(g->e_state != EGRAPHICS_TODRAW)
            {
                return -1;
            }
        }
    }
    if(g)
    {
        
        for(i = 0; i < g->e_number_objects; i++)
        {
            t_egobj* gobj = g->e_objects+i;
            t_pt * pt;
            ////////////// PATH & LINE ///////////////////////////
            if(gobj->e_type == E_GOBJ_PATH && gobj->e_npoints > 3)
            {
                char header[256];
                char bottom[256];
                int mode = E_PATH_MOVE;
                if(gobj->e_filled)
                {
                    sprintf(header, "%s create polygon ", x->b_drawing_id->s_name);
                    sprintf(bottom, "-smooth bezier -fill %s -width 0 -tags { %s %s }\n", rgba_to_hex(&gobj->e_color),  g->e_id->s_name, x->b_all_id->s_name);
                }
                else
                {
                    sprintf(header, "%s create line ", x->b_drawing_id->s_name);
                    sprintf(bottom, "-smooth bezier -fill %s -width %f -tags { %s %s }\n", rgba_to_hex(&gobj->e_color), gobj->e_width, g->e_id->s_name, x->b_all_id->s_name);
                }
                
                for(j = 0; j < gobj->e_npoints; )
                {
                    pt = gobj->e_points+j++;
                    if(pt->x == E_PATH_MOVE)
                    {
                        if(mode == E_PATH_CURVE)
                        {
                            sys_vgui("%s", bottom);
                        }
                        sys_vgui("%s", header);
                        pt = gobj->e_points+j++;
                        sys_vgui("%d %d ", (int)(pt->x + x_p), (int)(pt->y + y_p));
                        mode = E_PATH_MOVE;
                    }
                    else if(pt->x == E_PATH_CURVE)
                    {
                        pt = gobj->e_points+j++;
                        sys_vgui("%d %d %d %d %d %d ",
                                 (int)((pt+1)->x + x_p), (int)((pt+1)->y + y_p),
                                 (int)((pt+2)->x + x_p), (int)((pt+2)->y + y_p),
                                 (int)((pt+3)->x + x_p), (int)((pt+3)->y + y_p));
                        j += 3;
                        mode = E_PATH_CURVE;
                    }
                    else if(pt->x == E_PATH_LINE)
                    {
                        pt = gobj->e_points+j-1;
                        sys_vgui("%d %d %d %d %d %d ",
                                 (int)((pt-1)->x + x_p), (int)((pt-1)->y + y_p),
                                 (int)((pt+1)->x + x_p), (int)((pt+1)->y + y_p),
                                 (int)((pt+1)->x + x_p), (int)((pt+1)->y + y_p));
                        ++j;
                        mode = E_PATH_CURVE;
                    }
                    else
                    {
                        j++;
                    }
                }
                sys_vgui("%s", bottom);
            }
            ////////////// RECT ///////////////////////////
            else if(gobj->e_type == E_GOBJ_RECT)
            {
                if(gobj->e_filled)
                    sys_vgui("%s create polygon ", x->b_drawing_id->s_name);
                else
                    sys_vgui("%s create line ", x->b_drawing_id->s_name);
                
                for(j = 0; j < gobj->e_npoints; j ++)
                {
                    sys_vgui("%d %d ", (int)(gobj->e_points[j].x + x_p), (int)(gobj->e_points[j].y + y_p));
                }
                
                if(gobj->e_filled)
                    sys_vgui("-fill %s -width 0 -tags { %s %s }\n", rgba_to_hex(&gobj->e_color),  g->e_id->s_name, x->b_all_id->s_name);
                else
                    sys_vgui("-fill %s -width %f -tags { %s %s }\n", rgba_to_hex(&gobj->e_color), gobj->e_width, g->e_id->s_name, x->b_all_id->s_name);
                
            }
            ////////////// OVAL /////////////////
            else if (gobj->e_type == E_GOBJ_OVAL)
            {
                sys_vgui("%s create oval %d %d %d %d ",
                         x->b_drawing_id->s_name,
                         (int)(gobj->e_points[0].x + x_p),
                         (int)(gobj->e_points[0].y + y_p),
                         (int)(gobj->e_points[1].x + x_p),
                         (int)(gobj->e_points[1].y + y_p));
                if(gobj->e_filled)
                    sys_vgui("-fill %s -width 0 -tags { %s %s }\n", rgba_to_hex(&gobj->e_color),  g->e_id->s_name, x->b_all_id->s_name);
                else
                    sys_vgui("-outline %s -width %f -tags { %s %s }\n", rgba_to_hex(&gobj->e_color), gobj->e_width, g->e_id->s_name, x->b_all_id->s_name);
                
            }
            ////////////// ARC /////////////////
            else if (gobj->e_type == E_GOBJ_ARC)
            {
                start = (float)pd_angle(gobj->e_points[1].x - gobj->e_points[0].x,  gobj->e_points[1].y - gobj->e_points[0].y);
                
                extent = gobj->e_points[2].x;
                radius = gobj->e_points[2].y;
                
                sys_vgui("%s create arc %d %d %d %d -start %f -extent %f ",
                         x->b_drawing_id->s_name,
                         (int)(gobj->e_points[0].x - radius + x_p),
                         (int)(gobj->e_points[0].y - radius + y_p),
                         (int)(gobj->e_points[0].x + radius + x_p),
                         (int)(gobj->e_points[0].y + radius + y_p),
                         (float)start / EPD_2PI * 360.f,
                         (float)extent / EPD_2PI * 360.f);
                
                if(gobj->e_filled)
                    sys_vgui("-style pieslice -fill %s -width 0 -tags { %s %s }\n", rgba_to_hex(&gobj->e_color),  g->e_id->s_name, x->b_all_id->s_name);
                else
                    sys_vgui("-style arc -outline %s -width %f -tags { %s %s }\n", rgba_to_hex(&gobj->e_color), gobj->e_width, g->e_id->s_name, x->b_all_id->s_name);
                
            }
            ////////////// TEXT ////////////////
            else if(gobj->e_type == E_GOBJ_TEXT)
            {
                char text[256];
                stringifyJustification(gobj->e_justify, text);
                const t_pt np = recomputeRectangle(gobj->e_justify, gobj->e_points, gobj->e_points+1);
                sys_vgui("%s create text %d %d -text {%s} %s -font {{%s} %d %s %s} -fill %s -width %d -tags { %s %s }\n",
                         x->b_drawing_id->s_name,
                         (int)(np.x + x_p),
                         (int)(np.y + y_p),
                         gobj->e_text,
                         text,
                         gobj->e_font.family->s_name, (int)gobj->e_font.size, gobj->e_font.weight->s_name, gobj->e_font.slant->s_name,
                         rgba_to_hex(&gobj->e_color),
                         (int)(gobj->e_points[1].x) * (int)gobj->e_wrap,
                         g->e_id->s_name,
                         x->b_all_id->s_name);
            }
            else
            {
                return -1;
            }
        }
        g->e_state = EGRAPHICS_CLOSE;
    }
    else
    {
        return -1;
    }*/
    
    return 0;
}

static void ebox_draw_border(t_ebox* x)
{
    /*
    const float bdsize = (x->b_selected_box == EITEM_OBJ) ? pd_clip_min(x->b_boxparameters.d_borderthickness, 1.f) : x->b_boxparameters.d_borderthickness;
    int remove_null;
    t_elayer* g = ebox_start_layer(x, NULL, s_cream_eboxbd, x->b_rect.width, x->b_rect.height);
    if(g && bdsize)
    {
        if(x->b_selected_box == EITEM_OBJ)
        {
            egraphics_set_color_rgba(g, &rgba_blue);
        }
        else
        {
            egraphics_set_color_rgba(g, &x->b_boxparameters.d_bordercolor);
        }
        egraphics_set_line_width(g, bdsize);
        egraphics_rectangle(g, 0.f, 0.f, x->b_rect.width - bdsize, x->b_rect.height - bdsize);
        egraphics_stroke(g);
        
        ebox_end_layer(x, s_cream_eboxbd);
    }
    ebox_paint_layer(x, s_cream_eboxbd, 0.f, 0.f);
     */
}

static void ebox_draw_iolets(t_ebox* x)
{
    int i;
    /*
    const float bdsize = (x->b_selected_box == EITEM_OBJ) ? pd_clip_min(x->b_boxparameters.d_borderthickness, 1.f) : x->b_boxparameters.d_borderthickness;
    int remove_null;
    t_elayer* g = ebox_start_layer(x, NULL, s_cream_eboxio, x->b_rect.width, x->b_rect.height);
    if(g && !x->b_isinsubcanvas)
    {
        egraphics_set_line_width(g, 1);
        egraphics_set_color_rgba(g, &rgba_black);
        for(i = 0; i < obj_ninlets((t_object *)x); i++)
        {
            int pos_x_inlet = 0;
            if(obj_ninlets((t_object *)x) != 1)
                pos_x_inlet = (int)(i / (float)(obj_ninlets((t_object *)x) - 1) * (x->b_rect.width - 8));
            egraphics_rectangle(g, pos_x_inlet, 0, 7, 1);
            egraphics_stroke(g);
        }

        for(i = 0; i < obj_noutlets((t_object *)x); i++)
        {
            int pos_x_outlet = 0;
            if(obj_noutlets((t_object *)x) != 1)
                pos_x_outlet = (int)(i / (float)(obj_noutlets((t_object *)x) - 1) * (x->b_rect.width - 8));
            egraphics_rectangle(g, pos_x_outlet, x->b_rect.height - 2 + bdsize * 2, 7, 1);
            egraphics_stroke(g);
        }
        ebox_end_layer(x, s_cream_eboxio);
    }
    ebox_paint_layer(x, s_cream_eboxio, 0.f, 0.f);
     */
}

static void ebox_invalidate_all(t_ebox *x)
{
    /*
    int i;
    for(i = 0; i < x->b_number_of_layers; i++)
    {
        x->b_layers[i].e_state = EGRAPHICS_INVALID;
    }
     */
}

static void ebox_update(t_ebox *x)
{
    /*
    int i;
    for(i = 0; i < x->b_number_of_layers; i++)
    {
        if(x->b_layers[i].e_state == EGRAPHICS_INVALID)
        {
            sys_vgui("%s delete %s\n", x->b_drawing_id->s_name, x->b_layers[i].e_id->s_name);
        }
    }
     */
}

static void ebox_erase(t_ebox* x)
{
    /*
    if(x->b_obj.o_canvas && glist_isvisible(x->b_obj.o_canvas) && x->b_have_window)
    {
        sys_vgui("destroy %s \n", x->b_drawing_id->s_name);
        x->b_have_window = 0;
    }
    if(x->b_layers)
    {
        free(x->b_layers);
        x->b_layers = NULL;
    }
    x->b_number_of_layers = 0;
     */
}

static void ebox_select(t_ebox* x)
{
    /*
    if(glist_isvisible(x->b_obj.o_canvas))
    {
        if(x->b_selected_box == EITEM_OBJ)
        {
            sys_vgui("%s itemconfigure eboxbd%ld -fill %s\n", x->b_drawing_id->s_name, x,rgba_to_hex(&rgba_blue));
        }
        else
        {
            sys_vgui("%s itemconfigure eboxbd%ld -fill %s\n", x->b_drawing_id->s_name, x,rgba_to_hex(&x->b_boxparameters.d_bordercolor));
        }
    }
     */
}

static void ebox_move(t_ebox* x)
{
    /*
    if(glist_isvisible(x->b_obj.o_canvas))
    {
        sys_vgui("%s coords %s %d %d\n", x->b_canvas_id->s_name, x->b_window_id->s_name, (int)(x->b_rect.x - x->b_boxparameters.d_borderthickness), (int)(x->b_rect.y - x->b_boxparameters.d_borderthickness));
    }
    canvas_fixlinesfor(glist_getcanvas(x->b_obj.o_canvas), (t_text*)x);
     */
}

















void ebox_set_parameter_attribute(t_ebox *x, t_symbol *s, int argc, t_atom* argv)
{
    t_eparam* param;
    int ac = 0; t_atom* av = NULL;
    if(argc > 2 && argv && atom_gettype(argv) == A_FLOAT && atom_gettype(argv+1) == A_SYMBOL)
    {
        param = eparameter_getbyindex(x, (int)atom_getfloat(argv));
        if(param)
        {
            if(atom_getsymbol(argv+1) == gensym("name"))
            {
                unparse_atoms(argc-2, argv+2, &ac, &av);
                if(ac && av && atom_gettype(av) == A_SYMBOL)
                {
                    eparameter_setname(param, atom_getsymbol(av));
                    free(av);
                }
            }
            else if(atom_getsymbol(argv+1) == gensym("label"))
            {
                unparse_atoms(argc-2, argv+2, &ac, &av);
                if(ac && av && atom_gettype(av) == A_SYMBOL)
                {
                    eparameter_setlabel(param, atom_getsymbol(av));
                    free(av);
                }
            }
            else if(atom_getsymbol(argv+1) == gensym("index") && atom_gettype(argv+2) == A_FLOAT)
            {
                eparameter_setindex(param, atom_getfloat(argv+2));
            }
        }
    }
}

static void ebox_parameter_notify(t_eparam* param, t_symbol* action)
{
    t_atom av[3];
    if(is_valid_symbol(eobj_getid(param->p_owner)) && eobj_getid(param->p_owner)->s_thing)
    {
        atom_setsym(av, param->p_bind);
        atom_setsym(av+1, action);
        atom_setfloat(av+2, (float)param->p_index);
        pd_typedmess(param->p_owner->b_obj.o_camo_id->s_thing, s_cream_parameter, 3, av);
    }
}

static t_class* eparameter_setup()
{
    t_class* eparameter_class = NULL;
    t_symbol* eparameter1572_sym = gensym("eparameter1572");
    if(!eparameter1572_sym->s_thing)
    {
        eparameter_class = class_new(gensym("eparameter"), NULL, (t_method)NULL, sizeof(t_etexteditor), CLASS_PD, A_GIMME, 0);
        int aki;
        eparameter1572_sym->s_thing = (t_class **)eparameter_class;
        return eparameter_class;
    }
    else
    {
        return (t_class *)eparameter1572_sym->s_thing;
    }
}

t_eparam* ebox_parameter_create(t_ebox *x, int index)
{
    int i;
    char text[MAXPDSTRING];
    t_eparam **temp  = NULL, *param = NULL;
    t_class* c = eparameter_setup();
    if(c)
    {
        /*
        index--;
        if(index < x->b_nparams && x->b_params[index])
        {
            return x->b_params[index];
        }
        else
        {
            param = (t_eparam *)pd_new(c);
            if(param)
            {
                sprintf(text, "param%ld", (unsigned long)(param));
                param->p_bind       = gensym(text);
                param->p_name       = s_cream_empty;
                param->p_label      = s_cream_empty;
                param->p_owner      = x;
                param->p_index      = index+1;
                param->p_value      = 0.f;
                param->p_min        = 0.f;
                param->p_max        = 1.f;
                param->p_nstep      = (int)(1.f / FLT_EPSILON) + 1;
                param->p_getter     = (t_param_getter)NULL;
                param->p_setter     = (t_param_setter)NULL;
                param->p_getter_t   = (t_param_getter_t)NULL;
                param->p_setter_t   = (t_param_setter_t)NULL;
                param->p_auto       = 1;
                param->p_meta       = 0;
                param->p_enable     = 1;
                param->p_flags      = 0;
                pd_bind((t_pd *)param, param->p_bind);
                if(x->b_params && index < x->b_nparams)
                {
                    x->b_params[index] = param;
                    ebox_parameter_notify(param, s_cream_create);
                    return param;
                }
                else if(x->b_params && index >= x->b_nparams)
                {
                    temp = (t_eparam **)realloc(x->b_params, sizeof(t_eparam *) * (size_t)(index + 1));
                    if(temp)
                    {
                        x->b_params = temp;
                        for(i = (int)x->b_nparams; i < index; i++)
                        {
                            x->b_params[i] = NULL;
                        }
                        x->b_params[index] = param;
                        x->b_nparams = index+1;
                        ebox_parameter_notify(param, s_cream_create);
                        return param;
                    }
                    else
                    {
                        return NULL;
                    }
                }
                else if(!x->b_params || !x->b_nparams)
                {
                    x->b_params = (t_eparam **)malloc(sizeof(t_eparam *) * (size_t)(index + 1));
                    if(x->b_params)
                    {
                        for(i = 0; i < index; i++)
                        {
                            x->b_params[i] = NULL;
                        }
                        x->b_params[index] = param;
                        x->b_nparams = index+1;
                        ebox_parameter_notify(param, s_cream_create);
                        return param;
                    }
                    else
                    {
                        x->b_nparams = 0;
                        return NULL;
                    }
                }
            }
        }
         */
    }
    return NULL;
}

void ebox_parameter_destroy(t_ebox* x, int index)
{
    int i = 0;
    /*
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            ebox_parameter_notify(x->b_params[index], s_cream_destroy);
            pd_unbind((t_pd *)x->b_params[index], x->b_params[index]->p_bind);
            free(x->b_params[index]);
            x->b_params[index] = NULL;
            for(i = index; i < x->b_nparams - 1; i++)
            {
                x->b_params[i] = x->b_params[i+1];
            }
        }
    }
     */
}

float ebox_parameter_getvalue(t_ebox* x, int index)
{
    /*
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            if(x->b_params[index]->p_getter)
            {
                return x->b_params[index]->p_getter(x->b_params[index]->p_owner, x->b_params[index]);
            }
            return x->b_params[index]->p_value;
        }
    }
     */
    return 0;
}

void ebox_parameter_setvalue(t_ebox* x, int index, float value, char notify)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            if(x->b_params[index]->p_setter)
            {
                x->b_params[index]->p_setter(x->b_params[index]->p_owner, x->b_params[index], value);
            }
            else
            {
                if(x->b_params[index]->p_min < x->b_params[index]->p_max)
                {
                    x->b_params[index]->p_value = pd_clip(value, x->b_params[index]->p_min, x->b_params[index]->p_max);
                }
                else
                {
                    x->b_params[index]->p_value  = pd_clip(value, x->b_params[index]->p_max, x->b_params[index]->p_min);
                }
            }
            if(notify)
            {
                ebox_parameter_notify(x->b_params[index], s_cream_value_changed);
            }
        }
    }
}

float ebox_parameter_getvalue_normalized(t_ebox* x, int index)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            if(x->b_params[index]->p_max == x->b_params[index]->p_min)
            {
                return 1.;
            }
            else if(x->b_params[index]->p_min < x->b_params[index]->p_max)
            {
                return (ebox_parameter_getvalue(x, index+1) - x->b_params[index]->p_min) / (x->b_params[index]->p_max - x->b_params[index]->p_min);
            }
            else
            {
                return (ebox_parameter_getvalue(x, index+1) - x->b_params[index]->p_max) / (x->b_params[index]->p_min - x->b_params[index]->p_max);
            }
        }
    }
    return 0.f;
}

void ebox_parameter_setvalue_normalized(t_ebox* x, int index, float value, char notify)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            if(x->b_params[index]->p_min < x->b_params[index]->p_max)
            {
                ebox_parameter_setvalue(x, index+1, (value * (x->b_params[index]->p_max - x->b_params[index]->p_min) + x->b_params[index]->p_min), notify);
            }
            else
            {
                ebox_parameter_setvalue(x, index+1, (value * (x->b_params[index]->p_min - x->b_params[index]->p_max) + x->b_params[index]->p_max), notify);
            }
        }
    }
}

float ebox_parameter_getmin(t_ebox* x, int index)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            return x->b_params[index]->p_min;
        }
    }
    return 1.f;
}

float ebox_parameter_getmax(t_ebox* x, int index)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            return x->b_params[index]->p_max;
        }
    }
    return 1.f;
}

int ebox_parameter_getnstep(t_ebox* x, int index)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            return x->b_params[index]->p_nstep;
        }
    }
    return 1;
}

t_symbol* ebox_parameter_getbind(t_ebox* x, int index)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            return x->b_params[index]->p_bind;
        }
    }
    return NULL;
}

char ebox_parameter_isinverted(t_ebox* x, int index)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            return (char)(x->b_params[index]->p_max < x->b_params[index]->p_min);
        }
    }
    return 0;
}

void ebox_parameter_notify_changes(t_ebox *x, int index)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            ebox_parameter_notify(x->b_params[index], s_cream_value_changed);
        }
    }
}

void ebox_parameter_begin_changes(t_ebox *x, int index)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            ebox_parameter_notify(x->b_params[index], s_cream_beginchanges);
        }
    }
}

void ebox_parameter_end_changes(t_ebox *x, int index)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            ebox_parameter_notify(x->b_params[index], s_cream_endchanges);
        }
    }
}

void ebox_parameter_setname(t_ebox* x, int index, t_symbol* name)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            x->b_params[index]->p_name = get_valid_symbol(name);
            ebox_parameter_notify(x->b_params[index], s_cream_attr_modified);
        }
    }
}

void ebox_parameter_setlabel(t_ebox* x, int index, t_symbol* label)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            x->b_params[index]->p_label = get_valid_symbol(label);
            ebox_parameter_notify(x->b_params[index], s_cream_attr_modified);
        }
    }
}

void ebox_parameter_setminmax(t_ebox* x, int index, float min, float max)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            x->b_params[index]->p_min = min;
            x->b_params[index]->p_max = max;
            ebox_parameter_notify(x->b_params[index], s_cream_attr_modified);
            ebox_parameter_setvalue(x, index+1, x->b_params[index]->p_value, 1);
        }
    }
}

void ebox_parameter_setnstep(t_ebox* x, int index, int nstep)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            x->b_params[index]->p_nstep = nstep > 1 ? (int)nstep : 1;
            ebox_parameter_notify(x->b_params[index], s_cream_attr_modified);
        }
    }
}

void ebox_parameter_setindex(t_ebox* x, int index, int pindex)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            x->b_params[index]->p_index = pindex;
            ebox_parameter_notify(x->b_params[index], s_cream_attr_modified);
        }
    }
}

void ebox_parameter_setflags(t_ebox* x, int index, long flags)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            x->b_params[index]->p_flags = flags;
            ebox_parameter_notify(x->b_params[index], s_cream_attr_modified);
        }
    }
}

void ebox_parameter_enable(t_ebox* x, int index)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            x->b_params[index]->p_enable = 1;
            ebox_parameter_notify(x->b_params[index], s_cream_attr_modified);
        }
    }
}

void ebox_parameter_disable(t_ebox* x, int index)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            x->b_params[index]->p_enable = 0;
            ebox_parameter_notify(x->b_params[index], s_cream_attr_modified);
        }
    }
}

void ebox_parameter_setsettergetter(t_ebox* x, int index, t_param_setter setter, t_param_getter getter)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            x->b_params[index]->p_setter = setter;
            x->b_params[index]->p_getter = getter;
            ebox_parameter_notify(x->b_params[index], s_cream_attr_modified);
        }
    }
}

void ebox_parameter_setsettergetter_text(t_ebox* x, int index, t_param_setter_t setter, t_param_getter_t getter)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            x->b_params[index]->p_setter_t = setter;
            x->b_params[index]->p_getter_t = getter;
            ebox_parameter_notify(x->b_params[index], s_cream_attr_modified);
        }
    }
}


t_eparam* eparameter_getbyindex(t_ebox* x, int index)
{
    int tod_chaned;
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            return x->b_params[index];
        }
    }
    return NULL;
}








