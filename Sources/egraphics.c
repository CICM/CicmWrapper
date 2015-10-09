/*
 * CicmWrapper
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "egraphics.h"

static const float k = 0.55228474983079356430692996582365594804286956787109f;
static char ColBuf[10];
static const char HexDigits[] = "0123456789ABCDEF";

const t_rgba rgba_black = {0.f, 0.f, 0.f, 1.f};
const t_rgba rgba_greydark = {0.3f, 0.3, 0.3f, 1.f};
const t_rgba rgba_grey = {0.5f, 0.5, 0.5f, 1.f};
const t_rgba rgba_greylight = {0.8f, 0.8, 0.8f, 1.f};
const t_rgba rgba_white = {1.f, 1.f, 1.f, 1.f};
const t_rgba rgba_blue = {0.f, 0.f, 1.f, 1.f};
const t_rgba rgba_green = {0.f, 1.f, 0.f, 1.f};
const t_rgba rgba_red = {1.f, 0.f, 0.f, 1.f};

typedef enum
{
    E_GOBJ_INVALID           = 0,   /*!< This type is invalid. */
    E_GOBJ_PATH              = 1,   /*!< This is a path. */
    E_GOBJ_RECT              = 2,   /*!< This is a rect. */
    E_GOBJ_ARC               = 3,   /*!< This is an arc. */
    E_GOBJ_OVAL              = 4,   /*!< This is an oval. */
    E_GOBJ_TEXT              = 5,   /*!< This is a text. */
    E_GOBJ_IMAG              = 6    /*!< This is an image. */
} egraphics_types;

/**
 * @enum epath_types
 * @brief The types of path.
 * @details It define all the path type.
 */
typedef enum
{
    E_PATH_MOVE    = 0,   /*!< This type is move. */
    E_PATH_LINE    = 1,   /*!< This type is move. */
    E_PATH_CURVE   = 2,    /*!< This type is curve. */
    E_PATH_CLOSE   = 3    /*!< This type is curve. */
} epath_types;

/**
 * @enum elayer_flags
 * @brief The flags that defines the status of a layer.
 * @details It define all possible the status of a layer.
 */
typedef enum elayer_flags
{
    EGRAPHICS_OPEN      = 0,  /*!< Open. */
    EGRAPHICS_CLOSE     = -1, /*!< Closed. */
    EGRAPHICS_INVALID   = -2, /*!< Invalid. */
    EGRAPHICS_TODRAW    = -3  /*!< To be drawn. */
} elayer_flags;


/**
 * @struct t_etext
 * @brief The text structure.
 * @details It contains the all the informations to be drawn.
 */
struct _etext
{
    char*           c_text;     /*!< The text. */
    char            c_wrap;     /*!< If the text should be wrapped. */
    t_rgba          c_color;    /*!< The color of the text. */
    t_efont         c_font;     /*!< The font of the text. */
    t_rect          c_rect;     /*!< The rectangle of the text. */
    int             c_justify;  /*!< The justification of the graphical object. */
};

/**
 * @struct t_egobj
 * @brief The drawing object.
 * @details It contains the all the informations to be drawn.
 */
typedef struct t_egobj
{
    egraphics_types e_type;         /*!< The type of the graphical object. */
    int             e_filled;       /*!< The filled state of the graphical object. */
    t_rgba          e_color;        /*!< The color of the graphical object. */
    float           e_width;        /*!< The line width of the graphical object. */
    t_pt*           e_points;       /*!< The points of the graphical object. */
    int             e_npoints;      /*!< The number of points of the graphical object. */
    int             e_rspace;       /*!< The real number of points of the graphical object. */
    t_efont         e_font;         /*!< The font of the graphical object. */
    int             e_justify;      /*!< The justification of the graphical object. */
    char*           e_text;         /*!< The text of the graphical object. */
    char            e_wrap;         /*!< If the text should be wrapped. */
} t_egobj;

/**
 * @struct t_elayer
 * @brief The drawing layer.
 * @details It contains the all the informations and the graphical objects to be drawn.
 */
struct _elayer
{
    t_object*           l_owner;            /*!< The layer owner. */
    t_symbol*           l_name;             /*!< The layer name. */
    char                l_tag[MAXPDSTRING]; /*!< The layer tag. */
    char                l_state;            /*!< The layer state. */
    float               l_width;            /*!< The layer width. */
    float               l_height;           /*!< The layer height. */
    t_matrix            l_matrix;           /*!< The layer matrix. */
    t_egobj             l_current;          /*!< The layer new object. */
    t_egobj*            l_gobjs;            /*!< The layer objects. */
    size_t              l_ngobjs;           /*!< The number of layer objects. */
};

static void egraphics_apply_matrix(t_elayer *g, t_egobj* gobj);

t_elayer* egraphics_create(t_object *owner, t_symbol *name, float width, float height)
{
    t_elayer* graphic = (t_elayer *)malloc(sizeof(t_elayer));
    if(graphic)
    {
        graphic->l_owner        = owner;
        graphic->l_name         = name;
        sprintf(graphic->l_tag, "%s%ld", name->s_name, (long)owner);
        graphic->l_state        = EGRAPHICS_OPEN;
        graphic->l_height       = (float)pd_clip_min(height, 0.);
        graphic->l_width        = (float)pd_clip_min(width, 0.);

        egraphics_matrix_init(&graphic->l_matrix, 1., 0., 0., 1., 0., 0.);
        graphic->l_ngobjs       = 0;
        graphic->l_gobjs        = NULL;
        graphic->l_current.e_points     = NULL;
        graphic->l_current.e_npoints    = 0;
        graphic->l_current.e_rspace     = 0;
    }
    return graphic;
}

