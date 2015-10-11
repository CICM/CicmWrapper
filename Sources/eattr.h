/*
 * CicmWrapper
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

/**
 * \file eattr.h
 * \brief The header that declares all the methods for the t_eattr.
 * \author Pierre Guillot
 */

#ifndef DEF_EATTR
#define DEF_EATTR

#include "edefine.h"

/*! @defgroup groupattrpart The attributes part of the library.
 * @brief The attributes part of the library.
 * @details This part refers to the methods and structures that can be used
 * @details to manage attributes.
 */

/*! @addtogroup groupattrpart
 *  @{
 */

/**
 * @struct _eattr
 * @brief The attribute object.
 * @details It contains the members and the methods for an attribute.
 */
EXTERN_STRUCT _eattr;
typedef struct _eattr t_eattr;

/*!
 * \fn              t_eattr *eattr_new(t_symbol *name, t_symbol *type, size_t size, size_t maxsize, size_t offset)
 * \brief           Allocates and initializes a new attribute.
 * \param name      The name of the attribute.
 * \param type      The type of the attribute.
 * \param size      The size of the attribute.
 * \param maxsize   The maximum number of items of the attribute.
 * \param offset    The bit offset in the object structure
 * \return          The new attribute.
 */
t_eattr *eattr_new(t_symbol *name, t_symbol *type, size_t size, size_t maxsize, size_t offset);

/*!
 * \fn              void eattr_free(t_eattr *attr)
 * \brief           Frees an attribute.
 * \param attr      The attribute pointer.
 */
void eattr_free(t_eattr *attr);

/*!
 * \fn          t_symbol* eattr_getname(t_eattr *x)
 * \brief       Retrieves the name of an attribute.
 * \param attr  The attribute pointer.
 * \return      The name of the attribute.
 */
t_symbol* eattr_getname(t_eattr const* attr);

/*!
 * \fn          t_symbol* eattr_gettype(t_eattr *x)
 * \brief       Retrieves the type of an attribute.
 * \param attr  The attribute pointer.
 * \return      The type of the attribute.
 */
t_symbol* eattr_gettype(t_eattr const* attr);

/*!
 * \fn          t_symbol* eattr_getcategory(t_eattr *x)
 * \brief       Retrieves the category of an attribute.
 * \param attr  The attribute pointer.
 * \return      The category of the attribute.
 */
t_symbol* eattr_getcategory(t_eattr const* attr);

/*!
 * \fn          t_symbol* eattr_getlabel(t_eattr *x)
 * \brief       Retrieves the label of an attribute.
 * \param attr  The attribute pointer.
 * \return      The label of the attribute.
 */
t_symbol* eattr_getlabel(t_eattr const* attr);

/*!
 * \fn          t_symbol* eattr_getstyle(t_eattr *x)
 * \brief       Retrieves the style of an attribute.
 * \param attr  The attribute pointer.
 * \return      The style of the attribute.
 */
t_symbol* eattr_getstyle(t_eattr const* attr);

/*!
 * \fn          long eattr_getorder(t_eattr const* attr)
 * \brief       Retrieves the order of the attribute.
 * \param attr  The attribute pointer.
 * \return      The order of the attribute.
 */
long eattr_getorder(t_eattr const* attr);

/*!
 * \fn          char eattr_repaint(t_eattr *x)
 * \brief       Retrieves if the attribute should repaint its owner.
 * \param attr  The attribute pointer.
 * \return      A non-null value if the attribute should repaint its owner, otherwise 0.
 */
char eattr_repaint(t_eattr const* attr);

/*!
 * \fn          char eattr_isvisible(t_eattr const* attr)
 * \brief       Retrieves if the attribute should be displayed in the properties window.
 * \param attr  The attribute pointer.
 * \return      A non-null value if the attribute should be displayed in the properties 
 * \return      window, otherwise 0.
 */
char eattr_isvisible(t_eattr const* attr);

/*!
 * \fn          char eattr_issaved(t_eattr const* attr)
 * \brief       Retrieves if the attribute should be saved with its owner.
 * \param attr  The attribute pointer.
 * \return      A non-null value if the attribute should be saved with its owner, otherwise 0.
 */
