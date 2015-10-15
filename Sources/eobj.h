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
 * @file eobj.h
 * @brief The header that declares all the methods for the t_eobj and t_edspobj.
 * @author Pierre Guillot
 */

#ifndef DEF_EOBJ
#define DEF_EOBJ

#include "edefine.h"

/*! @addtogroup groupobj
 *  @{
 */

/*!
 * \fn          void *eobj_new(t_eclass *c)
 * \brief       The t_eobj creation function.
 * \details     Allocates the memory for an intance of a t_eobj of a specific eclass, intialize the defaults values and methods. \n This function should replace pd_new().
 * \param c     The t_eclass pointer.
 * \return      This function returns a pointer to a new instance of a t_eobj.
 */
void *eobj_new(t_eclass *c);

/*!
 * \fn          void eobj_free(void *x)
 * \brief       The t_eobj deletion function. \n This function should replace pd_free().
 * \details     Detaches the t_eobj from Pure Data and frees the proxy inlets.
 * \param x     The t_eobj pointer
 */
void eobj_free(void *x);

/*!
 * \fn          char eobj_iscicm(void const* x)
 * \brief       Checks if the t_object uses a t_eclass.
 * \param x     The t_eobj pointer.
 * \return      This function returns 1 if the t_object uses a t_eclass, otherwise it returns 0.
 */
char eobj_iscicm(void const* x);

/*!
 * \fn          char eobj_isgui(void const* x)
 * \brief       Retreives if a t_eobj is a GUI box or not.
 * \details     Check if the box flag is postive or null.
 * \param x     The t_eobj pointer.
 * \return      This function returns 1 if the t_eobj is a GUI and 0 if not.
 */
char eobj_isgui(void const* x);

/*!
 * \fn          char eobj_isdsp(void const* x)
 * \brief       Retreives if a t_eobj is a DSP object or not
 * \details     Check if the dsp method has been initialized.
 * \param x     The t_eobj pointer.
 * \return      This function returns 1 if the t_eobj is a DSP object and 0 if not.
 */
char eobj_isdsp(void const* x);

/*!
 * \fn          void eobj_proxynew(void* x)
 * \brief       Adds a proxy inlet to a t_eobj.
 * \details     Allocates and initializes a new proxy inlet. 
 * \details     This function should replace inlet_new().
 * \param x     The t_eobj pointer.
 * \todo Later try to use selector specification with va_list parameters.
 */
void eobj_proxynew(void* x);

/*!
 * \fn          void eobj_proxyfree(void* x);
 * \brief       Removes the last proxy inlet of a t_eobj.
 * \details     Frees a the last proxy inlet.
 * \param x     The t_eobj pointer.
 */
void eobj_proxyfree(void* x);

/*!
 * \fn          size_t eobj_getproxy(void const* x)
 * \brief       Retreives the index of the proxy that received the last message.
 * \details     Retreives the current proxy index. This function should only be used while a message has been received.
 * \param x     The t_eobj pointer.
 * \return      This function return the index of the current proxy.
 */
size_t eobj_getproxy(void const* x);

/*!
 * \fn          t_eclass* eobj_getclass(void const* x)
 * \brief       Retrieves the eclass of a t_eobj.
 * \details     Directly access to the eclass structure of the t_eobj.
 * \param x     The t_eobj pointer.
 * \return      This function returns a pointer to the eclass of the t_eobj.
 */
t_eclass* eobj_getclass(void const* x);

/*!
 * \fn          t_symbol* eobj_getclassname(void const* x)
 * \brief       Retrieves the classe name of a t_eobj
 * \details     Directly access to the name of the eclass of the t_eobj.
 * \param x     The t_eobj pointer.
 * \return      This function returns the name of the class of the t_eobj.
 */
t_symbol* eobj_getclassname(void const* x);

/*!
 * \fn          t_canvas* eobj_getcanvas(void const* x)
 * \brief       Retreives the canvas that owns the t_eobj.
 * \details     Directly access to the canvas structure of the t_eobj.
 * \param x     The t_eobj pointer.
 * \return      This function returns a pointer the canvas that owns the t_eobj.
 */
t_canvas* eobj_getcanvas(void const* x);

/*!
 * \fn          t_symbol* eobj_getid(void const* x)
 * \brief       Retreives the id of the t_eobj.
 * \details     Directly access to the t_symbol id structure of the t_eobj.
 * \param x     The t_eobj pointer.
 * \return      This function returns a pointer t_symbol id.
 */
t_symbol* eobj_getid(void const* x);

/*!
 * \fn          void eobj_bind(void const* b, void* const l)
 * \brief       Binds a listener t_object to another to a broadcaster t_eobj.
 * \details     When a listener is attached to a broadcaster, the broadcaster will notify the
 * \details     listener via its t_notify_method method.
 * \param b     The t_eobj broadcaster pointer.
 * \param l     The t_object listener pointer.
 * \see eobj_unbind, t_notify_method
 */
void eobj_bind(void const* b, void* const l);