void egraphics_set_line_width(t_elayer *g, float width)
{
    g->l_current.e_width = pd_clip_min(width, 0.);
}

void egraphics_set_color_rgba(t_elayer *g, const t_rgba *rgba)
{
    g->l_current.e_color.red    = rgba->red;
    g->l_current.e_color.green  = rgba->green;
    g->l_current.e_color.blue   = rgba->blue;
    g->l_current.e_color.alpha  = rgba->alpha;
}

void egraphics_set_color_rgb(t_elayer *g, const t_rgb *rgb)
{
    g->l_current.e_color.red    = rgb->red;
    g->l_current.e_color.green  = rgb->green;
    g->l_current.e_color.blue   = rgb->blue;
    g->l_current.e_color.alpha  = 1.f;
}

void egraphics_set_color_hsla(t_elayer *g, const t_hsla *hsla)
{
    const t_rgba rgba = hsla_to_rgba(hsla);
    g->l_current.e_color.red    = rgba.red;
    g->l_current.e_color.green  = rgba.green;
    g->l_current.e_color.blue   = rgba.blue;
    g->l_current.e_color.alpha  = rgba.alpha;
}

void egraphics_set_color_hsl(t_elayer *g, const t_hsl *hsl)
{
    const t_rgb rgb = hsl_to_rgb(hsl);
    g->l_current.e_color.red    = rgb.red;
    g->l_current.e_color.green  = rgb.green;
    g->l_current.e_color.blue   = rgb.blue;
    g->l_current.e_color.alpha  = 1.f;
}

void egraphics_set_color_hex(t_elayer *g, const char *hex)
{
    const t_rgba rgba = hex_to_rgba(hex);
    g->l_current.e_color.red    = rgba.red;
    g->l_current.e_color.green  = rgba.green;
    g->l_current.e_color.blue   = rgba.blue;
    g->l_current.e_color.alpha  = rgba.alpha;
}

static void egraphics_paint(t_elayer *g, int filled, int preserved)
{
    t_egobj *nobj, *temp;
    if(g->l_current.e_type != E_GOBJ_INVALID)
    {
        temp = (t_egobj *)realloc(g->l_objects, (size_t)(g->l_number_objects + 1) * sizeof(t_egobj));
        if(temp)
        {
            g->l_objects = temp;
            nobj = g->l_objects + g->l_number_objects;
            g->l_number_objects++;
            if (filled)
            {
                nobj->l_filled = 1;
            }
            else
            {
                nobj->l_filled = 0;
            }
            nobj->l_type      = g->l_current.e_type;
            nobj->l_npoints   = g->l_current.e_npoints;
            nobj->l_points    = (t_pt*)malloc((size_t)nobj->l_npoints * sizeof(t_pt));
            nobj->l_text      = NULL;
            if(!nobj->l_points)
            {
                nobj->l_npoints = 0;
                nobj->l_type = E_GOBJ_INVALID;
                return;
            }
            memcpy(nobj->l_points, g->l_current.e_points, sizeof(t_pt) * (size_t)nobj->l_npoints);
            
            nobj->l_color = g->l_color;
            nobj->l_width = g->l_line_width;
            nobj->l_text  = g->l_current.e_text;
            
            egraphics_apply_matrix(g, nobj);
            if(!preserved)
            {
                g->l_current.e_npoints   = 0;
                g->l_current.e_type     = E_GOBJ_INVALID;
            }
        }
    }
    else
    {
        g->l_number_objects = 0;
    }
}

void egraphics_fill_preserve(t_elayer *g)
{
    egraphics_paint(g, 1, 1);
}

void egraphics_fill(t_elayer *g)
{
    egraphics_paint(g, 1, 0);
}

void egraphics_stroke_preserve(t_elayer *g)
{
    egraphics_paint(g, 0, 1);
}

void egraphics_stroke(t_elayer *g)
{
    egraphics_paint(g, 0, 0);
}

void etext_layout_draw(t_etext* textlayout, t_elayer *g)
{
    t_egobj *temp;
    long index;
    temp = (t_egobj *)realloc(g->l_objects, (size_t)(g->l_number_objects + 1) * sizeof(t_egobj));
    if(temp)
    {
        g->l_objects = temp;
        index = g->l_number_objects;
        g->l_number_objects++;
        
        g->l_objects[index].e_type      = E_GOBJ_TEXT;
        g->l_objects[index].e_text      = malloc(MAXPDSTRING * sizeof(char));
        if(g->l_objects[index].e_text)
        {
            g->l_objects[index].e_npoints   = 2;
            g->l_objects[index].e_points    = (t_pt*)malloc(2 * sizeof(t_pt));
            if(g->l_objects[index].e_points)
            {
                memcpy(g->l_objects[index].e_text, textlayout->c_text, MAXPDSTRING * sizeof(char));
                g->l_objects[index].e_points[0].x = textlayout->c_rect.x;
                g->l_objects[index].e_points[0].y = textlayout->c_rect.y;
                g->l_objects[index].e_points[1].x = textlayout->c_rect.width;
                g->l_objects[index].e_points[1].y = textlayout->c_rect.height;
                g->l_objects[index].e_color       = textlayout->c_color;
                
                g->l_objects[index].e_font        = textlayout->c_font;
                g->l_objects[index].e_justify     = textlayout->c_justify;
                g->l_objects[index].e_wrap        = textlayout->c_wrap;
            }
            else
            {
                free(g->l_objects[index].e_text);
                g->l_objects[index].e_text = NULL;
                g->l_objects[index].e_npoints = 0;
                g->l_objects[index].e_type = E_GOBJ_INVALID;
            }
        }
        else
        {
            g->l_objects[index].e_text = NULL;
            g->l_objects[index].e_npoints = 0;
            g->l_objects[index].e_type = E_GOBJ_INVALID;
        }
    }
}