char eattr_issaved(t_eattr const* attr);

/*!
 * \fn          char eattr_hasminimum(t_eattr const* attr)
 * \brief       Retrieves if the attribute has a minimum value.
 * \param attr  The attribute pointer.
 * \return      A non-null value if the attribute has a minimum value, otherwise 0.
 */
char eattr_hasminimum(t_eattr const* attr);

/*!
 * \fn          char eattr_hasmaximum(t_eattr const* attr)
 * \brief       Retrieves if the attribute has a maximum value.
 * \param attr  The attribute pointer.
 * \return      A non-null value if the attribute has a maximum value, otherwise 0.
 */
char eattr_hasmaximum(t_eattr const* attr);

/*!
 * \fn          float eattr_getminimum(t_eattr const* attr)
 * \brief       Retrieves if the minimum value of an attribute.
 * \param attr  The attribute pointer.
 * \return      The minimum value of the attribute.
 */
float eattr_getminimum(t_eattr const* attr);

/*!
 * \fn          float eattr_getmaximum(t_eattr const* attr)
 * \brief       Retrieves if the maximum value of an attribute.
 * \param attr  The attribute pointer.
 * \return      The maximum value of the attribute.
 */
float eattr_getmaximum(t_eattr const* attr);

/*!
 * \fn          float eattr_getstep(t_eattr const* attr)
 * \brief       Retrieves if the step value of an attribute.
 * \param attr  The attribute pointer.
 * \return      The maximum step of the attribute.
 */
float eattr_getstep(t_eattr const* attr);

/*!
 * \fn              void eattr_getitems(t_eattr const*attr, size_t* nitems, t_symbol*** items)
 * \brief           Retrieves the available items possibilties of an attribute.
 * \param attr      The attribute pointer.
 * \param nitems    The number of items.
 * \param items     The items.
 */
void eattr_getitems(t_eattr const*attr, size_t* nitems, t_symbol*** items);

/**
 * @struct _eattrset
 * @brief The attributes container object.
 * @details It contains a set of attributes and methods to sorts and retrieves them.
 */
EXTERN_STRUCT _eattrset;
typedef struct _eattrset t_eattrset;

/*!
 * \fn              t_eattrset* eattrset_new(t_symbol* name)
 * \brief           Allocates and initializes a new attributes set.
 * \return          The new attributes set.
 */
t_eattrset* eattrset_new();

/*!
 * \fn              t_eattrset* eattrset_findbyname(t_symbol* name)
 * \brief           Retrieves the attributes set that matchs to the name.
 * \param name      The name of the attributes set.
 * \return          The attribute set that matchs to the name if it exists, othersise NULL.
 */
t_eattrset* eattrset_findbyname(t_symbol* name);

/*!
 * \fn              void eattrset_free(t_eattrset* x)
 * \brief           Frees an attributes set.
 * \param attrset   The attributes set pointer.
 */
void eattrset_free(t_eattrset* attrset);

/*!
 * \fn              size_t eattrset_getnattrs(t_eattrset const* attrset)
 * \brief           Retrieves the number of attributes of an attributes set.
 * \param attrset   The attributes set pointer.
 * \return          The number of attributes of the attributes set.
 */
size_t eattrset_getnattrs(t_eattrset const* attrset);

/*!
 * \fn              void eattrset_getattrs(t_eattrset const* attrset, size_t* nattrs, t_object*** attrs)
 * \brief           Retrieves the attributes of an attributes set.
 * \param attrset   The attributes set pointer.
 * \param nattrs    The number of attributes of the attributes set.
 * \param attrs     The attributes of the attributes set.
 */
void eattrset_getattrs(t_eattrset const* attrset, size_t* nattrs, t_object*** attrs);

/*!
 * \fn              t_eattr* eattrset_getattr(t_eattrset const* attrset, t_symbol const* name)
 * \brief           Retrieves the attribute that matches to a specific name of an attributes set.
 * \param attrset   The attributes set pointer.
 * \param name      The name of the attribute.
 * \return          The attribute that matches to the specific name if exists, otherwise NULL.
 */
