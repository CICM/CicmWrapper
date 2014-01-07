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

#include "eobj.h"

static t_pt mouse_global_pos;
static t_pt mouse_patcher_pos;

//! Retrieve the global mouse position
/*
 \ @memberof        eobj
 \ @param x         The eobj pointer
 \ @return          This function return a point that contains the global abscissa and ordiante of the mouse
 */
t_pt eobj_get_mouse_global_position(void* x)
{
    t_eobj* z  = (t_eobj *)x;
	sys_vgui("eobj_gmousepos %s\n", z->o_id->s_name);
    return mouse_global_pos;
}

//! Retrieve the canvas mouse position
/*
 \ @memberof        eobj
 \ @param x         The eobj pointer
 \ @return          This function return a point that contains abscissa and ordinate of the mouse in the ebox's canvas
 */
t_pt eobj_get_mouse_canvas_position(void* x)
{
    t_eobj* z  = (t_eobj *)x;
    t_pt point;
    sys_vgui("eobj_pmousepos %s %s\n", z->o_id->s_name, z->o_canvas_id->s_name);
    point.x = mouse_global_pos.x - mouse_patcher_pos.x;
    point.y = mouse_global_pos.y - mouse_patcher_pos.y;
    return point;
}

//! @cond

//! The global mouse position method called by tcl/tk (PRIVATE)
/*
 \ @memberof        eobj
 \ @param x         The eobj pointer
 \ @param x_p       The abscissa of the mouse position
 \ @param y_p       The ordinate of the mouse position
 \ @return          Nothing
*/
void eobj_retreive_gmouse(t_eobj* x, float x_p, float y_p)
{
    mouse_global_pos.x = x_p;
    mouse_global_pos.y = y_p;
}

//! The canvas mouse position method called by tcl/tk (PRIVATE)
/*
 \ @memberof        eobj
 \ @param x         The eobj pointer
 \ @param x_p       The abscissa of the mouse position
 \ @param y_p       The ordinate of the mouse position
 \ @return          Nothing
*/
void eobj_retreive_pmouse(t_eobj* x, float x_p, float y_p)
{
    mouse_patcher_pos.x = x_p;
    mouse_patcher_pos.y = y_p;
}

//! The popup method called by tcl/tk (PRIVATE)
/*
 \ @memberof        eobj
 \ @param x         The eobj pointer
 \ @param s         The message selector
 \ @param itemid    the id of the selected item
 \ @return          Nothing
*/
void eobj_popup(t_eobj* x, t_symbol* s, float itemid)
{
    t_eclass* c = eobj_getclass(x);

    if(c->c_widget.w_popup)
        c->c_widget.w_popup(x, s, (long)itemid);
}

//! The default save method for UI ebox (PRIVATE)
/*
 \ @memberof        eobj
 \ @param z         The eobj pointor
 \ @param b         The binbuf
 \ @return          Nothing
*/
void eobj_dosave(t_eobj* x, t_binbuf *b)
{
    t_eclass* c = eobj_getclass(x);
    
    binbuf_addv(b, "ssii", gensym("#X"), gensym("obj"), (t_int)x->o_obj.te_xpix, (t_int)x->o_obj.te_ypix);
    binbuf_add(b, binbuf_getnatom(x->o_obj.te_binbuf), binbuf_getvec(x->o_obj.te_binbuf));
    
    if(c->c_widget.w_save != NULL)
        c->c_widget.w_save(x, b);
    
    binbuf_addv(b, ";");
}

//! The default save method for eobj called by PD (PRIVATE)
/*
 \ @memberof        eobj
 \ @param z         The eobj pointor
 \ @param b         The binbuf
 \ @return          Nothing
*/
void eobj_save(t_eobj* x, t_binbuf *b)
{
    eobj_getclass(x)->c_widget.w_dosave(x, b);
}

//! The default write method for all eobj called by PD (PRIVATE)
/*
 \ @memberof        eobj
 \ @param x         The eobj pointor
 \ @param s         The symbol selector
 \ @param argc      The size of the array of atoms
 \ @param argv      The array of atoms
 \ @return          Nothing
 */
void eobj_write(t_eobj* x, t_symbol* s, long argc, t_atom* argv)
{
    char buf[MAXPDSTRING];
    char* pch;
    t_atom av[1];
    t_eclass* c = eobj_getclass(x);
    
    if(argc && argv && atom_gettype(argv) == A_SYM)
    {
        pch = strpbrk(atom_getsym(argv)->s_name, "/\"");
        if(pch != NULL)
        {
            atom_setsym(av, atom_getsym(argv));
            if(c->c_widget.w_write)
                c->c_widget.w_write(x, s, 1, av);
            return;
        }
        else
        {
            sprintf(buf, "%s/%s", canvas_getdir(x->o_canvas)->s_name, atom_getsym(argv)->s_name);
            atom_setsym(av, gensym(buf));
            if(c->c_widget.w_write)
                c->c_widget.w_write(x, s, 1, av);
            return;
        }
    }
    sys_vgui("eobj_saveas %s nothing nothing\n", x->o_id->s_name);
}

//! The default read method for all eobj called by PD (PRIVATE)
/*
 \ @memberof        eobj
 \ @param x         The eobj pointor
 \ @param s         The symbol selector
 \ @param argc      The size of the array of atoms
 \ @param argv      The array of atoms
 \ @return          Nothing
 */
void eobj_read(t_eobj* x, t_symbol* s, long argc, t_atom* argv)
{
    char buf[MAXPDSTRING];
    char* pch;
    t_atom av[1];
    t_eclass* c = eobj_getclass(x);
    
    if(argc && argv && atom_gettype(argv) == A_SYM)
    {
        pch = strpbrk(atom_getsym(argv)->s_name, "/\"");
        if(pch != NULL)
        {
            atom_setsym(av, atom_getsym(argv));
            if(c->c_widget.w_read)
                c->c_widget.w_read(x, s, 1, av);
            return;
        }
        else
        {
            sprintf(buf, "%s/%s", canvas_getdir(x->o_canvas)->s_name, atom_getsym(argv)->s_name);
            atom_setsym(av, gensym(buf));
            if(c->c_widget.w_read)
                c->c_widget.w_read(x, s, 1, av);
            return;
        }
    }
    sys_vgui("eobj_openfrom %s\n", x->o_id->s_name);
}

//! @endcond






