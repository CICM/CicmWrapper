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
#include <assert.h>

typedef enum
{
    G_NULL,
    G_TEXT,
    G_PATH
}  t_atomtype_g;


typedef struct _linternal
{
    t_epath* i_path;
    float    i_width;
    char     i_filled;
    t_rgba   i_color;
} t_linternal;

/**
 * @union t_ekernel
 * @brief The internal union of a graphical atom.
 * @details It contains the all the differents kinds of graphical objects.
 */
typedef union _ekernel_g
{
    t_epath*        k_path;
    t_etextlayout*  k_text;
} t_ekernel_g;

/**
 * @struct t_eatom_g
 * @brief The internal union of a graphical atom.
 * @details It contains the all the differents kinds of graphical objects.
 */
typedef struct _eatom_g
{
    t_atomtype_g    a_type;
    t_ekernel_g     a_kernel;
} t_eatom_g;

/**
 * @struct t_elayer
 * @brief The drawing layer.
 * @details It contains the all the informations and the graphical objects to be drawn.
 */
struct _elayer
{
    t_symbol*           l_name;             /*!< The name. */
    long                l_state;            /*!< The state. */
    float               l_width;            /*!< The width. */
    float               l_height;           /*!< The height. */
    t_matrix            l_matrix;           /*!< The matrix. */
    t_linternal         l_current;          /*!< The current internals. */
    t_linternal*        l_interns;          /*!< The internals. */
    size_t              l_ninterns;         /*!< The number of internals. */
};

static void einternal_init(t_linternal *i, char alloc)
{
    if(alloc)
    {
        i->i_path   = epath_new();
    }
    else
    {
        epath_clear(i->i_path);
    }
    i->i_filled = 0;
    i->i_width  = 1.f;
    i->i_color  = rgba_white;
}

static void einternal_free(t_linternal *i)
{
    if(i->i_path)
    {
        epath_free(i->i_path);
    }
    i->i_path = NULL;
}




void elayer_free(t_elayer *g)
{
    size_t i;
    einternal_free(&g->l_current);
    for(i = 0; i < g->l_ninterns; i++)
    {
        einternal_free(g->l_interns+i);
    }
    if(g->l_ninterns && g->l_interns)
    {
        free(g->l_interns);
    }
    free(g);
}

t_elayer* elayer_new(t_symbol *name, const float width, const float height)
{
    t_elayer* x = malloc(sizeof(t_elayer));
    if(x)
    {
        x->l_name                 = name;
        x->l_state                = EPD_LAYER_OPEN;
        x->l_width                = (width > 0.f) ? width : 0.f;
        x->l_height               = (height > 0.f) ? height : 0.f;
        elayer_matrix_init(&x->l_matrix, 1., 0., 0., 1., 0., 0.);
        einternal_init(&x->l_current, 1);
        x->l_interns              = NULL;
        x->l_ninterns             = 0;
    }
    else
    {
        pd_error(NULL, "can't allocate a new layer.");
    }
    return x;
}

long elayer_getstate(t_elayer const* g)
{
    return g->l_state;
}

t_symbol* elayer_getname(t_elayer const* g)
{
    return g->l_name;
}

t_pd_err elayer_invalidate(t_elayer* g)
{
    post("elayer_invalidate %s", g->l_name->s_name);
    if(g->l_state == EPD_LAYER_CLOSE)
    {
        size_t i;
        einternal_init(&g->l_current, 0);
        for(i = 0; i < g->l_ninterns; i++)
        {
            einternal_free(g->l_interns+i);
        }
        if(g->l_ninterns && g->l_interns)
        {
            free(g->l_interns);
        }
        g->l_state = EPD_LAYER_INVALID;
        return 0;
    }
    return -1;
}

t_pd_err elayer_end(t_elayer* g)
{
    post("elayer_end %s", g->l_name->s_name);
    if(g->l_state == EPD_LAYER_OPEN)
    {
        g->l_state = EPD_LAYER_READY;
        return 0;
    }
    return -1;
}

t_pd_err elayer_paint(t_elayer* g)
{
    post("elayer_paint %s", g->l_name->s_name);
    if(g->l_state == EPD_LAYER_READY)
    {
        g->l_state = EPD_LAYER_CLOSE;
        return 0;
    }
    return -1;
}

static t_pd_err elayer_record(t_elayer *g, char filled, char preserved)
{
    post("elayer_record %s", g->l_name->s_name);
    int todo;
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
    g->l_current.i_width = (width > 1.f) ? width : 1.f;
}

void elayer_set_color_rgba(t_elayer *g, const t_rgba *rgba)
{
    memcpy(&g->l_current.i_color, rgba, sizeof(t_rgba));
}

