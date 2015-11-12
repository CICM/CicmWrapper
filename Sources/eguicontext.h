/*
 * CicmWrapper
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#ifndef DEF_EGUICONTEXT
#define DEF_EGUICONTEXT

#include "edefine.h"
#include "eview.h"
#include "elayer.h"

/*! @addtogroup groupgraphics The Graphics Part
 * @brief The t_elayer part.
 * @details This part refers to the methods and structures used to paint.
 *  @{
 */

/*!
 * \fn              void eguicontext_init(void)
 * \brief           Initializes the GUI context
 */
void eguicontext_init(void);

/*!
 * \fn              t_eguicontext* eguicontext_get(void)
 * \brief           Retrieves the gui context of the application.
 * \return          A pointer to the gui context.
 */
t_eguicontext* eguicontext_get(void);

/*!
 * \fn              t_pd_err eguicontext_fontfamilies(t_eguicontext *ctxt, size_t nfonts, t_symbol** families)
 * \brief           Retrieves the availables font families in the gui context
 * \param ctxt      The pointer of the guicontext.
 * \param nfonts    The number of font families of the guicontext.
 * \param fonts     The font families of the guicontext.
 * \return          A null value if nothing goes wrong, otherwise a non-null value.
 */
t_pd_err eguicontext_fontfamilies(t_eguicontext *ctxt, size_t* nfonts, t_symbol*** fonts);







/*!
 * \fn              t_pd_err eguicontext_view_add(t_eguicontext *ctxt, t_eview *view)
 * \brief           Adds a new view in a gui context.
 * \param ctxt      The pointer of the guicontext.
 * \param view      The pointer of the view.
 * \return          A null value if nothing goes wrong, otherwise a non-null value.
 */
t_pd_err eguicontext_view_add(t_eguicontext *ctxt, t_eview *view);

/*!
 * \fn              t_pd_err eguicontext_view_remove(t_eguicontext *ctxt, t_eview *view)
 * \brief           Removes a view in a gui context.
 * \param ctxt      The pointer of the guicontext.
 * \param view      The pointer of the view.
 * \return          A null value if nothing goes wrong, otherwise a non-null value.
 */
t_pd_err eguicontext_view_remove(t_eguicontext *ctxt, t_eview *view);

/*!
 * \fn              t_pd_err eguicontext_view_setcursor(t_eguicontext *ctxt, t_eview *view, ecursor_types type)
 * \brief           Changes the cursor of a view.
 * \param ctxt      The pointer of the guicontext.
 * \param view      The pointer of the view.
 * \param type      The type of cursor.
 * \return          A null value if nothing goes wrong, otherwise a non-null value.
 */
t_pd_err eguicontext_view_setcursor(t_eguicontext *ctxt, t_eview *view, ecursor_types type);

/*!
 * \fn              t_pd_err eguicontext_view_boundschanged(t_eguicontext *ctxt, t_eview* view)
 * \brief           Notifies the gui context that the bounds of a view changed.
 * \param ctxt      The pointer of the guicontext.
 * \param view      The pointer of the view.
 * \return          A null value if nothing goes wrong, otherwise a non-null value.
 */
t_pd_err eguicontext_view_boundschanged(t_eguicontext *ctxt, t_eview* view);

/*!
 * \fn              t_pd_err eguicontext_view_paint_layer(t_eguicontext *ctxt, t_elayer *l)
 * \brief           Paints a layer.
 * \param ctxt      The pointer of the guicontext.
 * \param l         The pointer of the layer.
 * \param xoffset   The abscissa offset of the layer.
 * \param yoffset   The ordinate yoffset of the layer.
 * \return          A null value if nothing goes wrong, otherwise a non-null value.
 */
t_pd_err eguicontext_view_paint_layer(t_eguicontext *ctxt, t_eview *view, t_elayer *l, const float xoffset, const float yoffset);



/*!
 * \fn          t_object* eguicontext_propw_new(t_eguicontext *ctxt, t_object* owner)
 * \brief       The window properties creation function.
 * \details     Displays a properties window for an object.
 * \param ctxt  The pointer of the guicontext.
 * \param owner The owner object pointer.
 * \return      This function returns a pointer to the window properties object.
 */
t_object* eguicontext_propw_new(t_eguicontext *ctxt, t_object* owner);

/*!
 * \fn          void ewindowprop_destroy(t_eobj* x)
 * \brief       The t_ewindowprop free function.
 * \details     Frees the memory for a t_ewindowprop and deletes its window.
 * \param x     The t_eobj pointer that owns the t_ewindowprop.
 */
void ewindowprop_destroy(t_eobj* x);

/*!
 * \fn          void ewindowprop_update(t_eobj* x)
 * \brief       The t_ewindowprop update function.
 * \details     Updates the values of the properties window.
 * \param x     The t_eobj pointer that owns the t_ewindowprop.
 */
void ewindowprop_update(t_eobj* x);

/*!
 * \fn          t_ewindowprop* ewindowprop_getfromobject(t_eobj* x)
 * \brief       Retrieves a t_ewindowprop of a t_eobj.
 * \param x     The t_eobj pointer that owns the t_ewindowprop.
 * \return      This function returns a pointer to the t_ewindowprop if it exists.
 */
t_object* epropw_getfromname(t_symbol* name);

#ifdef CICM_WRAPPER_TCL

/*!
 * \fn              t_pd_err eguicontext_canvas_mousedown(t_eguicontext *ctxt, t_canvas* cnv, emod_flags mods)
 * \brief           Generates a mouse down event to the canvas.
 * \param ctxt      The pointer of the guicontext.
 * \param cnv       The pointer of the canvas.
 * \param mod       The modifiers of events.
 * \return          A null value if nothing goes wrong, otherwise a non-null value.
 */
t_pd_err eguicontext_canvas_mousedown(t_eguicontext *ctxt, t_canvas* cnv, long mod);

/*!
 * \fn              t_pd_err eguicontext_canvas_mouseup(t_eguicontext *ctxt, t_canvas* cnv, emod_flags mods)
 * \brief           Generates a mouse up event to the canvas.
 * \param ctxt      The pointer of the guicontext.
 * \param cnv       The pointer of the canvas.
 * \param mod       The modifiers of events.
 * \return          A null value if nothing goes wrong, otherwise a non-null value.
 */
t_pd_err eguicontext_canvas_mouseup(t_eguicontext *ctxt, t_canvas* cnv, long mod);

#endif

/** @} */

#endif
