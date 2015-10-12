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

#ifndef DEF_EVIEW
#define DEF_EVIEW

#include "edefine.h"

/*! @defgroup groupviewart The gui part of the library.
 * @brief The gui part of the library.
 * @details This part refers to the methods and structures that can be used
 * @details to manage a view.
 */

/*! @addtogroup groupviewart
 *  @{
 */

/**
 * @struct _egui
 * @brief The view object.
 * @details It contains the members and the methods for view management.
 */
EXTERN_STRUCT _eview;
typedef struct _eview t_eview;

/*!
 * \fn          t_object* eview_create(t_ebox* x, t_canvas* cnv)
 * \brief       The view creation function.
 * \details     Allocates and initializes a new view for a t_ebox and a t_canvas.
 * \param x     The t_ebox pointer.
 * \param cnv   The t_canvas pointer.
 * \return      This function returns a pointer to a new view.
 */
t_eview* eview_new(t_object* owner, t_canvas* cnv);

/** @} */

#endif



