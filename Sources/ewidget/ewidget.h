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

#ifndef DEF_EWIDGET
#define DEF_EWIDGET

#include "../estruct.h"
#include "../ebox/ebox.h"
#include "../epopup/epopup.h"

void ewidget_init(t_eclass* c);
void ewidget_getrect(t_gobj *z,     t_glist *glist, int *xp1, int *yp1, int *xp2, int *yp2);
void ewidget_vis(t_gobj *z,         t_glist *glist, int vis);
void ewidget_displace(t_gobj *z,    t_glist *glist, int dx, int dy);
void ewidget_select(t_gobj *z,      t_glist *glist, int selected);
void ewidget_delete(t_gobj *z,      t_glist *glist);
#endif 