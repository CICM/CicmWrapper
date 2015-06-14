/*
 * CicmWrapper - Pure Data Enhanced
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
 * \fn          t_symbol* ebox_getfontname(t_ebox* x)
 * \brief       Retrieves the name of the font of the t_ebox.
 * \param x     The t_ebox pointer.
 * \return      The font name.
 */
t_symbol* ebox_getfontname(t_ebox* x);

/*!
 * \fn          t_symbol* ebox_getfontslant(t_ebox* x)
 * \brief       Retrieves the slant of the font of the t_ebox.
 * \param x     The t_ebox pointer.
 * \return      The font slant.
 */
t_symbol* ebox_getfontslant(t_ebox* x);

/*!
 * \fn          t_symbol* ebox_getfontweight(t_ebox* x)
 * \brief       Retrieves the weight of the font of the t_ebox.
 * \param x     The t_ebox pointer.
 * \return      The font weight.
 */
t_symbol* ebox_getfontweight(t_ebox* x);

/*!
 * \fn          float ebox_getfontsize(t_ebox* x)
 * \brief       Retrieves the size of the font of the t_ebox.
 * \param x     The t_ebox pointer.
 * \return      The font size.
 */
float ebox_getfontsize(t_ebox* x);

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
 * \param b The t_ebox pointer.
 * \param name The name of the layer.
 * \param width The width of the layer.
 * \param height The height of the layer.
 * \see ebox_end_layer ebox_invalidate_layer ebox_paint_layer t_elayer_flags t_elayer
 */
t_elayer* ebox_start_layer(t_ebox *b, t_symbol *name, float width, float height);

/*!
 * \fn      t_pd_err ebox_end_layer(t_ebox *b, t_symbol *name)
 * \brief   Marks a layer as ready to be drawn.
 * \details After the several painting methods, you should call this function to notify  that the layer is ready to be drawn.
 * \param b The t_ebox pointer.
 * \param name The name of the layer.
 * \see ebox_start_layer ebox_invalidate_layer ebox_paint_layer t_elayer_flags t_elayer
 */
t_pd_err ebox_end_layer(t_ebox *b, t_symbol *name);

/*!
 * \fn      t_pd_err ebox_paint_layer(t_ebox *b, t_symbol *name, float x, float y)
 * \brief   Paints a layer in the t_ebox.
 * \details If a layer is ready to be drawn, the function will paint it in the t_ebox.
 * \param name The name of the layer.
 * \param x The abscissa of the layer in the t_ebox.
 * \param y The ordinate of the layer in the t_ebox.
 * \see ebox_start_layer ebox_end_layer ebox_invalidate_layer t_elayer_flags t_elayer
 */
t_pd_err ebox_paint_layer(t_ebox *b, t_symbol *name, float x, float y);

/*!
 * \fn      ebox_invalidate_layer(t_ebox *b,t_symbol *name)
 * \brief   Marks a layer as invalid.
 * \details When you want to redraw a layer you have to call this function and then call ebox_redraw. You should always prefer to invalidate several layer before calling ebox_redraw.
 * \param b The t_ebox pointer.
 * \param name The name of the layer.
 * \see ebox_start_layer ebox_end_layer ebox_redraw ebox_paint_layer t_elayer_flags t_elayer
 */
t_pd_err ebox_invalidate_layer(t_ebox *b,t_symbol *name);

/** @} */


//! @cond
//! The mouse enter method called by tcl/tk (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The ebox pointer
 \ @return          Nothing
 */
void ebox_mouse_enter(t_ebox* x);

//! The mouse leave method called by tcl/tk (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The ebox pointer
 \ @return          Nothing
 */
void ebox_mouse_leave(t_ebox* x);

//! The mouse move method called by tcl/tk (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The ebox pointer
 \ @param s         The message selector
 \ @param argc      The size of the array of atoms
 \ @param argv      The array of atoms
 \ @return          Nothing
 */
void ebox_mouse_move(t_ebox* x, t_symbol* s, int argc, t_atom *argv);

//! The mouse up method called by tcl/tk (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The ebox pointer
 \ @param s         The message selector
 \ @param argc      The size of the array of atoms
 \ @param argv      The array of atoms
 \ @return          Nothing
 */
void ebox_mouse_up(t_ebox* x, t_symbol* s, int argc, t_atom *argv);

//! The mouse down method called by tcl/tk (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The ebox pointer
 \ @param s         The message selector
 \ @param argc      The size of the array of atoms
 \ @param argv      The array of atoms
 \ @return          Nothing
 */
void ebox_mouse_down(t_ebox* x, t_symbol* s, int argc, t_atom *argv);

//! The mouse double click method called by tcl/tk (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The ebox pointer
 \ @param s         The message selector
 \ @param argc      The size of the array of atoms
 \ @param argv      The array of atoms
 \ @return          Nothing
 */
void ebox_mouse_dblclick(t_ebox* x, t_symbol* s, int argc, t_atom *argv);

//! The mouse wheel method called by tcl/tk (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The ebox pointer
 \ @param s         The message selector
 \ @param argc      The size of the array of atoms
 \ @param argv      The array of atoms
 \ @return          Nothing
 */
void ebox_mouse_wheel(t_ebox* x, t_symbol* s, int argc, t_atom *argv);

//! The key down method called by tcl/tk (PRIVATE AND NOT READY)
/*
 \ @memberof        ebox
 \ @param x         The ebox pointer
 \ @param s         The message selector
 \ @param argc      The size of the array of atoms
 \ @param argv      The array of atoms
 \ @return          Nothing
 */
void ebox_key(t_ebox* x, t_symbol* s, int argc, t_atom *argv);

