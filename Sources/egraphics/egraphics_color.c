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

char* hsla_to_hex(t_hsla color)
{
    t_rgba ncolor = hsla_to_rgba(color);
    return rgba_to_hex(ncolor);
}

char* hsl_to_hex(t_hsl color)
{
    t_rgb ncolor = hsl_to_rgb(color);
    return rgb_to_hex(ncolor);
}

t_rgba hex_to_rgba(char* color)
{
    int hexvalue = atoi(color+1);
    t_rgba ncolor;
    ncolor.red = ((hexvalue >> 16) & 0xFF) / 255.0;
    ncolor.green = ((hexvalue >> 8) & 0xFF) / 255.0;
    ncolor.blue = ((hexvalue) & 0xFF) / 255.0;
    ncolor.alpha = 1;
    return ncolor;
}

t_rgb hex_to_rgb(char* color)
{
    int hexvalue = atoi(color+1);
    t_rgb ncolor;
    ncolor.red = ((hexvalue >> 16) & 0xFF) / 255.0;
    ncolor.green = ((hexvalue >> 8) & 0xFF) / 255.0;
    ncolor.blue = ((hexvalue) & 0xFF) / 255.0;
    return ncolor;
}

t_hsla rgba_to_hsla(t_rgba color)
{
    t_hsla ncolor;
    float delta, deltar, deltag, deltab;
    float max = color.red;
    float min = color.red;
    if(min > color.green)
        min = color.green;
    if(min > color.blue)
        min = color.blue;
    if(max < color.green)
        max = color.green;
    if(max < color.blue)
        max = color.blue;
    delta = max - min;
    ncolor.alpha = color.alpha;
    ncolor.lightness = (max + min) / 2.;
    if(max == 0)
    {
        ncolor.hue = 0;
        ncolor.saturation = 0;
    }
    else
    {
        if(ncolor.lightness < 0.5)
            ncolor.saturation = delta / (max + min);
        else
            ncolor.saturation = delta / (2. - max - min);
        
        deltar = (((max - color.red ) / 6 ) + (delta / 2)) / delta;
        deltag = (((max - color.green ) / 6 ) + (delta / 2)) / delta;
        deltab = (((max - color.blue ) / 6 ) + (delta / 2)) / delta;
        
        if(color.red == max)
            ncolor.hue = deltab - deltag;
        else if(color.green == max)
            ncolor.hue = (1. / 3.) + deltar - deltab;
        else if(color.blue == max)
            ncolor.hue = (2. / 3.) + deltag - deltar;
                    
        if(ncolor.hue < 0.)
            ncolor.hue += 1;
        if(ncolor.hue > 1.)
            ncolor.hue -= 1;
    }
    
    return ncolor;
}

t_hsl rgb_to_hsl(t_rgb color)
{
    t_hsl ncolor;
    float delta, deltar, deltag, deltab;
    float max = color.red;
    float min = color.red;
    if(min > color.green)
        min = color.green;
    if(min > color.blue)
        min = color.blue;
    if(max < color.green)
        max = color.green;
    if(max < color.blue)
        max = color.blue;
    delta = max - min;
    ncolor.lightness = (max + min) / 2.;
    if(max == 0)
    {
        ncolor.hue = 0;
        ncolor.saturation = 0;
    }
    else
    {
        if(ncolor.lightness < 0.5)
            ncolor.saturation = delta / (max + min);
        else
            ncolor.saturation = delta / (2. - max - min);
        
        deltar = (((max - color.red ) / 6 ) + (delta / 2)) / delta;
        deltag = (((max - color.green ) / 6 ) + (delta / 2)) / delta;
        deltab = (((max - color.blue ) / 6 ) + (delta / 2)) / delta;
        
        if(color.red == max)
            ncolor.hue = deltab - deltag;
        else if(color.green == max)
            ncolor.hue = (1. / 3.) + deltar - deltab;
        else if(color.blue == max)
            ncolor.hue = (2. / 3.) + deltag - deltar;
        
        if(ncolor.hue < 0.)
            ncolor.hue += 1;
        if(ncolor.hue > 1.)
            ncolor.hue -= 1;
    }
    
    return ncolor;
}

float Hue_2_RGB(float v1,float v2,float vH)
{
    if(vH < 0.)
        vH += 1.;
    if(vH > 1.)
        vH -= 1.;
    if((6. * vH) < 1.)
        return (v1 + (v2 - v1) * 6. * vH);
    if((2. * vH) < 1.)
        return v2;
    if(( 3 * vH) < 2.)
        return (v1 + (v2 - v1) * ((2. / 3.) - vH) * 6.);
    return v1;
}

t_rgb hsl_to_rgb(t_hsl color)
{
    float var1, var2;
    t_rgb ncolor;
    if(color.saturation == 0.)
    {
        ncolor.red = color.lightness;
        ncolor.green = color.lightness;
        ncolor.blue = color.lightness;
    }
    else
    {
        if(color.lightness < 0.5)
            var2 = color.lightness * (1. + color.saturation);
        else
            var2 = (color.lightness + color.saturation) - (color.saturation * color.lightness);
                
        var1 = 2 * color.lightness - var2;
                
        ncolor.red = Hue_2_RGB(var1, var2, color.hue + (1. / 3.));
        ncolor.green = Hue_2_RGB(var1, var2, color.hue);
        ncolor.blue = Hue_2_RGB(var1, var2, color.hue - (1. / 3.));
    }
    return ncolor;
}

t_rgba hsla_to_rgba(t_hsla color)
{
    float var1, var2;
    t_rgba ncolor;
    if(color.saturation == 0.)
    {
        ncolor.red = color.lightness;
        ncolor.green = color.lightness;
        ncolor.blue = color.lightness;
    }
    else
    {
        if(color.lightness < 0.5)
            var2 = color.lightness * (1. + color.saturation);
        else
            var2 = (color.lightness + color.saturation) - (color.saturation * color.lightness);
        
        var1 = 2. * color.lightness - var2;
        
        ncolor.red = Hue_2_RGB(var1, var2, color.hue + (1. / 3.));
        ncolor.green = Hue_2_RGB(var1, var2, color.hue);
        ncolor.blue = Hue_2_RGB(var1, var2, color.hue - (1. / 3.));
    }
    
    ncolor.alpha = color.alpha;
    return ncolor;
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

void hsla_set(t_hsla *color, float hue, float saturation, float lightness, float alpha)
{
    color->hue = hue;
    color->saturation = saturation;
    color->lightness = lightness;
    color->alpha = alpha;
}

void hsl_set(t_hsl *color, float hue, float saturation, float lightness)
{
    color->hue = hue;
    color->saturation = saturation;
    color->lightness = lightness;
}
