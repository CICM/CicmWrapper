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

//! Initalize the default value of an attribute
/*
 \ @memberof        eattr
 \ @param c         The eclass pointer
 \ @param attrname  The attribute name
 \ @param flags     The flags of the attribute
 \ @param value     The default value
 \ @return          Nothing
*/
void eclass_attr_default(t_eclass* c, char* attrname, long flags, char* value)
{
	int i;
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i]->name == gensym(attrname))
        {
            c->c_attr[i]->defvals = gensym(value);
            return ;
        }
    }
}

//! Initalize the category of an attribute
/*
 \ @memberof        eattr
 \ @param c         The eclass pointer
 \ @param attrname  The attribute name
 \ @param flags     The flags of the attribute
 \ @param category  The category of the attribute
 \ @return          Nothing
*/
void eclass_attr_category(t_eclass* c, char* attrname, long flags, char* category)
{
	int i;
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i]->name == gensym(attrname))
        {
            c->c_attr[i]->category = gensym(category);
            return ;
        }
    }
}

//! Initalize the order of an attribute
/*
 \ @memberof        eattr
 \ @param c         The eclass pointer
 \ @param attrname  The attribute name
 \ @param flags     The flags of the attribute
 \ @param category  The order of the attribute
 \ @return          Nothing
*/
void eclass_attr_order(t_eclass* c, char* attrname, long flags, char* order)
{
	int i;
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i]->name == gensym(attrname))
        {
            if(isdigit(atoi(order)))
                c->c_attr[i]->order = atoi(order);
            return ;
        }
    }
}

//! Initalize the category of an attribute
/*
 \ @memberof        eattr
 \ @param c         The eclass pointer
 \ @param attrname  The attribute name
 \ @param flags     The flags of the attribute
 \ @param label     The label of the attribute
 \ @return          Nothing
*/
void eclass_attr_label(t_eclass* c, char* attrname, long flags, char* labelname)
{
	int i;
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i]->name == gensym(attrname))
        {
            c->c_attr[i]->label = gensym(labelname);
            return ;
        }
    }
}

//! Initalize the category of an attribute
/*
 \ @memberof        eattr
 \ @param c         The eclass pointer
 \ @param attrname  The attribute name
 \ @param flags     The flags of the attribute
 \ @param style     The style of the attribute
 \ @return          Nothing
*/
void eclass_attr_style(t_eclass* c, char* attrname, long flags, char* style)
{
	int i;
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i]->name == gensym(attrname))
        {
            if(gensym(style) == gensym("checkbutton") || gensym(style) == gensym("onoff"))
            {
                c->c_attr[i]->style = gensym("checkbutton");
            }
            else if(gensym(style) == gensym("rgb") || gensym(style) == gensym("rgba") || gensym(style) == gensym("color"))
            {
                c->c_attr[i]->style = gensym("color");
            }
            else if(gensym(style) == gensym("spinbox") || gensym(style) == gensym("number"))
            {
                c->c_attr[i]->style = gensym("number");
            }
            else if(gensym(style) == gensym("menu"))
            {
                c->c_attr[i]->style = gensym("menu");
            }
            else
            {
                c->c_attr[i]->style = gensym("entry");
            }
            return ;
        }
    }
}

//! Initalize the items list of an attribute
/*
 \ @memberof        eattr
 \ @param c         The eclass pointer
 \ @param attrname  The attribute name
 \ @param flags     The flags of the attribute
 \ @param style     The style of the attribute
 \ @return          Nothing
 */
void eclass_attr_itemlist(t_eclass* c, char* attrname, long flags, char* list)
{
	int i, j = 0;
    char* pch;
    int size = 0;
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i]->name == gensym(attrname))
        {
            pch = strtok(gensym(list)->s_name," ,");
            
            while(pch != NULL)
            {
                pch = strtok(NULL, " ,");
                size++;
            }
            if(size > 0)
            {
                if(c->c_attr[i]->itemssize)
                {
                    c->c_attr[i]->itemslist = (t_symbol **)realloc(c->c_attr[i]->itemslist, size * sizeof(t_symbol *));
                    if(c->c_attr[i]->itemslist)
                        c->c_attr[i]->itemssize = size;
                }
                else
                {
                    c->c_attr[i]->itemslist = (t_symbol **)calloc(size, sizeof(t_symbol *));
                    if(c->c_attr[i]->itemslist)
                        c->c_attr[i]->itemssize = size;
                }
                if(c->c_attr[i]->itemssize)
                {
                    pch = strtok(gensym(list)->s_name," ,");
                    while(pch != NULL)
                    {
                        c->c_attr[i]->itemslist[j] = gensym(pch);
                        pch = strtok(NULL, " ,");
                        j++;
                    }
                }
                
            }
            else
            {
                if(c->c_attr[i]->itemssize)
                    free(c->c_attr[i]->itemslist);
                c->c_attr[i]->itemssize = 0;
            }
            return ;
        }
    }
}

