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

#include "egraphics.h"

int egraphics_point_in_rect(t_pt pt, t_rect rect)
{
    int code = CLIP_INSIDE;
    
    if (pt.x < rect.x)
        code |= CLIP_LEFT;
    else if (pt.x > rect.x + rect.width)
        code |= CLIP_RIGHT;
    if (pt.y < rect.y)
        code |= CLIP_BOTTOM;
    else if (pt.y > rect.y + rect.height)
        code |= CLIP_TOP;
    
    return code;
}

t_pt egraphics_clip_point(t_pt pt, t_rect rect)
{
    int code = egraphics_point_in_rect(pt, rect);
    if(code & CLIP_TOP)
    {
        pt.y = rect.y + rect.height;
    }
    else if(code & CLIP_BOTTOM)
    {
        pt.y = rect.y;
    }
    if(code & CLIP_RIGHT)
    {
        pt.x = rect.x + rect.width;
    }
    else if(code & CLIP_LEFT)
    {
        pt.x = rect.x;
    }
    return pt;
}

#define N_EDGE 4

int inside(t_pt pt, int edge, t_rect rect)
{
    switch (edge)
    {
        case 0:
            if (pt.x < rect.x)
            {
                return 0;
            }
            break;
        case 1:
            if (pt.x > rect.x + rect.width)
            {
                return 0;
            }
            break;
        case 2:
            if (pt.y < rect.y)
            {
                return 0;
            }
            break;
        case 3:
            if (pt.y > rect.y + rect.height)
            {
                return 0;
            }
            break;
    }
    return 1;
}

int cross(t_pt pt1, t_pt pt2, int edge, t_rect rect)
{
    if(inside(pt1, edge, rect) == inside(pt2, edge, rect))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

t_pt intersect(t_pt pt1, t_pt pt2, int edge, t_rect rect)
{
    t_pt iPt;
    float m = 0.f;
    
    if(pt1.x != pt2.x)
    {
        m = (pt1.y - pt2.y) / (pt1.x - pt2.x);
    }
    switch (edge)
    {
        case 0:
            iPt.x = rect.x;
            iPt.y = pt2.y + (rect.x - pt2.x) * m;
            break;
        case 1:
            iPt.x = rect.x + rect.width;
            iPt.y = pt2.y + (rect.x + rect.width - pt2.x) * m;
            break;
        case 2:
            iPt.y = rect.y;
            if(pt1.x != pt2.x)
            {
                if (m != 0)
                    iPt.x = pt2.x + (rect.y - pt2.y) / m;
                else
                    iPt.x = 0.f;
            }
            else
            {
                iPt.x = pt2.x;
            }
            break;
        case 3:
            iPt.y = rect.y + rect.height;
            if(pt1.x != pt2.x)
            {
                if (m != 0)
                    iPt.x = pt2.x + (rect.y + rect.height - pt2.y) / m;
                else
                    iPt.x = 0.f;
            }
            else
            {
                iPt.x = pt2.x;
            }
            break;
    }
    return iPt;
}

void clip_point(t_pt pt, int edge, t_rect rect, t_pt* pt_out, int* cnt, t_pt* first[], t_pt *s)
{
    t_pt iPt;
    
    if(!first[edge])
    {
        first[edge] = &pt;
    }
    else
    {
        if(cross(pt, s[edge], edge, rect))
        {
            iPt = intersect(pt, s[edge], edge, rect);
            if(edge < 3)
            {
                clip_point(iPt, edge+1, rect, pt_out, cnt, first, s);
            }
            else
            {
                pt_out[*cnt] = iPt;
                (*cnt)++;
            }
        }
    }
    s[edge] = pt;
    if(inside(pt, edge, rect))
    {
        if (edge < 3)
        {
            clip_point(pt, edge+1, rect, pt_out, cnt, first, s);
        }
        else
        {
            pt_out[*cnt] = pt;
            (*cnt)++;
        }
     }
}

void close_clip(t_rect rect, t_pt* pt_out, int* cnt, t_pt* first[], t_pt *s)
{
    t_pt i;
    int edge;
    
    for(edge = 0; edge <= 3; edge++)
    {
        if(cross(s[edge], *first[edge], edge, rect))
        {
            i = intersect(s[edge], *first[edge], edge, rect);
            if(edge < 3)
            {
                clip_point(i, edge+1, rect, pt_out, cnt, first, s);
            }
            else
            {
                pt_out[*cnt] = i;
                (*cnt)++;
            }
        }
    }
}

long egraphics_clip_path_perform(t_rect rect, long n, t_pt* in, t_pt* out)
{
    t_pt* first[N_EDGE] = {0, 0, 0, 0};
    t_pt  s[N_EDGE];
    int cnt = 0;
    
    for(int i = 0; i < n; i++)
    {
        clip_point(in[i], 0, rect, out, &cnt, first, s);
    }
    //close_clip(rect, out, &cnt, first, s);
    
    for(int i = 0; i < cnt; i++) // We could avoid this but... there's a bug in the clipping function
    {
        out[i] = egraphics_clip_point(out[i], rect);
    }
    return cnt;
}

void egraphics_clip_path(t_elayer *g, t_egobj* gobj)
{
    // I think, we really can't have more than 2 * n new point
    t_pt* new_path = (t_pt *)calloc(gobj->e_npoints * 2, sizeof(t_pt));
    gobj->e_npoints = egraphics_clip_path_perform(g->e_rect, gobj->e_npoints, gobj->e_points, new_path);
    if(gobj->e_npoints && new_path)
    {
        free(gobj->e_points);
        gobj->e_points = new_path;
    }
    else
    {
        free(gobj->e_points);
        gobj->e_type = E_GOBJ_INVALID;
    }
}



