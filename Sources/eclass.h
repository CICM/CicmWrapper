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

/**
 * @file eclass.h
 * @brief The header that declares all the methods for the t_ebox.
 * @author Pierre Guillot
 */

#ifndef DEF_ECLASS
#define DEF_ECLASS

#include "edefine.h"

/*! @addtogroup groupclass
 *  @{
 */

/*!
 * \fn          t_eclass* eclass_new(char *name, method newm, method freem, size_t size, int flags, t_atomtype arg1, int arg2)
 * \brief       Allocates the memory and initialize a new t_eclass.
 * \details     Allocates and intializes the default Pd t_class then enlarges it to fit a t_eclass. \n This function should be replace class_new().
 * \param name  The class name.
 * \param newm  The new method.
 * \param freem The free method.
 * \param size  The size of the object.
 * \param flags The class flags.
 * \param arg1  The type of parameters the new function.
 * \param arg2  The type of object.
 * \return      This function return the new eclass
 */
t_eclass* eclass_new(const char *name, t_typ_method newm, t_typ_method freem, size_t size, int flags, t_atomtype arg1, int arg2);

/*!
 * \fn          void eclass_guiinit(t_eclass* c, long flags)
 * \brief       Initialize the t_eclass for the GUI behavior.
 * \details     Sets the defaults values and initializes the methods of the t_eclass for the GUI behavior.
 * \param c     The t_eclass pointer.
 * \param flags The dummy flags.
 */
void eclass_guiinit(t_eclass* c, long flags);

/*!
 * \fn          void eclass_init(t_eclass* c, long flags)
 * \brief       Initialize the t_eclass for the GUI behavior (Deprecated.
 * \details     Sets the defaults values and initializes the methods of the t_eclass for the GUI behavior.
 * \param c     The t_eclass pointer.
 * \param flags The dummy flags.
* \deprecated Please use eclass_guiinit.
 */
void eclass_init(t_eclass* c, long flags) _FUNCTION_DEPRECTAED_;

/*!
 * \fn          void eclass_dspinit(t_eclass* c)
 * \brief       Initialize the t_eclass for the DSP behavior.
 * \details     Sets the defaults values and initializes the methods of the t_eclass for the DSP behavior.
 * \param c     The t_eclass pointer.
 */
void eclass_dspinit(t_eclass* c);

/*!
 * \fn          void eclass_addmethod(t_eclass* c, method m, const char* name, t_atomtype type, long anything)
 * \brief       Adds a method to the t_eclass.
 * \details     Adds and wraps the methods of the class. For the moment the funtion takes only 1 type the second type is a dummy one and should always be 0.
 * \param c     The t_eclass pointer.
 * \param m     The method.
 * \param name  The name of the method.
 * \param type  The type of the method.
 * \param dummy The dummy type that should be 0.
 */
void eclass_addmethod(t_eclass* c, t_typ_method m, const char* name, t_atomtype type, long dummy);

//! @cond

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
void eclass_new_attr_typed(t_eclass* c, const char* attrname, const char* type, long size, long maxsize, long flags, long offset);

//! Method to set the attributes
/*
 \ @memberof        eattr
 \ @param x         The object pointer
 \ @param s         The attribute name
 \ @param argc      The size of the array of atoms
 \ @param argv      The array of atoms that contains the attributes values
 \ @return          Nothing
 */
void eclass_attr_setter(t_object* x, t_symbol *s, int argc, t_atom *argv);

//! Method to get the attributes
/*
 \ @memberof        eattr
 \ @param x         The object pointer
 \ @param s         The attribute name
 \ @param argc      The pointer to an int that will contains the size of the attributes
 \ @param argv      The pointer to an array of atoms that will contains the attributes values
 \ @return          Nothing
 */
void eclass_attr_getter(t_object* x, t_symbol *s, int* argc, t_atom** argv);

//! Initalize the category of an attribute
/*
 \ @memberof        eattr
 \ @param c         The eclass pointer
 \ @param attrname  The attribute name
 \ @param flags     The flags of the attribute
 \ @param category  The category of the attribute
 \ @return          Nothing
 */
