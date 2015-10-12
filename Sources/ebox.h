/*
 * CicmWrapper - Pure Data Enhanced

 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

/**
 * @file eobj.h
 * @brief The header that declares all the methods for the t_ebox.
 * @author Pierre Guillot
 */

#ifndef DEF_EBOX
#define DEF_EBOX

#include "edefine.h"
#include "eparam.h"

/*! @addtogroup groupbox
 * @{
 */


/*!
 * \fn      void ebox_set_cursor(t_ebox* x, int cursor)
 * \brief   Changes the cursor of the mouse.
 * \param x The t_ebox pointer.
 * \param cursor The type of cursor.
 * \see ebox_cursors
 */
void ebox_set_cursor(t_ebox* x, t_object* view, int cursor);

/*!
 * \fn      void ebox_redraw(t_ebox* x)
 * \brief   Notifies the t_ebox that it should be redrawn.
 * \param x The t_ebox pointer.
 */
void ebox_redraw(t_ebox *x);

/*!
 * \fn      void ebox_get_rect_for_view(t_ebox* x, t_rect *rect)
 * \brief   Retrieves the rectangle of the t_ebox.
 * \details This function should be used while the paint method is called to retrieves the current size of the t_ebox.
 * \param x The t_ebox pointer.
 * \param rect The t_rect pointer.
 */
void ebox_get_rect_for_view(t_ebox* x, t_rect *rect);

/*!
 * \fn      t_elayer* ebox_start_layer(t_ebox *b, t_object* view, t_symbol *name, float width, float height)
 * \brief   Creates or initializes a layer for the t_ebox.
 * \details If the layer is new, the function allocate a new empty layer. if the layer already exist and is marked as invalid the layer is returned otherwise(if the layer is maked as redeay to be drawn for example) the function returns NULL. You should always check the returned layer of the function before using it.
 * \param x         The t_ebox pointer.
 * \param view      The view pointer.
 * \param name      The name of the layer.
 * \param width     The width of the layer.
 * \param height    The height of the layer.
 * \see ebox_end_layer ebox_invalidate_layer ebox_paint_layer
 */
t_elayer* ebox_start_layer(t_ebox *x, t_object* view, t_symbol *name, float width, float height);

/*!
 * \fn      t_pd_err ebox_end_layer(t_ebox *b, t_symbol *name)
 * \brief   Marks a layer as ready to be drawn.
 * \details After the several painting methods, you should call this function to notify  that the layer is ready to be drawn.
 * \param x The t_ebox pointer.
 * \param name The name of the layer.
 * \see ebox_start_layer ebox_invalidate_layer ebox_paint_layer
 */
t_pd_err ebox_end_layer(t_ebox *x, t_symbol *name);

/*!
 * \fn      t_pd_err ebox_paint_layer(t_ebox *x, t_symbol *name, float x_p, float y_p)
 * \brief   Paints a layer in the t_ebox.
 * \details If a layer is ready to be drawn, the function will paint it in the t_ebox.
 * \param x The t_ebox pointer.
 * \param name The name of the layer.
 * \param x_p The abscissa of the layer in the t_ebox.
 * \param y_p The ordinate of the layer in the t_ebox.
 * \see ebox_start_layer ebox_end_layer ebox_invalidate_layer
 */
t_pd_err ebox_paint_layer(t_ebox *x, t_symbol *name, float x_p, float y_p);

/*!
 * \fn      ebox_invalidate_layer(t_ebox *b, t_symbol *name)
 * \brief   Marks a layer as invalid.
 * \details When you want to redraw a layer you have to call this function and then call ebox_redraw. You should always prefer to invalidate several layer before calling ebox_redraw.
 * \param x The t_ebox pointer.
 * \param name The name of the layer.
 * \see ebox_start_layer ebox_end_layer ebox_redraw ebox_paint_layer t_elayer_flags t_elayer
 */
t_pd_err ebox_invalidate_layer(t_ebox *x, t_symbol *name);

/** @} */

//! @cond


//! The default notify method of ebox called when an attribute has changed // PRIVATE
/*
 * \memberof        ebox
 * \param x         The ebox
 * \param s         The name of the attribute
 * \param msg       Nothing (for Max 6 compatibility)
 * \param sender    Nothing (for Max 6 compatibility)
 * \param data      Nothing (for Max 6 compatibility)
 * \return          Always 0 (for the moment)
 */
t_pd_err ebox_notify(t_ebox *x, t_symbol *s, t_symbol *msg, void *sender, void *data);


//!
/*
 * \memberof        ebox
 * \param x         The object
 * \param s         Nothing (for Max 6 compatibility)
 * \return          Nothing
 */
void ebox_texteditor_keypress(t_ebox *x,  t_symbol *s, float f);

//!
/*
 * \memberof        ebox
 * \param x         The object
 * \param s         Nothing (for Max 6 compatibility)
 * \return          Nothing
 */
void ebox_texteditor_keyfilter(t_ebox *x, t_symbol *s, float f);

