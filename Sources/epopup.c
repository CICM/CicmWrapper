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

#include "epopup.h"

t_epopup* epopupmenu_create(t_eobj* x, t_symbol* name)
{
    t_epopup* popup = (t_epopup *)getbytes(sizeof(t_epopup));
    if(popup)
    {
        popup->c_send = x->o_id;
        popup->c_name = name;
        sys_vgui("destroy .eboxpopup%s\n", popup->c_name->s_name);
        sys_vgui("popup .eboxpopup%s -tearoff 0\n", popup->c_name->s_name);
    }    
    return popup;
}


void epopupmenu_setfont(t_epopup* popup, t_efont *font)
{
    sys_vgui(".eboxpopup%s configure -font {%s %d %s italic}\n", popup->c_name->s_name, font[0].c_family->s_name, (int)font[0].c_size, font[0].c_weight->s_name, font[0].c_slant->s_name);
}

void epopupmenu_additem(t_epopup* popup, int itemid, const char *text, char checked, char disabled)
{
    sys_vgui(".eboxpopup%s add command ", popup->c_name->s_name);
    sys_vgui("-command {pdsend {%s popup %s %f}} ", popup->c_send->s_name, popup->c_name->s_name, (float)itemid);
    sys_vgui("-label {%s} ", text);
    if(disabled)
        sys_vgui("-state disable\n");
    else
        sys_vgui("-state active\n");
}

void epopupmenu_addseperator(t_epopup* popup)
{
    sys_vgui(".eboxpopup%s add separator\n", popup->c_name->s_name);
}

void epopupmenu_popup(t_epopup* popup, t_pt pos)
{
    sys_vgui(".eboxpopup%s post %i %i\n", popup->c_name->s_name, (int)pos.x, (int)pos.y);
}


