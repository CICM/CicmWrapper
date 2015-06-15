/*
 * CicmWrapper
 *
 * A wrapper for Pure Data
 *
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 *
 * Website  : http://www.mshparisnord.fr/HoaLibrary/
 * Contacts : cicm.mshparisnord@gmail.com
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Library General Public License as published
 * by the Free Software Foundation; either version 2 of the License.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
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
 * \fn          void eobj_proxynew(void* x)
 * \brief       Adds a proxy inlet to a t_eobj.
 * \details     Allocates and initializes a new proxy inlet. \n This function should replace inlet_new().
 * \param x     The t_eobj pointer.
  * \return     This function returns a pointer to the new proxy inlet.
 */
t_eproxy* eobj_proxynew(void* x);

/*!
 * \fn          int eobj_getproxy(void* x)
 * \brief       Retreives the index of the proxy that received the last message.
 * \details     Retreives the current proxy index. This function should only be used while a message has been received.
 * \param x     The t_eobj pointer.
 * \return      This function return the index of the current proxy.
 */
int  eobj_getproxy(void* x);

/*!
 * \fn          t_eclass* eobj_getclass(void* x)
 * \brief       Retrieves the eclass of a t_eobj.
 * \details     Directly access to the eclass structure of the t_eobj.
 * \param x     The t_eobj pointer.
 * \return      This function returns a pointer to the eclass of the t_eobj.
 */
t_eclass* eobj_getclass(void* x);

/*!
 * \fn          t_symbol* eobj_getclassname(void* x)
 * \brief       Retrieves the classe name of a t_eobj
 * \details     Directly access to the name of the eclass of the t_eobj.
 * \param x     The t_eobj pointer.
 * \return      This function returns the name of the class of the t_eobj.
 */
t_symbol* eobj_getclassname(void* x);

/*!
 * \fn          t_canvas* eobj_getcanvas(void *x)
 * \brief       Retreives the canvas that owns the t_eobj.
 * \details     Directly access to the canvas structure of the t_eobj.
 * \param x     The t_eobj pointer.
 * \return      This function returns a pointer the canvas that owns the t_eobj.
 */
t_canvas* eobj_getcanvas(void *x);

/*!
 * \fn          char eobj_isbox(void *x)
 * \brief       Retreives if a t_eobj is a GUI box or not.
 * \details     Check if the box flag is postive or null.
 * \param x     The t_eobj pointer.
 * \return      This function returns 1 if the t_eobj is a GUI and 0 if not.
 */
char eobj_isbox(void *x);

/*!
 * \fn          char eobj_isdsp(void *x)
 * \brief       Retreives if a t_eobj is a DSP object or not
 * \details     Check if the dsp method has been initialized.
 * \param x     The t_eobj pointer.
 * \return      This function returns 1 if the t_eobj is a DSP object and 0 if not.
 */
char eobj_isdsp(void *x);

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
 * \details Retrieves the attribute getter and calls it if it exist otherwise it does nothing. You should free the atoms if they has been allocated.
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
 * \brief       Initializes the dsp members of the t_edspobj.
 * \details     Allocate the memory for digital dignal processing of a t_edspobj. \n This funtion should replace inlet_new and outlet_new for the signal.
 * \param x     The edspobj pointer
 * \param nins  The number of signal inputs.
 * \param nouts The number of signal outputs
 */
void eobj_dspsetup(void *x, long nins, long nouts);

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
 */
void eobj_resize_inputs(void *x, long nins);

/*!
 * \fn          t_sample* eobj_getsignalinput(void *x, long index)
 * \brief       Retrieves the pointer to the signal of an input.
 * \details     Retrieves the pointer to the signal vector of an input. \n The can function can only be used during the dsp method call.
 * \param x     The edspobj pointer.
 * \param index The index of the input.
 * \return      A pointer to the signal vector or NULL.
 */
t_sample* eobj_getsignalinput(void *x, long index);

/*!
 * \fn          t_sample* eobj_getsignaloutput(void *x, long index)
 * \brief       Retrieves the pointer to the signal of an output.
 * \details     Retrieves the pointer to the signal vector of an output. \n The can function can only be used during the dsp method call.
 * \param x     The edspobj pointer.
 * \param index The index of the output.
 * \return      A pointer to the signal vector or NULL.
 */
t_sample* eobj_getsignaloutput(void *x, long index);

/** @} */


//! @cond

//! The default save method for eobj called by PD (PRIVATE)
/*
 \ @memberof        eobj
 \ @param z         The eobj pointor
 \ @param b         The binbuf
 \ @return          Nothing
 */
void eobj_save(t_gobj* x, t_binbuf *b);

//! The default save method for UI ebox (PRIVATE)
/*
 \ @memberof        eobj
 \ @param z         The eobj pointor
 \ @param b         The binbuf
 \ @return          Nothing
 */
void eobj_dosave(t_eobj* x, t_binbuf *b);

//! The popup method called by tcl/tk (PRIVATE)
/*
 \ @memberof        eobj
 \ @param x         The eobj pointer
 \ @param s         The message selector
 \ @param itemid    the id of the selected item
 \ @return          Nothing
 */
void eobj_popup(t_eobj* x, t_symbol* s, float itemid);

//! The default write method for all eobj called by PD (PRIVATE)
/*
 \ @memberof        eobj
 \ @param x         The eobj pointor
 \ @param s         The symbol selector
 \ @param argc      The size of the array of atoms
 \ @param argv      The array of atoms
 \ @return          Nothing
 */
void eobj_write(t_eobj* x, t_symbol* s, int argc, t_atom *argv);

//! The default read method for all eobj called by PD (PRIVATE)
/*
 \ @memberof        eobj
 \ @param x         The eobj pointor
 \ @param s         The symbol selector
 \ @param argc      The size of the array of atoms
 \ @param argv      The array of atoms
 \ @return          Nothing
 */
void eobj_read(t_eobj* x, t_symbol* s, int argc, t_atom *argv);

//! The dsp method (PRIVATE)
/*
 \ @memberof    edspobj
 \ @param x     The edspobj pointer
 \ @param sp    The pointers to signal structures
 \ @return      Nothing
 */
void eobj_dsp(void *x, t_signal **sp);

//! The perform method for no inplace(PRIVATE)
/*
 \ @memberof    edspobj
 \ @param w     The pointer sent by the dsp method
 \ @return      Nothing
 */
t_int* eobj_perform(t_int* w);

//! The perform method for no inplace(PRIVATE)
/*
 \ @memberof    edspobj
 \ @param w     The pointer sent by the dsp method
 \ @return      Nothing
 */
t_int* eobj_perform_no_inplace(t_int* w);

//! The perform method (PRIVATE)
/*
 \ @memberof    edspobj
 \ @param w     The pointer sent by the dsp method
 \ @return      Nothing
 */
t_int* eobj_perform_box(t_int* w);

//! The perform method for no inplace(PRIVATE)
/*
 \ @memberof    edspobj
 \ @param w     The pointer sent by the dsp method
 \ @return      Nothing
 */
t_int* eobj_perform_box_no_inplace(t_int* w);

//! The dsp add method (PRIVATE)
/*
 \ @memberof        edspobj
 \ @param x         The edspobj
 \ @param s         Nothing (for Max 6 compatibility)
 \ @param obj       Nothing (for Max 6 compatibility)
 \ @param m         The user perform method
 \ @param flags     The user perform flags
 \ @param userparam The user perform parameters
 \ @return          Nothing
 */
void eobj_dsp_add(void *x, t_symbol* s, t_object* obj, t_typ_method m, long flags, void *userparam);
//! @endcond

#endif



