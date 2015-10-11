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
#include "eclass.h"
#include "edsp.h"

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
 * \details     Allocates and initializes a new proxy inlet. \n This function should replace inlet_new().
 * \param x     The t_eobj pointer.
 */
void eobj_proxynew(void* x);

/*!
 * \fn          int eobj_getproxy(void const* x)
 * \brief       Retreives the index of the proxy that received the last message.
 * \details     Retreives the current proxy index. This function should only be used while a message has been received.
 * \param x     The t_eobj pointer.
 * \return      This function return the index of the current proxy.
 */
int  eobj_getproxy(void const* x);

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
 * \fn      void eobj_attrprocess_viabinbuf(void *x, t_binbuf *d)
 * \brief   Changes the attributes with a binbuf.
 * \details Retrieves and interpretes a binbuf to initialize the attributes.
 * \param x The t_ebox pointer.
 * \param d The binbuf pointer.
 */
void eobj_attrprocess_viabinbuf(void *x, t_binbuf *d);

/*!
 * \fn      void eobj_attrprocess_viatoms(void *x, int argc, t_atom *argv)
 * \brief   Changes the attributes with an array of atoms.
 * \details Retrieves and interpretes a binbuf to initialize the attributes.
 * \param x The t_ebox pointer.
 * \param argc The number of atoms.
 * \param argv The pointer to the atoms.
 */
void eobj_attrprocess_viatoms(void *x, int argc, t_atom *argv);

/*!
 * \fn      void eobj_attr_setvalueof(void *x, t_symbol* s, int argc, t_atom *argv)
 * \brief   Sets the values of an attribute.
 * \details Retrieves the attribute setter and calls it if it exist otherwise it does nothing.
 * \param x The t_eobj pointer.
 * \param s The name of the attribute.
 * \param argc The number of the atoms.
 * \param argv The array of the atoms.
 */
void eobj_attr_setvalueof(void *x, t_symbol* s, int argc, t_atom *argv);

/*!
 * \fn      void eobj_attr_getvalueof(void *x, t_symbol* s, int *argc, t_atom **argv)
 * \brief   Gets the values of an attribute.
 * \details Retrieves the attribute getter method and calls it if it exist otherwise it does nothing.
 * \details You should free the atoms if they has been allocated.
 * \param x The t_eobj pointer.
 * \param s The name of the attribute.
 * \param argc The pointer to an int that will be initialize with the number of the atoms.
 * \param argv The pointer to the array of the atoms that will be allocated if the attribute exist.
 */
void eobj_attr_getvalueof(void *x, t_symbol *s, int *argc, t_atom **argv);

/** @} */

/*! @addtogroup groupdspobj
 * @{
 */

/*!
 * \fn void eobj_dspsetup(void *x, long nins, long nouts)
 * \brief       Initializes the dsp members of the t_edsp.
 * \details     Allocate the memory for digital dignal processing of a t_edspobj. \n This funtion should replace inlet_new and outlet_new for the signal.
 * \param x     The edspobj pointer
 * \param nins  The number of signal inputs.
 * \param nouts The number of signal outputs
 */
void eobj_dspsetup(void *x, long nins, long nouts);

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
 * \fn          void eobj_resize_inputs(void *x, long nins)
 * \brief       Resizes the number of signal inputs of a t_edspobj.
 * \details     Allocates or free the signal inputs. The method can be called after the allocation of the object.
 * \param x     The edspobj pointer.
 * \param nins  The number of signal inputs.
 * \deprecated change the name
 */
void eobj_resize_inputs(void *x, long nins);

/*!
 * \fn          t_sample* eobj_getsignalinput(void *x, long index)
 * \brief       Retrieves the pointer to the signal of an input.
 * \details     Retrieves the pointer to the signal vector of an input. \n The can function can only be used during the dsp method call.
 * \param x     The edspobj pointer.
 * \param index The index of the input.
 * \return      A pointer to the signal vector or NULL.
 * \deprecated change the name
 */
t_sample* eobj_getsignalinput(void *x, long index);

/*!
 * \fn          t_sample* eobj_getsignaloutput(void *x, long index)
 * \brief       Retrieves the pointer to the signal of an output.
 * \details     Retrieves the pointer to the signal vector of an output. \n The can function can only be used during the dsp method call.
 * \param x     The edspobj pointer.
 * \param index The index of the output.
 * \return      A pointer to the signal vector or NULL.
 * \deprecated change the name
 */
t_sample* eobj_getsignaloutput(void *x, long index);

/** @} */

#endif