void eclass_attr_category(t_eclass* c, const char* attrname, long flags, const char* category);

//! Initalize the order of an attribute
/*
 \ @memberof        eattr
 \ @param c         The eclass pointer
 \ @param attrname  The attribute name
 \ @param flags     The flags of the attribute
 \ @param category  The order of the attribute
 \ @return          Nothing
 */
void eclass_attr_order(t_eclass* c, const char* attrname, long flags, const char* order);

//! Initalize the category of an attribute
/*
 \ @memberof        eattr
 \ @param c         The eclass pointer
 \ @param attrname  The attribute name
 \ @param flags     The flags of the attribute
 \ @param label     The label of the attribute
 \ @return          Nothing
 */
void eclass_attr_label(t_eclass* c, const char* attrname, long flags, const char* label);

//! Initalize the category of an attribute
/*
 \ @memberof        eattr
 \ @param c         The eclass pointer
 \ @param attrname  The attribute name
 \ @param flags     The flags of the attribute
 \ @param style     The style of the attribute
 \ @return          Nothing
 */
void eclass_attr_style(t_eclass* c, const char* attrname, long flags, const char* style);

//! Initalize the default value of an attribute
/*
 \ @memberof        eattr
 \ @param c         The eclass pointer
 \ @param attrname  The attribute name
 \ @param flags     The flags of the attribute
 \ @param value     The default value
 \ @return          Nothing
 */
void eclass_attr_default(t_eclass* c, const char* attrname, long flags, const char* value);

//! Initalize the minimum value of an attribute
/*
 \ @memberof        eattr
 \ @param c         The eclass pointer
 \ @param attrname  The attribute name
 \ @param value     The minimum value of the attribute
 \ @return          Nothing
 */
void eclass_attr_filter_min(t_eclass* c, const char* attrname, float value);

//! Initalize the maximum value of an attribute
/*
 \ @memberof        eattr
 \ @param c         The eclass pointer
 \ @param attrname  The attribute name
 \ @param value     The maximum value of the attribute
 \ @return          Nothing
 */
void eclass_attr_filter_max(t_eclass* c, const char* attrname, float value);

//! Initalize the step value of an attribute
/*
 \ @memberof        eattr
 \ @param c         The eclass pointer
 \ @param attrname  The attribute name
 \ @param value     The maximum value of the attribute
 \ @return          Nothing
 */
void eclass_attr_step(t_eclass* c, const char* attrname, float value);

//! Initalize the attribute to be saved with the object
/*
 \ @memberof        eattr
 \ @param c         The eclass pointer
 \ @param attrname  The attribute name
 \ @param flags     The flags of the attribute
 \ @return          Nothing
 */
void eclass_attr_save(t_eclass* c, const char* attrname, long flags);

//! Initalize the attribute to redraw the object when its value has changed
/*
 \ @memberof        eattr
 \ @param c         The eclass pointer
 \ @param attrname  The attribute name
 \ @param flags     The flags of the attribute
 \ @return          Nothing
 */
void eclass_attr_paint(t_eclass* c, const char* attrname, long flags);

//! Initalize the attribute to be invisible in the properties window
/*
 \ @memberof        eattr
 \ @param c         The eclass pointer
 \ @param attrname  The attribute name
 \ @param flags     The flags of the attribute
 \ @return          Nothing
 */
void eclass_attr_invisible(t_eclass* c, const char* attrname, long flags);

//! Initalize the user getter and setter of an attribute
/*
 \ @memberof        eattr
 \ @param c         The eclass pointer
 \ @param attrname  The attribute name
 \ @param getter    The getter function
 \ @param setter    The setter function
 \ @return          Nothing
 */
void eclass_attr_accessor(t_eclass* c, const char* attrname, t_err_method getter, t_err_method setter);

//! Initalize the items list of an attribute
/*
 \ @memberof        eattr
 \ @param c         The eclass pointer
 \ @param attrname  The attribute name
 \ @param flags     The flags of the attribute
 \ @param style     The style of the attribute
 \ @return          Nothing
 */
