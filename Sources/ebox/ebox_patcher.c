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

#include "ebox.h"

//! Get the patcher notification when editmode has changed (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The ebox pointer
 \ @return          Nothing
*/
void ebox_patcher_editmode(t_ebox* x, t_symbol* s, int argc, t_atom *argv)
{
    ebox_redraw(x);
    if(x->b_obj.o_canvas->gl_edit)
        eobj_poll_mouse(x);
    else
        eobj_nopoll_mouse(x);
}

//! Get the patcher notification when mouse has moved (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The ebox pointer
 \ @return          Nothing
*/
void ebox_patcher_motion(t_ebox* x, t_symbol* s, int argc, t_atom *argv)
{
	;
}

//! Get the patcher notification when keys has been pressed (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The ebox pointer
 \ @return          Nothing
*/
void ebox_patcher_key(t_ebox* x, t_symbol* s, int argc, t_atom *argv)
{
	;
}

//! Get the patcher notification when mouse has been pressed (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The ebox pointer
 \ @return          Nothing
*/
void ebox_patcher_mouse(t_ebox* x, t_symbol* s, int argc, t_atom *argv)
{
    ;
}

//! Get the patcher notification when focus has changed (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The ebox pointer
 \ @return          Nothing
*/
void ebox_patcher_focus(t_ebox* x, t_symbol* s, int argc, t_atom *argv)
{
    ebox_deserted(x);
}