void elayer_set_color_rgb(t_elayer *g, const t_rgb *rgb)
{
    memcpy(&g->l_current.i_color, rgb, sizeof(t_rgb));
    g->l_current.i_color.alpha = 1.f;
}

void elayer_set_color_hsla(t_elayer *g, const t_hsla *hsla)
{
    const t_rgba rgba = hsla_to_rgba(hsla);
    memcpy(&g->l_current.i_color, &rgba, sizeof(t_rgba));
}

void elayer_set_color_hsl(t_elayer *g, const t_hsl *hsl)
{
    const t_rgb rgb = hsl_to_rgb(hsl);
    memcpy(&g->l_current.i_color, &rgb, sizeof(t_rgb));
    g->l_current.i_color.alpha = 1.f;
}

void elayer_set_color_hex(t_elayer *g, const char *hex)
{
    const t_rgba rgba = hex_to_rgba(hex);
    memcpy(&g->l_current.i_color, &rgba, sizeof(t_rgba));
}

void etextlayout_draw(t_etextlayout* textlayout, t_elayer *g)
{
    ;
}

void elayer_move_to(t_elayer *g, const float x, const float y)
{
    epath_move_to(g->l_current.i_path, x, y);
}

void elayer_line_to(t_elayer *g, const float x, const float y)
{
    epath_line_to(g->l_current.i_path, x, y);
}

void elayer_curve_to(t_elayer *g, const float ctrl1x, const float ctrl1y, const float ctrl2x, const float ctrl2y, const float endx, const float endy)
{
    epath_curve_to(g->l_current.i_path, ctrl1x, ctrl1y, ctrl2x, ctrl2y, endx, endy);
}

void elayer_close_path(t_elayer *g)
{
    epath_close(g->l_current.i_path);
}

void elayer_line(t_elayer *g, const float x0, const float y0, const float x1, const float y1)
{
    epath_line(g->l_current.i_path, x0, y0, x1, y1);
}

void elayer_line_fast(t_elayer *g, const float x0, const float y0, const float x1, const float y1)
{
    epath_line(g->l_current.i_path, x0, y0, x1, y1);
    elayer_stroke(g);
}

void elayer_curve(t_elayer *g, const float startx, const float starty, const float ctrl1x, const float ctrl1y, const float ctrl2x, const float ctrl2y, const float endx, const float endy)
{
    epath_curve(g->l_current.i_path, startx, starty, ctrl1x, ctrl1y, ctrl2x, ctrl2y, endx, endy);
}

void elayer_rectangle(t_elayer *g, const float x, const float y, const float width, const float height)
{
    epath_rectangle(g->l_current.i_path, x, y, width, height);
}

void elayer_rectangle_rounded(t_elayer *g, const float x, const float y, const float width, const float height, const float roundness)
{
    epath_rectangle_rounded(g->l_current.i_path, x, y, width, height, roundness);
}

void elayer_oval(t_elayer *g, const float xc, const float yc, const float radiusx, const float radiusy)
{
    epath_oval(g->l_current.i_path, xc, yc, radiusx, radiusy);
}

void elayer_circle(t_elayer *g, const float xc, const float yc, const float radius)
{
    epath_circle(g->l_current.i_path, xc, yc, radius);
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
    epath_arc(g->l_current.i_path, xc, yc, radius, angle1, angle2);
}

void elayer_arc_oval(t_elayer *g, const float xc, const float yc, const float radiusx, const float radiusy, const float angle1, const float angle2)
{
    epath_arc_oval(g->l_current.i_path, xc, yc, radiusx, radiusy, angle1, angle2);
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
}

void efont_init(t_efont* font, t_symbol* family, char bold, char italic, const float size)
{
    font->family = family;
    font->slant = italic ? gensym("italic") : gensym("roman");
    font->weight = bold ? gensym("bold") : gensym("normal");
    font->size = (size > 1.f) ? size : 1.f;
}


/*
t_pd_err ebox_paint_layer(t_ebox *x, t_object* view, t_symbol *name, float x_p, float y_p)
{
    if(1)//!x->b_drawing_id)
    {
        return 0;
    }
    int i, j;
    float start, extent, radius;
    t_elayer* g = NULL;
    for(i = 0; i < x->b_number_of_layers; i++)
    {
        if(x->b_layers[i].e_name == name)
        {
            g = &x->b_layers[i];
            if(g->e_state == elayer_CLOSE)
            {
                sys_vgui("%s raise %s\n", x->b_drawing_id->s_name, g->e_id->s_name);
                return -1;
            }
            else if(g->e_state != elayer_TODRAW)
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
        g->e_state = elayer_CLOSE;
    }
    else
    {
        return -1;
    }
    
    return 0;
}
*/




