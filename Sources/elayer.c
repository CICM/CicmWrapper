/*
 * CicmWrapper
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "elayer.h"
#include "eguiimp.h"

static inline void egpath_init(t_gpath_impl *p, char new)
{
    if(new)
    {
        p->i_path.p_points = NULL;
        p->i_path.p_rsize  = 0;
    }
    p->i_path.p_size  = 0;
    p->i_filled = 0;
    p->i_width  = 1.f;
    p->i_color  = rgba_white;
}

static void egatom_init(t_gatom *i, char new)
{
    /*
    if(i->a_type == EPD_SHAPE_PATH)
    {
        egpath_init(&i->a_word.w_path, new);
    }
    else if(i->a_type == EPD_SHAPE_TEXT)
    {
        int todo;
    }
     */
}

static void egatom_free(t_gatom *i)
{
    if(i->a_type == EPD_GATOM_PATH)
    {
        if(i->a_word.w_path.i_path.p_points && i->a_word.w_path.i_path.p_rsize)
        {
            free(i->a_word.w_path.i_path.p_points);
            i->a_word.w_path.i_path.p_points = NULL;
            i->a_word.w_path.i_path.p_rsize  = 0;
            i->a_word.w_path.i_path.p_size   = 0;
        }
    }
    else if(i->a_type == EPD_GATOM_TEXT)
    {
        free(i->a_word.w_text.c_text);
    }
}

static void elayer_clear(t_elayer *g)
{
    size_t i;
    for(i = 0; i < g->l_natoms; i++)
    {
        egatom_free(g->l_atoms+i);
    }
    g->l_atoms  = NULL;
    g->l_natoms = 0;
}

void elayer_free(t_elayer *g)
{
    elayer_clear(g);
    free(g);
}

t_elayer* elayer_new(t_symbol *name, const float width, const float height)
{
    t_elayer* x = (t_elayer *)malloc(sizeof(t_elayer));
    if(x)
    {
        x->l_name                   = name;
        x->l_state                  = EPD_LAYER_OPEN;
        x->l_width                  = (width > 0.f) ? width : 0.f;
        x->l_height                 = (height > 0.f) ? height : 0.f;
        x->l_transform              = 0;
        elayer_matrix_init(&x->l_matrix, 1., 0., 0., 1., 0., 0.);
        egpath_init(&x->l_path, 1);
        x->l_atoms                  = NULL;
        x->l_natoms                 = 0;
    }
    else
    {
        pd_error(NULL, "can't allocate a new layer.");
    }
    return x;
}

t_elayer* elayer_reopen(t_elayer* g)
{
    if(g->l_state == EPD_LAYER_INVALID)
    {
        elayer_matrix_init(&g->l_matrix, 1., 0., 0., 1., 0., 0.);
        egpath_init(&g->l_path, 0);
        elayer_clear(g);
        g->l_transform  = 0;
        g->l_state      = EPD_LAYER_OPEN;
        return g;
    }
    return NULL;
}

t_symbol* elayer_getname(t_elayer const* g)
{
    return g->l_name;
}

t_pd_err elayer_invalidate(t_elayer* g)
{
    if(g->l_state == EPD_LAYER_CLOSE)
    {
        g->l_state = EPD_LAYER_INVALID;
        return 0;
    }
    return -1;
}

t_pd_err elayer_end(t_elayer* g)
{
    if(g->l_state == EPD_LAYER_OPEN)
    {
        g->l_state = EPD_LAYER_READY;
        return 0;
    }
    return -1;
}

t_pd_err elayer_painted(t_elayer* g)
{
    if(g->l_state == EPD_LAYER_READY)
    {
        g->l_state = EPD_LAYER_CLOSE;
        return 0;
    }
    return -1;
}

static t_pd_err elayer_add_gatoms(t_elayer *g)
{
    t_gatom* temp;
    if(g->l_natoms && g->l_atoms)
    {
        temp = realloc(g->l_atoms, (g->l_natoms + 1) * sizeof(t_gatom));
        if(temp)
        {
            g->l_atoms = temp;
            g->l_atoms[g->l_natoms].a_type = EPD_GATOM_INVALID;
            g->l_natoms++;
            return 0;
        }
        else
        {
            error("layer %s can't increase the number of graphical atoms.", g->l_name->s_name);
            return -1;
        }
    }
    else
    {
        g->l_atoms = malloc(sizeof(t_gatom));
        if(g->l_atoms)
        {
            g->l_atoms[g->l_natoms].a_type = EPD_GATOM_INVALID;
            g->l_natoms = 1;
            return 0;
        }
        else
        {
            error("layer %s can't increase the number of graphical atoms.", g->l_name->s_name);
            return -1;
        }
    }
}

