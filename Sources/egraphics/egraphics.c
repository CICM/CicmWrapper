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

void egraphics_set_line_width(t_elayer *g, float width)
{
    g->e_line_width= pd_clip_min(width, 0.);
}

void egraphics_set_color_rgba(t_elayer *g, t_rgba *rgba)
{
    g->e_color = gensym(rgba_to_hex(*rgba));
}

void egraphics_set_color_rgb(t_elayer *g, t_rgb *rgb)
{
    g->e_color = gensym(rgb_to_hex(*rgb));
}

void egraphics_set_color_hex(t_elayer *g, t_symbol *hex)
{
    g->e_color = hex;
}

void egraphics_set_color_hsla(t_elayer *g, t_hsla *hsla)
{
    t_rgba color = hsla_to_rgba(*hsla);
    g->e_color = gensym(rgba_to_hex(color));
}

void egraphics_set_color_hsl(t_elayer *g, t_hsl *hsl)
{
    t_rgb color = hsl_to_rgb(*hsl);
    g->e_color = gensym(rgb_to_hex(color));
}

void egraphics_set_line_splinestep(t_elayer *g, float smooth)
{
    g->e_new_objects.e_roundness = pd_clip_min(smooth, 0);
}

void egraphics_paint(t_elayer *g, int filled, int preserved)
{
    t_egobj* nobj;
    if(g->e_new_objects.e_type != E_GOBJ_INVALID)
    {
        g->e_objects = (t_egobj *)realloc(g->e_objects, (g->e_number_objects + 1) * sizeof(t_egobj));
        if(g->e_objects)
        {
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
            nobj->e_roundness = g->e_new_objects.e_roundness;
            nobj->e_npoints   = g->e_new_objects.e_npoints;
            nobj->e_points = (t_pt*)calloc(nobj->e_npoints, sizeof(t_pt));
            if(!nobj->e_points)
            {
                nobj->e_type = E_GOBJ_INVALID;
                return;
            }
            memcpy(nobj->e_points, g->e_new_objects.e_points, sizeof(t_pt) * nobj->e_npoints);
            nobj->e_roundness = g->e_new_objects.e_roundness;
            
            nobj->e_color = g->e_color;
            nobj->e_width = g->e_line_width;
            nobj->e_text  = g->e_new_objects.e_text;
            
            egraphics_apply_matrix(g, nobj);
            if(!preserved)
            {
                g->e_new_objects.e_roundness = 0;
                g->e_new_objects.e_npoints   = 0;
                free(g->e_new_objects.e_points);
                g->e_new_objects.e_points   = NULL;
                g->e_new_objects.e_type     = E_GOBJ_INVALID;
            }
        }
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
    g->e_objects = (t_egobj *)realloc(g->e_objects, (g->e_number_objects + 1) * sizeof(t_egobj));
    if(g->e_objects)
    {
        long index = g->e_number_objects;
        g->e_number_objects++;
        
        g->e_objects[index].e_type      = E_GOBJ_TEXT;
        g->e_objects[index].e_npoints   = 1;
        g->e_objects[index].e_points    = (t_pt*)calloc(2, sizeof(t_pt));
        g->e_objects[index].e_points[0].x = textlayout->c_rect.x;
        g->e_objects[index].e_points[0].y = textlayout->c_rect.y;
        g->e_objects[index].e_points[1].x = textlayout->c_rect.width;
        g->e_objects[index].e_points[1].y = textlayout->c_rect.height;
        g->e_objects[index].e_color       = gensym(rgba_to_hex(textlayout->c_color));
        
        g->e_objects[index].e_font        = textlayout->c_font;
        g->e_objects[index].e_justify     = textlayout->c_justify;
        g->e_objects[index].e_text        = textlayout->c_text;
        g->e_objects[index].e_anchor      = textlayout->c_anchor;
        egraphics_apply_matrix(g, g->e_objects+index);
    }
}