static void egraphics_prealloc(t_elayer *g, const size_t size)
{
    t_pt* temp;
    t_egobj* obj = &g->l_current;
    if(size > (size_t)(obj->l_rspace - obj->l_npoints))
    {
        if(obj->l_rspace && obj->l_npoints)
        {
            temp = (t_pt *)realloc(obj->l_points, ((size_t)obj->l_npoints + size) * sizeof(t_pt));
            if(temp)
            {
                obj->l_points = temp;
                obj->l_rspace = obj->l_npoints + (int)size;
            }
            else
            {
                free(obj->l_points);
                obj->l_points = NULL;
                obj->l_rspace = 0;
            }
        }
        else
        {
            obj->l_points = (t_pt *)realloc(obj->l_points, size * sizeof(t_pt));
            if(!obj->l_points)
            {
                obj->l_rspace = 0;
            }
            else
            {
                obj->l_rspace = (int)size;
            }
        }
    }
}

void egraphics_move_to(t_elayer *g, float x, float y)
{
    if(g->l_state == EGRAPHICS_OPEN)
    {
        egraphics_prealloc(g,, 2);
        if(g->l_current.e_points)
        {
            g->l_current.e_type = E_GOBJ_PATH;
            g->l_current.e_points[g->l_current.e_npoints].x  = E_PATH_MOVE;
            g->l_current.e_points[g->l_current.e_npoints+1].x  = x;
            g->l_current.e_points[g->l_current.e_npoints+1].y  = y;
            g->l_current.e_npoints += 2;
        }
        else
        {
            g->l_current.e_type = E_GOBJ_INVALID;
        }
    }
}

void egraphics_line_to(t_elayer *g, float x, float y)
{
    if(g->l_state == EGRAPHICS_OPEN)
    {
        egraphics_prealloc(g,, 2);
        if(g->l_current.e_points)
        {
            g->l_current.e_type = E_GOBJ_PATH;
            g->l_current.e_points[g->l_current.e_npoints].x  = E_PATH_LINE;
            g->l_current.e_points[g->l_current.e_npoints+1].x  = x;
            g->l_current.e_points[g->l_current.e_npoints+1].y  = y;
            g->l_current.e_npoints += 2;
        }
        else
        {
            g->l_current.e_type = E_GOBJ_INVALID;
        }
    }
}

void egraphics_curve_to(t_elayer *g, float ctrl1x, float ctrl1y, float ctrl2x, float ctrl2y, float endx, float endy)
{
    if(g->l_state == EGRAPHICS_OPEN)
    {
        egraphics_prealloc(g,, 4);
        if(g->l_current.e_points)
        {
            g->l_current.e_type = E_GOBJ_PATH;
            g->l_current.e_points[g->l_current.e_npoints].x  = E_PATH_CURVE;
            g->l_current.e_points[g->l_current.e_npoints+1].x  = ctrl1x;
            g->l_current.e_points[g->l_current.e_npoints+1].y  = ctrl1y;
            g->l_current.e_points[g->l_current.e_npoints+2].x  = ctrl2x;
            g->l_current.e_points[g->l_current.e_npoints+2].y  = ctrl2y;
            g->l_current.e_points[g->l_current.e_npoints+3].x  = endx;
            g->l_current.e_points[g->l_current.e_npoints+3].y  = endy;
            g->l_current.e_npoints += 4;
        }
        else
        {
            g->l_current.e_type = E_GOBJ_INVALID;
        }
    }
}

void egraphics_close_path(t_elayer *g)
{
    if(g->l_state == EGRAPHICS_OPEN && g->l_current.e_npoints >= 1)
    {
        if(g->l_current.e_type == E_GOBJ_PATH)
        {
            egraphics_line_to(g, g->l_current.e_points[1].x, g->l_current.e_points[1].y);
        }
    }
}

void egraphics_line(t_elayer *g, float x0, float y0, float x1, float y1)
{
    if(g->l_state == EGRAPHICS_OPEN)
    {
        egraphics_prealloc(g,, 4);
        egraphics_move_to(g, x0, y0);
        egraphics_line_to(g, x1, y1);
    }
}

void egraphics_line_fast(t_elayer *g, float x0, float y0, float x1, float y1)
{
    if(g->l_state == EGRAPHICS_OPEN)
    {
        egraphics_prealloc(g,, 4);
        egraphics_move_to(g, x0, y0);
        egraphics_line_to(g, x1, y1);
        egraphics_stroke(g);
    }
}

void egraphics_curve(t_elayer *g, float startx, float starty, float ctrl1x, float ctrl1y, float ctrl2x, float ctrl2y, float endx, float endy)
{
    if(g->l_state == EGRAPHICS_OPEN)
    {
        egraphics_prealloc(g,, 6);
        egraphics_move_to(g, startx, starty);
        egraphics_curve_to(g, ctrl1x, ctrl1y, ctrl2x, ctrl2y, endx, endy);
    }
}

void egraphics_rectangle(t_elayer *g, float x, float y, float width, float height)
{
    if(g->l_state == EGRAPHICS_OPEN)
    {
        egraphics_prealloc(g,, 10);
        egraphics_move_to(g, x, y);
        egraphics_line_to(g, x + width, y);
        egraphics_line_to(g, x + width, y + height);
        egraphics_line_to(g, x, y + height);
        if(g->l_line_width > 1.)
        {
            if(height < 0.f)
                egraphics_line_to(g, x, y + g->l_line_width * 0.5f);
            else
                egraphics_line_to(g, x, y - g->l_line_width * 0.5f);
        }
        else
        {
            egraphics_line_to(g, x, y);
        }
    }
}