t_eattr* eattrset_getattr(t_eattrset const* attrset, t_symbol const* name);

/*!
 * \fn              size_t eattrset_getncategories(t_eattrset const* attrset)
 * \brief           Retrieves the number of categories of an attributes set.
 * \param attrset   The attributes set pointer.
 * \return          The number of categories of the attributes set.
 */
size_t eattrset_getncategories(t_eattrset const* attrset);

/*!
 * \fn              size_t eattrset_getncategories(t_eattrset const* attrset)
 * \brief           Retrieves the categories of an attributes set.
 * \param attrset   The attributes set pointer.
 * \param ncates    The number of categories of the attributes set.
 * \param cates     The categories of the attributes set.
 */
void eattrset_getcategories(t_eattrset const* attrset, size_t* ncates, t_symbol*** cates);

/*!
 * \fn          void eattrset_attr_new(t_eattrset* attrset, t_symbol* name, t_symbol* type, size_t size, size_t maxsize, size_t offset)
 * \brief           Creates a new attribute.
 * \details         Allocates the memory and intializes an new attribute for an eclass. You should prefer to use the MACROS.
 * \param attrset   The attributes set pointer.
 * \param name          The name of the attribute.
 * \param type      The attribute type
 * \param size      The attribute size
 * \param maxsize   The attribute max size
 * \param offset    The attribute bit offset in the object structure
 */
void eattrset_attr_new(t_eattrset* attrset, t_symbol* name, t_symbol* type, size_t size, size_t maxsize, size_t offset);

/*!
 * \fn              void eattrset_attr_category(t_eattrset* attrset, t_symbol* name, t_symbol* category)
 * \brief           Sets the category of an attribute.
 * \param attrset   The attributes set pointer.
 * \param name      The name of the attribute.
 * \param category  The category of the attribute.
 */
void eattrset_attr_category(t_eattrset* attrset, t_symbol* name, t_symbol* category);

/*!
 * \fn              void eattrset_attr_order(t_eattrset* attrset, t_symbol* name, long order)
 * \brief           Sets the order of an attribute.
 * \param attrset   The attributes set pointer.
 * \param name      The name of the attribute.
 * \param order     The order of the attribute.
 */
void eattrset_attr_order(t_eattrset* attrset, t_symbol* name, long order);

/*!
 * \fn              void eattrset_attr_flags(t_eattrset* attrset, t_symbol* name, long order)
 * \brief           Sets the flags of an attribute.
 * \param attrset   The attributes set pointer.
 * \param name      The name of the attribute.
 * \param flags     The flags of the attribute.
 */
void eattrset_attr_flags(t_eattrset* attrset, t_symbol* name, long flags);

/*!
 * \fn              void eattrset_attr_label(t_eattrset* attrset, t_symbol* name, t_symbol* label)
 * \brief           Sets the label of an attribute.
 * \details         The label is the message that will be displayed in the properties window of the object.
 * \param attrset   The attributes set pointer.
 * \param name      The name of the attribute.
 * \param label     The label of the attribute
 */
void eattrset_attr_label(t_eattrset* attrset, t_symbol* name, t_symbol* label);

/*!
 * \fn              void eattrset_attr_style(t_eattrset* attrset, t_symbol* name, t_symbol* style)
 * \brief           Sets the style of an attribute.
 * \details         The style is how the attribute will be displayed in the properties window of the object. it could be \"onoff\", \"color\", \"number\", \"menu\" or \"entry\".
 * \param attrset   The attributes set pointer.
 * \param name      The name of the attribute.
 * \param style     The style of the attribute
 */
void eattrset_attr_style(t_eattrset* attrset, t_symbol* name, t_symbol* style);

/*!
 * \fn              void eattrset_attr_default(t_eattrset* attrset, t_symbol* name, size_t ndefaults, t_atom* defaults);
 * \brief           Sets the default values of an attribute.
 * \param attrset   The attributes set pointer.
 * \param name      The name of the attribute.
 * \param ndefaults The number of values.
 * \param defaults  The defualt values.
 */
