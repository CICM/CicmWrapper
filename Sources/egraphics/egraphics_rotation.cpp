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

void egraphics_rotate_set(t_egraphics *g, float angle)
{
    g->e_rotation = angle;
}

void egraphics_rotate_add(t_egraphics *g, float angle)
{
    g->e_rotation += angle;
}

void egraphics_apply_rotation(t_egraphics *g, t_egraphics_obj* gobj)
{
    if(g->e_rotation)
    {
        float x_p, y_p;
        float radius, angle;
        for(int  i = 0; i < gobj->e_npoints; i ++)
        {
            x_p     = gobj->e_points[i].x - g->e_rect.width / 2.;
            y_p     = gobj->e_points[i].y - g->e_rect.height / 2.;
            
            radius  = pd_radius(x_p, y_p);
            angle   = pd_angle(x_p , y_p) + g->e_rotation;
            
            gobj->e_points[i].x = pd_abscissa(radius, angle) + g->e_rect.width / 2.;
            gobj->e_points[i].y = pd_ordinate(radius, angle) + g->e_rect.height / 2.;
        }
        gobj->e_angles[0] += g->e_rotation;
        gobj->e_angles[1] += g->e_rotation;
    }
}



