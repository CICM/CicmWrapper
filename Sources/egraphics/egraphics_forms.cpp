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

void egraphics_move_to(t_elayer *g, float x, float y)
{
    if(g->e_state == EGRAPHICS_OPEN)
    {
        if(g->e_new_objects.e_points == NULL)
            g->e_new_objects.e_points   = (t_pt *)calloc(1, sizeof(t_pt));
        else
            g->e_new_objects.e_points   = (t_pt *)realloc(g->e_new_objects.e_points, sizeof(t_pt));
        if(g->e_new_objects.e_points)
        {
            g->e_new_objects.e_type         = E_GOBJ_PATH;
            g->e_new_objects.e_points[0].x  = x;
            g->e_new_objects.e_points[0].y  = y;
            g->e_new_objects.e_npoints      = 1;
        }
        else
        {
            g->e_new_objects.e_type = E_GOBJ_INVALID;
        }
    }
}

void egraphics_line_to(t_elayer *g, float x, float y)
{
    if(g->e_state == EGRAPHICS_OPEN && g->e_new_objects.e_npoints >= 1)
    {
        if(g->e_new_objects.e_type == E_GOBJ_PATH)
        {
            g->e_new_objects.e_points = (t_pt *)realloc(g->e_new_objects.e_points, (g->e_new_objects.e_npoints + 1) * sizeof(t_pt));
            if(g->e_new_objects.e_points)
            {
                g->e_new_objects.e_points[g->e_new_objects.e_npoints].x  = x;
                g->e_new_objects.e_points[g->e_new_objects.e_npoints].y  = y;
                g->e_new_objects.e_npoints++;
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

void egraphics_close_path(t_elayer *g)
{
    if(g->e_state == EGRAPHICS_OPEN && g->e_new_objects.e_npoints >= 1)
    {
        if(g->e_new_objects.e_type == E_GOBJ_PATH)
        {
            g->e_new_objects.e_points = (t_pt *)realloc(g->e_new_objects.e_points, (g->e_new_objects.e_npoints + 1) * sizeof(t_pt));
            if(g->e_new_objects.e_points)
            {
                g->e_new_objects.e_points[g->e_new_objects.e_npoints].x  = g->e_new_objects.e_points[0].x;
                g->e_new_objects.e_points[g->e_new_objects.e_npoints].y  = g->e_new_objects.e_points[0].y;
                g->e_new_objects.e_npoints++;
            }
            else
            {
                g->e_new_objects.e_type = E_GOBJ_INVALID;
            }
        }
    }    
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
            g->e_new_objects.e_type         = E_GOBJ_PATH;
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
            g->e_new_objects.e_type         = E_GOBJ_PATH;
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

