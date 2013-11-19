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

#include "ebox.h"

void ebox_patcher_editmode(t_ebox* x, t_symbol* s, long argc, t_atom* argv)
{
    ebox_redraw(x);
}

void ebox_patcher_motion(t_ebox* x, t_symbol* s, long argc, t_atom* argv)
{
	;
}

void ebox_patcher_key(t_ebox* x, t_symbol* s, long argc, t_atom* argv)
{
	;
}

void ebox_patcher_mouse(t_ebox* x, t_symbol* s, long argc, t_atom* argv)
{
    ;
}

void ebox_patcher_focus(t_ebox* x, t_symbol* s, long argc, t_atom* argv)
{
    ;
}