void egraphics_rectangle_rounded(t_elayer *g, float x, float y, float width, float height, float roundness)
{
    if(g->l_state == EGRAPHICS_OPEN)
    {
        int todo;
        if(g->l_current.e_points == NULL)
            g->l_current.e_points   = (t_pt *)malloc(9 * sizeof(t_pt));
        else
            g->l_current.e_points   = (t_pt *)realloc(g->l_current.e_points , 9 * sizeof(t_pt));
        if(g->l_current.e_points)
        {
            g->l_current.e_type         = E_GOBJ_RECT;
            g->l_current.e_points[0].x  = x + roundness;
            g->l_current.e_points[0].y  = y;
            g->l_current.e_points[1].x  = x + width - roundness;
            g->l_current.e_points[1].y  = y;
            
            g->l_current.e_points[2].x  = x + width;
            g->l_current.e_points[2].y  = y + roundness;
            g->l_current.e_points[3].x  = x + width;
            g->l_current.e_points[3].y  = y + height - roundness;
            
            g->l_current.e_points[4].x  = x + width - roundness;
            g->l_current.e_points[4].y  = y + height;
            g->l_current.e_points[5].x  = x + roundness;
            g->l_current.e_points[5].y  = y + height;
            
            g->l_current.e_points[6].x  = x;
            g->l_current.e_points[6].y  = y + height - roundness;
            g->l_current.e_points[7].x  = x;
            g->l_current.e_points[7].y  = y + roundness;
            g->l_current.e_points[8].x  = x + roundness;
            g->l_current.e_points[8].y  = y;
            g->l_current.e_npoints      = 9;
        }
        else
        {
            g->l_current.e_type         = E_GOBJ_INVALID;
        }
    }
}

void egraphics_oval(t_elayer *g, float xc, float yc, float radiusx, float radiusy)
{
    if(g->l_state == EGRAPHICS_OPEN)
    {
        egraphics_prealloc(g,, 4);
        if(g->l_current.e_points)
        {
            
            g->l_current.e_type         = E_GOBJ_OVAL;
            g->l_current.e_points[0].x  = xc - radiusx;
            g->l_current.e_points[0].y  = yc - radiusy;
            g->l_current.e_points[1].x  = xc + radiusx;
            g->l_current.e_points[1].y  = yc + radiusy;
            g->l_current.e_npoints      = 2;
        }
        else
        {
            g->l_current.e_type         = E_GOBJ_INVALID;
        }
    }
}

void egraphics_circle(t_elayer *g, float xc, float yc, float radius)
{
    egraphics_oval(g, xc, yc, radius, radius);
}


static void rotate(const float cosz, const float sinz, t_pt* p1)
{
    const float rx = p1->x * cosz - p1->y * sinz;
    p1->y = (p1->x * sinz + p1->y * cosz);
    p1->x = rx;
}

static void create_small_arc(const float r, const float start, const float extend, t_pt ct, t_pt* p2, t_pt* p3, t_pt* p4)
{
    const float a = extend;
    const float cosz = cosf(a * 0.5f + start);
    const float sinz = sinf(a * 0.5f + start);
    t_pt p1;
    p4->x = (float)(r * cosf(a * 0.5f));
    p4->y = (float)(r * sinf(a * 0.5f));
    p1.x = p4->x;
    p1.y = -p4->y;
    p2->x = (float)(p1.x + k * tan(a * 0.5f) * p4->y);
    p2->y = (float)(p1.y + k * tan(a * 0.5f) * p4->x);
    p3->x = p2->x;
    p3->y = -p2->y;
    
    rotate(cosz, sinz, p2); rotate(cosz, sinz, p3); rotate(cosz, sinz, p4);
    p2->x += ct.x; p2->y += ct.y; p3->x += ct.x; p3->y += ct.y; p4->x += ct.x; p4->y += ct.y;
}

void egraphics_arc_to(t_elayer *g, float cx, float cy, float extend)
{
    if(g->l_state == EGRAPHICS_OPEN)
    {
        if(g->l_current.e_type == E_GOBJ_PATH && g->l_current.e_points)
        {
            t_pt p2, p3, p4, c = {cx, cy}, prev = g->l_current.e_points[g->l_current.e_npoints-1];
            float radius   = pd_radius(prev.x - cx, prev.y - cy);
            float angle    = pd_angle(prev.x - cx, prev.y - cy);
            
            while(extend > EPD_2PI)
            {
                extend -= EPD_2PI;
            }
            while(extend < -EPD_2PI)
            {
                extend += EPD_2PI;
            }
            
            while(fabsf(extend) >= EPD_PI4)
            {
                if(extend < 0.)
                {
                    create_small_arc(radius, angle, -EPD_PI4, c, &p2, &p3, &p4);
                    extend += EPD_PI4;
                    angle  -= EPD_PI4;
                }
                else
                {
                    create_small_arc(radius, angle, EPD_PI4, c, &p2, &p3, &p4);
                    extend -= EPD_PI4;
                    angle  += EPD_PI4;
                }
                egraphics_curve_to(g, p2.x, p2.y, p3.x, p3.y,  p4.x, p4.y);
            }
            if(fabsf(extend) > 1e-6)
            {
                create_small_arc(radius, angle, extend, c, &p2, &p3, &p4);
                egraphics_curve_to(g, p2.x, p2.y, p3.x, p3.y,  p4.x, p4.y);
            }
        }
        else
        {
            g->l_current.e_type = E_GOBJ_INVALID;
        }
    }
}

