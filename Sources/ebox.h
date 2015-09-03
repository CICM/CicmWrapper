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

/*! @addtogroup groupbox
 * @{
 */

/*!
 * \fn          void ebox_new(t_ebox *x, long flags)
 * \brief       Initializes the graphical members of the t_ebox.
 * \details     Sets the defaults values and initializes the attributes. \n This function should be call after eobj_new().
 * \param x     The t_ebox pointer.
 * \param flags A set of flags that defines the t_ebox behavior.
 * \see ebox_flags
 * \todo Not here but for doc (in ebox_wvis vis is called sevral times with vis = 1 perhaps we can avoid to recreate the stuff at each time)
 */
void ebox_new(t_ebox *x, long flags);


/*!
 * \fn          void ebox_ready(t_ebox *x)
 * \brief       Indicates that the t_ebox can be drawn.
 * \details     Actives the drawind methods. \n This function should be call after durng the new method just before returning the object.
 * \param x     The t_ebox pointer.
 */
void ebox_ready(t_ebox *x);

/*!
 * \fn          void ebox_free(t_ebox* x)
 * \brief       Indicates that the t_ebox can be drawn.
 * \details     Deletes the drawings. \n This function should replace pd_free() and you shouldn't have to call eobj_free() or eobj_dspfree();
 * \param x     The t_ebox pointer.
 */
void ebox_free(t_ebox* x);

/*!
 * \fn          t_pd* ebox_getsender(t_ebox* x)
 * \brief       Retrieves the link list of object binded to the t_ebox.
 * \param x     The t_ebox pointer.
 * \return      The pointer to the link list.
 */
t_pd* ebox_getsender(t_ebox* x);

/*!
 * \fn          char ebox_isdrawable(t_ebox* x)
 * \brief       Retrieves if a t_ebox is drawable.
 * \details     Checks several things that ensure that the t_ebox can be drawn;
 * \param x     The t_ebox pointer.
 * \return      The function returns 1 if the box is drawable otherwise 0.
 */
char ebox_isdrawable(t_ebox* x);

/*!
 * \fn      void ebox_attrprocess_viabinbuf(void *x, t_binbuf *d)
 * \brief   Changes the attributes with a binbuf.
 * \details Retrieves and interpretes a binbuf to initialize the attributes.
 * \param x The t_ebox pointer.
 * \param d The binbuf pointer.
 */
void ebox_attrprocess_viabinbuf(void *x, t_binbuf *d);

/*!
 * \fn      void ebox_attrprocess_viatoms(void *x, int argc, t_atom *argv)
 * \brief   Changes the attributes with an array of atoms.
 * \details Retrieves and interpretes a binbuf to initialize the attributes.
 * \param x The t_ebox pointer.
 * \param argc The number of atoms.
 * \param argv The pointer to the atoms.
 */
void ebox_attrprocess_viatoms(void *x, int argc, t_atom *argv);

/*!
 * \fn      void ebox_set_cursor(t_ebox* x, int cursor)
 * \brief   Changes the cursor of the mouse.
 * \param x The t_ebox pointer.
 * \param cursor The type of cursor.
 * \see ebox_cursors
 */
void ebox_set_cursor(t_ebox* x, int cursor);

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
 * \fn      t_elayer* ebox_start_layer(t_ebox *b, t_symbol *name, float width, float height)
 * \brief   Creates or initializes a layer for the t_ebox.
 * \details If the layer is new, the function allocate a new empty layer. if the layer already exist and is marked as invalid the layer is returned otherwise(if the layer is maked as redeay to be drawn for example) the function returns NULL. You should always check the returned layer of the function before using it.
 * \param x The t_ebox pointer.
 * \param name The name of the layer.
 * \param width The width of the layer.
 * \param height The height of the layer.
 * \see ebox_end_layer ebox_invalidate_layer ebox_paint_layer t_elayer_flags t_elayer
 */
t_elayer* ebox_start_layer(t_ebox *x, t_symbol *name, float width, float height);

/*!
 * \fn      t_pd_err ebox_end_layer(t_ebox *b, t_symbol *name)
 * \brief   Marks a layer as ready to be drawn.
 * \details After the several painting methods, you should call this function to notify  that the layer is ready to be drawn.
 * \param x The t_ebox pointer.
 * \param name The name of the layer.
 * \see ebox_start_layer ebox_invalidate_layer ebox_paint_layer t_elayer_flags t_elayer
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
 * \see ebox_start_layer ebox_end_layer ebox_invalidate_layer t_elayer_flags t_elayer
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
//! The mouse enter method called by tcl/tk (PRIVATE)
/*
 * \memberof        ebox
 * \param x         The ebox pointer
 * \return          Nothing
 */