void etextlayout_draw(t_etextlayout* textlayout, t_elayer *g)
{
    size_t const index = g->l_natoms;
    if(!elayer_add_gatoms(g))
    {
        g->l_atoms[index].a_word.w_text.c_text = (char *)malloc(MAXPDSTRING * sizeof(char));
        if(g->l_atoms[index].a_word.w_text.c_text)
        {
            etextlayout_copy(&g->l_atoms[index].a_word.w_text, textlayout);
            g->l_atoms[index].a_type = EPD_GATOM_TEXT;
        }
        else
        {
            error("layer %s can't copy the textlayout.", g->l_name->s_name);
        }
    }
}

static t_pd_err elayer_record(t_elayer *g, char filled, char preserved)
{
    size_t const index = g->l_natoms;
    if(!elayer_add_gatoms(g))
    {
        g->l_atoms[index].a_word.w_path.i_path.p_points = NULL;
        g->l_atoms[index].a_word.w_path.i_path.p_rsize  = 0;
        g->l_atoms[index].a_word.w_path.i_path.p_size   = 0;
        g->l_atoms[index].a_type = EPD_GATOM_PATH;
        if(!preserved)
        {
            epath_swap_path(&g->l_atoms[index].a_word.w_path.i_path, &g->l_path.i_path);
        }
        else
        {
            epath_copy_path(&g->l_atoms[index].a_word.w_path.i_path, &g->l_path.i_path);
        }
        if(g->l_transform)
        {
            epath_apply_matrix(&g->l_atoms[index].a_word.w_path.i_path, &g->l_matrix);
        }
        g->l_atoms[index].a_word.w_path.i_width = g->l_path.i_width;
        g->l_atoms[index].a_word.w_path.i_filled = filled;
        g->l_atoms[index].a_word.w_path.i_color = g->l_path.i_color;
        g->l_path.i_width = 1;
        g->l_path.i_color = rgba_black;
    }
    return 0;
}

void elayer_fill_preserve(t_elayer *g)
{
    elayer_record(g, 1, 1);
}

void elayer_fill(t_elayer *g)
{
    elayer_record(g, 1, 0);
}

void elayer_stroke_preserve(t_elayer *g)
{
    elayer_record(g, 0, 1);
}

void elayer_stroke(t_elayer *g)
{
    elayer_record(g, 0, 0);
}


void elayer_set_line_width(t_elayer *g, const float width)
{
    g->l_path.i_width = (width > 1.f) ? width : 1.f;
}

void elayer_set_color_rgba(t_elayer *g, const t_rgba *rgba)
{
    memcpy(&g->l_path.i_color, rgba, sizeof(t_rgba));
}

void elayer_set_color_rgb(t_elayer *g, const t_rgb *rgb)
{
    memcpy(&g->l_path.i_color, rgb, sizeof(t_rgb));
    g->l_path.i_color.alpha = 1.f;
}

void elayer_set_color_hsla(t_elayer *g, const t_hsla *hsla)
{
    const t_rgba rgba = hsla_to_rgba(hsla);
    memcpy(&g->l_path.i_color, &rgba, sizeof(t_rgba));
}

void elayer_set_color_hsl(t_elayer *g, const t_hsl *hsl)
{
    const t_rgb rgb = hsl_to_rgb(hsl);
    memcpy(&g->l_path.i_color, &rgb, sizeof(t_rgb));
    g->l_path.i_color.alpha = 1.f;
}

void elayer_set_color_hex(t_elayer *g, const char *hex)
{
    const t_rgba rgba = hex_to_rgba(hex);
    memcpy(&g->l_path.i_color, &rgba, sizeof(t_rgba));
}

void elayer_move_to(t_elayer *g, const float x, const float y)
{
    epath_move_to(&g->l_path.i_path, x, y);
}

