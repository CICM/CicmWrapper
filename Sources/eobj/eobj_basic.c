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

/*!
 * \fn          void *eobj_new(t_eclass *c)
 * \brief       The eobj creation function
 * \details     Allocate the memory for an intance of an eobj that contains a specific eclass, intialize the standard values and methods and attach it to the router.
 * \param c     The eclass pointer
 * \return      This function return a new instance of an ebox
 */
void *eobj_new(t_eclass *c)
{
    t_eobj* x = NULL;
    char buffer[MAXPDSTRING];
    if(c)
    {
        x  = (t_eobj *)t_getbytes(c->c_class.c_size);
        if(x)
        {
            *((t_pd *)x)  = (t_pd)c;
            
            if(c->c_class.c_patchable)
            {
                ((t_object *)x)->ob_inlet = 0;
                ((t_object *)x)->ob_outlet = 0;
            }
            
            x->o_nproxy = 0;
            x->o_proxy = NULL;
            x->o_canvas = canvas_getcurrent();
            sprintf(buffer,"#%s%lx", c->c_class.c_name->s_name, (long unsigned int)x);
            x->o_id = gensym(buffer);
            pd_bind(&x->o_obj.ob_pd, x->o_id);
            sprintf(buffer,".x%lx.c", (long unsigned int)x->o_canvas);
            c->c_widget.w_dosave = (method)eobj_dosave;
        }
        else
        {
            bug("pd_new: object %s allocation failed", c->c_class.c_name->s_name);
        }
    }
    else
    {
        bug("pd_new: apparently called before setup routine");
    }
    
    return (x);
}

/*!
 * \fn          void eobj_free(void *x)
 * \brief       The eobj deletion function
 * \details     Detach the eobj from Pd and from the router
 * \param x     The eobj pointer
 */
void eobj_free(void *x)
{
    t_eobj*     z = (t_eobj *)x;
    pd_unbind((t_pd *)x, z->o_id);
    if(z->o_proxy && z->o_nproxy)
    {
        freebytes(z->o_proxy, (size_t)z->o_nproxy * sizeof(t_eproxy *));
    }
}

/*!
 * \fn          void eobj_inletnew(void* x)
 * \brief       Add a proxy inlet to an eobj
 * \details     Allocate the memory for a new inlet and initialize a proxy
 * \param x     The eobj pointer
 * \return      This function return nothing
 */
t_eproxy* eobj_proxynew(void* x)
{
    return eproxy_new(x, NULL);
}

/*!
 * \fn          int eobj_getproxy(void* x)
 * \brief       Retreive the index of the proxy that received the last message
 * \details     Check the current proxy index, this function should only be used inlet receive a message
 * \param x     The eobj pointer
 * \return      This function return nothing
 */
int eobj_getproxy(void* x)
{
    t_eobj *z = (t_eobj *)x;
    return z->o_current_proxy;
}

/*!
 * \fn          t_eclass* eobj_getclass(void* x)
 * \brief       Retrieve the eclass of an eobj
 * \details     Directly access to eclass structure
 * \param x     The eobj pointer
 * \return      This function return a pointer to the eclass of the eobj
 */
t_eclass* eobj_getclass(void* x)
{
    t_eobj *z = (t_eobj *)x;
    return (t_eclass *)z->o_obj.te_g.g_pd;
}

/*!
 * \fn          t_symbol* eobj_getclassname(void* x)
 * \brief       Retrieve the classe name of an eobj
 * \details     Directly access to the name of the eclass structure
 * \param x     The eobj pointer
 * \return      This function return the name of the class of the eobj
 */
t_symbol* eobj_getclassname(void* x)
{
    t_eobj *z = (t_eobj *)x;
    return z->o_obj.te_g.g_pd->c_name;
}

/*!
 * \fn          t_canvas* eobj_getcanvas(void *x)
 * \brief       Retreive the canvas of an eobj
 * \details     Directly access to the canvas structure
 * \param x     The eobj pointer
 * \return      This function return a pointer to canvas of the eobj
 */
t_canvas* eobj_getcanvas(void *x)
{
    t_eobj*     z = (t_eobj *)x;
    return z->o_canvas;
}

/*!
 * \fn          char eobj_isbox(void *x)
 * \brief       Retreive if an eobj is a GUI or not
 * \details     Check if the box flag is postive or null
 * \param x     The eobj pointer
 * \return      This function return 1 if the eobj is a GUI and 0 if not
 */
char eobj_isbox(void *x)
{
    t_eobj*     z = (t_eobj *)x;
    t_eclass*   c = eobj_getclass(z);
    return c->c_box;
}

/*!
 * \fn          char eobj_isdsp(void *x)
 * \brief       Retreive if an eobj is a DSP object or not
 * \details     Check if the dsp method has been initializzed
 * \param x     The eobj pointer
 * \return      This function return 1 if the eobj is a DSP object and 0 if not
 */
char eobj_isdsp(void *x)
{
    t_eobj*     z = (t_eobj *)x;
    t_eclass*   c = eobj_getclass(z);
    if(c->c_widget.w_dsp)
        return 1;
    else
        return 0;
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
    {
        c->c_widget.w_popup(x, s, (long)itemid);
    }
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
    if(argc && argv && atom_gettype(argv) == A_SYMBOL)
    {
        pch = strpbrk(atom_getsymbol(argv)->s_name, "/\"");
        // The folder seems defined
        if(pch != NULL)
        {
            atom_setsym(av, atom_getsymbol(argv));
            if(c->c_widget.w_write)
                c->c_widget.w_write(x, s, 1, av);
            return;
        }
        // The folder isn't defined so write it in the canvas folder
        else
        {
            sprintf(buf, "%s/%s", canvas_getdir(x->o_canvas)->s_name, atom_getsymbol(argv)->s_name);
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
    if(argc && argv && atom_gettype(argv) == A_SYMBOL)
    {
        // Valid path
        if((access(atom_getsymbol(argv)->s_name, O_RDONLY) != -1))
        {
            if(c->c_widget.w_read)
                c->c_widget.w_read(x, s, 1, argv);
        }
        // Invalid path or no path
        else
        {
            // Wrong path but we don't care
            pch = strpbrk(atom_getsymbol(argv)->s_name, "/\"");
            if(pch != NULL)
            {
                if(c->c_widget.w_read)
                    c->c_widget.w_read(x, s, 1, argv);
            }
            else
            {
                // Look in the canvas folder
                sprintf(buf, "%s/%s", canvas_getdir(x->o_canvas)->s_name, atom_getsymbol(argv)->s_name);
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
                    sprintf(buf, "%s/%s", var->nl_string, atom_getsymbol(argv)->s_name);
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

//! @endcond




