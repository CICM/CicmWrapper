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

#include "epopup.h"

t_epopupmenu* epopupmenu_create(t_ebox *x, t_symbol* name)
{
    t_epopupmenu* popmenu = new t_epopupmenu[1];
    popmenu->c_name = name;
    popmenu->c_obj = (t_object *)x;
    x->e_popup = popmenu;
    return popmenu;
}

void epopupmenu_setfont(t_epopupmenu *menu, t_efont *font)
{
    menu->c_font = font[0];
}

void epopupmenu_additem(t_epopupmenu *menu, int itemid, char *text, t_rgba *textColor, bool checked, bool disabled, void *icon)
{
    t_epopupitem item;
    item.c_id = itemid;
    item.c_text = gensym(text);
    if(textColor != NULL)
        item.c_color = textColor[0];
    else
        item.c_color = rgba_black;
    item.c_checked = checked;
    item.c_disable = disabled;
    item.c_separator = 0;
    menu->c_items.push_back(item);
}

void epopupmenu_addseperator(t_epopupmenu *menu)
{
    t_epopupitem item;
    item.c_separator = 1;
    item.c_text = gensym("");
    menu->c_items.push_back(item);
}

void epopupmenu_popup(t_epopupmenu *menu, t_pt screen, int defitemid)
{
    t_ebox* x =(t_ebox *)menu->c_obj;
    menu->c_rect.x = screen.x;
    menu->c_rect.y = screen.y;
    long max_width = 0;
    for(int i = 0; i < menu->c_items.size(); i++)
    {
        if(max_width < strlen(menu->c_items[i].c_text->s_name))
        {
            max_width = strlen(menu->c_items[i].c_text->s_name);
        }
    }
    menu->c_rect.width = (menu->c_font.c_size / 1.5) * max_width + 7.;
    menu->c_rect.height = menu->c_font.c_size * 1.5 * (menu->c_items.size() + 1);
    
    t_elayer* g = ebox_start_layer((t_object *)x, (t_object *)x->e_canvas, gensym("epopupmenu"), menu->c_rect.width, menu->c_rect.height);
    if(g)
    {
        t_etext *jtl = etext_layout_create();
        egraphics_set_line_width(g, 1.);
        egraphics_set_color_rgba(g, &rgba_greylight);
        egraphics_rectangle(g, 0., 0., menu->c_rect.width, menu->c_rect.height);
        egraphics_fill_preserve(g);
        egraphics_set_color_rgba(g, &rgba_black);
        egraphics_stroke(g);
        
        for(int i = 0; i < menu->c_items.size(); i++)
        {
            double y  = menu->c_font.c_size * 1.5 * (i + 1);
            if(menu->c_items[i].c_separator)
            {
                egraphics_move_to(g, 0., y);
                egraphics_line_to(g, menu->c_rect.width, y);
                egraphics_fill(g);
            }
            else
            {
                if(!menu->c_items[i].c_disable)
                    etext_layout_settextcolor(jtl, &menu->c_items[i].c_color);
                else
                    etext_layout_settextcolor(jtl, &rgba_greydark);
                
                etext_layout_set(jtl, menu->c_items[i].c_text->s_name, &menu->c_font, 7., y, 0., 0., ETEXT_LEFT, ETEXT_NOWRAP);
                etext_layout_draw(jtl, g);
            }
        }
        ebox_end_layer((t_object *)menu->c_obj, (t_object *)x->e_canvas, gensym("epopupmenu"));
    }
    if(menu->c_rect.x + menu->c_rect.width >= x->e_rect.width)
    {
        menu->c_rect.x -= menu->c_rect.width;
    }
    if(menu->c_rect.y + menu->c_rect.height >= x->e_rect.height)
    {
        menu->c_rect.y -= menu->c_rect.height;
    }
        
    ebox_paint_layer((t_object *)x, (t_object *)x->e_canvas, gensym("epopupmenu"), menu->c_rect.x , menu->c_rect.y);
}

void epopupmenu_destroy(t_epopupmenu *menu)
{
    menu->c_items.clear();
    t_ebox* x = (t_ebox*)menu->c_obj;
    x->e_popup = NULL;
    free(menu);    
}

int epopupmenu_mousemove(t_epopupmenu *menu, t_pt pt, int mousedown)
{
    t_ebox* x   = (t_ebox *)menu->c_obj;
    t_eclass* c = (t_eclass *)x->e_obj.te_g.g_pd;
    if(menu)
    {
        ebox_invalidate_layer((t_object *)x, (t_object *)x->e_canvas, gensym("epopuplight"));
        ebox_update(x, x->e_canvas);
        double ratio = (menu->c_rect.height  - 7. ) / (double)menu->c_items.size();
        for(int i = 0; i < menu->c_items.size(); i++)
        {
            double y1  = ratio * i;
            double y2  = ratio * (i + 1);
            if(pt.x > menu->c_rect.x && pt.x < menu->c_rect.x + menu->c_rect.width && pt.y > y1 + menu->c_rect.y && pt.y < y2  + menu->c_rect.y)
            {
                if(!menu->c_items[i].c_disable && !menu->c_items[i].c_separator)
                {
                    t_elayer* g = ebox_start_layer((t_object *)x, (t_object *)x->e_canvas, gensym("epopuplight"), menu->c_rect.width, menu->c_rect.height);
                    t_etext *jtl = etext_layout_create();
                    if(g)
                    {
                        egraphics_set_line_width(g, 1.);
                        egraphics_set_color_rgba(g, &rgba_bluelight);
                        egraphics_rectangle(g, 1., y1, menu->c_rect.width-1, ratio);
                        egraphics_fill(g);
                        
                        etext_layout_settextcolor(jtl, &menu->c_items[i].c_color);
                        etext_layout_set(jtl, menu->c_items[i].c_text->s_name, &menu->c_font, 7., y2 - 7., 0., 0., ETEXT_LEFT, ETEXT_NOWRAP);
                        etext_layout_draw(jtl, g);
                        
                        ebox_end_layer((t_object *)menu->c_obj, (t_object *)x->e_canvas, gensym("epopuplight"));
                    }
                    ebox_paint_layer((t_object *)x, (t_object *)x->e_canvas, gensym("epopuplight"), menu->c_rect.x , menu->c_rect.y);
                }
                if(mousedown)
                {
                    ebox_invalidate_layer((t_object *)x, (t_object *)x->e_canvas, gensym("epopuplight"));
                    ebox_invalidate_layer((t_object *)x, (t_object *)x->e_canvas, gensym("epopupmenu"));
                    c->c_widget.w_popup(x, menu->c_name, menu->c_items[i].c_id, pt);
                    menu->c_items.clear();
                    x->e_popup = NULL;
                    free(menu);
                    return 1;
                }
            }
        }
        if(mousedown)
        {   
            ebox_invalidate_layer((t_object *)x, (t_object *)x->e_canvas, gensym("epopupmenu"));
            menu->c_items.clear();
            x->e_popup = NULL;
            free(menu);
            ebox_redraw(x);
        }
        return 1;
    }
    else
    {
        return 0;
    }    
}





