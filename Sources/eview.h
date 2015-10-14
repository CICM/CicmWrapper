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
 * \fn              t_object* eview_create(t_ebox* x, t_canvas* cnv)
 * \brief           The view creation function.
 * \details         Allocates and initializes a new view for a t_ebox and a t_canvas.
 * \param owner     The view owner.
 * \param cnv       The t_canvas pointer.
 * \param bounds    The bounds of the view.
 * \return          This function returns a pointer to a new view.
 */
t_eview* eview_new(t_object* owner, t_canvas* cnv, t_rect const* bounds);

/*!
 * \fn              t_eview* eview_findbyname(t_symbol* name)
 * \brief           Retrieves the view that matchs to the name.
 * \param name      The name of the view.
 * \return          The view that matchs to the name if it exists, othersise NULL.
 */
t_eview* eview_findbyname(t_symbol* name);

/*!
 * \fn          t_canvas* eview_getcanvas(t_eview* view)
 * \brief       Retrieves the canvas of a view.
 * \param view  The view pointer.
 * \return      The canvas of the view.
 */
t_canvas* eview_getcanvas(t_eview const* view);

/*!
 * \fn          void eview_getposition(t_eview* view, t_pt* pos)
 * \brief       Retrieves the position of a view.
 * \param view  The view pointer.
 * \param pos   The position of the view.
 */
void eview_getposition(t_eview const* view, t_pt* pos);

/*!
 * \fn          void eview_getsize(t_eview const* view, t_pt* size)
 * \brief       Retrieves the size of a view.
 * \param view  The view pointer.
 * \param size  The size of the view.
 */
void eview_getsize(t_eview const* view, t_pt* size);

/*!
 * \fn              void eview_getbounds(t_eview const* view, t_rect* bounds)
 * \brief           Retrieves the bounds of a view.
 * \param view      The view pointer.
 * \param bounds    The bounds of the view.
 */
void eview_getbounds(t_eview const* view, t_rect* bounds);

/*!
 * \fn          void eview_setposition(t_eview* view, t_pt const* pos)
 * \brief       Sets the position of a view.
 * \param view  The view pointer.
 * \param pos   The position of the view.
 */
void eview_setposition(t_eview* view, t_pt const* pos);

/*!
 * \fn          void eview_getsize(t_eview const* view, t_pt* size)
 * \brief       Sets the size of a view.
 * \param view  The view pointer.
 * \param size  The size of the view.
 */
void eview_setsize(t_eview* view, t_pt const* size);

/*!
 * \fn              void eview_getbounds(t_eview const* view, t_rect* bounds)
 * \brief           Sets the bounds of a view.
 * \param view      The view pointer.
 * \param bounds    The bounds of the view.
 */
void eview_setbounds(t_eview* view, t_rect const* bounds);

/*!
 * \fn              void eview_setcursor(t_eview* view, ebox_cursors cursor)
 * \brief           Sets the cursor of a view.
 * \param view      The view pointer.
 * \param cursor    The type of cursor.
 */
void eview_setcursor(t_eview* view, ebox_cursors cursor);

/*!
 * \fn              void eview_layers_update(t_eview* view)
 * \brief           Updates the layers of a view.
 * \param view      The view pointer.
 */
void eview_layers_update(t_eview* view);

/** @} */

#endif