void eattrset_attr_default(t_eattrset* attrset, t_symbol* name, size_t ndefaults, t_atom* defaults);

/*!
 * \fn          void eattrset_attr_filter_min(t_eattrset* attrset, t_symbol* name, float value)
 * \brief           Sets the minimum value of an attribute.
 * \details         The minimum value is only valid for number attributes.
 * \param attrset   The attributes set pointer.
 * \param name      The name of the attribute.
 * \param value     The minimum value of the attribute
 */
void eattrset_attr_filter_min(t_eattrset* attrset, t_symbol* name, float value);

/*!
 * \fn              void eattrset_attr_filter_max(t_eattrset* attrset, t_symbol* name, float value)
 * \brief           Sets the maximum value of an attribute for number attributes.
 * \details         The maximum value is only valid for number attributes.
 * \param attrset   The attributes set pointer.
 * \param name      The name of the attribute.
 * \param value     The maximum value of the attribute
 */
void eattrset_attr_filter_max(t_eattrset* attrset, t_symbol* name, float value);

/*!
 * \fn              void eattrset_attr_step(t_eattrset* attrset, t_symbol* name, float value)
 * \brief           Sets the step value of an attribute.
 * \details         The step value is only valid for number attributes and defines the step in the properties window.
 * \param attrset   The attributes set pointer.
 * \param name      The name of the attribute.
 * \param value     The maximum value of the attribute
 */
void eattrset_attr_step(t_eattrset* attrset, t_symbol* name, float value);

/*!
 * \fn              void eattrset_attr_save(t_eattrset* attrset, t_symbol* name, char saved)
 * \brief           Sets if the attribute should be saved with the object.
 * \details         Only GUI can save their attributes.
 * \param attrset   The attributes set pointer.
 * \param name      The name of the attribute.
 * \param saved     A non-null value if the attribute should be saved, otherwise 0.
 */
void eattrset_attr_save(t_eattrset* attrset, t_symbol* name, char saved);

/*!
 * \fn              void eattrset_attr_paint(t_eattrset* attrset, t_symbol* name, char repaint)
 * \brief           Sets if the attribute should repaint its object.
 * \details         Only GUI can be repainted.
 * \param attrset   The attributes set pointer.
 * \param name      The name of the attribute.
 * \param repaint   A non-null value if the attribute should repaint, otherwise 0.
 */
void eattrset_attr_paint(t_eattrset* attrset, t_symbol* name, char repaint);

/*!
 * \fn              void eattrset_attr_visible(t_eattrset* attrset, t_symbol* name, char visible)
 * \brief           Sets if the attribute should be displayed in the properties window.
 * \param attrset   The attributes set pointer.
 * \param name      The name of the attribute.
 * \param visible   A non-null value if the attribute should be visible, otherwise 0.
 */
void eattrset_attr_visible(t_eattrset* attrset, t_symbol* name, char visible);

/*!
 * \fn              void eattrset_attr_accessor(t_eattrset* attrset, t_symbol* name, t_err_method getter, t_err_method setter)
 * \brief           Sets the getter and setter methods of an the attributes.
 * \details         By default the attribute is initialized with default getter and setter methods. If you just want to be notify when an attribute has changed, you should prefer to use the notify method.
 * \param attrset   The attributes set pointer.
 * \param name      The name of the attribute.
 * \param getter    The getter method.
 * \param setter    The setter method.
 */
void eattrset_attr_accessor(t_eattrset* attrset, t_symbol* name, t_getter_method getter, t_setter_method setter);

/*!
 * \fn              void eattrset_attr_items(t_eattrset* attrset, t_symbol* name, size_t nitems, t_symbol** items)
 * \brief           Sets the available items of an attribute.
 * \details         The items are only valid for menu style attributes.
 * \param attrset   The attributes set pointer.
 * \param name      The name of the attribute.
 * \param nitems    The number of items.
 * \param items     The items.
 */
void eattrset_attr_items(t_eattrset* attrset, t_symbol* name, size_t nitems, t_symbol** items);

/** @} */

#endif