//!
/*
 * \memberof        ebox
 * \param x         The object
 * \param s         Nothing (for Max 6 compatibility)
 * \return          Nothing
 */
void ebox_texteditor_focus(t_ebox *x, t_symbol *s, float f);

void ebox_set_parameter_attribute(t_ebox *x, t_symbol *s, int argc, t_atom* argv);

// The defaults pd widgets
//! @endcond


/*!
 * \fn          t_eparam* ebox_parameter_create(t_ebox *x, int index)
 * \brief       The t_eparam creation function.
 * \details     Allocates the memory for a t_eparam, intializes the defaults values and links the t_eparam to the t_ebox.
 * \param x     The t_ebox pointer.
 * \param index The index of the parameter.
 * \return      This function returns a pointer to a new t_eparam.
 */
t_eparam* ebox_parameter_create(t_ebox *x, int index);

/*!
 * \fn          void ebox_parameter_destroy(t_ebox *x, int index)
 * \brief       The t_eparam deletion function.
 * \details     Frees and removes a t_eparam from the t_ebox parameter list.
 * \param x     The t_ebox pointer.
 * \param index The index of the parameter.
 */
void ebox_parameter_destroy(t_ebox *x, int index);

/*!
 * \fn          float ebox_parameter_getvalue(t_ebox *x, int index)
 * \brief       Retrieves the current (non-normalized) value of the parameter.
 * \param x     The t_ebox pointer.
 * \param index The index of the parameter.
 * \return      This function returns the current value of the parameter.
 */
float ebox_parameter_getvalue(t_ebox *x, int index);

/*!
 * \fn              void ebox_parameter_setvalue(t_ebox* x, int index, float value, char notify)
 * \brief           Sets the current (non-normalized) value of the parameter and notify the listeners if needed.
 * \param x         The t_ebox pointer.
 * \param index     The index of the parameter.
 * \param value     The new value of the parameter.
 * \param notify    A non null value if the listener should be notified.
 */
void ebox_parameter_setvalue(t_ebox* x, int index, float value, char notify);

/*!
 * \fn              float ebox_parameter_getvalue_normalized(t_ebox* x, int index)
 * \brief           Retrieves the current normalized value of the parameter.
 * \param x         The t_ebox pointer.
 * \param index     The index of the parameter.
 * \return          This function returns the current value of the parameter.
 */
float ebox_parameter_getvalue_normalized(t_ebox* x, int index);

/*!
 * \fn              void ebox_parameter_setvalue_normalized(t_ebox* x, int index, float value, char notify)
 * \brief           Sets the current normalized value of the parameter and notify the listeners if needed.
 * \param x         The t_ebox pointer.
 * \param index     The index of the parameter.
 * \param value     The new value of the parameter.
 * \param notify    A non null value if the listener should be notified.
 */
void ebox_parameter_setvalue_normalized(t_ebox* x, int index, float value, char notify);

/*!
 * \fn              float ebox_parameter_getmin(t_ebox* x, int index)
 * \brief           Retrieves the minimum value of the parameter.
 * \param x         The t_ebox pointer.
 * \param index     The index of the parameter.
 * \return          The minimum value of the parameter.
 */
float ebox_parameter_getmin(t_ebox* x, int index);

/*!
 * \fn              float ebox_parameter_getmax(t_ebox* x, int index)
 * \brief           Retrieves the maximum value of the parameter.
 * \param x         The t_ebox pointer.
 * \param index     The index of the parameter.
 * \return          The maximum value of the parameter.
 */
float ebox_parameter_getmax(t_ebox* x, int index);

/*!
 * \fn              int ebox_parameter_getnstep(t_ebox* x, int index)
 * \brief           Retrieves the number of steps of the parameter.
 * \param x         The t_ebox pointer.
 * \param index     The index of the parameter.
 * \return          The number of steps of the parameter.
 */
int ebox_parameter_getnstep(t_ebox* x, int index);

/*!
 * \fn              t_symbol* ebox_parameter_getbind(t_ebox* x, int index)
 * \brief           Retrieves the binding t_symbol of the parameter.
 * \param x         The t_ebox pointer.
 * \param index     The index of the parameter.
 * \return          The binding t_symbol of the parameter.
 */
t_symbol* ebox_parameter_getbind(t_ebox* x, int index);

/*!
 * \fn              char ebox_parameter_isinverted(t_ebox* x, int index)
 * \brief           Retrieves if the minimum and the maximum are inverted.
 * \param x         The t_ebox pointer.
 * \param index     The index of the parameter.
 * \return          A non null value if the minimum and the maximum are inverted, otherwise 0.
 */
char ebox_parameter_isinverted(t_ebox* x, int index);

/*!
 * \fn              void ebox_parameter_notify_changes(t_ebox *x, int index)
 * \brief           Notifies the listeners that the value changed.
 * \param x         The t_ebox pointer.
 * \param index     The index of the parameter.
 */
void ebox_parameter_notify_changes(t_ebox *x, int index);

