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

#include "eclass.h"

//! Allocate the memory and intializa an new attribute for an eclass (You should prefer to use the MACROS)
/*
 \ @memberof        eattr
 \ @param c         The eclass pointer
 \ @param attrname  The attribute name
 \ @param type      The attribute type
 \ @param size      The attribute size
 \ @param maxsize   The attribute max size
 \ @param maxsize   The attribute flags
 \ @param offset    The attribute bit offset in the object structure
 \ @return          Nothing
*/
void eclass_new_attr_typed(t_eclass* c, char* attrname, char* type, long size, long maxsize, long flags, long offset)
{
    int i;
    t_eattr* attr;
    t_eattr** attrs;
    char getattr[MAXPDSTRING];
    if(size >= 1)
    {
        for(i = 0; i < c->c_nattr; i++)
        {
            if(c->c_attr[i]->name == gensym(attrname))
            {
                error("%s already have %s attribute.", c->c_class.c_name->s_name, attrname);
                return ;
            }
        }
        attr = getbytes(sizeof(t_eattr));
        if(attr)
        {
            attr->name = gensym(attrname);
            attr->type = gensym(type);
            attr->category = c->c_class.c_name;
            attr->label = gensym("");
            attr->style = gensym("entry");
            attr->order = c->c_nattr+1;
            attr->save  = 0;
            attr->paint = 0;
            attr->invisible = 0;
            attr->flags     = flags;
            attr->offset    = offset;
            attr->size      = size;
            attr->sizemax   = maxsize;
            attr->getter    = NULL;
            attr->setter    = NULL;
            attr->clipped   = 0;
            attr->minimum   = 0;
            attr->maximum   = 1;
            attr->step      = 1;
            attr->defvals   = NULL;
            attr->itemslist = NULL;
            attr->itemssize = 0;
            
            attrs = (t_eattr **)realloc(c->c_attr, (size_t)(c->c_nattr + 1) * sizeof(t_eattr *));
            if(attrs)
            {
                c->c_attr = attrs;
                c->c_attr[c->c_nattr] = attr;
                class_addmethod((t_class *)c, (t_method)eclass_attr_setter, gensym(attrname), A_GIMME, 0);
                sprintf(getattr, "get%s", attrname);
                class_addmethod((t_class *)c, (t_method)eclass_attr_getter, gensym(getattr), A_GIMME, 0);
                c->c_nattr++;
            }
            else
            {
                error("%s can't increase memory for %s attribute.", c->c_class.c_name->s_name, attrname);
            }
            
        }
        else
        {
             error("%s can't allocate memory for %s attribute.", c->c_class.c_name->s_name, attrname);
        }
    }
    else
    {
        error("%s %s attribute size is too null.", c->c_class.c_name->s_name, attrname);
    }
}