void eclass_attr_itemlist(t_eclass* c, const char* attrname, long flags, const char* list);

#define calcoffset(x,y) ((long)(&(((x *)0L)->y)))
//! @endcond

//! Macros that create an int attribute
#define CLASS_ATTR_INT(c,name,flags,struct,member)   \
eclass_new_attr_typed(c,name, "int", 1, 0, flags, calcoffset(struct,member))
//! Macros that create a long attribute
#define CLASS_ATTR_LONG(c,name,flags,struct,member)   \
eclass_new_attr_typed(c,name, "long", 1, 0, flags, calcoffset(struct,member))
//! Macros that create a float attribute
#define CLASS_ATTR_FLOAT(c,name,flags,struct,member)  \
eclass_new_attr_typed(c,name, "float", 1, 0, flags, calcoffset(struct,member))
//! Macros that create a double attribute
#define CLASS_ATTR_DOUBLE(c,name,flags,struct,member)  \
eclass_new_attr_typed(c,name, "double", 1, 0, flags, calcoffset(struct,member))
//! Macros that create a rgb attribute
#define CLASS_ATTR_RGB(c,name,flags,struct,member) \
eclass_new_attr_typed(c,name, "float", 3, 0, flags, calcoffset(struct,member))
//! Macros that create a rgba attribute
#define CLASS_ATTR_RGBA(c,name,flags,struct,member) \
eclass_new_attr_typed(c,name, "float", 4, 0, flags, calcoffset(struct,member))
//! Macros that create a symbol attribute
#define CLASS_ATTR_SYMBOL(c,name,flags,struct,member) \
eclass_new_attr_typed(c,name, "symbol", 1, 0, flags, calcoffset(struct,member))
//! Macros that create a atom attribute
#define CLASS_ATTR_ATOM(c,name,flags,struct,member) \
eclass_new_attr_typed(c,name, "atom", 1, 0, flags, calcoffset(struct,member))
//! Macros that create an int array attribute
#define CLASS_ATTR_INT_ARRAY(c,name,flags,struct,member,size)   \
eclass_new_attr_typed(c,name, "int", size, 0, flags, calcoffset(struct,member))
//! Macros that create a long array attribute
#define CLASS_ATTR_LONG_ARRAY(c,name,flags,struct,member,size)   \
eclass_new_attr_typed(c,name, "long", size, 0, flags, calcoffset(struct,member))
//! Macros that create a float array attribute
#define CLASS_ATTR_FLOAT_ARRAY(c,name,flags,struct,member,size)  \
eclass_new_attr_typed(c,name, "float", size, 0, flags, calcoffset(struct,member))
//! Macros that create a double array attribute
#define CLASS_ATTR_DOUBLE_ARRAY(c,name,flags,struct,member,size)  \
eclass_new_attr_typed(c,name, "double", size, 0, flags, calcoffset(struct,member))
//! Macros that create a symbol array attribute
#define CLASS_ATTR_SYMBOL_ARRAY(c,name,flags,struct,member, size) \
eclass_new_attr_typed(c,name, "symbol", size, 0, flags, calcoffset(struct,member))
//! Macros that create a atom array attribute
#define CLASS_ATTR_ATOM_ARRAY(c,name,flags,struct,member, size) \
eclass_new_attr_typed(c,name, "atom", size, 0, flags, calcoffset(struct,member))
//! Macros that create an int array with a variable size attribute
#define CLASS_ATTR_INT_VARSIZE(c,name,flags,struct,member, size, maxsize)   \
eclass_new_attr_typed(c,name, "int", calcoffset(struct,size), maxsize, flags, calcoffset(struct,member))
//! Macros that create a long array with a variable size attribute
#define CLASS_ATTR_LONG_VARSIZE(c,name,flags,struct,member, size, maxsize)  \
eclass_new_attr_typed(c,name, "long", calcoffset(struct,size), maxsize, flags, calcoffset(struct,member))
//! Macros that create a float array with a variable size attribute
#define CLASS_ATTR_FLOAT_VARSIZE(c,name,flags,struct,member, size, maxsize) \
eclass_new_attr_typed(c,name, "float", calcoffset(struct,size), maxsize, flags, calcoffset(struct,member))
//! Macros that create a double array with a variable size attribute
#define CLASS_ATTR_DOUBLE_VARSIZE(c,name,flags,struct, member, size, maxsize)  \
eclass_new_attr_typed(c,name, "double", calcoffset(struct,size), maxsize, flags, calcoffset(struct,member))
//! Macros that create a symbol array with a variable size attribute
#define CLASS_ATTR_SYMBOL_VARSIZE(c,name,flags,struct, member, size, maxsize) \
eclass_new_attr_typed(c,name, "symbol", calcoffset(struct,size), maxsize, flags, calcoffset(struct,member))
//! Macros that create a atom array with a variable size attribute
#define CLASS_ATTR_ATOM_VARSIZE(c,name,flags,struct, member, size, maxsize) \
eclass_new_attr_typed(c,name, "atom", calcoffset(struct,size), maxsize, flags, calcoffset(struct,member))

