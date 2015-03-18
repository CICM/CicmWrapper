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


void egraphics_apply_matrix(t_elayer *g, t_egobj* gobj)
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
            if(gobj->e_points[i].x == E_PATH_MOVE)
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
                i += 4;
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



