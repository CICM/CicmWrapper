/*
 * CicmWrapper
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "ecolor.h"

static char ColBuf[10];
static const char HexDigits[] = "0123456789ABCDEF";

const t_rgba rgba_black = {0.f, 0.f, 0.f, 1.f};
const t_rgba rgba_greydark = {0.3f, 0.3, 0.3f, 1.f};
const t_rgba rgba_grey = {0.5f, 0.5, 0.5f, 1.f};
const t_rgba rgba_greylight = {0.8f, 0.8, 0.8f, 1.f};
const t_rgba rgba_white = {1.f, 1.f, 1.f, 1.f};
const t_rgba rgba_blue = {0.f, 0.f, 1.f, 1.f};
const t_rgba rgba_green = {0.f, 1.f, 0.f, 1.f};
const t_rgba rgba_red = {1.f, 0.f, 0.f, 1.f};

void rgba_set(t_rgba *color, const float red, const float green, const float blue, const float alpha)
{
    color->red      = (red < 0.f) ? 0.f : ((red > 1.f) ? 1.f : red);
    color->green    = (green < 0.f) ? 0.f : ((green > 1.f) ? 1.f : green);
    color->blue     = (blue < 0.f) ? 0.f : ((blue > 1.f) ? 1.f : blue);
    color->alpha    = (alpha < 0.f) ? 0.f : ((alpha > 1.f) ? 1.f : alpha);
}

void rgb_set(t_rgb *color, const float red, const float green, const float blue)
{
    color->red      = (red < 0.f) ? 0.f : ((red > 1.f) ? 1.f : red);
    color->green    = (green < 0.f) ? 0.f : ((green > 1.f) ? 1.f : green);
    color->blue     = (blue < 0.f) ? 0.f : ((blue > 1.f) ? 1.f : blue);
}

void hsla_set(t_hsla *color, const float hue, const float saturation, const float lightness, const float alpha)
{
    color->hue          = (hue < 0.f) ? 0.f : ((hue > 1.f) ? 1.f : hue);
    color->saturation   = (saturation < 0.f) ? 0.f : ((saturation > 1.f) ? 1.f : saturation);
    color->lightness    = (lightness < 0.f) ? 0.f : ((lightness > 1.f) ? 1.f : lightness);
    color->alpha        = (alpha < 0.f) ? 0.f : ((alpha > 1.f) ? 1.f : alpha);
}

void hsl_set(t_hsl *color, const float hue, const float saturation, const float lightness)
{
    color->hue          = (hue < 0.f) ? 0.f : ((hue > 1.f) ? 1.f : hue);
    color->saturation   = (saturation < 0.f) ? 0.f : ((saturation > 1.f) ? 1.f : saturation);
    color->lightness    = (lightness < 0.f) ? 0.f : ((lightness > 1.f) ? 1.f : lightness);
}

char* rgba_to_hex(t_rgba const* color)
{
    int r = (int)(color->red * 255.f);
    int g = (int)(color->green * 255.f);
    int b = (int)(color->blue * 255.f);
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

char* rgb_to_hex(t_rgb const* color)
{
    int r = (int)(color->red * 255.f);
    int g = (int)(color->green * 255.f);
    int b = (int)(color->blue * 255.f);
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

char* hsla_to_hex(t_hsla const* color)
{
    t_rgba ncolor = hsla_to_rgba(color);
    return rgba_to_hex(&ncolor);
}

char* hsl_to_hex(t_hsl const* color)
{
    t_rgb ncolor = hsl_to_rgb(color);
    return rgb_to_hex(&ncolor);
}

t_rgba hex_to_rgba(char const* color)
{
    int hexvalue = (int)strtol(color+1, NULL, 16);
    t_rgba ncolor;
    ncolor.red = (float)((hexvalue >> 16) & 0xFF) / 255.f;
    ncolor.green = (float)((hexvalue >> 8) & 0xFF) / 255.f;
    ncolor.blue = (float)((hexvalue) & 0xFF) / 255.f;
    ncolor.alpha = 1.f;
    return ncolor;
}

t_rgb hex_to_rgb(char const* color)
{
    int hexvalue = (int)strtol(color+1, NULL, 16);
    t_rgb ncolor;
    ncolor.red = (float)((hexvalue >> 16) & 0xFF) / 255.f;
    ncolor.green = (float)((hexvalue >> 8) & 0xFF) / 255.f;
    ncolor.blue = (float)((hexvalue) & 0xFF) / 255.f;
    return ncolor;
}

t_hsla rgba_to_hsla(t_rgba const* color)
{
    t_hsla ncolor = {0., 0., 0., 0.};
    float delta, deltar, deltag, deltab;
    float max = color->red;
    float min = color->red;
    if(min > color->green)
        min = color->green;
    if(min > color->blue)
        min = color->blue;
    if(max < color->green)
        max = color->green;
    if(max < color->blue)
        max = color->blue;
    delta = max - min;
    ncolor.alpha = color->alpha;
    ncolor.lightness = (max + min) / 2.f;
    if(max == 0)
    {
        ncolor.hue = 0;
        ncolor.saturation = 0;
    }
    else
    {
        if(ncolor.lightness < 0.5f)
            ncolor.saturation = delta / (max + min);
        else
            ncolor.saturation = delta / (2.f - max - min);
        
        deltar = (((max - color->red ) / 6 ) + (delta / 2)) / delta;
        deltag = (((max - color->green ) / 6 ) + (delta / 2)) / delta;
        deltab = (((max - color->blue ) / 6 ) + (delta / 2)) / delta;
        
        if(color->red == max)
            ncolor.hue = deltab - deltag;
        else if(color->green == max)
            ncolor.hue = (1.f / 3.f) + deltar - deltab;
        else if(color->blue == max)
            ncolor.hue = (2.f / 3.f) + deltag - deltar;
        
        if(ncolor.hue < 0.f)
            ncolor.hue += 1;
        if(ncolor.hue > 1.f)
            ncolor.hue -= 1;
    }
    
    return ncolor;
}

t_hsl rgb_to_hsl(t_rgb const* color)
{
    t_hsl ncolor = {0., 0., 0.};
    float delta, deltar, deltag, deltab;
    float max = color->red;
    float min = color->red;
    if(min > color->green)
        min = color->green;
    if(min > color->blue)
        min = color->blue;
    if(max < color->green)
        max = color->green;
    if(max < color->blue)
        max = color->blue;
    delta = max - min;
    
    ncolor.lightness = (max + min) / 2.f;
    if(max == 0)
    {
        ncolor.hue = 0;
        ncolor.saturation = 0;
    }
    else
    {
        if(ncolor.lightness < 0.5f)
            ncolor.saturation = delta / (max + min);
        else
            ncolor.saturation = delta / (2.f - max - min);
        
        deltar = (((max - color->red ) / 6 ) + (delta / 2)) / delta;
        deltag = (((max - color->green ) / 6 ) + (delta / 2)) / delta;
        deltab = (((max - color->blue ) / 6 ) + (delta / 2)) / delta;
        
        if(color->red == max)
            ncolor.hue = deltab - deltag;
        else if(color->green == max)
            ncolor.hue = (1.f / 3.f) + deltar - deltab;
        else if(color->blue == max)
            ncolor.hue = (2.f / 3.f) + deltag - deltar;
        
        if(ncolor.hue < 0.f)
            ncolor.hue += 1;
        if(ncolor.hue > 1.f)
            ncolor.hue -= 1;
    }
    
    return ncolor;
}

static float Hue_2_RGB(float v1,float v2,float vH)
{
    if(vH < 0.f)
        vH += 1.;
    if(vH > 1.f)
        vH -= 1.;
    if((6. * vH) < 1.f)
        return (v1 + (v2 - v1) * 6.f * vH);
    if((2. * vH) < 1.f)
        return v2;
    if(( 3 * vH) < 2.f)
        return (v1 + (v2 - v1) * ((2.f / 3.f) - vH) * 6.f);
    return v1;
}

t_rgb hsl_to_rgb(t_hsl const* color)
{
    float var1, var2;
    t_rgb ncolor;
    if(color->saturation == 0.f)
    {
        ncolor.red = color->lightness;
        ncolor.green = color->lightness;
        ncolor.blue = color->lightness;
    }
    else
    {
        if(color->lightness < 0.5f)
            var2 = color->lightness * (1.f + color->saturation);
        else
            var2 = (color->lightness + color->saturation) - (color->saturation * color->lightness);
        
        var1 = 2 * color->lightness - var2;
        
        ncolor.red = Hue_2_RGB(var1, var2, color->hue + (1.f / 3.f));
        ncolor.green = Hue_2_RGB(var1, var2, color->hue);
        ncolor.blue = Hue_2_RGB(var1, var2, color->hue - (1.f / 3.f));
    }
    return ncolor;
}

t_rgba hsla_to_rgba(t_hsla const* color)
{
    float var1, var2;
    t_rgba ncolor;
    if(color->saturation == 0.f)
    {
        ncolor.red = color->lightness;
        ncolor.green = color->lightness;
        ncolor.blue = color->lightness;
    }
    else
    {
        if(color->lightness < 0.5f)
            var2 = color->lightness * (1.f + color->saturation);
        else
            var2 = (color->lightness + color->saturation) - (color->saturation * color->lightness);
        
        var1 = 2 * color->lightness - var2;
        
        ncolor.red = Hue_2_RGB(var1, var2, color->hue + (1.f / 3.f));
        ncolor.green = Hue_2_RGB(var1, var2, color->hue);
        ncolor.blue = Hue_2_RGB(var1, var2, color->hue - (1.f / 3.f));
    }
    
    ncolor.alpha = color->alpha;
    return ncolor;
}

t_rgba rgba_add_contrast(t_rgba const* color, float contrast)
{
    t_rgba new_color;
    rgba_set(&new_color, color->red + contrast, color->green + contrast, color->blue + contrast, color->alpha);
    return new_color;
}

t_rgb rgb_add_contrast(t_rgb const* color, float contrast)
{
    t_rgb new_color;
    rgb_set(&new_color, color->red + contrast, color->green + contrast, color->blue + contrast);
    return new_color;
}

char rgba_is_equal(t_rgba const* color, t_rgba const* other)
{
    return color->red == other->red && color->green == other->green &&
    color->blue == other->blue && color->alpha == other->alpha;
}