static void create_small_arc_oval(const float r1, const float r2, const float start, const float extend, t_pt ct, t_pt* p2, t_pt* p3, t_pt* p4)
{
    t_pt p1;
    
    const float a = extend;
    const float cosz = cosf(a * 0.5f + start);
    const float sinz = sinf(a * 0.5f + start);
    const float cosa = cosf(a * 0.5f);
    const float sina = sinf(a * 0.5f);
	const float k1 = (4.f * (1.f - cosf(a * 0.5f)) / sinf(a * 0.5f)) / 3.f;
	const float k2 = (4.f * (1.f - cosf(-a * 0.5f)) / sinf(-a * 0.5f)) / 3.f;
    p4->x = (float)(r2 * cosa);
    p4->y = (float)(r2 * sina);
    p1.x = (float)(r1 * cosa);
    p1.y = (float)(-r1 * sina);
    p2->x = (float)(p1.x + k1 * p4->y);
    p2->y = (float)(p1.y + k1 * p4->x);
    p3->x = (float)(p4->x + k2 * p1.y);
    p3->y = (float)(p4->y + k2 * p1.y);
    
    rotate(cosz, sinz, p2); rotate(cosz, sinz, p3); rotate(cosz, sinz, p4);
    p2->x += ct.x; p2->y += ct.y; p3->x += ct.x; p3->y += ct.y; p4->x += ct.x; p4->y += ct.y;
}

void egraphics_arc_oval_to(t_elayer *g, float cx, float cy, float radius, float extend)
{
    if(g->l_state == EGRAPHICS_OPEN)
    {
        if(g->l_current.e_type == E_GOBJ_PATH && g->l_current.e_points)
        {
            t_pt p2, p3, p4, c = {cx, cy}, prev = g->l_current.e_points[g->l_current.e_npoints-1];
            float r1   = pd_radius(prev.x - cx, prev.y - cy);
            float angle = pd_angle(prev.x - cx, prev.y - cy);
            float ratio = (radius - r1) / (fabsf(extend) / EPD_PI4);
            
            while(extend > EPD_2PI)
            {
                extend -= EPD_2PI;
            }
            while(extend < -EPD_2PI)
            {
                extend += EPD_2PI;
            }
            
            while(fabsf(extend) >= EPD_PI4)
            {
                if(extend < 0.)
                {
                    
                    create_small_arc_oval(r1, r1 + ratio, angle, -EPD_PI4, c, &p2, &p3, &p4);
                    extend += EPD_PI4;
                    angle  -= EPD_PI4;
                    r1 += ratio;
                }
                else
                {
                    create_small_arc_oval(r1, r1 + ratio, angle, EPD_PI4, c, &p2, &p3, &p4);
                    extend -= EPD_PI4;
                    angle  += EPD_PI4;
                    r1 += ratio;
                }
                egraphics_curve_to(g, p2.x, p2.y, p3.x, p3.y,  p4.x, p4.y);
            }
            if(fabsf(extend) > 1e-6)
            {
                create_small_arc_oval(r1, radius, angle, extend, c, &p2, &p3, &p4);
                egraphics_curve_to(g, p2.x, p2.y, p3.x, p3.y,  p4.x, p4.y);
            }
        }
        else
        {
            g->l_current.e_type = E_GOBJ_INVALID;
        }
    }
}

void egraphics_arc(t_elayer *g, float xc, float yc, float radius, float angle1, float angle2)
{
    if(g->l_state == EGRAPHICS_OPEN)
    {
        if(g->l_current.e_points == NULL)
            g->l_current.e_points   = (t_pt *)malloc(3 * sizeof(t_pt));
        else
            g->l_current.e_points   = (t_pt *)realloc(g->l_current.e_points , 3 * sizeof(t_pt));
        if(g->l_current.e_points)
        {
            g->l_current.e_type         = E_GOBJ_ARC;
            g->l_current.e_points[0].x  = xc;
            g->l_current.e_points[0].y  = yc;
            g->l_current.e_points[1].x  = xc + (float)pd_abscissa(radius, angle1);
            g->l_current.e_points[1].y  = yc + (float)pd_ordinate(radius, angle1);
            g->l_current.e_points[2].x  = angle2 - angle1;
            g->l_current.e_points[2].y  = radius;
            g->l_current.e_npoints      = 3;
        }
        else
        {
            g->l_current.e_type         = E_GOBJ_INVALID;
        }
    }
}

void egraphics_arc_oval(t_elayer *g, float xc, float yc, float radiusx, float radiusy, float angle1, float angle2)
{
    if(g->l_state == EGRAPHICS_OPEN)
    {
        if(g->l_current.e_points == NULL)
            g->l_current.e_points   = (t_pt *)malloc(3 * sizeof(t_pt));
        else
            g->l_current.e_points   = (t_pt *)realloc(g->l_current.e_points , 3 * sizeof(t_pt));
        if(g->l_current.e_points)
        {
            g->l_current.e_type         = E_GOBJ_ARC;
            g->l_current.e_points[0].x  = xc;
            g->l_current.e_points[0].y  = yc;
            g->l_current.e_points[1].x  = xc + (float)pd_abscissa(radiusx, angle1);
            g->l_current.e_points[1].y  = yc + (float)pd_ordinate(radiusy, angle1);
            g->l_current.e_points[2].x  = angle2 - angle1;
            g->l_current.e_points[2].y  = angle2 - angle1;
            g->l_current.e_npoints      = 3;
        }
        else
        {
            g->l_current.e_type         = E_GOBJ_INVALID;
        }
    }
}

