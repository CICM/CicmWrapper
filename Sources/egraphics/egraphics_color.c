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

static char ColBuf[10];
static char HexDigits[] = "0123456789ABCDEF";

char* rgba_to_hex(t_rgba color)
{
    int r = color.red * 255;
    int g = color.green * 255;
    int b = color.blue * 255;
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

char* rgb_to_hex(t_rgb color)
{
    int r = color.red * 255;
    int g = color.green * 255;
    int b = color.blue * 255;
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

t_rgba rgba_addContrast(t_rgba color, float contrast)
{
    t_rgba new_color = color;
    new_color.red = pd_clip_minmax(new_color.red += contrast, 0., 1.);
    new_color.green = pd_clip_minmax(new_color.green += contrast, 0., 1.);
    new_color.blue = pd_clip_minmax(new_color.blue += contrast, 0., 1.);
    return new_color;
}

t_rgb rgb_addContrast(t_rgb color, float contrast)
{
    t_rgb new_color = color;
    new_color.red = pd_clip_minmax(new_color.red += contrast, 0., 1.);
    new_color.green = pd_clip_minmax(new_color.green += contrast, 0., 1.);
    new_color.blue = pd_clip_minmax(new_color.blue += contrast, 0., 1.);
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