/*!
 * \fn              void ebox_parameter_begin_changes(t_eparam* param);
 * \brief           Notifies the listeners that the value begins to change.
 * \param x         The t_ebox pointer.
 * \param index     The index of the parameter.
 */
void ebox_parameter_begin_changes(t_ebox *x, int index);

/*!
 * \fn              void ebox_parameter_end_changes(t_eparam* param)
 * \brief           Notifies the listeners that the value finishes to change.
 * \param x         The t_ebox pointer.
 * \param index     The index of the parameter.
 */
void ebox_parameter_end_changes(t_ebox *x, int index);

/*!
 * \fn              void ebox_parameter_setname(t_ebox* x, int index, t_symbol* name)
 * \brief           Sets the name of the parameter.
 * \param x         The t_ebox pointer.
 * \param index     The index of the parameter.
 * \param name      The name of the parameter.
 */
void ebox_parameter_setname(t_ebox* x, int index, t_symbol* name);

/*!
 * \fn              void ebox_parameter_setlabel(t_ebox* x, int index, t_symbol* label)
 * \brief           Sets the label of the parameter.
 * \param x         The t_ebox pointer.
 * \param index     The index of the parameter.
 * \param label     The label of the parameter.
 */
void ebox_parameter_setlabel(t_ebox* x, int index, t_symbol* label);

/*!
 * \fn              void ebox_parameter_setminmax(t_ebox* x, int index, float max)
 * \brief           Sets the maximum and maximum value of the parameter.
 * \param x         The t_ebox pointer.
 * \param index     The index of the parameter.
 * \param min       The minimum value of the parameter.
 * \param max       The maximum value of the parameter.
 */
void ebox_parameter_setminmax(t_ebox* x, int index, float min, float max);

/*!
 * \fn              void ebox_parameter_setnstep(t_ebox* x, int index, int nstep)
 * \brief           Sets the step value of the parameter.
 * \param x         The t_ebox pointer.
 * \param index     The index of the parameter.
 * \param nstep     The number of step of the parameter.
 */
void ebox_parameter_setnstep(t_ebox* x, int index, int nstep);

/*!
 * \fn              void ebox_parameter_setindex(t_ebox* x, int index, int pindex)
 * \brief           Sets the index of the parameter.
 * \param x         The t_ebox pointer.
 * \param index     The index of the parameter.
 * \param pindex    The index of the parameter.
 */
void ebox_parameter_setindex(t_ebox* x, int index, int pindex);

/*!
 * \fn              void ebox_parameter_setflags(t_ebox* x, int index, long flags)
 * \brief           Sets the flags of the parameter.
 * \param x         The t_ebox pointer.
 * \param index     The index of the parameter.
 * \param flags     The flags the parameter.
 */
void ebox_parameter_setflags(t_ebox* x, int index, long flags);

/*!
 * \fn              void ebox_parameter_setsettergetter(t_ebox* x, int index, t_param_setter setter, t_param_setter getter)
 * \brief           Sets the setter and getter methods of the parameter.
 * \param x         The t_ebox pointer.
 * \param index     The index of the parameter.
 * \param setter    The setter method or NULL for the default one.
 * \param getter    The getter method or NULL for the default one.
 */
void ebox_parameter_setsettergetter(t_ebox* x, int index, t_param_setter setter, t_param_getter getter);

/*!
 * \fn              void ebox_parameter_setsettergetter_text(t_ebox* x, int index, t_param_setter_t setter, t_param_getter_t getter)
 * \brief           Sets the setter and getter methods for text values of the parameter.
 * \param x         The t_ebox pointer.
 * \param index     The index of the parameter.
 * \param setter    The setter method or NULL for the default one.
 * \param getter    The getter method or NULL for the default one.
 */
void ebox_parameter_setsettergetter_text(t_ebox* x, int index, t_param_setter_t setter, t_param_getter_t getter);

/*!
 * \fn              void ebox_parameter_enable(t_ebox* x, int index)
 * \brief           Enables a parameter.
 * \param x         The t_ebox pointer.
 * \param index     The index of the parameter.
 */
void ebox_parameter_enable(t_ebox* x, int index);

/*!
 * \fn              void ebox_parameter_disable(t_ebox* x, int index)
 * \brief           Disables a parameter.
 * \param x         The t_ebox pointer.
 * \param index     The index of the parameter.
 */
void ebox_parameter_disable(t_ebox* x, int index);

/*!
 * \fn              void ebox_parameter_setflags(t_ebox* x, int index, long flags)
 * \brief           Sets the flags of the parameter.
 * \param x         The t_ebox pointer.
 * \param index     The index of the parameter.
 * \param flags     The flags the parameter.
 */
void ebox_parameter_setflags(t_ebox* x, int index, long flags);

//! @cond
/*!
 * \fn          t_eparam* eparameter_getbyindex(t_ebox* x, int index)
 * \brief       Retrieves a t_eparam from a symbol.
 * \param x     The t_ebox that owns the parameter.
 * \param index The index of the parameter.
 */
t_eparam* eparameter_getbyindex(t_ebox* x, int index);

//! @endcond

#endif