char* rgba_to_hex(t_rgba const* color)
{
    int r = (int)(color->red * 255.f);
    int g = (int)(color->green * 255.f);
    int b = (int)(color->blue * 255.f);
    ColBuf[0] = '#';
    ColBuf[1] = HexDigits[(r >> 4) & 15];
    ColBuf[2] = HexDigits[r & 15];
    ColBuf[3] = HexDigits[(g >> 4) & 15];
    ColBuf[4] = HexDigits[g & 15];
    ColBuf[5] = HexDigits[(b >> 4) & 15];
    ColBuf[6] = HexDigits[b & 15];
    ColBuf[7] = '\0';
    return &ColBuf[0];
}

char* rgb_to_hex(t_rgb const* color)
{
    int r = (int)(color->red * 255.f);
    int g = (int)(color->green * 255.f);
    int b = (int)(color->blue * 255.f);
    ColBuf[0] = '#';
    ColBuf[1] = HexDigits[(r >> 4) & 15];
    ColBuf[2] = HexDigits[r & 15];
    ColBuf[3] = HexDigits[(g >> 4) & 15];
    ColBuf[4] = HexDigits[g & 15];
    ColBuf[5] = HexDigits[(b >> 4) & 15];
    ColBuf[6] = HexDigits[b & 15];
    ColBuf[7] = '\0';
    return &ColBuf[0];
}

char* hsla_to_hex(t_hsla  const* color)
{
    t_rgba ncolor = hsla_to_rgba(color);
    return rgba_to_hex(&ncolor);
}

char* hsl_to_hex(t_hsl  const* color)
{
    t_rgb ncolor = hsl_to_rgb(color);
    return rgb_to_hex(&ncolor);
}

t_rgba hex_to_rgba(char const* color)
{
    int hexvalue = (int)strtol(color+1, NULL, 16);
    t_rgba ncolor;
    ncolor.red = (float)((hexvalue >> 16) & 0xFF) / 255.f;
    ncolor.green = (float)((hexvalue >> 8) & 0xFF) / 255.f;
    ncolor.blue = (float)((hexvalue) & 0xFF) / 255.f;
    ncolor.alpha = 1.f;
    return ncolor;
}

t_rgb hex_to_rgb(char const* color)
{
    int hexvalue = (int)strtol(color+1, NULL, 16);
    t_rgb ncolor;
    ncolor.red = (float)((hexvalue >> 16) & 0xFF) / 255.f;
    ncolor.green = (float)((hexvalue >> 8) & 0xFF) / 255.f;
    ncolor.blue = (float)((hexvalue) & 0xFF) / 255.f;
    return ncolor;
}

t_hsla rgba_to_hsla(t_rgba const* color)
{
    t_hsla ncolor = {0., 0., 0., 0.};
    float delta, deltar, deltag, deltab;
    float max = color->red;
    float min = color->red;
    if(min > color->green)
        min = color->green;
    if(min > color->blue)
        min = color->blue;
    if(max < color->green)
        max = color->green;
    if(max < color->blue)
        max = color->blue;
    delta = max - min;
    ncolor.alpha = color->alpha;
    ncolor.lightness = (max + min) / 2.f;
    if(max == 0)
    {
        ncolor.hue = 0;
        ncolor.saturation = 0;
    }
    else
    {
        if(ncolor.lightness < 0.5f)
            ncolor.saturation = delta / (max + min);
        else
            ncolor.saturation = delta / (2.f - max - min);
        
        deltar = (((max - color->red ) / 6 ) + (delta / 2)) / delta;
        deltag = (((max - color->green ) / 6 ) + (delta / 2)) / delta;
        deltab = (((max - color->blue ) / 6 ) + (delta / 2)) / delta;
        
        if(color->red == max)
            ncolor.hue = deltab - deltag;
        else if(color->green == max)
            ncolor.hue = (1.f / 3.f) + deltar - deltab;
        else if(color->blue == max)
            ncolor.hue = (2.f / 3.f) + deltag - deltar;
        
        if(ncolor.hue < 0.f)
            ncolor.hue += 1;
        if(ncolor.hue > 1.f)
            ncolor.hue -= 1;
    }
    
    return ncolor;
}

t_hsl rgb_to_hsl(t_rgb const* color)
{
    t_hsl ncolor = {0., 0., 0.};
    float delta, deltar, deltag, deltab;
    float max = color->red;
    float min = color->red;
    if(min > color->green)
        min = color->green;
    if(min > color->blue)
        min = color->blue;
    if(max < color->green)
        max = color->green;
    if(max < color->blue)
        max = color->blue;
    delta = max - min;

    ncolor.lightness = (max + min) / 2.f;
    if(max == 0)
    {
        ncolor.hue = 0;
        ncolor.saturation = 0;
    }
    else
    {
        if(ncolor.lightness < 0.5f)
            ncolor.saturation = delta / (max + min);
        else
            ncolor.saturation = delta / (2.f - max - min);
        
        deltar = (((max - color->red ) / 6 ) + (delta / 2)) / delta;
        deltag = (((max - color->green ) / 6 ) + (delta / 2)) / delta;
        deltab = (((max - color->blue ) / 6 ) + (delta / 2)) / delta;
        
        if(color->red == max)
            ncolor.hue = deltab - deltag;
        else if(color->green == max)
            ncolor.hue = (1.f / 3.f) + deltar - deltab;
        else if(color->blue == max)
            ncolor.hue = (2.f / 3.f) + deltag - deltar;
        
        if(ncolor.hue < 0.f)
            ncolor.hue += 1;
        if(ncolor.hue > 1.f)
            ncolor.hue -= 1;
    }
    
    return ncolor;
}

