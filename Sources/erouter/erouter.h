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

#ifndef DEF_EROUTER
#define DEF_EROUTER

#include "../estruct.h"
#include "../egraphics/egraphics.h"
#include "../ewidget/ewidget.h"

t_class *erouter_class;

typedef struct _erouter
{
    t_object            e_obj;
    t_symbol**          e_childs;
    long                e_nchilds;
}t_erouter;

void ebox_router(t_ebox* x, t_symbol* s, long argc, t_atom* argv);
void erouter_setup();
void erouter_free(t_erouter *x);
void erouter_attach(t_erouter *x, t_symbol* child);
void erouter_detach(t_erouter *x, t_symbol* child);
void erouter_anything(t_erouter *x, t_symbol *s, long argc, t_atom *argv);

#endif