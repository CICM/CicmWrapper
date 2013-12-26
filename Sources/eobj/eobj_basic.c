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

#include "eobj.h"

//! Allocate the memory for an intance of an eobj that contains a specific eclass
/*
 * @memberof    eobj
 * @param c     The eclass pointer
 * @return      This function return a new instance of an ebox
*/
void *eobj_alloc(t_eclass *c)
{
    t_pd*   x;
    t_eobj* z;
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
    
    return (x);
}

//! Retreive if an eobj is a GUI or not
/*
 * @memberof    eobj
 * @param c     The eclass pointer
 * @return      This function return 0 if the eobj is a GUI and 1 if not
 */
char eobj_getregister(void *x)
{
    t_eobj*     z = (t_eobj *)x;
    t_eclass*   c = (t_eclass *)z->o_obj.te_g.g_pd;
    return c->c_box;
}

//! Add a proxy inlet to an eobj
/*
 \ @memberof    ebox
 \ @param x     The eobj pointer
 \ @return      Nothing
 */
void eobj_inletnew(void* x)
{
    t_eobj *z = (t_eobj *)x;
    eproxy_init(&z->o_proxy[z->o_nproxy], z, z->o_nproxy+1);
    inlet_new(&z->o_obj, &z->o_proxy[z->o_nproxy].p_pd, NULL, NULL);
    z->o_nproxy++;
}

//! Retrieve the index of the last usued proxy
/*
 \ @memberof    ebox
 \ @param x     The eobj pointer
 \ @return      Nothing
 */
int eobj_getproxy(void* x)
{
    t_eobj *z = (t_eobj *)x;
    return z->o_current_proxy;
}







