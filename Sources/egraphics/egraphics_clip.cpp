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

t_pt egraphics_clip_object_point(t_pt pt, t_rect rect)
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
    for(int i = 0; i < cnt; i++) // We could avoid this but... there's a bug in the clipping function
    {
        out[i] = egraphics_clip_object_point(out[i], rect);
    }
    return cnt;
}

t_symbol* clip_text(t_rect rect, t_pt* in, t_symbol* text)
{
    //We assume that people use the system font...
    //sys_fontwidth()
    /*
     double x1, y1;
     x1 = g->c_obj_coords[index][0];
     y1 = g->c_obj_coords[index][1];
     if(x1 < -1 * (strlen(textlayout->c_text->s_name) * textlayout->c_font.c_size / 1.75) || x1 > g->e_rect.width || y1 < textlayout->c_font.c_size / 2.  || y1 > g->e_rect.height - textlayout->c_font.c_size / 2.)
     {
     g->c_obj_names.pop_back();
     g->c_obj_types.pop_back();
     g->c_obj_coords.pop_back();
     g->c_obj_options.pop_back();
     }
     else if(x1 > g->e_rect.width - strlen(textlayout->c_text->s_name) * textlayout->c_font.c_size / 1.75)
     {
     int strsize = (g->e_rect.width - x1) / (textlayout->c_font.c_size / 1.75);
     std::string newtext = textlayout->c_text->s_name;
     newtext.resize(strsize);
     g->c_obj_options[index].assign("-text ");
     sprintf(text, "{%s} ", newtext.c_str());
     g->c_obj_options[index].append(text);
     g->c_obj_options[index].append("-anchor ");
     g->c_obj_options[index].append(textlayout->c_justification->s_name);
     g->c_obj_options[index].append(" -font ");
     sprintf(text, "{%s %d %s} ", textlayout->c_font.c_family->s_name, (int)textlayout->c_font.c_size, textlayout->c_font.c_weight->s_name);
     g->c_obj_options[index].append(text);
     sprintf(text, "-fill %s -width %d -tags %s\n", gensym(cicm_rgba_to_hex(textlayout->c_color))->s_name, (int)textlayout->c_rect.width, g->c_obj_names[index].c_str());
     g->c_obj_options[index].append(text);
     }
     else if(x1 < 0)
     {
     int strsize = x1 / (textlayout->c_font.c_size / 1.75) * -1;
     g->c_obj_coords[index][0] = 0.;
     g->c_obj_options[index].assign("-text ");
     sprintf(text, "{%s} ", textlayout->c_text->s_name+strsize);
     g->c_obj_options[index].append(text);
     g->c_obj_options[index].append("-anchor ");
     g->c_obj_options[index].append(textlayout->c_justification->s_name);
     g->c_obj_options[index].append(" -font ");
     sprintf(text, "{%s %d %s} ", textlayout->c_font.c_family->s_name, (int)textlayout->c_font.c_size, textlayout->c_font.c_weight->s_name);
     g->c_obj_options[index].append(text);
     sprintf(text, "-fill %s -width %d -tags %s\n", gensym(cicm_rgba_to_hex(textlayout->c_color))->s_name, (int)textlayout->c_rect.width, g->c_obj_names[index].c_str());
     g->c_obj_options[index].append(text);
     }*/

    return NULL;
}

void egraphics_clip_object(t_elayer *g, t_egobj* gobj)
{
    if(gobj->e_type == E_GOBJ_PATH)
    {
        // I think, we really can't have more than 2 * n new point
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
    else if(gobj->e_type == E_GOBJ_TEXT)
    {
        
    }
}



