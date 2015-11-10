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
 * \fn              void eview_getdrawparams(t_eview const* view, t_edrawparams* params)
 * \brief           Retrieves the draw parameters of a view.
 * \param view      The view pointer.
 * \param params    The draw parameters to fill.
 */
void eview_getdrawparams(t_eview const* view, t_edrawparams* params);

/*!
 * \fn              void eview_getdrawbounds(t_eview const* view, t_rect* bounds)
 * \brief           Retrieves the drawing bounds of a view (without the border size).
 * \param view      The view pointer.
 * \param bounds    The bounds of the view.
 */
void eview_getdrawbounds(t_eview const* view, t_rect* bounds);

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
 * \fn              void eview_select(t_eview* view)
 * \brief           Notifies a view that it has been selected.
 * \param view      The view pointer.
 */
void eview_select(t_eview* view);

/*!
 * \fn              void eview_deselect(t_eview* view)
 * \brief           Notifies a view that it has been deselected.
 * \param view      The view pointer.
 */
void eview_deselect(t_eview* view);

/*!
 * \fn              void eview_setcursor(t_eview* view, ecursor_types cursor)
 * \brief           Sets the cursor of a view.
 * \param view      The view pointer.
 * \param cursor    The type of cursor.
 */
void eview_setcursor(t_eview* view, ecursor_types cursor);

/*!
 * \fn              void eview_draw(t_eview* view)
 * \brief           Notifies a view that it should drawn.
 * \param view      The view pointer.
 */
void eview_draw(t_eview* view);

/*!
 * \fn              t_elayer* eview_layer_start(t_eview* view, t_symbol *name, float width, float height)
 * \brief           Starts a new layers.
 * \param view      The view pointer.
 * \param name      The name of the layer.
 * \param width     The width of the layer.
 * \param height    The height of the layer.
 */
t_elayer* eview_layer_start(t_eview* view, t_symbol *name, float width, float height);

/*!
 * \fn              t_pd_err eview_layer_invalidate(t_eview* view, t_symbol *name)
 * \brief           Invalidates a layer in a view.
 * \param view      The view pointer.
 * \param name      The name of the layer.
 * \return A null value if nothing goes wrong, otherwise a non-null value.
 */
t_pd_err eview_layer_invalidate(t_eview* view, t_symbol *name);

/*!
 * \fn              t_pd_err eview_layer_end(t_eview* view, t_symbol *name)
 * \brief           Ends a layer in a view.
 * \param view      The view pointer.
 * \param name      The name of the layer.
 * \return A null value if nothing goes wrong, otherwise a non-null value.
 */
t_pd_err eview_layer_end(t_eview* view, t_symbol *name);

/*!
 * \fn              t_pd_err eview_layer_paint(t_eview* view, t_symbol *name, const float xoffset, const float yoffset)
 * \brief           Paints a layer in a view.
 * \param view      The view pointer.
 * \param name      The name of the layer.
 * \param xoffset   The abscissa offset of the layer.
 * \param yoffset   The ordinate yoffset of the layer.
 * \return A null value if nothing goes wrong, otherwise a non-null value.
 */
t_pd_err eview_layer_paint(t_eview* view, t_symbol *name, const float xoffset, const float yoffset);


/** @} */

#endif