/*!
 * \fn          void eobj_unbind(void const* b, void* const l)
 * \brief       Unbind a listener t_object to another to a broadcaster t_eobj.
 * \details     When a listener is attached to a broadcaster, the broadcaster will notify the
 * \details     listener via its t_notify_method method.
 * \param b     The t_eobj broadcaster pointer.
 * \param l     The t_object listener pointer.
 * \see eobj_bind, t_notify_method
 */
void eobj_unbind(void const* b, void* const l);

/*!
 * \fn      void eobj_attr_read(void *x, t_binbuf *b)
 * \brief   Initializes the attributes values with a binbuf.
 * \param x The t_ebox pointer.
 * \param b The binbuf pointer.
 */
void eobj_attr_read(void *x, t_binbuf const* b);

/*!
 * \fn      void eobj_attr_write(void *x, t_binbuf *b)
 * \brief   Writes the attributes values in a binbuf
 * \param x The t_ebox pointer.
 * \param b The binbuf pointer.
 */
void eobj_attr_write(void const* x, t_binbuf *b);

/*!
 * \fn      void eobj_attr_setvalue(void *x, t_symbol* s, int argc, t_atom *argv)
 * \brief   Sets the values of an attribute.
 * \param x The t_eobj pointer.
 * \param s The name of the attribute.
 * \param argc The number of the atoms.
 * \param argv The array of the atoms.
 */
void eobj_attr_setvalue(void *x, t_symbol* s, int argc, t_atom *argv);

/*!
 * \fn      void eobj_attr_getvalue(void *x, t_symbol* s, int *argc, t_atom **argv)
 * \brief   Gets the values of an attribute.
 * \details You should free the atoms if they has been allocated.
 * \param x The t_eobj pointer.
 * \param s The name of the attribute.
 * \param argc The pointer to an int that will be initialize with the number of the atoms.
 * \param argv The pointer to the array of the atoms that will be allocated if the attribute exist.
 */
void eobj_attr_getvalue(void *x, t_symbol *s, int *argc, t_atom **argv);









/*!
 * \fn void     eobj_dspsetup(void *x, size_t nins, size_t nouts)
 * \brief       Initializes the dsp members of the t_edsp.
 * \details     Allocate the memory for digital dignal processing of a t_edspobj. \n This funtion should replace inlet_new and outlet_new for the signal.
 * \param x     The edspobj pointer
 * \param nins  The number of signal inputs.
 * \param nouts The number of signal outputs
 */
void eobj_dspsetup(void *x, size_t nins, size_t nouts);

/*!
 * \fn void eobj_dspflags(void *x, long flags)
 * \brief       Initializes the flags of the t_edsp.
 * \details     Sets the dsp inplace (default) or not inplace. You should use this function before the call of the dsp method (in the new method for example).
 * \param x     The edspobj pointer
 * \param flags The dsp flags.
 * \see edsp_flags
 */
void eobj_dspflags(void *x, long flags);

/*!
 * \fn          void eobj_dspfree(void *x)
 * \brief       The t_edspobj deletion function. \n This function should replace pd_free().
 * \details     Frees the memory allocated for digital dignal processing of a t_edspobj. \n This function should replace pd_free() and you shouldn't have to call eobj_free();
 * \param x     The edspobj pointer
 */
void eobj_dspfree(void *x);

/*!
 * \fn          t_sample* eobj_dspgetinsamples(void *x, size_t index)
 * \brief       Retrieves the pointer to the signal of an input.
 * \details     Retrieves the pointer to the signal vector of an input.
 * \details     The can function can only be used during the dsp method call.
 * \param x     The edspobj pointer.
 * \param index The index of the input.
 * \return      A pointer to the signal vector or NULL.
 */
t_sample* eobj_dspgetinsamples(void *x, size_t index);

/*!
 * \fn          t_sample* eobj_dspgetoutsamples(void *x, size_t index)
 * \brief       Retrieves the pointer to the signal of an output.
 * \details     Retrieves the pointer to the signal vector of an output.
 * \details     The can function can only be used during the dsp method call.
 * \param x     The edspobj pointer.
 * \param index The index of the output.
 * \return      A pointer to the signal vector or NULL.
 */
t_sample* eobj_dspgetoutsamples(void *x, size_t index);







/*!
 * \fn          void ebox_new(t_ebox *x, long flags)
 * \brief       Initializes the graphical members of the box.
 * \details     Sets the defaults values and initializes the attributes. 
 * \details     This function should be call after eobj_new().
 * \param x     The pointer of the box.
 * \param flags A set of flags that defines the gui behavior.
 */
void ebox_new(t_ebox *x, long flags);

/*!
 * \fn          void ebox_ready(t_ebox *x)
 * \brief       Indicates that the box can be drawn.
 * \details     This function should be call during the new method just before returning the object.
 * \param x     The pointer of the box.
 */
void ebox_ready(t_ebox *x);

/*!
 * \fn          void ebox_free(t_ebox* x)
 * \brief       Frees a box.
 * \details     Deletes the drawings. 
 * \details     This function should replace pd_free().
 * \details     You shouldn't have to call eobj_free() or eobj_dspfree();
 * \param x     The pointer of the box.
 */