//! The default save method for UI ebox (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The ebox pointer
 \ @param b         The binbuf
 \ @return          Nothing
 */
void ebox_dosave(t_ebox* x, t_binbuf *b);

//! The method to move an UI ebox (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The ebox pointer
 \ @param newx      The new abscissa
 \ @param newy      The new ordinate
 \ @return          Nothing
 */
void ebox_pos(t_ebox* x, float newx, float newy);

//! The method to show or hide an UI ebox (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The ebox pointer
 \ @param vis       The visible state
 \ @return          Nothing
 */
void ebox_vis(t_ebox* x, int vis);

//! The default user id method for all ebox called by PD (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The gobj
 \ @param attr      Nothing (for Max 6 compatibility)
 \ @param argc      The size of the array of atoms
 \ @param argv      The array of atoms
 \ @return          Always 0 (for the moment)
 */
t_pd_err ebox_set_receiveid(t_ebox *x, t_object *attr, int argc, t_atom *argv);

//! The default user send id method for all ebox called by PD (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The gobj
 \ @param attr      Nothing (for Max 6 compatibility)
 \ @param argc      The size of the array of atoms
 \ @param argv      The array of atoms
 \ @return          Always 0 (for the moment)
 */
t_pd_err ebox_set_sendid(t_ebox *x, t_object *attr, int argc, t_atom *argv);

//! The default user preset id method for all ebox called by PD (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The ebox
 \ @param attr      Nothing (for Max 6 compatibility)
 \ @param argc      The size of the array of atoms
 \ @param argv      The array of atoms
 \ @return          Always 0 (for the moment)
 */
t_pd_err ebox_set_presetid(t_ebox *x, t_object *attr, int argc, t_atom *argv);

//! Retrive the preset id of an ebox
/*
 \ @memberof        ebox
 \ @param x         The ebox
 */
t_symbol* ebox_get_presetid(t_ebox* x);

//! The default user font method for all ebox called by PD (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The gobj
 \ @param attr      Nothing (for Max 6 compatibility)
 \ @param argc      The size of the array of atoms
 \ @param argv      The array of atoms
 \ @return          Always 0 (for the moment)
 */
t_pd_err ebox_set_font(t_ebox *x, t_object *attr, int argc, t_atom *argv);

//! The default user fontweight method for all ebox called by PD (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The gobj
 \ @param attr      Nothing (for Max 6 compatibility)
 \ @param argc      The size of the array of atoms
 \ @param argv      The array of atoms
 \ @return          Always 0 (for the moment)
 */
t_pd_err ebox_set_fontweight(t_ebox *x, t_object *attr, int argc, t_atom *argv);

//! The default user fontslant method for all ebox called by PD (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The gobj
 \ @param attr      Nothing (for Max 6 compatibility)
 \ @param argc      The size of the array of atoms
 \ @param argv      The array of atoms
 \ @return          Always 0 (for the moment)
 */
t_pd_err ebox_set_fontslant(t_ebox *x, t_object *attr, int argc, t_atom *argv);

//! The default user fontsize method for all ebox called by PD (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The gobj
 \ @param attr      Nothing (for Max 6 compatibility)
 \ @param argc      The size of the array of atoms
 \ @param argv      The array of atoms
 \ @return          Always 0 (for the moment)
 */
t_pd_err ebox_set_fontsize(t_ebox *x, t_object *attr, int argc, t_atom *argv);

//! Open the properties window (PRIVATE)
/*
 \ @memberof        ebox
 \ @param z         The gobj object
 \ @param glist     The canvas
 \ @return          Nothing
 */
void ebox_properties(t_ebox *x, t_glist *glist);

//! Receive the properties window messages and change the attributes values (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The object
 \ @param s         Nothing (for Max 6 compatibility)
 \ @return          Nothing
 */
void ebox_dialog(t_ebox *x, t_symbol *s, int argc, t_atom *argv);

//! The default notify method of ebox called when an attribute has changed // PRIVATE
/*
 \ @memberof        ebox
 \ @param x         The ebox
 \ @param s         The name of the attribute
 \ @param msg       Nothing (for Max 6 compatibility)
 \ @param sender    Nothing (for Max 6 compatibility)
 \ @param data      Nothing (for Max 6 compatibility)
 \ @return          Always 0 (for the moment)
 */
t_pd_err ebox_notify(t_ebox *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

//! The default size attribute method of ebox called when an size attribute has changed. This function restrains the width and the height depending of the ebox flags EBOX_GROWNO, EBOX_GROWLINK and EBOX_GROWINDI // PRIVATE
/*
 \ @memberof        ebox
 \ @param x         The ebox
 \ @param attr      Nothing (for Max 6 compatibility)
 \ @param argc      The size of the array of atoms
 \ @param argv      The array of atoms that contains the new width and the new height
 \ @return          Always 0 (for the moment)
 */
t_pd_err ebox_size_set(t_ebox *x, t_object *attr, int argc, t_atom *argv);

//! The attribute print method that post all the attributes characteristics in the PD console // PRIVATE
/*
 \ @memberof        ebox
 \ @param x         The ebox
 \ @return          Nothing
 */
void ebox_attrprint(t_ebox* x);

// The defaults pd widgets
void ebox_wgetrect(t_gobj *z,     t_glist *glist, int *xp1, int *yp1, int *xp2, int *yp2);
void ebox_wvis(t_gobj *z,         t_glist *glist, int vis);
void ebox_wdisplace(t_gobj *z,    t_glist *glist, int dx, int dy);
void ebox_wselect(t_gobj *z,      t_glist *glist, int selected);
void ebox_wdelete(t_gobj *z,      t_glist *glist);
//! @endcond


#endif



