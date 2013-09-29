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

const int CLIP_INSIDE    = 0;   // 0000
const int CLIP_LEFT      = 1;   // 0001
const int CLIP_RIGHT     = 2;   // 0010
const int CLIP_BOTTOM    = 4;   // 0100
const int CLIP_TOP       = 8;   // 1000

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

typedef enum _edge
{
    LEFT = 0,
    RIGHT = 1,
    BOTTOM = 2,
    TOP = 3
    
}t_edge;

#define N_EDGE 4

int inside(t_pt pt, int edge, t_rect rect)
{
    switch (edge)
    {
        case LEFT:
            if (pt.x < rect.x)
            {
                return 0;
            }
            break;
        case RIGHT:
            if (pt.x > rect.x + rect.width)
            {
                return 0;
            }
            break;
        case BOTTOM:
            if (pt.y < rect.y)
            {
                return 0;
            }
            break;
        case TOP:
            if (pt.y > rect.y + rect.height)
            {
                return 0;
            }
            break;
    }
    return 1;
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
        case LEFT:
            iPt.x = rect.x;
            iPt.y = pt2.y + (rect.x - pt2.x) * m;
            break;
        case RIGHT:
            iPt.x = rect.x + rect.width;
            iPt.y = pt2.y + (rect.x + rect.width - pt2.x) * m;
            break;
        case BOTTOM:
            iPt.y = rect.y;
            if(pt1.x != pt2.x)
            {
                iPt.x = pt2.x + (rect.y - pt2.y) / m;
            }
            else
            {
                iPt.x = pt2.x;
            }
            break;
        case TOP:
            iPt.y = rect.y + rect.height;
            if(pt1.x != pt2.x)
            {
                iPt.x = pt2.x + (rect.y + rect.height - pt2.y) / m;
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
            if(edge < TOP)
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
        if (edge < TOP)
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
    t_edge edge;
    
    for(edge = LEFT; edge <= TOP; edge++)
    {
        if(cross(s[edge], *first[edge], edge, rect))
        {
            i = intersect(s[edge], *first[edge], edge, rect);
            if(edge < TOP)
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

int clip_polygon(t_rect rect, int n, t_pt* in, t_pt* out)
{
    t_pt* first[N_EDGE] = {0, 0, 0, 0};
    t_pt  s[N_EDGE];
    int cnt = 0;
    
    for(int i = 0; i < n; i++)
    {
        clip_point(in[i], LEFT, rect, out, &cnt, first, s);
    }
    close_clip(rect, out, &cnt, first, s);
    for(int i = 0; i < cnt; i++) // We could avoid this but...
    {
        out[i] = egraphics_clip_point(out[i], rect);
    }
    return cnt;
}

void egraphics_clip(t_egraphics *g, t_egraphics_obj* gobj)
{
    if(gobj->e_type == E_GOBJ_LINE || gobj->e_type == E_GOBJ_PATH || gobj->e_type == E_GOBJ_RECTANGLE)
    {
        t_pt* new_path = (t_pt *)calloc(gobj->e_npoints * 2, sizeof(t_pt));
        gobj->e_npoints = clip_polygon(g->e_rect, gobj->e_npoints, gobj->e_points, new_path);
        if(!gobj->e_npoints)
        {
            gobj->e_type = E_GOBJ_INVALID;
        }
        else
        {
            free(gobj->e_points);
            gobj->e_points = new_path;
        }
    }
    
}

/*


int egraphics_clip_line(t_pt* pt, t_rect rect)
{
    int code0 = egraphics_point_in_rect(pt[0], rect);
    int code1 = egraphics_point_in_rect(pt[1], rect);
    int accept = false;
    
    while (true)
    {
        if(!(code0 | code1))
        {
            accept = true;
            break;
        }
        else if(code0 & code1)
        {
            break;
        }
        else
        {
            float ymax = rect.y + rect.height;
            float xmax = rect.x + rect.width;
            float x, y;
            int codeOut = code0 ? code0 : code1;
            
            // Now find the intersection point;
            // use formulas y = pt[0].y + slope * (x - pt[0].x), x = pt[0].x + (1 / slope) * (y - pt[0].y)
            if (codeOut & CLIP_TOP)
            {
                x = pt[0].x + (pt[1].x - pt[0].x) * (ymax - pt[0].y) / (pt[1].y - pt[0].y);
                y = ymax;
            }
            else if (codeOut & CLIP_BOTTOM) // point is below the clip rectangle
            {
                x = pt[0].x + (pt[1].x - pt[0].x) * (rect.y - pt[0].y) / (pt[1].y - pt[0].y);
                y = rect.y;
            }
            else if (codeOut & CLIP_RIGHT) // point is to the right of clip rectangle
            {
                y = pt[0].y + (pt[1].y - pt[0].y) * (xmax - pt[0].x) / (pt[1].x - pt[0].x);
                x = xmax;
            }
            else if (codeOut & CLIP_LEFT) // point is to the left of clip rectangle
            {
                y = pt[0].y + (pt[1].y - pt[0].y) * (rect.x - pt[0].x) / (pt[1].x - pt[0].x);
                x = rect.x;
            }
            
            // Now we move outside point to intersection point to clip
            // and get ready for next pass.
            if (codeOut == code0)
            {
                pt[0].x = x;
                pt[0].y = y;
                code0 = egraphics_point_in_rect(pt[0], rect);
            }
            else
            {
                pt[1].x = x;
                pt[1].y = y;
                code1 = egraphics_point_in_rect(pt[1], rect);
            }
        }
    }
    if (accept)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

t_pt* clip_polygon(t_pt* input_path, int input_size, long* size, t_rect rect)
{
    t_pt current_pt, previous_pt;
    t_pt new_pt;
    int output_size = 0;
    t_pt* output_path = (t_pt *)malloc(sizeof(t_pt));
    
    previous_pt = input_path[input_size-1];
    for(int i = 0; i < input_size; i++)
    {
        egraphics_clip_line
    }
    
}

t_pt* clip_polygon(t_pt* input_path, int input_size, long* size, t_rect rect)
{
    t_pt current_pt, previous_pt;
    t_pt new_pt;
    int output_size = 0;
    t_pt* output_path = (t_pt *)malloc(sizeof(t_pt));
    
    previous_pt = input_path[input_size-1];
    for(int i = 0; i < input_size; i++)
    {
        current_pt = input_path[i];
        if(!egraphics_point_in_rect(current_pt, rect)) // Current Point inside the rectangle
        {
            if(!egraphics_point_in_rect(previous_pt, rect)) // Previous Poinr inside the rectangle
            {
                output_path = (t_pt *)realloc(output_path, sizeof(t_pt) * (output_size+1)); // We add this point
                if(!output_path)
                {
                    return 0;
                }
                else
                {
                    output_path[output_size].x = current_pt.x;
                    output_path[output_size].y = current_pt.y;
                    output_size++;
                }
            }
            else // Previous point outside the rectangle
            {
                if(!find_intersection(current_pt, previous_pt, rect, &new_pt)) // We fint the intersection
                {
                    return 0;
                }
                output_path = (t_pt *)realloc(output_path, sizeof(t_pt) * (output_size+1)); // We add this new point
                if(!output_path)
                {
                    return 0;
                }
                else
                {
                    output_path[output_size].x = new_pt.x;
                    output_path[output_size].y = new_pt.y;
                    output_size++;
                }
                
                if(current_pt.x != new_pt.x || current_pt.y != new_pt.y) // We add current point if it is not similar
                {
                    output_path = (t_pt *)realloc(output_path, sizeof(t_pt) * (output_size+1));
                    if(!output_path)
                    {
                        return 0;
                    }
                    else
                    {
                        output_path[output_size].x = current_pt.x;
                        output_path[output_size].y = current_pt.y;
                        output_size++;
                    }
                }
            }
        }
        else
        {
            if(pt_inside_rect(previous_pt, rect))
            {
                if(!find_intersection(current_pt, previous_pt, rect, &new_pt))
                {
                    return 0;
                }
                if(previous_pt.x != new_pt.x || previous_pt.y != new_pt.y)
                {
                    output_path = (t_pt *)realloc(output_path, sizeof(t_pt) * (output_size+1));
                    if(!output_path)
                    {
                        return 0;
                    }
                    else
                    {
                        output_path[output_size].x = new_pt.x;
                        output_path[output_size].y = new_pt.y;
                        output_size++;
                    }
                }
            }
        }
        previous_pt = current_pt;
    }
    size[0] = output_size;
    return output_path;
}

 char pt_inside_rect(t_pt pt, t_rect rect)
 {
 if(pt.x >= rect.x && pt.x <= rect.width && pt.y >= rect.y && rect.y <= rect.width)
 return 1;
 else
 return 0;
 }
 
 float get_new_x(t_rect rect, float y)
 {
 return rect.height - rect.y  + (y - rect.width - rect.x) * rect.height / rect.width;
 }
 
 float get_new_y(t_rect rect, float x)
 {
 return rect.y + (x - rect.x) * rect.height / rect.width;
 }
 
 char find_intersection(t_pt pt1, t_pt pt2, t_rect rect, t_pt* output)
 {
 if(pt1.x == pt2.x)
 {
 output->x = pt1.x;
 output->y = get_new_y(rect, pt1.x);
 return 1;
 }
 if(pt1.y == pt2.y)
 {
 output->y = pt1.y;
 output->x = get_new_y(rect , pt1.y);
 return 1;
 }
 return 1;
 }*/

/*
int egraphics_clip_path_filled(t_pt* pt, int size, t_rect rect)
{
    t_pt* new_points = NULL;
    int new_size = size;
    
    int previous, current, next;
    // We compute the new number of points //
    previous    = egraphics_point_in_rect(pt[0], rect);
    current     = egraphics_point_in_rect(pt[1], rect);
    for(int i = 1; i < size - 1; i++)
    {
        next    = egraphics_point_in_rect(pt[i+1], rect);
        if(previous && current && next)
        {
            new_size--;
        }
        else if(current)
        {
            new_size++;
        }
        previous = current;
        current  = next;
    }
    
    if(new_size > 0)
        new_points = (t_pt *)calloc(new_size, sizeof(t_pt));
    if(new_points)
    {
        // We fill the new points //
        previous    = egraphics_point_in_rect(pt[0], rect);
        current     = egraphics_point_in_rect(pt[1], rect);
        for(int i = 1, j = 1; i < size - 1; i++, j++)
        {
            next    = egraphics_point_in_rect(pt[i+1], rect);
            if(previous && current && next)
            {
                j--;
            }
            else if(current)
            {
                new_points[j++] = pt[i];
                new_points[j] = pt[i];
            }
            else
            {
                new_points[j] = pt[i];
            }
            previous = current;
            current  = next;
        }
        
        // We clip the new points //
        for(int i = 0; i < new_size; i++)
        {
            egraphics_clip_line(new_points+i, rect);
        }
        free(pt);
        pt = new_points;
        return new_size;
    }
    else
    {
        return 0;
    }
}
 */



