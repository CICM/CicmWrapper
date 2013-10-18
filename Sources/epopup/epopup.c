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

static t_symbol* popupme;

t_epopupmenu* epopupmenu_create(t_symbol* name)
{
	/*
    sys_vgui("destroy %s\n", popupme->s_name);
    popupme = name;
    sys_vgui("menubutton %s\n", name->s_name);
    
    t_epopupmenu* popmenu = malloc(sizeof(t_epopupmenu));
    popmenu->c_name = name;
    return popmenu;
	*/
}


void epopupmenu_setfont(t_epopupmenu *menu, t_efont *font)
{
    menu->c_font = font[0];
}

void epopupmenu_additem(t_epopupmenu *menu, int itemid, char *text, t_rgba *textColor, char checked, char disabled, void *icon)
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
    menu->c_items = realloc(menu->c_items, sizeof(t_epopupitem) * 1);
}

void epopupmenu_addseperator(t_epopupmenu* menu)
{
    t_epopupitem item;
    item.c_separator = 1;
    item.c_text = gensym("");
}

void epopupmenu_popup(t_epopupmenu* menu, t_pt screen, int defitemid)
{
    ;
}

void epopupmenu_destroy(t_epopupmenu *menu)
{
    free(menu);    
}

int epopupmenu_mousemove(t_epopupmenu *menu, t_pt pt, int mousedown)
{
    ;
}