//! Macros that define the category of the attributes
#define CLASS_ATTR_CATEGORY(c,name,flags,categoryname)  eclass_attr_category(c,name,flags,categoryname)
//! Macros that define the order of the attributes
#define CLASS_ATTR_ORDER(c,name,flags,order)            eclass_attr_order(c,name,flags,order)
//! Macros that define the label of the attributes
#define CLASS_ATTR_LABEL(c,name,flags,label)            eclass_attr_label(c,name,flags,label)
//! Macros that define the style of the attributes
#define CLASS_ATTR_STYLE(c,name,flags,style)            eclass_attr_style(c,name,flags,style)
//! Macros that define the default value of the attributes
#define CLASS_ATTR_DEFAULT(c,name,flags,val)            eclass_attr_default(c,name,flags,val)
//! Macros that define the minimum value of the attributes
#define CLASS_ATTR_FILTER_MIN(c, name, value)           eclass_attr_filter_min(c, name, value)
//! Macros that define the maximum value of the attributes
#define CLASS_ATTR_FILTER_MAX(c, name, value)           eclass_attr_filter_max(c, name, value)
//! Macros that define the minimum and maximmum values of the attributes
#define CLASS_ATTR_FILTER_CLIP(c, name, minval, maxval) eclass_attr_filter_min(c, name, minval); eclass_attr_filter_max(c, name, maxval);
//! Macros that define the step value of the attributes
#define CLASS_ATTR_STEP(c, name, value)                 eclass_attr_step(c, name, value)
//! Macros that define the save behavior of the attributes
#define CLASS_ATTR_SAVE(c,name,flags)                   eclass_attr_save(c,name,flags)
//! Macros that define the paint behavior of the attributes
#define CLASS_ATTR_PAINT(c,name,flags)                  eclass_attr_paint(c,name,flags)
//! Macros that define the visible behavior of the attributes
#define CLASS_ATTR_INVISIBLE(c,name,flags)              eclass_attr_invisible(c,name,flags)
//! Macros that define the setter and getter of the attributes
#define CLASS_ATTR_ACCESSORS(c,name,getter,setter)      eclass_attr_accessor(c,name,(t_err_method)getter,(t_err_method)setter)
//! Macros that define the items list of the attributes
#define CLASS_ATTR_ITEMS(c,name,flags, list)            eclass_attr_itemlist(c,name,flags, list)
//! Macros that define the deault value, save and paint bbehavior of the attributes
#define CLASS_ATTR_DEFAULT_SAVE_PAINT(c,attrname,flags,parsestr) \
{ CLASS_ATTR_DEFAULT(c,attrname,flags,parsestr); CLASS_ATTR_SAVE(c,attrname,flags); CLASS_ATTR_PAINT(c,attrname,flags); }

/** @} */

#endif



