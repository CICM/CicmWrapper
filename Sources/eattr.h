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
 * \fn              t_eattr *eattr_new(t_symbol *name, t_symbol *type, long size, long maxsize, long flags, long offset)
 * \brief           Allocates and initializes a new attribute.
 * \param name      The name of the attribute.
 * \param type      The type of the attribute.
 * \param size      The size of the attribute.
 * \param maxsize   The maximum number of items of the attribute.
 * \param flags     The flags of the attribute.
 * \param offset    The bit offset in the object structure
 * \return          The new attribute.
 */
t_eattr *eattr_new(t_symbol *name, t_symbol *type, long size, long maxsize, long flags, long offset);

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
 * \fn          char eattr_shouldrepaint(t_eattr *x)
 * \brief       Retrieves if the attribute should repaint its owner.
 * \param attr  The attribute pointer.
 * \return      A non-null value if the attribute should repaint its owner, otherwise 0.
 */
char eattr_shouldrepaint(t_eattr const* attr);

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
 * \fn              void eattr_getitems(t_eattr const*attr, long* nitems, t_symbol*** items)
 * \brief           Retrieves the available items possibilties of an attribute.
 * \param attr      The attribute pointer.
 * \param nitems    The number of items.
 * \param items     The items.
 */
void eattr_getitems(t_eattr const*attr, long* nitems, t_symbol*** items);

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
 * \fn              void eclass_getattrs(t_eattrset const* attrset, size_t* nattrs, t_object*** attrs)
 * \brief           Retrieves the attributes of an attributes set.
 * \param attrset   The attributes set pointer.
 * \param nattrs    The number of attributes of the attributes set.
 * \param attrs     The attributes of the attributes set.
 */
void eclass_getattrs(t_eattrset const* attrset, size_t* nattrs, t_object*** attrs);

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
void eclass_getcategories(t_eattrset const* attrset, size_t* ncates, t_symbol*** cates);

/** @} */

#endif



