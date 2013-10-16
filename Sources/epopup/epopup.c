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

t_epopup* epopupmenu_create(t_ebox* x, t_symbol* name)
{
    t_epopup* menu = (t_epopup *)malloc(sizeof(t_epopup));
    menu->c_send = x->e_name_rcv;
    menu->c_name = name;
    sys_vgui("destroy .eboxpopup%s\n", menu->c_name->s_name);
    sys_vgui("menu .eboxpopup%s -tearoff 0\n", menu->c_name->s_name);
    
    return menu;
}


void epopupmenu_setfont(t_epopup* menu, t_efont *font)
{
    sys_vgui(".eboxpopup%s configure -font {%s %d %s italic}\n", menu->c_name->s_name, font[0].c_family->s_name, (int)font[0].c_size, font[0].c_weight->s_name, font[0].c_slant->s_name);
}

void epopupmenu_additem(t_epopup* menu, int itemid, char *text, char checked, char disabled)
{
    sys_vgui(".eboxpopup%s add command ", menu->c_name->s_name);
    sys_vgui("-command {pdsend {%s popup %s %f}} ", menu->c_send->s_name, menu->c_name->s_name, (float)itemid);
    sys_vgui("-label {%s} ", text);
    if(disabled)
        sys_vgui("-state disable\n");
    else
        sys_vgui("-state active\n");
}

void epopupmenu_addseperator(t_epopup* menu)
{
    sys_vgui(".eboxpopup%s add separator\n", menu->c_name->s_name);
}

void epopupmenu_popup(t_epopup* menu, t_pt screen, int defitemid)
{
    sys_vgui(".eboxpopup%s post %i %i\n", menu->c_name->s_name, (int)screen.x, (int)screen.y);
}




