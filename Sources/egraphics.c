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

static void egraphics_apply_matrix(t_elayer *g, t_egobj* gobj);

void egraphics_set_line_width(t_elayer *g, float width)
{
    g->e_line_width= (int)pd_clip_min(width, 0.);
}

void egraphics_set_color_rgba(t_elayer *g, const t_rgba *rgba)
{
    memcpy(&g->e_color, rgba, sizeof(t_rgba));
}

void egraphics_set_color_rgb(t_elayer *g, const t_rgb *rgb)
{
    memcpy(&g->e_color, rgb, sizeof(t_rgb));
    g->e_color.alpha = 1.f;
}

void egraphics_set_color_hex(t_elayer *g, t_symbol *hex)
{
    g->e_color = hex_to_rgba(hex->s_name);
}

void egraphics_set_color_hsla(t_elayer *g, const t_hsla *hsla)
{
    g->e_color = hsla_to_rgba(hsla);
}

void egraphics_set_color_hsl(t_elayer *g, const t_hsl *hsl)
{
    const t_rgb color = hsl_to_rgb(hsl);
    memcpy(&g->e_color, &color, sizeof(t_rgb));
}

static void egraphics_paint(t_elayer *g, int filled, int preserved)
{
    t_egobj *nobj, *temp;
    if(g->e_new_objects.e_type != E_GOBJ_INVALID)
    {
        temp = (t_egobj *)realloc(g->e_objects, (size_t)(g->e_number_objects + 1) * sizeof(t_egobj));
        if(temp)
        {
            g->e_objects = temp;
            nobj = g->e_objects + g->e_number_objects;
            g->e_number_objects++;
            if (filled)
            {
                nobj->e_filled = 1;
            }
            else
            {
                nobj->e_filled = 0;
            }
            nobj->e_type      = g->e_new_objects.e_type;
            nobj->e_npoints   = g->e_new_objects.e_npoints;
            nobj->e_points    = (t_pt*)malloc((size_t)nobj->e_npoints * sizeof(t_pt));
            nobj->e_text      = NULL;
            if(!nobj->e_points)
            {
                nobj->e_npoints = 0;
                nobj->e_type = E_GOBJ_INVALID;
                return;
            }
            memcpy(nobj->e_points, g->e_new_objects.e_points, sizeof(t_pt) * (size_t)nobj->e_npoints);
            
            nobj->e_color = g->e_color;
            nobj->e_width = g->e_line_width;
            nobj->e_text  = g->e_new_objects.e_text;
            
            egraphics_apply_matrix(g, nobj);
            if(!preserved)
            {
                g->e_new_objects.e_npoints   = 0;
                g->e_new_objects.e_type     = E_GOBJ_INVALID;
            }
        }
    }
    else
    {
        g->e_number_objects = 0;
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
    temp = (t_egobj *)realloc(g->e_objects, (size_t)(g->e_number_objects + 1) * sizeof(t_egobj));
    if(temp)
    {
        g->e_objects = temp;
        index = g->e_number_objects;
        g->e_number_objects++;
        
        g->e_objects[index].e_type      = E_GOBJ_TEXT;
        g->e_objects[index].e_text      = malloc(MAXPDSTRING * sizeof(char));
        if(g->e_objects[index].e_text)
        {
            g->e_objects[index].e_npoints   = 2;
            g->e_objects[index].e_points    = (t_pt*)malloc(2 * sizeof(t_pt));
            if(g->e_objects[index].e_points)
            {
                memcpy(g->e_objects[index].e_text, textlayout->c_text, MAXPDSTRING * sizeof(char));
                g->e_objects[index].e_points[0].x = textlayout->c_rect.x;
                g->e_objects[index].e_points[0].y = textlayout->c_rect.y;
                g->e_objects[index].e_points[1].x = textlayout->c_rect.width;
                g->e_objects[index].e_points[1].y = textlayout->c_rect.height;
                g->e_objects[index].e_color       = textlayout->c_color;
                
                g->e_objects[index].e_font        = textlayout->c_font;
                g->e_objects[index].e_justify     = textlayout->c_justify;
            }
            else
            {
                free(g->e_objects[index].e_text);
                g->e_objects[index].e_text = NULL;
                g->e_objects[index].e_npoints = 0;
                g->e_objects[index].e_type = E_GOBJ_INVALID;
            }
        }
        else
        {
            g->e_objects[index].e_text = NULL;
            g->e_objects[index].e_npoints = 0;
            g->e_objects[index].e_type = E_GOBJ_INVALID;
        }
    }
}

static void egraphics_gobj_prealloc(t_egobj* obj, const size_t size)
{
    t_pt* temp;
    if(size > (size_t)(obj->e_rspace - obj->e_npoints))
    {
        if(obj->e_rspace && obj->e_npoints)
        {
            temp = (t_pt *)realloc(obj->e_points, ((size_t)obj->e_npoints + size) * sizeof(t_pt));
            if(temp)
            {
                obj->e_points = temp;
                obj->e_rspace = obj->e_npoints + (int)size;
            }
            else
            {
                free(obj->e_points);
                obj->e_points = NULL;
                obj->e_rspace = 0;
            }
        }
        else
        {
            obj->e_points = (t_pt *)realloc(obj->e_points, size * sizeof(t_pt));
            if(!obj->e_points)
            {
                obj->e_rspace = 0;
            }
            else
            {
                obj->e_rspace = (int)size;
            }
        }
    }
}

void egraphics_move_to(t_elayer *g, float x, float y)
{
    if(g->e_state == EGRAPHICS_OPEN)
    {
        egraphics_gobj_prealloc(&g->e_new_objects, 2);
        if(g->e_new_objects.e_points)
        {
            g->e_new_objects.e_type = E_GOBJ_PATH;
            g->e_new_objects.e_points[g->e_new_objects.e_npoints].x  = E_PATH_MOVE;
            g->e_new_objects.e_points[g->e_new_objects.e_npoints+1].x  = x;
            g->e_new_objects.e_points[g->e_new_objects.e_npoints+1].y  = y;
            g->e_new_objects.e_npoints += 2;
        }
        else
        {
            g->e_new_objects.e_type = E_GOBJ_INVALID;
        }
    }
}

void egraphics_line_to(t_elayer *g, float x, float y)
{
    if(g->e_state == EGRAPHICS_OPEN)
    {
        egraphics_gobj_prealloc(&g->e_new_objects, 2);
        if(g->e_new_objects.e_points)
        {
            g->e_new_objects.e_type = E_GOBJ_PATH;
            g->e_new_objects.e_points[g->e_new_objects.e_npoints].x  = E_PATH_LINE;
            g->e_new_objects.e_points[g->e_new_objects.e_npoints+1].x  = x;
            g->e_new_objects.e_points[g->e_new_objects.e_npoints+1].y  = y;
            g->e_new_objects.e_npoints += 2;
        }
        else
        {
            g->e_new_objects.e_type = E_GOBJ_INVALID;
        }
    }
}

void egraphics_curve_to(t_elayer *g, float ctrl1x, float ctrl1y, float ctrl2x, float ctrl2y, float endx, float endy)
{
    if(g->e_state == EGRAPHICS_OPEN)
    {
        egraphics_gobj_prealloc(&g->e_new_objects, 4);
        if(g->e_new_objects.e_points)
        {
            g->e_new_objects.e_type = E_GOBJ_PATH;
            g->e_new_objects.e_points[g->e_new_objects.e_npoints].x  = E_PATH_CURVE;
            g->e_new_objects.e_points[g->e_new_objects.e_npoints+1].x  = ctrl1x;
            g->e_new_objects.e_points[g->e_new_objects.e_npoints+1].y  = ctrl1y;
            g->e_new_objects.e_points[g->e_new_objects.e_npoints+2].x  = ctrl2x;
            g->e_new_objects.e_points[g->e_new_objects.e_npoints+2].y  = ctrl2y;
            g->e_new_objects.e_points[g->e_new_objects.e_npoints+3].x  = endx;
            g->e_new_objects.e_points[g->e_new_objects.e_npoints+3].y  = endy;
            g->e_new_objects.e_npoints += 4;
        }
        else
        {
            g->e_new_objects.e_type = E_GOBJ_INVALID;
        }
    }
}

void egraphics_close_path(t_elayer *g)
{
    if(g->e_state == EGRAPHICS_OPEN && g->e_new_objects.e_npoints >= 1)
    {
        if(g->e_new_objects.e_type == E_GOBJ_PATH)
        {
            egraphics_line_to(g, g->e_new_objects.e_points[1].x, g->e_new_objects.e_points[1].y);
        }
    }
}

void egraphics_line(t_elayer *g, float x0, float y0, float x1, float y1)
{
    if(g->e_state == EGRAPHICS_OPEN)
    {
        egraphics_gobj_prealloc(&g->e_new_objects, 4);
        egraphics_move_to(g, x0, y0);
        egraphics_line_to(g, x1, y1);
    }
}

void egraphics_line_fast(t_elayer *g, float x0, float y0, float x1, float y1)
{
    if(g->e_state == EGRAPHICS_OPEN)
    {
        egraphics_gobj_prealloc(&g->e_new_objects, 4);
        egraphics_move_to(g, x0, y0);
        egraphics_line_to(g, x1, y1);
        egraphics_stroke(g);
    }
}

void egraphics_curve(t_elayer *g, float startx, float starty, float ctrl1x, float ctrl1y, float ctrl2x, float ctrl2y, float endx, float endy)
{
    if(g->e_state == EGRAPHICS_OPEN)
    {
        egraphics_gobj_prealloc(&g->e_new_objects, 6);
        egraphics_move_to(g, startx, starty);
        egraphics_curve_to(g, ctrl1x, ctrl1y, ctrl2x, ctrl2y, endx, endy);
    }
}

void egraphics_rectangle(t_elayer *g, float x, float y, float width, float height)
{
    if(g->e_state == EGRAPHICS_OPEN)
    {
        egraphics_gobj_prealloc(&g->e_new_objects, 10);
        egraphics_move_to(g, x, y);
        egraphics_line_to(g, x + width, y);
        egraphics_line_to(g, x + width, y + height);
        egraphics_line_to(g, x, y + height);
        if(g->e_line_width > 1.)
        {
            egraphics_line_to(g, x, y - g->e_line_width * 0.5f);
        }
        else
        {
            egraphics_line_to(g, x, y);
        }
    }
}

void egraphics_rectangle_rounded(t_elayer *g, float x, float y, float width, float height, float roundness)
{
    if(g->e_state == EGRAPHICS_OPEN)
    {
        int todo;
        if(g->e_new_objects.e_points == NULL)
            g->e_new_objects.e_points   = (t_pt *)malloc(9 * sizeof(t_pt));
        else
            g->e_new_objects.e_points   = (t_pt *)realloc(g->e_new_objects.e_points , 9 * sizeof(t_pt));
        if(g->e_new_objects.e_points)
        {
            g->e_new_objects.e_type         = E_GOBJ_RECT;
            g->e_new_objects.e_points[0].x  = x + roundness;
            g->e_new_objects.e_points[0].y  = y;
            g->e_new_objects.e_points[1].x  = x + width - roundness;
            g->e_new_objects.e_points[1].y  = y;
            
            g->e_new_objects.e_points[2].x  = x + width;
            g->e_new_objects.e_points[2].y  = y + roundness;
            g->e_new_objects.e_points[3].x  = x + width;
            g->e_new_objects.e_points[3].y  = y + height - roundness;
            
            g->e_new_objects.e_points[4].x  = x + width - roundness;
            g->e_new_objects.e_points[4].y  = y + height;
            g->e_new_objects.e_points[5].x  = x + roundness;
            g->e_new_objects.e_points[5].y  = y + height;
            
            g->e_new_objects.e_points[6].x  = x;
            g->e_new_objects.e_points[6].y  = y + height - roundness;
            g->e_new_objects.e_points[7].x  = x;
            g->e_new_objects.e_points[7].y  = y + roundness;
            g->e_new_objects.e_points[8].x  = x + roundness;
            g->e_new_objects.e_points[8].y  = y;
            g->e_new_objects.e_npoints      = 9;
        }
        else
        {
            g->e_new_objects.e_type         = E_GOBJ_INVALID;
        }
    }
}

void egraphics_oval(t_elayer *g, float xc, float yc, float radiusx, float radiusy)
{
    if(g->e_state == EGRAPHICS_OPEN)
    {
        egraphics_gobj_prealloc(&g->e_new_objects, 4);
        if(g->e_new_objects.e_points)
        {
            
            g->e_new_objects.e_type         = E_GOBJ_OVAL;
            g->e_new_objects.e_points[0].x  = xc - radiusx;
            g->e_new_objects.e_points[0].y  = yc - radiusy;
            g->e_new_objects.e_points[1].x  = xc + radiusx;
            g->e_new_objects.e_points[1].y  = yc + radiusy;
            g->e_new_objects.e_npoints      = 2;
        }
        else
        {
            g->e_new_objects.e_type         = E_GOBJ_INVALID;
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
    if(g->e_state == EGRAPHICS_OPEN)
    {
        if(g->e_new_objects.e_type == E_GOBJ_PATH && g->e_new_objects.e_points)
        {
            t_pt p2, p3, p4, c = {cx, cy}, prev = g->e_new_objects.e_points[g->e_new_objects.e_npoints-1];
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
            g->e_new_objects.e_type = E_GOBJ_INVALID;
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
    if(g->e_state == EGRAPHICS_OPEN)
    {
        if(g->e_new_objects.e_type == E_GOBJ_PATH && g->e_new_objects.e_points)
        {
            t_pt p2, p3, p4, c = {cx, cy}, prev = g->e_new_objects.e_points[g->e_new_objects.e_npoints-1];
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
            g->e_new_objects.e_type = E_GOBJ_INVALID;
        }
    }
}

void egraphics_arc(t_elayer *g, float xc, float yc, float radius, float angle1, float angle2)
{
    if(g->e_state == EGRAPHICS_OPEN)
    {
        if(g->e_new_objects.e_points == NULL)
            g->e_new_objects.e_points   = (t_pt *)malloc(3 * sizeof(t_pt));
        else
            g->e_new_objects.e_points   = (t_pt *)realloc(g->e_new_objects.e_points , 3 * sizeof(t_pt));
        if(g->e_new_objects.e_points)
        {
            g->e_new_objects.e_type         = E_GOBJ_ARC;
            g->e_new_objects.e_points[0].x  = xc;
            g->e_new_objects.e_points[0].y  = yc;
            g->e_new_objects.e_points[1].x  = xc + (float)pd_abscissa(radius, angle1);
            g->e_new_objects.e_points[1].y  = yc + (float)pd_ordinate(radius, angle1);
            g->e_new_objects.e_points[2].x  = angle2 - angle1;
            g->e_new_objects.e_points[2].y  = radius;
            g->e_new_objects.e_npoints      = 3;
        }
        else
        {
            g->e_new_objects.e_type         = E_GOBJ_INVALID;
        }
    }
}

void egraphics_arc_oval(t_elayer *g, float xc, float yc, float radiusx, float radiusy, float angle1, float angle2)
{
    if(g->e_state == EGRAPHICS_OPEN)
    {
        if(g->e_new_objects.e_points == NULL)
            g->e_new_objects.e_points   = (t_pt *)malloc(3 * sizeof(t_pt));
        else
            g->e_new_objects.e_points   = (t_pt *)realloc(g->e_new_objects.e_points , 3 * sizeof(t_pt));
        if(g->e_new_objects.e_points)
        {
            g->e_new_objects.e_type         = E_GOBJ_ARC;
            g->e_new_objects.e_points[0].x  = xc;
            g->e_new_objects.e_points[0].y  = yc;
            g->e_new_objects.e_points[1].x  = xc + (float)pd_abscissa(radiusx, angle1);
            g->e_new_objects.e_points[1].y  = yc + (float)pd_ordinate(radiusy, angle1);
            g->e_new_objects.e_points[2].x  = angle2 - angle1;
            g->e_new_objects.e_points[2].y  = angle2 - angle1;
            g->e_new_objects.e_npoints      = 3;
        }
        else
        {
            g->e_new_objects.e_type         = E_GOBJ_INVALID;
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

t_rgba hex_to_rgba(char* color)
{
    int hexvalue = (int)strtol(color+1, NULL, 16);
    t_rgba ncolor;
    ncolor.red = (float)((hexvalue >> 16) & 0xFF) / 255.f;
    ncolor.green = (float)((hexvalue >> 8) & 0xFF) / 255.f;
    ncolor.blue = (float)((hexvalue) & 0xFF) / 255.f;
    ncolor.alpha = 1.f;
    return ncolor;
}

t_rgb hex_to_rgb(char* color)
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
    new_color.red = (float)pd_clip_minmax(new_color.red += contrast, 0., 1.);
    new_color.green = (float)pd_clip_minmax(new_color.green += contrast, 0., 1.);
    new_color.blue = (float)pd_clip_minmax(new_color.blue += contrast, 0., 1.);
    return new_color;
}

t_rgb rgb_addContrast(t_rgb color, float contrast)
{
    t_rgb new_color = color;
    new_color.red = (float)pd_clip_minmax(new_color.red += contrast, 0., 1.);
    new_color.green = (float)pd_clip_minmax(new_color.green += contrast, 0., 1.);
    new_color.blue = (float)pd_clip_minmax(new_color.blue += contrast, 0., 1.);
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
    egraphics_matrix_init(&g->e_matrix, matrix->xx, matrix->yx, matrix->xy, matrix->yy, matrix->x0, matrix->y0);
}

void egraphics_rotate(t_elayer *g, float angle)
{
    const float cosRad = cosf(angle);
    const float sinRad = sinf(angle);
    t_matrix temp = g->e_matrix;
    
    g->e_matrix.xx = temp.xx * cosRad - temp.yx * sinRad;
    g->e_matrix.yx = temp.xx * sinRad + temp.yx * cosRad;
    g->e_matrix.xy = temp.xy * cosRad - temp.yy * sinRad;
    g->e_matrix.yy = temp.xy * sinRad + temp.yy * cosRad;
}


static void egraphics_apply_matrix(t_elayer *g, t_egobj* gobj)
{
    int i;
    float x_p, y_p;
    if(gobj->e_type == E_GOBJ_ARC)
    {
        for(i = 0; i < 2; i++)
        {
            x_p     = gobj->e_points[i].x * g->e_matrix.xx + gobj->e_points[i].y * g->e_matrix.xy + g->e_matrix.x0;
            y_p     = gobj->e_points[i].x * g->e_matrix.yx + gobj->e_points[i].y * g->e_matrix.yy + g->e_matrix.y0;
            gobj->e_points[i].x    = x_p;
            gobj->e_points[i].y    = y_p;
        }
    }
    else if(gobj->e_type == E_GOBJ_PATH)
    {
        for(i = 0; i < gobj->e_npoints; )
        {
            if(gobj->e_points[i].x == E_PATH_MOVE || gobj->e_points[i].x == E_PATH_LINE)
            {
                x_p     = gobj->e_points[i+1].x * g->e_matrix.xx + gobj->e_points[i+1].y * g->e_matrix.xy + g->e_matrix.x0;
                y_p     = gobj->e_points[i+1].x * g->e_matrix.yx + gobj->e_points[i+1].y * g->e_matrix.yy + g->e_matrix.y0;
                gobj->e_points[i+1].x    = x_p;
                gobj->e_points[i+1].y    = y_p;
                i += 2;
            }
            else if(gobj->e_points[i].x == E_PATH_CURVE)
            {
                x_p     = gobj->e_points[i+1].x * g->e_matrix.xx + gobj->e_points[i+1].y * g->e_matrix.xy + g->e_matrix.x0;
                y_p     = gobj->e_points[i+1].x * g->e_matrix.yx + gobj->e_points[i+1].y * g->e_matrix.yy + g->e_matrix.y0;
                gobj->e_points[i+1].x    = x_p;
                gobj->e_points[i+1].y    = y_p;
                x_p     = gobj->e_points[i+2].x * g->e_matrix.xx + gobj->e_points[i+2].y * g->e_matrix.xy + g->e_matrix.x0;
                y_p     = gobj->e_points[i+2].x * g->e_matrix.yx + gobj->e_points[i+2].y * g->e_matrix.yy + g->e_matrix.y0;
                gobj->e_points[i+2].x    = x_p;
                gobj->e_points[i+2].y    = y_p;
                x_p     = gobj->e_points[i+3].x * g->e_matrix.xx + gobj->e_points[i+3].y * g->e_matrix.xy + g->e_matrix.x0;
                y_p     = gobj->e_points[i+3].x * g->e_matrix.yx + gobj->e_points[i+3].y * g->e_matrix.yy + g->e_matrix.y0;
                gobj->e_points[i+3].x    = x_p;
                gobj->e_points[i+3].y    = y_p;
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
        for(i = 0; i < gobj->e_npoints; i++)
        {
            x_p     = gobj->e_points[i].x * g->e_matrix.xx + gobj->e_points[i].y * g->e_matrix.xy + g->e_matrix.x0;
            y_p     = gobj->e_points[i].x * g->e_matrix.yx + gobj->e_points[i].y * g->e_matrix.yy + g->e_matrix.y0;
            gobj->e_points[i].x    = x_p;
            gobj->e_points[i].y    = y_p;
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

float pd_clip_minmax(float aValue, float aMinimum, float aMaximum)
{
    if(aValue < aMinimum)
        return aMinimum;
    else if(aValue > aMaximum)
        return aMaximum;
    else
        return aValue;
}

float pd_clip_min(float aValue, float aMinimum)
{
    if(aValue < aMinimum)
        return aMinimum;
    else
        return aValue;
}

float pd_clip_max(float aValue, float aMaximum)
{
    if(aValue > aMaximum)
        return aMaximum;
    else
        return aValue;
}

float pd_ordinate(float radius, float angle)
{
    return radius * sinf(angle);
}

float pd_abscissa(float radius, float angle)
{
    return radius * cosf(angle);
}

float pd_radius(float x, float y)
{
    return sqrtf(x*x + y*y);
}

float pd_angle(float x, float y)
{
    return atan2f(y, x);
}