void ebox_free(t_ebox* x);

/*!
 * \fn          t_pd* ebox_getsender(t_ebox* x)
 * \brief       Retrieves the link list of object binded to the box.
 * \param x     The pointer of the box.
 * \return      The pointer to the link list.
 */
t_pd* ebox_getsender(t_ebox const* x);

/*!
 * \fn          long ebox_getflags(t_ebox* x)
 * \brief       Retrieves the flags of a box.
 * \param x     The pointer of the box.
 * \return      The flags of the t_ebox.
 */
long ebox_getflags(t_ebox const* x);

/*!
 * \fn          void ebox_redraw(t_ebox* x)
 * \brief       Notifies the box that it should be redrawn.
 * \param x     The pointer of the box.
 */
void ebox_redraw(t_ebox *x);

/*!
 * \fn              void ebox_setcursor(t_ebox* x, t_object* view, ebox_cursors cursor)
 * \brief           Changes the cursor of the mouse.
 * \param x         The pointer of the box.
 * \param view      The pointer of the view (if NULL the function uses the first view).
 * \param cursor    The type of cursor.
 */
void ebox_setcursor(t_ebox const* x, t_object* view, ebox_cursors cursor);

/*!
 * \fn              void ebox_getbounds(t_ebox const* x, t_object const* view, t_rect *rect)
 * \brief           Retrieves the bounds of a view of a box.
 * \param x         The pointer of the box.
 * \param view      The pointer of the view (if NULL the function uses the first view).
 * \param rect      The rectangle pointer.
 */
void ebox_getbounds(t_ebox const* x, t_object const* view, t_rect *rect);

/*!
 * \fn              t_elayer* ebox_start_layer(t_ebox *b, t_object* view, t_symbol *name,
                                                           const float width, const float height)
 * \brief           Creates or initializes a new layer for a view of a box.
 * \details         If the layer is new, the function allocate a new empty layer. 
 * \details         If the layer already exist and is marked as invalid the layer is returned.
 * \details         Otherwise, if the layer is maked as ready to be drawn for example, the 
 * \details         function returns NULL. You should always check the validity of the pointer
 * \details         before using it.
 * \param x         The pointer of the box.
 * \param view      The pointer of the view (if NULL the function uses the first view).
 * \param name      The name of the layer.
 * \param width     The width of the layer.
 * \param height    The height of the layer.
 * \return A null value if nothing goes wrong, otherwise a non-null value.
 * \see ebox_end_layer ebox_invalidate_layer ebox_paint_layer
 */
t_elayer* ebox_start_layer(t_ebox *x, t_object* view, t_symbol *name,
                                      const float width, const float height);

/*!
 * \fn              t_pd_err ebox_end_layer(t_ebox *b, t_symbol *name)
 * \brief           Marks a layer as ready to be drawn.
 * \details         After the several painting methods, you should call this function to 
 * \details         notify a layer that he is ready to be drawn.
 * \param x         The pointer of the box.
 * \param view      The pointer of the view (if NULL the function uses the first view).
 * \param name      The name of the layer.
 * \return A null value if nothing goes wrong, otherwise a non-null value.
 * \see ebox_start_layer ebox_invalidate_layer ebox_paint_layer
 */
t_pd_err ebox_end_layer(t_ebox *x, t_object* view, t_symbol *name);

/*!
 * \fn              t_pd_err ebox_paint_layer(t_ebox *x, t_object* view, t_symbol *name,
                                                         const float xoffset, const float yoffset)
 * \brief           Paints a layer of a box.
 * \details         If a layer is ready to be drawn, the function performs the painting.
 * \param x         The pointer of the box.
 * \param name      The name of the layer.
 * \param view      The pointer of the view (if NULL the function uses the first view).
 * \param xoffset   The abscissa offset of the layer.
 * \param yoffset   The ordinate yoffset of the layer.
 * \return A null value if nothing goes wrong, otherwise a non-null value.
 * \see ebox_start_layer ebox_end_layer ebox_invalidate_layer
 */
t_pd_err ebox_paint_layer(t_ebox *x, t_object* view, t_symbol *name,
                                     const float xoffset, const float yoffset);

/*!
 * \fn              t_pd_err ebox_invalidate_layer(t_ebox *x, t_object* view, t_symbol *name)
 * \brief           Marks a layer as invalid.
 * \details         When you want to redraw a layer you have to call this function before 
 * \details         calling ebox_redraw. If several layers must be invalidate, you should 
 * \details         always prefer to invalidate all of them before calling ebox_redraw .
 * \param x         The pointer of the box.
 * \param view      The pointer of the view (if NULL the function uses the first view).
 * \param name      The name of the layer.
 * \return A null value if nothing goes wrong, otherwise a non-null value.
 * \see ebox_start_layer ebox_end_layer ebox_redraw ebox_paint_layer t_elayer_flags t_elayer
 */
t_pd_err ebox_invalidate_layer(t_ebox *x, t_object* view, t_symbol *name);

/** @} */

#endif



