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

#include "eattr.h"

void cicm_class_new_attr_typed(t_eclass* c, char* attrname, char* type, long size, long flags, long offset)
{
    char getattr[256];
    sprintf(getattr, "get%s", attrname);
    for(int i = 0; i < c->c_attr.size(); i++)
    {
        if(c->c_attr[i].name == gensym(attrname))
            return ;
    }
    if(size < 1)
        return;
        
    t_eattr new_attr;
    new_attr.name = gensym(attrname);
    new_attr.type = gensym(type);
    new_attr.flags = flags;
    new_attr.offset = offset;
    new_attr.size = size;
    new_attr.category = c->c_class.c_name;
    c->c_attr.push_back(new_attr);
    class_addmethod((t_class *)c, (t_method)cicm_class_attr_setter, gensym(attrname), A_GIMME, 0);
    class_addmethod((t_class *)c, (t_method)cicm_class_attr_getter, gensym(getattr), A_GIMME, 0);
}



