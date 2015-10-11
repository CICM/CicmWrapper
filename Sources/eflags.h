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

#ifndef DEF_EFLAGS
#define DEF_EFLAGS

#include "edefine.h"

/*! @defgroup groupattrpart The flags part of the library.
 * @brief The flags part of the library.
 * @details This part refers to the methods and structures that can be used
 * @details to manage flags.
 */

/*! @addtogroup groupattrpart
 *  @{
 */

/**
 * @struct _eflagman
 * @brief The flags manager object.
 * @details It contains the members and the methods for flags management.
 */
EXTERN_STRUCT _eflagman;
typedef struct _eflagman t_eflagman;

/*!
 * \fn              t_eflagman* eflags_new(void)
 * \brief           Allocates and initializes a new flags manager.
 * \return          The new flags manager.
 */
t_eflagman* eflags_new(void);

/*!
 * \fn              t_eflagman* eflags_findbyname(t_symbol* name)
 * \brief           Retrieves the flags manager that matchs to the name.
 * \param name      The name of the flags manager.
 * \return          The flags manager that matchs to the name if it exists, othersise NULL.
 */
t_eflagman* eflags_findbyname(t_symbol* name);

/*!
 * \fn              long eflags_get(t_eflagman const* flagman)
 * \brief           Retrieves the flags from a flags manager.
 * \param flagman   The flags manager pointer.
 * \return          The flags from the flags manager.
 */
long eflags_get(t_eflagman const* flagman);

/*!
 * \fn              char eflags_has(t_eflagman const* flagman)
 * \brief           Retrieves if a flags manager has a specific flag.
 * \param flagman   The flags manager pointer.
 * \param flag      The flag to check.
 * \return A non-null value if the flags manager has the flag, otherwise 0.
 */
char eflags_has(t_eflagman const* flagman, long flag);

/*!
 * \fn              void eflags_set(t_eflagman* flagman, long flags)
 * \brief           Sets the flags of a flags manager.
 * \param flagman   The flags manager pointer.
 * \param flags     The new flags.
 */
void eflags_set(t_eflagman* flagman, long flags);

/*!
 * \fn              void eflags_add(t_eflagman* flagman, long flag)
 * \brief           Adds a flag to a flags manager.
 * \param flagman   The flags manager pointer.
 * \param flag      The flag to add.
 */
void eflags_add(t_eflagman* flagman, long flag);

/*!
 * \fn              void eflags_add(t_eflagman* flagman, long flag)
 * \brief           Removes a flag from a flags manager.
 * \param flagman   The flags manager pointer.
 * \param flag      The flag to remove.
 */
void eflags_remove(t_eflagman* flagman, long flag);



/** @} */

#endif



