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

char* cicm_rgba_to_hex(t_ergba color)
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

t_ergba cicm_rgba_addContrast(t_ergba baseColor, double contrast, bool preserveAlpha)
{
    t_ergba maxColor = baseColor;
    maxColor.red = pd_clip_minmax(maxColor.red += contrast, 0., 1.);
    maxColor.green = pd_clip_minmax(maxColor.green += contrast, 0., 1.);
    maxColor.blue = pd_clip_minmax(maxColor.blue += contrast, 0., 1.);
    if(!preserveAlpha)
        maxColor.alpha = pd_clip_minmax(maxColor.alpha += contrast, 0., 1.);
    return maxColor;
}


