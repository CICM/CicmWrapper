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

//! Retrieve the global mouse position
/*
 \ @memberof        eobj
 \ @param x         The eobj pointer
 \ @return          This function return a point that contains the global abscissa and ordiante of the mouse
 */
t_pt eobj_get_mouse_global_position(void* x)
{
    return erouter_getmouse_global_position();
}

//! Retrieve the mouse status
/*
 \ @memberof        eobj
 \ @param x         The eobj pointer
 \ @return          This function return 1 is the mouse is down and 0 if not
 */
char eobj_get_mouse_status(void* x)
{
    return erouter_getmouse_status();
}

//! Retrieve the mouse status
/*
 \ @memberof        eobj
 \ @param x         The eobj pointer
 \ @return          This function return the mouse modifiers
 */
long eobj_get_mouse_modifier(void* x)
{
    return erouter_getmouse_modifier();
}

//! @cond

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
    t_binbuf* d;
    t_eclass* c = eobj_getclass(x);
    if(c && b)
    {
        binbuf_addv(b, "ssii", &s__X, s_obj, (t_int)x->o_obj.te_xpix, (t_int)x->o_obj.te_ypix);
        d = x->o_obj.te_binbuf;
        if(d)
        {
            binbuf_addbinbuf(b, d);
        }
        if(c->c_widget.w_save != NULL)
        {
            c->c_widget.w_save(x, b);
        }
        binbuf_addv(b, ";");
    }
}

//! The default save method for eobj called by PD (PRIVATE)
/*
 \ @memberof        eobj
 \ @param z         The eobj pointor
 \ @param b         The binbuf
 \ @return          Nothing
*/
void eobj_save(t_gobj* x, t_binbuf *b)
{
    t_eclass* c;
    if(x && b)
    {
        c = eobj_getclass(x);
        if(c && c->c_widget.w_dosave != NULL)
        {
            c->c_widget.w_dosave((t_eobj* )x, b);
        }
        
    }
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
void eobj_write(t_eobj* x, t_symbol* s, int argc, t_atom *argv)
{
    char buf[MAXPDSTRING];
    char* pch;
    t_atom av[1];
    t_eclass* c = eobj_getclass(x);
    
    // The file name is defined
    if(argc && argv && atom_gettype(argv) == A_SYM)
    {
        pch = strpbrk(atom_getsym(argv)->s_name, "/\"");
        // The folder seems defined
        if(pch != NULL)
        {
            atom_setsym(av, atom_getsym(argv));
            if(c->c_widget.w_write)
                c->c_widget.w_write(x, s, 1, av);
            return;
        }
        // The folder isn't defined so write it in the canvas folder
        else
        {
            sprintf(buf, "%s/%s", canvas_getdir(x->o_canvas)->s_name, atom_getsym(argv)->s_name);
            atom_setsym(av, gensym(buf));
            if(c->c_widget.w_write)
                c->c_widget.w_write(x, s, 1, av);
            return;
        }
    }
    // The file name is not defined so we popup a window
    else
    {
        sys_vgui("eobj_saveas %s nothing nothing\n", x->o_id->s_name);
    }
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
void eobj_read(t_eobj* x, t_symbol* s, int argc, t_atom *argv)
{
    char buf[MAXPDSTRING];
    char* pch;
    t_atom av[1];
    t_namelist* var;
    t_eclass* c = eobj_getclass(x);
    
    // Name
    if(argc && argv && atom_gettype(argv) == A_SYM)
    {
        // Valid path
        if((access(atom_getsym(argv)->s_name, O_RDONLY) != -1))
        {
            if(c->c_widget.w_read)
                c->c_widget.w_read(x, s, 1, argv);
        }
        // Invalid path or no path
        else
        {
            // Wrong path but we don't care
            pch = strpbrk(atom_getsym(argv)->s_name, "/\"");
            if(pch != NULL)
            {
                if(c->c_widget.w_read)
                    c->c_widget.w_read(x, s, 1, argv);
            }
            else
            {
                // Look in the canvas folder
                sprintf(buf, "%s/%s", canvas_getdir(x->o_canvas)->s_name, atom_getsym(argv)->s_name);
                if((access(buf, O_RDONLY) != -1))
                {
                    atom_setsym(av, gensym(buf));
                    if(c->c_widget.w_read)
                        c->c_widget.w_read(x, s, 1, av);
                    return;
                }
                // Look in the search path
                var = sys_searchpath;
                while (var)
                {
                    sprintf(buf, "%s/%s", var->nl_string, atom_getsym(argv)->s_name);
                    if((access(buf, O_RDONLY) != -1))
                    {
                        atom_setsym(av, gensym(buf));
                        if(c->c_widget.w_read)
                            c->c_widget.w_read(x, s, 1, av);
                        return;
                    }
                    var = var->nl_next;
                }
                
                // Nothing work but we don't care
                if(c->c_widget.w_read)
                    c->c_widget.w_read(x, s, 1, av);
                return;
            }
        }
    }
    // No name so we popup a window
    else
    {
        sys_vgui("eobj_openfrom %s\n", x->o_id->s_name);
    }
}

void eobj_poll_mouse(void* x)
{
    t_eobj* obj  = (t_eobj *)x;
    clock_set(obj->o_clock, 20.);
}


void eobj_nopoll_mouse(void* x)
{
    t_eobj* obj  = (t_eobj *)x;
    clock_unset(obj->o_clock);
}

void eobj_tick(t_eobj* x)
{
    t_eobj* obj  = (t_eobj *)x;
    t_ebox* box = (t_ebox*)x;
    if(eobj_isbox(x))
    {
        if(box->b_ready_to_draw && glist_isvisible(box->b_obj.o_canvas))
        {
            sys_vgui("eobj_canvas_mouse %s %s\n", box->b_obj.o_id->s_name, box->b_canvas_id->s_name);
            clock_delay(box->b_obj.o_clock, 20.);
        }
        else
        {
            eobj_nopoll_mouse(x);
        }
    }
    else
    {
        sys_vgui("eobj_canvas_mouse %s %s\n", obj->o_id->s_name, obj->o_canvas_id->s_name);
        clock_delay(obj->o_clock, 20.);
    }
}

//! @endcond