void elayer_line_to(t_elayer *g, const float x, const float y)
{
    epath_line_to(&g->l_path.i_path, x, y);
}

void elayer_curve_to(t_elayer *g, const float ctrl1x, const float ctrl1y, const float ctrl2x, const float ctrl2y, const float endx, const float endy)
{
    epath_curve_to(&g->l_path.i_path, ctrl1x, ctrl1y, ctrl2x, ctrl2y, endx, endy);
}

void elayer_close_path(t_elayer *g)
{
    epath_close(&g->l_path.i_path);
}

void elayer_line(t_elayer *g, const float x0, const float y0, const float x1, const float y1)
{
    epath_line(&g->l_path.i_path, x0, y0, x1, y1);
}

void elayer_line_fast(t_elayer *g, const float x0, const float y0, const float x1, const float y1)
{
    epath_line(&g->l_path.i_path, x0, y0, x1, y1);
    elayer_stroke(g);
}

void elayer_curve(t_elayer *g, const float startx, const float starty, const float ctrl1x, const float ctrl1y, const float ctrl2x, const float ctrl2y, const float endx, const float endy)
{
    epath_curve(&g->l_path.i_path, startx, starty, ctrl1x, ctrl1y, ctrl2x, ctrl2y, endx, endy);
}

void elayer_rectangle(t_elayer *g, const float x, const float y, const float width, const float height)
{
    epath_rectangle(&g->l_path.i_path, x, y, width, height);
}

void elayer_rectangle_rounded(t_elayer *g, const float x, const float y, const float width, const float height, const float roundness)
{
    epath_rectangle_rounded(&g->l_path.i_path, x, y, width, height, roundness);
}

void elayer_oval(t_elayer *g, const float xc, const float yc, const float radiusx, const float radiusy)
{
    epath_oval(&g->l_path.i_path, xc, yc, radiusx, radiusy);
}

void elayer_circle(t_elayer *g, const float xc, const float yc, const float radius)
{
    epath_circle(&g->l_path.i_path, xc, yc, radius);
}

void elayer_arc_to(t_elayer *g, const float cx, const float cy, const float extend)
{
    error("elayer_arc_to not implemented.");
}

void elayer_arc_oval_to(t_elayer *g, const float cx, const float cy, const float radius, const float extend)
{
    error("elayer_arc_oval_to not implemented.");
}

void elayer_arc(t_elayer *g, const float xc, const float yc, const float radius, const float angle1, const float angle2)
{
    epath_arc(&g->l_path.i_path, xc, yc, radius, angle1, angle2);
}

void elayer_arc_oval(t_elayer *g, const float xc, const float yc, const float radiusx, const float radiusy, const float angle1, const float angle2)
{
    epath_arc_oval(&g->l_path.i_path, xc, yc, radiusx, radiusy, angle1, angle2);
}




void elayer_matrix_init(t_matrix *x, const float xx, const float yx, const float xy, const float yy, const float x0, const float y0)
{
    x->xx = xx;
    x->yx = yx;
    x->xy = xy;
    x->yy = yy;
    x->x0 = x0;
    x->y0 = y0;
}

void elayer_set_matrix(t_elayer *g, const t_matrix* matrix)
{
    g->l_transform = 1;
    elayer_matrix_init(&g->l_matrix, matrix->xx, matrix->yx, matrix->xy, matrix->yy, matrix->x0, matrix->y0);
}

void elayer_rotate(t_elayer *g, const float angle)
{
    const float cosRad = cosf(angle);
    const float sinRad = sinf(angle);
    const t_matrix temp = g->l_matrix;
    g->l_matrix.xx = temp.xx * cosRad - temp.yx * sinRad;
    g->l_matrix.yx = temp.xx * sinRad + temp.yx * cosRad;
    g->l_matrix.xy = temp.xy * cosRad - temp.yy * sinRad;
    g->l_matrix.yy = temp.xy * sinRad + temp.yy * cosRad;
    g->l_transform = 1;
}

void efont_init(t_efont* font, t_symbol* family, char bold, char italic, const float size)
{
    int todo;
    font->family = family;
    font->slant = italic ? gensym("italic") : gensym("roman");
    font->weight = bold ? gensym("bold") : gensym("normal");
    font->size = (size > 1.f) ? size : 1.f;
}