void ebox_mouse_enter(t_ebox* x);

//! The mouse leave method called by tcl/tk (PRIVATE)
/*
 * \memberof        ebox
 * \param x         The ebox pointer
 * \return          Nothing
 */
void ebox_mouse_leave(t_ebox* x);

//! The mouse move method called by tcl/tk (PRIVATE)
/*
 * \memberof        ebox
 * \param x         The ebox pointer
 * \param s         The message selector
 * \param argc      The size of the array of atoms
 * \param argv      The array of atoms
 * \return          Nothing
 */
void ebox_mouse_move(t_ebox* x, t_symbol* s, int argc, t_atom *argv);

//! The mouse up method called by tcl/tk (PRIVATE)
/*
 * \memberof        ebox
 * \param x         The ebox pointer
 * \param s         The message selector
 * \param argc      The size of the array of atoms
 * \param argv      The array of atoms
 * \return          Nothing
 */
void ebox_mouse_up(t_ebox* x, t_symbol* s, int argc, t_atom *argv);

//! The mouse down method called by tcl/tk (PRIVATE)
/*
 * \memberof        ebox
 * \param x         The ebox pointer
 * \param s         The message selector
 * \param argc      The size of the array of atoms
 * \param argv      The array of atoms
 * \return          Nothing
 */
void ebox_mouse_down(t_ebox* x, t_symbol* s, int argc, t_atom *argv);

//! The mouse double click method called by tcl/tk (PRIVATE)
/*
 * \memberof        ebox
 * \param x         The ebox pointer
 * \param s         The message selector
 * \param argc      The size of the array of atoms
 * \param argv      The array of atoms
 * \return          Nothing
 */
void ebox_mouse_dblclick(t_ebox* x, t_symbol* s, int argc, t_atom *argv);

//! The mouse wheel method called by tcl/tk (PRIVATE)
/*
 * \memberof        ebox
 * \param x         The ebox pointer
 * \param s         The message selector
 * \param argc      The size of the array of atoms
 * \param argv      The array of atoms
 * \return          Nothing
 */
void ebox_mouse_wheel(t_ebox* x, t_symbol* s, int argc, t_atom *argv);

//! The key down method called by tcl/tk (PRIVATE AND NOT READY)
/*
 * \memberof        ebox
 * \param x         The ebox pointer
 * \param s         The message selector
 * \param argc      The size of the array of atoms
 * \param argv      The array of atoms
 */
void ebox_key(t_ebox* x, t_symbol* s, int argc, t_atom *argv);

//! The method to move an UI ebox (PRIVATE)
/*
 * \memberof        ebox
 * \param x         The ebox pointer
 * \param newx      The new abscissa
 * \param newy      The new ordinate
 */
void ebox_pos(t_ebox* x, float newx, float newy);

//! The method to show or hide an UI ebox (PRIVATE)
/*
 * \memberof        ebox
 * \param x         The ebox pointer
 * \param vis       The visible state
 */
void ebox_vis(t_ebox* x, int vis);

//! The default user id method for all ebox called by PD (PRIVATE)
/*
 * \memberof        ebox
 * \param x         The gobj
 * \param attr      Nothing (for Max 6 compatibility)
 * \param argc      The size of the array of atoms
 * \param argv      The array of atoms
 * \return          Always 0 (for the moment)
 */
t_pd_err ebox_set_receiveid(t_ebox *x, t_object *attr, int argc, t_atom *argv);

//! The default user send id method for all ebox called by PD (PRIVATE)
/*
 * \memberof        ebox
 * \param x         The gobj
 * \param attr      Nothing (for Max 6 compatibility)
 * \param argc      The size of the array of atoms
 * \param argv      The array of atoms
 * \return          Always 0 (for the moment)
 */
t_pd_err ebox_set_sendid(t_ebox *x, t_object *attr, int argc, t_atom *argv);

//! The default user preset id method for all ebox called by PD (PRIVATE)
/*
 * \memberof        ebox
 * \param x         The ebox
 * \param attr      Nothing (for Max 6 compatibility)
 * \param argc      The size of the array of atoms
 * \param argv      The array of atoms
 * \return          Always 0 (for the moment)
 */
t_pd_err ebox_set_presetid(t_ebox *x, t_object *attr, int argc, t_atom *argv);

//! Retrive the preset id of an ebox
/*
 * \memberof        ebox
 * \param x         The ebox
 */