//! Initalize the minimum value of an attribute
/*
 \ @memberof        eattr
 \ @param c         The eclass pointer
 \ @param attrname  The attribute name
 \ @param value     The minimum value of the attribute
 \ @return          Nothing
*/
void eclass_attr_filter_min(t_eclass* c, char* attrname, double value)
{
	int i;
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i]->name == gensym(attrname))
        {
            if(c->c_attr[i]->clipped == 0)
                c->c_attr[i]->clipped = 1;
            else if(c->c_attr[i]->clipped == 2)
                c->c_attr[i]->clipped = 3;
            
            c->c_attr[i]->minimum = value;
            return ;
        }
    }
}

//! Initalize the maximum value of an attribute
/*
 \ @memberof        eattr
 \ @param c         The eclass pointer
 \ @param attrname  The attribute name
 \ @param value     The maximum value of the attribute
 \ @return          Nothing
*/
void eclass_attr_filter_max(t_eclass* c, char* attrname, double value)
{
	int i;
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i]->name == gensym(attrname))
        {
            if(c->c_attr[i]->clipped == 0)
                c->c_attr[i]->clipped = 2;
            else if(c->c_attr[i]->clipped == 1)
                c->c_attr[i]->clipped = 3;
            
            c->c_attr[i]->maximum = value;
            return ;
        }
    }
}

//! Initalize the step value of an attribute
/*
 \ @memberof        eattr
 \ @param c         The eclass pointer
 \ @param attrname  The attribute name
 \ @param value     The maximum value of the attribute
 \ @return          Nothing
 */
void eclass_attr_step(t_eclass* c, char* attrname, double value)
{
	int i;
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i]->name == gensym(attrname))
        {
            c->c_attr[i]->step = value;
            return ;
        }
    }
}

//! Initalize the attribute to be saved with the object
/*
 \ @memberof        eattr
 \ @param c         The eclass pointer
 \ @param attrname  The attribute name
 \ @param flags     The flags of the attribute
 \ @return          Nothing
*/
void eclass_attr_save(t_eclass* c, char* attrname, long flags)
{
	int i;
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i]->name == gensym(attrname))
        {
            c->c_attr[i]->save = 1;
            return ;
        }
    }
}

//! Initalize the attribute to redraw the object when its value has changed
/*
 \ @memberof        eattr
 \ @param c         The eclass pointer
 \ @param attrname  The attribute name
 \ @param flags     The flags of the attribute
 \ @return          Nothing
*/
void eclass_attr_paint(t_eclass* c, char* attrname, long flags)
{
	int i;
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i]->name == gensym(attrname))
        {
            c->c_attr[i]->paint = 1;
            return ;
        }
    }
}

//! Initalize the attribute to be invisible in the properties window
/*
 \ @memberof        eattr
 \ @param c         The eclass pointer
 \ @param attrname  The attribute name
 \ @param flags     The flags of the attribute
 \ @return          Nothing
*/
void eclass_attr_invisible(t_eclass* c, char* attrname, long flags)
{
	int i;
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i]->name == gensym(attrname))
        {
            c->c_attr[i]->invisible = 1;
            return ;
        }
    }
}

//! Initalize the user getter and setter of an attribute
/*
 \ @memberof        eattr
 \ @param c         The eclass pointer
 \ @param attrname  The attribute name
 \ @param getter    The getter function
 \ @param setter    The setter function
 \ @return          Nothing
*/
void eclass_attr_accessor(t_eclass* c, char* attrname, method getter, method setter)
{
	int i;
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i]->name == gensym(attrname))
        {
            c->c_attr[i]->getter = getter;
            c->c_attr[i]->setter = setter;
            return ;
        }
    }
}


