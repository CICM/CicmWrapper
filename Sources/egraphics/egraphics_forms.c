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

#include "egraphics.h"

static const double k = 0.55228474983079356430692996582365594804286956787109;

void egraphics_move_to(t_elayer *g, float x, float y)
{
    if(g->e_state == EGRAPHICS_OPEN)
    {
        if(g->e_new_objects.e_type != E_GOBJ_PATH)
        {
            if(!g->e_new_objects.e_npoints || g->e_new_objects.e_points == NULL)
            {
                g->e_new_objects.e_points   = (t_pt *)calloc(2, sizeof(t_pt));
            }
            else
            {
                g->e_new_objects.e_points = (t_pt *)realloc(g->e_new_objects.e_points, 2 * sizeof(t_pt));
            }
            g->e_new_objects.e_type = E_GOBJ_PATH;
            g->e_new_objects.e_npoints = 0;
        }
        else if(g->e_new_objects.e_type == E_GOBJ_PATH)
        {
            if(!g->e_new_objects.e_npoints || g->e_new_objects.e_points == NULL)
            {
                g->e_new_objects.e_points   = (t_pt *)calloc(2, sizeof(t_pt));
                g->e_new_objects.e_npoints = 0;
            }
            else
            {
                g->e_new_objects.e_points = (t_pt *)realloc(g->e_new_objects.e_points, (g->e_new_objects.e_npoints + 2) * sizeof(t_pt));
            }
        }
        if(g->e_new_objects.e_points)
        {
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
        if(g->e_new_objects.e_type == E_GOBJ_PATH)
        {
            g->e_new_objects.e_points = (t_pt *)realloc(g->e_new_objects.e_points, (g->e_new_objects.e_npoints + 4) * sizeof(t_pt));
            if(g->e_new_objects.e_points)
            {
                const t_pt pt = g->e_new_objects.e_points[g->e_new_objects.e_npoints-1];
                g->e_new_objects.e_points[g->e_new_objects.e_npoints].x  = E_PATH_CURVE;
                g->e_new_objects.e_points[g->e_new_objects.e_npoints+1].x  = pt.x;
                g->e_new_objects.e_points[g->e_new_objects.e_npoints+1].y  = pt.y;
                g->e_new_objects.e_points[g->e_new_objects.e_npoints+2].x  = x;
                g->e_new_objects.e_points[g->e_new_objects.e_npoints+2].y  = y;
                g->e_new_objects.e_points[g->e_new_objects.e_npoints+3].x  = x;
                g->e_new_objects.e_points[g->e_new_objects.e_npoints+3].y  = y;
                g->e_new_objects.e_npoints += 4;
            }
            else
            {
                g->e_new_objects.e_type = E_GOBJ_INVALID;
            }
        }
    }
}

void egraphics_curve_to(t_elayer *g, float ctrl1x, float ctrl1y, float ctrl2x, float ctrl2y, float endx, float endy)
{
    if(g->e_state == EGRAPHICS_OPEN)
    {
        if(g->e_new_objects.e_type == E_GOBJ_PATH)
        {
            g->e_new_objects.e_points = (t_pt *)realloc(g->e_new_objects.e_points, (g->e_new_objects.e_npoints + 4) * sizeof(t_pt));
            if(g->e_new_objects.e_points)
            {
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
}

static void rotate(const double cosz, const double sinz, t_pt* p1)
{
    const double rx = p1->x * cosz - p1->y * sinz;
    p1->y = p1->x * sinz + p1->y * cosz;
    p1->x = rx;
}

static void create_small_arc(const double r, const double start, const double extend, t_pt ct, t_pt* p2, t_pt* p3, t_pt* p4)
{
    const double a = extend;
    const double cosz = cos(a * 0.5 + start);
    const double sinz = sin(a * 0.5 + start);
    t_pt p1;
    p4->x = r * cos(a * 0.5);
    p4->y = r * sin(a * 0.5);
    p1.x = p4->x;
    p1.y = -p4->y;
    p2->x = p1.x + k * tan(a * 0.5) * p4->y;
    p2->y = p1.y + k * tan(a * 0.5) * p4->x;
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
            double radius   = pd_radius(prev.x - cx, prev.y - cy);
            double angle    = pd_angle(prev.x - cx, prev.y - cy);
            
            while(extend > EPD_2PI)
            {
                extend -= EPD_2PI;
            }
            while(extend < -EPD_2PI)
            {
                extend += EPD_2PI;
            }
            
            while(fabs(extend) >= EPD_PI4)
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
            if(fabs(extend) > 1e-6)
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

static void create_small_arc_oval(const double r1, const double r2, const double start, const double extend, t_pt ct, t_pt* p2, t_pt* p3, t_pt* p4)
{
    t_pt p1;
    
    const double a = extend;
    const double cosz = cos(a * 0.5 + start);
    const double sinz = sin(a * 0.5 + start);
    const double cosa = cos(a * 0.5);
    const double sina = sin(a * 0.5);
    p4->x = r2 * cosa;
    p4->y = r2 * sina;
    p1.x = r1 * cosa;
    p1.y = -r1 * sina;
    const double k1 = (4. * (1. - cos(a * 0.5)) / sin(a * 0.5)) / 3.;
    const double k2 = (4. * (1. - cos(-a * 0.5)) / sin(-a * 0.5)) / 3.;
    p2->x = p1.x + k1 * p4->y;
    p2->y = p1.y + k1 * p4->x;
    p3->x = p4->x + k2 * p1.y;
    p3->y = p4->y + k2 * p1.y;
    
    rotate(cosz, sinz, p2); rotate(cosz, sinz, p3); rotate(cosz, sinz, p4);
    p2->x += ct.x; p2->y += ct.y; p3->x += ct.x; p3->y += ct.y; p4->x += ct.x; p4->y += ct.y;
}

void egraphics_arc_oval_to(t_elayer *g, float cx, float cy, float r2, float extend)
{
    if(g->e_state == EGRAPHICS_OPEN)
    {
        if(g->e_new_objects.e_type == E_GOBJ_PATH && g->e_new_objects.e_points)
        {
            t_pt p2, p3, p4, c = {cx, cy}, prev = g->e_new_objects.e_points[g->e_new_objects.e_npoints-1];
            double r1   = pd_radius(prev.x - cx, prev.y - cy);
            double angle = pd_angle(prev.x - cx, prev.y - cy);
            double ratio = (r2 - r1) / (fabs(extend) / EPD_PI4);
            
            while(extend > EPD_2PI)
            {
                extend -= EPD_2PI;
            }
            while(extend < -EPD_2PI)
            {
                extend += EPD_2PI;
            }
            
            while(fabs(extend) >= EPD_PI4)
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
            if(fabs(extend) > 1e-6)
            {
                create_small_arc_oval(r1, r2, angle, extend, c, &p2, &p3, &p4);
                egraphics_curve_to(g, p2.x, p2.y, p3.x, p3.y,  p4.x, p4.y);
            }
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
            if(g->e_new_objects.e_points)
            {
                egraphics_line_to(g, g->e_new_objects.e_points[1].x, g->e_new_objects.e_points[1].y);
            }
            else
            {
                g->e_new_objects.e_type = E_GOBJ_INVALID;
            }
        }
    }
}

void egraphics_line(t_elayer *g, float x0, float y0, float x1, float y1)
{
    if(g->e_state == EGRAPHICS_OPEN)
    {
        egraphics_move_to(g, x0, y0);
        egraphics_line_to(g, x1, y1);
    }
}

void egraphics_curve(t_elayer *g, float startx, float starty, float ctrl1x, float ctrl1y, float ctrl2x, float ctrl2y, float endx, float endy)
{
    if(g->e_state == EGRAPHICS_OPEN)
    {
        egraphics_move_to(g, startx, starty);
        egraphics_curve_to(g, ctrl1x, ctrl1y, ctrl2x, ctrl2y, endx, endy);
    }
}

void egraphics_line_fast(t_elayer *g, float x0, float y0, float x1, float y1)
{
    if(g->e_state == EGRAPHICS_OPEN)
    {
        egraphics_move_to(g, x0, y0);
        egraphics_line_to(g, x1, y1);
    }
    egraphics_stroke(g);
}

void egraphics_rectangle(t_elayer *g, float x, float y, float width, float height)
{
    if(g->e_state == EGRAPHICS_OPEN)
    {
        if(g->e_new_objects.e_points == NULL)
            g->e_new_objects.e_points   = (t_pt *)calloc(5, sizeof(t_pt));
        else
            g->e_new_objects.e_points   = (t_pt *)realloc(g->e_new_objects.e_points , 5 * sizeof(t_pt));
        if(g->e_new_objects.e_points)
        {
            g->e_new_objects.e_type         = E_GOBJ_RECT;
            g->e_new_objects.e_points[0].x  = x;
            g->e_new_objects.e_points[0].y  = y;
            g->e_new_objects.e_points[1].x  = x + width;
            g->e_new_objects.e_points[1].y  = y;
            g->e_new_objects.e_points[2].x  = x + width;
            g->e_new_objects.e_points[2].y  = y + height;
            g->e_new_objects.e_points[3].x  = x;
            g->e_new_objects.e_points[3].y  = y + height;
            g->e_new_objects.e_points[4].x  = x;
            g->e_new_objects.e_points[4].y  = y;
            g->e_new_objects.e_npoints      = 5;
            g->e_new_objects.e_roundness    = 0.;
        }
        else
        {
            g->e_new_objects.e_type         = E_GOBJ_INVALID;
        }
    }
}

void egraphics_rectangle_rounded(t_elayer *g, float x, float y, float width, float height, float roundness)
{
    if(g->e_state == EGRAPHICS_OPEN)
    {
        if(g->e_new_objects.e_points == NULL)
            g->e_new_objects.e_points   = (t_pt *)calloc(9, sizeof(t_pt));
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
            g->e_new_objects.e_roundness = pd_clip_min(roundness, 0.);
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

void egraphics_oval(t_elayer *g, float xc, float yc, float radiusx, float radiusy)
{
    if(g->e_state == EGRAPHICS_OPEN)
    {
        if(g->e_new_objects.e_points == NULL)
            g->e_new_objects.e_points   = (t_pt *)calloc(2, sizeof(t_pt));
        else
            g->e_new_objects.e_points   = (t_pt *)realloc(g->e_new_objects.e_points , 2 * sizeof(t_pt));
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

void egraphics_arc(t_elayer *g, float xc, float yc, float radius, float angle1, float angle2)
{
    if(g->e_state == EGRAPHICS_OPEN)
    {
        if(g->e_new_objects.e_points == NULL)
            g->e_new_objects.e_points   = (t_pt *)calloc(3, sizeof(t_pt));
        else
            g->e_new_objects.e_points   = (t_pt *)realloc(g->e_new_objects.e_points , 3 * sizeof(t_pt));
        if(g->e_new_objects.e_points)
        {
            g->e_new_objects.e_type         = E_GOBJ_ARC;
            g->e_new_objects.e_points[0].x  = xc;
            g->e_new_objects.e_points[0].y  = yc;
            g->e_new_objects.e_points[1].x  = xc + pd_abscissa(radius, angle1);
            g->e_new_objects.e_points[1].y  = yc + pd_ordinate(radius, angle1);
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
            g->e_new_objects.e_points   = (t_pt *)calloc(3, sizeof(t_pt));
        else
            g->e_new_objects.e_points   = (t_pt *)realloc(g->e_new_objects.e_points , 3 * sizeof(t_pt));
        if(g->e_new_objects.e_points)
        {
            g->e_new_objects.e_type         = E_GOBJ_ARC;
            g->e_new_objects.e_points[0].x  = xc;
            g->e_new_objects.e_points[0].y  = yc;
            g->e_new_objects.e_points[1].x  = xc + pd_abscissa(radiusx, angle1);
            g->e_new_objects.e_points[1].y  = yc + pd_ordinate(radiusy, angle1);
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