static float Hue_2_RGB(float v1,float v2,float vH)
{
    if(vH < 0.f)
        vH += 1.;
    if(vH > 1.f)
        vH -= 1.;
    if((6. * vH) < 1.f)
        return (v1 + (v2 - v1) * 6.f * vH);
    if((2. * vH) < 1.f)
        return v2;
    if(( 3 * vH) < 2.f)
        return (v1 + (v2 - v1) * ((2.f / 3.f) - vH) * 6.f);
    return v1;
}

t_rgb hsl_to_rgb(t_hsl const* color)
{
    float var1, var2;
    t_rgb ncolor;
    if(color->saturation == 0.f)
    {
        ncolor.red = color->lightness;
        ncolor.green = color->lightness;
        ncolor.blue = color->lightness;
    }
    else
    {
        if(color->lightness < 0.5f)
            var2 = color->lightness * (1.f + color->saturation);
        else
            var2 = (color->lightness + color->saturation) - (color->saturation * color->lightness);
        
        var1 = 2 * color->lightness - var2;
        
        ncolor.red = Hue_2_RGB(var1, var2, color->hue + (1.f / 3.f));
        ncolor.green = Hue_2_RGB(var1, var2, color->hue);
        ncolor.blue = Hue_2_RGB(var1, var2, color->hue - (1.f / 3.f));
    }
    return ncolor;
}

t_rgba hsla_to_rgba(t_hsla const* color)
{
    float var1, var2;
    t_rgba ncolor;
    if(color->saturation == 0.f)
    {
        ncolor.red = color->lightness;
        ncolor.green = color->lightness;
        ncolor.blue = color->lightness;
    }
    else
    {
        if(color->lightness < 0.5f)
            var2 = color->lightness * (1.f + color->saturation);
        else
            var2 = (color->lightness + color->saturation) - (color->saturation * color->lightness);
        
        var1 = 2 * color->lightness - var2;
        
        ncolor.red = Hue_2_RGB(var1, var2, color->hue + (1.f / 3.f));
        ncolor.green = Hue_2_RGB(var1, var2, color->hue);
        ncolor.blue = Hue_2_RGB(var1, var2, color->hue - (1.f / 3.f));
    }
    
    ncolor.alpha = color->alpha;
    return ncolor;
}

t_rgba rgba_addContrast(t_rgba color, float contrast)
{
    t_rgba new_color = color;
    new_color.red = (float)pd_clip(new_color.red += contrast, 0., 1.);
    new_color.green = (float)pd_clip(new_color.green += contrast, 0., 1.);
    new_color.blue = (float)pd_clip(new_color.blue += contrast, 0., 1.);
    return new_color;
}

t_rgb rgb_addContrast(t_rgb color, float contrast)
{
    t_rgb new_color = color;
    new_color.red = (float)pd_clip(new_color.red += contrast, 0., 1.);
    new_color.green = (float)pd_clip(new_color.green += contrast, 0., 1.);
    new_color.blue = (float)pd_clip(new_color.blue += contrast, 0., 1.);
    return new_color;
}

void rgba_set(t_rgba *color, float red, float green, float blue, float alpha)
{
    color->red = red;
    color->green = green;
    color->blue = blue;
    color->alpha = alpha;
}

void rgb_set(t_rgb *color, float red, float green, float blue)
{
    color->red = red;
    color->green = green;
    color->blue = blue;
}

void hsla_set(t_hsla *color, float hue, float saturation, float lightness, float alpha)
{
    color->hue = hue;
    color->saturation = saturation;
    color->lightness = lightness;
    color->alpha = alpha;
}

void hsl_set(t_hsl *color, float hue, float saturation, float lightness)
{
    color->hue = hue;
    color->saturation = saturation;
    color->lightness = lightness;
}

char rgba_is_equal(t_rgba const* color, t_rgba const* other)
{
    return color->red == other->red && color->green == other->green &&
    color->blue == other->blue && color->alpha == other->alpha;
}

void egraphics_matrix_init(t_matrix *x, float xx, float yx, float xy, float yy, float x0, float y0)
{
    x->xx = xx;
    x->yx = yx;
    x->xy = xy;
    x->yy = yy;
    x->x0 = x0;
    x->y0 = y0;
}

void egraphics_set_matrix(t_elayer *g, const t_matrix* matrix)
{
    egraphics_matrix_init(&g->l_matrix, matrix->xx, matrix->yx, matrix->xy, matrix->yy, matrix->x0, matrix->y0);
}

void egraphics_rotate(t_elayer *g, float angle)
{
    const float cosRad = cosf(angle);
    const float sinRad = sinf(angle);
    t_matrix temp = g->l_matrix;
    
    g->l_matrix.xx = temp.xx * cosRad - temp.yx * sinRad;
    g->l_matrix.yx = temp.xx * sinRad + temp.yx * cosRad;
    g->l_matrix.xy = temp.xy * cosRad - temp.yy * sinRad;
    g->l_matrix.yy = temp.xy * sinRad + temp.yy * cosRad;
}


