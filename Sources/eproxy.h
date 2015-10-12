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

#ifndef DEF_EPROXY
#define DEF_EPROXY

#include "edefine.h"

/*! @defgroup groupproxypart The proxy inlet part of the library.
 * @brief The proxy inlet part of the library.
 * @details This part refers to the methods and structures that can be used
 * @details to manage proxy inlets.
 */

/*! @addtogroup groupproxypart
 *  @{
 */

/**
 * @struct _eproxy
 * @brief The proxy inlet object.
 * @details It contains the members and the methods for proxy inlet.
 */
EXTERN_STRUCT _eproxy;
typedef struct _eproxy t_eproxy;

/*!
 * \fn              t_eproxy* eproxy_new(t_object *owner, t_symbol* s)
 * \brief           Allocates and initializes a new proxy inlet.
 * \param owner     The owner of the proxy inlet.
 * \param s         The symbol of the proxy inlet.
 * \param index     The index of the proxy inlet.
 * \return          The new proxy inlet.
 */
t_eproxy* eproxy_new(t_object *owner, t_symbol* s, size_t index);



/** @} */

#endif