t_symbol* ebox_get_presetid(t_ebox* x);

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

//! The default size attribute method of ebox called when an size attribute has changed. This function restrains the width and the height depending of the ebox flags EBOX_GROWNO, EBOX_GROWLINK and EBOX_GROWINDI // PRIVATE
/*
 * \memberof        ebox
 * \param x         The ebox
 * \param attr      Nothing (for Max 6 compatibility)
 * \param argc      The size of the array of atoms
 * \param argv      The array of atoms that contains the new width and the new height
 * \return          Always 0 (for the moment)
 */
t_pd_err ebox_size_set(t_ebox *x, t_object *attr, int argc, t_atom *argv);

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

void ebox_set_parameter_attribute(t_ebox *x, t_symbol *s, int argc, t_atom* argv);

// The defaults pd widgets
void ebox_wgetrect(t_gobj *z,     t_glist *glist, int *xp1, int *yp1, int *xp2, int *yp2);
void ebox_wvis(t_gobj *z,         t_glist *glist, int vis);
void ebox_wdisplace(t_gobj *z,    t_glist *glist, int dx, int dy);
void ebox_wselect(t_gobj *z,      t_glist *glist, int selected);
void ebox_wdelete(t_gobj *z,      t_glist *glist);
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
 * \fn              void ebox_parameter_setmin(t_ebox* x, int index, float min)
 * \brief           Sets the minimum value of the parameter.
 * \param x         The t_ebox pointer.
 * \param index     The index of the parameter.
 * \param min       The minimum value of the parameter.
 */
void ebox_parameter_setmin(t_ebox* x, int index, float min);

/*!
 * \fn              void ebox_parameter_setmax(t_ebox* x, int index, float max)
 * \brief           Sets the maximum value of the parameter.
 * \param x         The t_ebox pointer.
 * \param index     The index of the parameter.
 * \param max       The maximum value of the parameter.
 */
void ebox_parameter_setmax(t_ebox* x, int index, float max);

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
 * \fn              float eparameter_getvalue(t_eparam* param)
 * \brief           Retrieves the current (non-normalized) value of the parameter.
 * \details         This function shoudl be called from outside the owner object.
 * \param param     The t_eparam pointer.
 * \return          This function returns the current value of the parameter.
 */
float eparameter_getvalue(t_eparam* param);

/*!
 * \fn              float eparameter_getvalue_normalized(t_eparam* param)
 * \brief           Retrieves the current normalized value of the parameter.
 * \details         This function shoudl be called from outside the owner object.
 * \param param     The t_eparam pointer.
 * \return          This function returns the current value of the parameter.
 */
float eparameter_getvalue_normalized(t_eparam* param);

/*!
 * \fn              void eparameter_getvalue_text(t_eparam* param, char* text);
 * \brief           Retrieves the current value of the parameter as a string.
 * \details         This function shoudl be called from outside the owner object.
 * \param param     The t_eparam pointer.
 * \param text      The string to fill (the size of the string should be big enough to retrieve the value).
 */
void eparameter_getvalue_text(t_eparam* param, char* text);

/*!
 * \fn              void eparameter_setvalue(t_eparam* param, float value)
 * \brief           Sets the current (non-normalized) value of the parameter and notify the owner that the value changed.
 * \details         This function shoudl be called from outside the owner object.
 * \param param     The t_eparam pointer.
 * \param value     The new value of the parameter.
 */
void eparameter_setvalue(t_eparam* param, float value);

/*!
 * \fn              void eparameter_setvalue_normalized(t_eparam* param, float value)
 * \brief           Sets the current normalized value of the parameter and notify the owner that the value changed.
 * \details         This function shoudl be called from outside the owner object.
 * \param param     The t_eparam pointer.
 * \param value     The new value of the parameter.
 */
void eparameter_setvalue_normalized(t_eparam* param, float value);

/*!
 * \fn              void eparameter_setvalue_text(t_eparam* param, char* text)
 * \brief           Sets the current value of the parameter with a string and notify the owner that the value changed.
 * \details         This function shoudl be called from outside the owner object.
 * \param param     The t_eparam pointer.
 * \param text      The value as a string.
 */
void eparameter_setvalue_text(t_eparam* param, char const* text);

/*!
 * \fn          t_eparam* eparameter_getfromsymbol(t_symbol* name)
 * \brief       Retrieves a t_eparam from a symbol.
 * \param name  The binding symbol of the t_eparam.
 */
t_eparam* eparameter_getfromsymbol(t_symbol* name);

//! @endcond

#endif



