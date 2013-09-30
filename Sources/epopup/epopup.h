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

#ifndef DEF_EPOPUP
#define DEF_EPOPUP

#include "../estruct.h"
#include "../ebox/ebox.h"

t_epopupmenu* epopupmenu_create(t_ebox *x, t_symbol* name);
void epopupmenu_setfont(t_epopupmenu *menu, t_efont *font);
void epopupmenu_additem(t_epopupmenu *menu, int itemid, char *text, t_rgba *textColor, bool checked, bool disabled, void *icon);
void epopupmenu_addseperator(t_epopupmenu *menu);
void epopupmenu_popup(t_epopupmenu *menu, t_pt screen, int defitemid);
void epopupmenu_destroy(t_epopupmenu *menu);
int epopupmenu_mousemove(t_epopupmenu *menu, t_pt points, int mousedown);

#endif