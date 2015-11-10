/*
 * CicmWrapper
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "etextlayout.h"
#include "eguiimp.h"

t_etextlayout* etextlayout_new(void)
{
    t_etextlayout* txt = (t_etextlayout *)malloc(sizeof(t_etextlayout));
    if(txt)
    {
        txt->c_text = (char *)malloc(MAXPDSTRING * sizeof(char));
        if(txt->c_text)
        {
            memset(txt->c_text, 0, MAXPDSTRING * sizeof(char));
            txt->c_color        = rgba_black;
            return txt;
        }
        else
        {
            free(txt);
            return NULL;
        }
    }
    return NULL;
}

void etextlayout_copy(t_etextlayout* textlayout, t_etextlayout const* other)
{
    memcpy(textlayout->c_text, other->c_text, sizeof(char) * MAXPDSTRING);
    textlayout->c_font          = other->c_font;
    textlayout->c_rect          = other->c_rect;
    textlayout->c_wrap          = other->c_wrap;
    textlayout->c_justify       = other->c_justify;
}

void etextlayout_destroy(t_etextlayout* textlayout)
{
    free(textlayout->c_text);
    free(textlayout);
}

void etextlayout_set(t_etextlayout* textlayout, const char* text, t_efont const* font,
                     const float x, const float y, const float width,  const float height,
                     const etextjustify_flags justify, const etextwrap_flags wrap)
{
    strncpy(textlayout->c_text, text, MAXPDSTRING);
    textlayout->c_font          = *font;
    textlayout->c_rect.x        = (float)x;
    textlayout->c_rect.y        = (float)y;
    textlayout->c_rect.width    = (float)width;
    textlayout->c_rect.height   = (float)height;
    textlayout->c_wrap          = wrap;
    textlayout->c_justify       = justify;
}

void etextlayout_settextcolor(t_etextlayout* textlayout, t_rgba const* color)
{
    textlayout->c_color = *color;
}


