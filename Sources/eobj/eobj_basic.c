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
    t_pd*   x;
    t_eobj* z;
    char buffer[MAXPDSTRING];
    if(!c)
        bug("pd_new: apparently called before setup routine");
    x   = (t_pd *)t_getbytes(c->c_class.c_size);
    *x  = (t_pd)c;
    
    if (c->c_class.c_patchable)
    {
        ((t_object *)x)->ob_inlet = 0;
        ((t_object *)x)->ob_outlet = 0;
    }
    z = (t_eobj *)x;
    z->o_nproxy = 0;
    z->o_canvas = canvas_getcurrent();
    sprintf(buffer,"#%s%lx", c->c_class.c_name->s_name, (long unsigned int)z);
    z->o_id = gensym(buffer);
    pd_bind(&z->o_obj.ob_pd, z->o_id);
    eobj_attach_torouter((t_object *)z);
    sprintf(buffer,".x%lx.c", (long unsigned int)z->o_canvas);
    z->o_canvas_id = gensym(buffer);
    c->c_widget.w_dosave = (method)eobj_dosave;
    return (x);
}

/*!
 * \fn          void eobj_free(void *x)
 * \brief       The eobj deletion function
 * \details     Detach the eobj from Pd and from the router
 * \param x     The eobj pointer
 * \return      This function return nothing
 */
void eobj_free(void *x)
{
    t_eobj*     z = (t_eobj *)x;
    
    pd_unbind(&z->o_obj.ob_pd, z->o_id);
    eobj_detach_torouter((t_object *)x);
}

/*
 * \fn          void eobj_proxletnew(void* x)
 * \brief       Add a proxy inlet to an eobj
 * \details     Allocate the memory for a new inlet and initialize a proxy
 * \param x     The eobj pointer
 * \return      This function return nothing
 *
t_proxlet* eobj_proxletnew(void* x)
{
    return proxlet_new((t_object *)x);
}*/

/*!
 * \fn          void eobj_inletnew(void* x)
 * \brief       Add a proxy inlet to an eobj
 * \details     Allocate the memory for a new inlet and initialize a proxy
 * \param x     The eobj pointer
 * \return      This function return nothing
 */
t_eproxy* eobj_proxynew(void* x)
{
    return eproxy_new(x);
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
    return gensym(z->o_obj.te_g.g_pd->c_name->s_name);
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