static void egraphics_apply_matrix(t_elayer *g, t_egobj* gobj)
{
    int i;
    float x_p, y_p;
    if(gobj->l_type == E_GOBJ_ARC)
    {
        for(i = 0; i < 2; i++)
        {
            x_p     = gobj->l_points[i].x * g->l_matrix.xx + gobj->l_points[i].y * g->l_matrix.xy + g->l_matrix.x0;
            y_p     = gobj->l_points[i].x * g->l_matrix.yx + gobj->l_points[i].y * g->l_matrix.yy + g->l_matrix.y0;
            gobj->l_points[i].x    = x_p;
            gobj->l_points[i].y    = y_p;
        }
    }
    else if(gobj->l_type == E_GOBJ_PATH)
    {
        for(i = 0; i < gobj->l_npoints; )
        {
            if(gobj->l_points[i].x == E_PATH_MOVE || gobj->l_points[i].x == E_PATH_LINE)
            {
                x_p     = gobj->l_points[i+1].x * g->l_matrix.xx + gobj->l_points[i+1].y * g->l_matrix.xy + g->l_matrix.x0;
                y_p     = gobj->l_points[i+1].x * g->l_matrix.yx + gobj->l_points[i+1].y * g->l_matrix.yy + g->l_matrix.y0;
                gobj->l_points[i+1].x    = x_p;
                gobj->l_points[i+1].y    = y_p;
                i += 2;
            }
            else if(gobj->l_points[i].x == E_PATH_CURVE)
            {
                x_p     = gobj->l_points[i+1].x * g->l_matrix.xx + gobj->l_points[i+1].y * g->l_matrix.xy + g->l_matrix.x0;
                y_p     = gobj->l_points[i+1].x * g->l_matrix.yx + gobj->l_points[i+1].y * g->l_matrix.yy + g->l_matrix.y0;
                gobj->l_points[i+1].x    = x_p;
                gobj->l_points[i+1].y    = y_p;
                x_p     = gobj->l_points[i+2].x * g->l_matrix.xx + gobj->l_points[i+2].y * g->l_matrix.xy + g->l_matrix.x0;
                y_p     = gobj->l_points[i+2].x * g->l_matrix.yx + gobj->l_points[i+2].y * g->l_matrix.yy + g->l_matrix.y0;
                gobj->l_points[i+2].x    = x_p;
                gobj->l_points[i+2].y    = y_p;
                x_p     = gobj->l_points[i+3].x * g->l_matrix.xx + gobj->l_points[i+3].y * g->l_matrix.xy + g->l_matrix.x0;
                y_p     = gobj->l_points[i+3].x * g->l_matrix.yx + gobj->l_points[i+3].y * g->l_matrix.yy + g->l_matrix.y0;
                gobj->l_points[i+3].x    = x_p;
                gobj->l_points[i+3].y    = y_p;
                i += 3;
            }
            else
            {
                i++;
            }
        }
    }
    else
    {
        for(i = 0; i < gobj->l_npoints; i++)
        {
            x_p     = gobj->l_points[i].x * g->l_matrix.xx + gobj->l_points[i].y * g->l_matrix.xy + g->l_matrix.x0;
            y_p     = gobj->l_points[i].x * g->l_matrix.yx + gobj->l_points[i].y * g->l_matrix.yy + g->l_matrix.y0;
            gobj->l_points[i].x    = x_p;
            gobj->l_points[i].y    = y_p;
        }
    }
}

t_etext* etext_layout_create(void)
{
    t_etext* new_text_layout = (t_etext *)malloc(sizeof(t_etext));
    if(new_text_layout)
    {
        new_text_layout->c_text = (char *)malloc(MAXPDSTRING * sizeof(char));
        if(new_text_layout->c_text)
        {
            memset(new_text_layout->c_text, 0, MAXPDSTRING * sizeof(char));
            new_text_layout->c_color.red = 0.;
            new_text_layout->c_color.green = 0.;
            new_text_layout->c_color.blue = 0.;
            new_text_layout->c_color.alpha = 1.;
            return new_text_layout;
        }
    }
    return NULL;
}

void etext_layout_destroy(t_etext* textlayout)
{
    free(textlayout->c_text);
    free(textlayout);
}

void etext_layout_set(t_etext* textlayout, const char* text, t_efont *font,  float x, float y, float width,  float height, etextjustify_flags justify, etextwrap_flags wrap)
{
    strncpy(textlayout->c_text, text, MAXPDSTRING);
    textlayout->c_font = font[0];
    textlayout->c_rect.x = (float)x;
    textlayout->c_rect.y = (float)y;
    textlayout->c_rect.width = (float)width;
    textlayout->c_rect.height = (float)height;
    textlayout->c_wrap = wrap;
    textlayout->c_justify = justify;
}

void etext_layout_settextcolor(t_etext* textlayout, t_rgba* color)
{
    textlayout->c_color = color[0];
}

void efont_init(t_efont* font, t_symbol* family, char bold, char italic, float size)
{
    font->family = family;
    font->slant = italic ? gensym("italic") : gensym("roman");
    font->weight = bold ? gensym("bold") : gensym("normal");
    font->size = pd_clip_min(size, 1.f);
}

float pd_wrap(float f, const float min, const float max)
{
    if(min < max)
    {
        const float ratio = max - min;
        while(f < min){f += ratio;}
        while(f > max){f -= ratio;}
        return f;
    }
    else
    {
        const float ratio = min - max;
        while(f < max){f += ratio;}
        while(f > min){f -= ratio;}
        return f;
    }
}

float pd_clip(const float f, const float min, const float max)
{
    return (f < min) min : ((f > max) ? max : f);
}

float pd_clip_min(const float f, const float min)
{
    return (f < min) min : f;
}

float pd_clip_max(const float f, const float max)
{
    return (f > max) max : f;
}

float pd_ordinate(const float radius, const float angle)
{
    return radius * sinf(angle);
}

float pd_abscissa(const float radius, const float angle)
{
    return radius * cosf(angle);
}

float pd_radius(const float x, const float y)
{
    return sqrtf(x*x + y*y);
}

float pd_angle(const float x, const float y)
{
    return atan2f(y, x);
}



