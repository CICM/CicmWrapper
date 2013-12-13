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

#ifndef DEF_EATTR
#define DEF_EATTR

#include "../estruct.h"
#include "../ebox/ebox.h"

#define calcoffset(x,y) ((long)(&(((x *)0L)->y)))

void eclass_new_attr_typed(t_eclass* c, char* attrname, char* type, long size, long maxsize, long flags, long offset);

void eclass_attr_setter(t_object* x, t_symbol *s, int argc, t_atom *argv);
void eclass_attr_getter(t_object* x, t_symbol *s, int* argc, t_atom** argv);

void eclass_attr_category(t_eclass* c, char* attrname, long flags, char* categoryname);
void eclass_attr_order(t_eclass* c, char* attrname, long flags, char* order);
void eclass_attr_label(t_eclass* c, char* attrname, long flags, char* labelname);
void eclass_attr_style(t_eclass* c, char* attrname, long flags, char* stylename);
void eclass_attr_default(t_eclass* c, char* attrname, long flags, char* value);
void eclass_attr_filter_min(t_eclass* c, char* attrname, double value);
void eclass_attr_filter_max(t_eclass* c, char* attrname, double value);
void eclass_attr_save(t_eclass* c, char* attrname, long flags);
void eclass_attr_paint(t_eclass* c, char* attrname, long flags);
void eclass_attr_invisible(t_eclass* c, char* attrname, long flags);
void eclass_attr_accessor(t_eclass* c, char* attrname, method getter, method setter);

////////////////////////////////////////////////////////////////////////////////////////////////////////

#define CLASS_ATTR_INT(c,name,flags,struct,member)   \
eclass_new_attr_typed(c,name, "int", 1, 0, flags, calcoffset(struct,member))
#define CLASS_ATTR_LONG(c,name,flags,struct,member)   \
eclass_new_attr_typed(c,name, "long", 1, 0, flags, calcoffset(struct,member))
#define CLASS_ATTR_FLOAT(c,name,flags,struct,member)  \
eclass_new_attr_typed(c,name, "float", 1, 0, flags, calcoffset(struct,member))
#define CLASS_ATTR_DOUBLE(c,name,flags,struct,member)  \
eclass_new_attr_typed(c,name, "double", 1, 0, flags, calcoffset(struct,member))
#define CLASS_ATTR_RGB(c,name,flags,struct,member) \
eclass_new_attr_typed(c,name, "float", 3, 0, flags, calcoffset(struct,member))
#define CLASS_ATTR_RGBA(c,name,flags,struct,member) \
eclass_new_attr_typed(c,name, "float", 4, 0, flags, calcoffset(struct,member))
#define CLASS_ATTR_SYMBOL(c,name,flags,struct,member) \
eclass_new_attr_typed(c,name, "symbol", 1, 0, flags, calcoffset(struct,member))

#define CLASS_ATTR_INT_ARRAY(c,name,flags,struct,member,size)   \
eclass_new_attr_typed(c,name, "int", size, 0, flags, calcoffset(struct,member))
#define CLASS_ATTR_LONG_ARRAY(c,name,flags,struct,member,size)   \
eclass_new_attr_typed(c,name, "long", size, 0, flags, calcoffset(struct,member))
#define CLASS_ATTR_FLOAT_ARRAY(c,name,flags,struct,member,size)  \
eclass_new_attr_typed(c,name, "float", size, 0, flags, calcoffset(struct,member))
#define CLASS_ATTR_DOUBLE_ARRAY(c,name,flags,struct,member,size)  \
eclass_new_attr_typed(c,name, "double", size, 0, flags, calcoffset(struct,member))
#define CLASS_ATTR_SYMBOL_ARRAY(c,name,flags,struct,member, size) \
eclass_new_attr_typed(c,name, "symbol", size, 0, flags, calcoffset(struct,member))

#define CLASS_ATTR_INT_VARSIZE(c,name,flags,struct,member, size, maxsize)   \
eclass_new_attr_typed(c,name, "int", calcoffset(struct,size), maxsize, flags, calcoffset(struct,member))
#define CLASS_ATTR_LONG_VARSIZE(c,name,flags,struct,member, size, maxsize)  \
eclass_new_attr_typed(c,name, "long", calcoffset(struct,size), maxsize, flags, calcoffset(struct,member))
#define CLASS_ATTR_FLOAT_VARSIZE(c,name,flags,struct,member, size, maxsize) \
eclass_new_attr_typed(c,name, "float", calcoffset(struct,size), maxsize, flags, calcoffset(struct,member))
#define CLASS_ATTR_DOUBLE_VARSIZE(c,name,flags,struct, member, size, maxsize)  \
eclass_new_attr_typed(c,name, "double", calcoffset(struct,size), maxsize, flags, calcoffset(struct,member))
#define CLASS_ATTR_SYMBOL_VARSIZE(c,name,flags,struct, member, size, maxsize) \
eclass_new_attr_typed(c,name, "symbol", calcoffset(struct,size), maxsize, flags, calcoffset(struct,member))

////////////////////////////////////////////////////////////////////////////////////////////////////////

#define CLASS_ATTR_CATEGORY(c,name,flags,categoryname)  eclass_attr_category(c,name,flags,categoryname)
#define CLASS_ATTR_ORDER(c,name,flags,order)            eclass_attr_order(c,name,flags,order)
#define CLASS_ATTR_LABEL(c,name,flags,label)            eclass_attr_label(c,name,flags,label)
#define CLASS_ATTR_STYLE(c,name,flags,style)            eclass_attr_label(c,name,flags,style)
#define CLASS_ATTR_DEFAULT(c,name,flags,val)            eclass_attr_default(c,name,flags,val)
#define CLASS_ATTR_FILTER_MIN(c, name, value)           eclass_attr_filter_min(c, name, value)
#define CLASS_ATTR_FILTER_MAX(c, name, value)           eclass_attr_filter_max(c, name, value)
#define CLASS_ATTR_FILTER_CLIP(c, name, minval, maxval) eclass_attr_filter_min(c, name, minval); eclass_attr_filter_max(c, name, maxval);
#define CLASS_ATTR_SAVE(c,name,flags)                   eclass_attr_save(c,name,flags)
#define CLASS_ATTR_PAINT(c,name,flags)                  eclass_attr_paint(c,name,flags)
#define CLASS_ATTR_INVISIBLE(c,name,flags)              eclass_attr_invisible(c,name,flags)
#define CLASS_ATTR_ACCESSORS(c,name,getter,setter)      eclass_attr_accessor(c,name,(method)getter,(method)setter)

#define CLASS_ATTR_DEFAULT_SAVE_PAINT(c,attrname,flags,parsestr) \
{ CLASS_ATTR_DEFAULT(c,attrname,flags,parsestr); CLASS_ATTR_SAVE(c,attrname,flags); CLASS_ATTR_PAINT(c,attrname,flags); }

////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif