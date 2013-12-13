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

void eclass_attr_default(t_eclass* c, char* attrname, long flags, char* value)
{
	int i;
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i].name == gensym(attrname))
        {
            c->c_attr[i].defvals = gensym(value);
            return ;
        }
    }
}

void eclass_attr_category(t_eclass* c, char* attrname, long flags, char* categoryname)
{
	int i;
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i].name == gensym(attrname))
        {
            c->c_attr[i].category = gensym(categoryname);
            return ;
        }
    }
}

void eclass_attr_order(t_eclass* c, char* attrname, long flags, char* order)
{
	int i;
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i].name == gensym(attrname))
        {
            if(isdigit(atoi(order)))
                c->c_attr[i].order = atoi(order);
            return ;
        }
    }
}

void eclass_attr_label(t_eclass* c, char* attrname, long flags, char* labelname)
{
	int i;
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i].name == gensym(attrname))
        {
            c->c_attr[i].label = gensym(labelname);
            return ;
        }
    }
}

void eclass_attr_style(t_eclass* c, char* attrname, long flags, char* stylename)
{
	int i;
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i].name == gensym(attrname))
        {
            c->c_attr[i].style = gensym(stylename);
            return ;
        }
    }
}

void eclass_attr_filter_min(t_eclass* c, char* attrname, double value)
{
	int i;
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i].name == gensym(attrname))
        {
            if(c->c_attr[i].clipped == 0)
                c->c_attr[i].clipped = 1;
            else if(c->c_attr[i].clipped == 2)
                c->c_attr[i].clipped = 3;
            
            c->c_attr[i].minimum = value;
            return ;
        }
    }
}

void eclass_attr_filter_max(t_eclass* c, char* attrname, double value)
{
	int i;
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i].name == gensym(attrname))
        {
            if(c->c_attr[i].clipped == 0)
                c->c_attr[i].clipped = 2;
            else if(c->c_attr[i].clipped == 1)
                c->c_attr[i].clipped = 3;
            
            c->c_attr[i].maximum = value;
            return ;
        }
    }
}

void eclass_attr_save(t_eclass* c, char* attrname, long flags)
{
	int i;
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i].name == gensym(attrname))
        {
            c->c_attr[i].save = 1;
            return ;
        }
    }
}

void eclass_attr_paint(t_eclass* c, char* attrname, long flags)
{
	int i;
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i].name == gensym(attrname))
        {
            c->c_attr[i].paint = 1;
            return ;
        }
    }
}

void eclass_attr_invisible(t_eclass* c, char* attrname, long flags)
{
	int i;
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i].name == gensym(attrname))
        {
            c->c_attr[i].invisible = 1;
            return ;
        }
    }
}

void eclass_attr_accessor(t_eclass* c, char* attrname, method getter, method setter)
{
	int i;
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i].name == gensym(attrname))
        {
            if(getter != NULL)
                c->c_attr[i].getter = getter;
            if(setter != NULL)
                c->c_attr[i].setter = setter;
            return ;
        }
    }
}


