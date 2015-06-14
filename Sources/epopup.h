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
 * @file epopup.h
 * @brief The header that declares all the methods for the t_epopup.
 * @author Pierre Guillot
 */

#ifndef DEF_EPOPUP
#define DEF_EPOPUP

#include "edefine.h"

/*! @addtogroup grouppopup
 *  @{
 */

/*!
 * \fn          t_epopup* epopupmenu_create(t_eobj* x, t_symbol* name)
 * \brief       The t_epopup creation function.
 * \details     Allocates the memory for a t_epopup, intialize the defaults values and link it to the t_eobj.
 * \param x     The t_eobj pointer.
 * \param name  The name of the t_epopup.
 * \return      This function returns a pointer to a new t_epopup.
 */
t_epopup* epopupmenu_create(t_eobj* x, t_symbol* name);

/*!
 * \fn          void epopupmenu_setfont(t_epopup *menu, t_efont *font)
 * \brief       Sets the font of a t_epopup.
 * \param popup The t_epopup pointer.
 * \param font  The t_efont pointer.
 */
void epopupmenu_setfont(t_epopup *popup, t_efont *font);

/*!
 * \fn          void epopupmenu_additem(t_epopup *menu, int itemid, char *text, char checked, char disabled)
 * \brief       Adds a new item in the popup list.
 * \param popup  The t_epopup pointer.
 * \param itemid The id of the item.
 * \param text   The text of the item.
 * \param checked If the item is marked as checked or not.
 * \param disabled If the item is marked as disabled or not.
 */
void epopupmenu_additem(t_epopup *popup, int itemid, char *text, char checked, char disabled);

/*!
 * \fn          void epopupmenu_addseperator(t_epopup *popup)
 * \brief       Adds a new seprator in the popup list.
 * \param popup  The t_epopup pointer.
 */
void epopupmenu_addseperator(t_epopup *popup);

/*!
 * \fn          void epopupmenu_popup(t_epopup *popup, t_pt screen)
 * \brief       Shows the popup on the screen.
 * \param popup The t_epopup pointer.
 * \param pos   The position on the screen.
 */
void epopupmenu_popup(t_epopup *popup, t_pt pos);

/** @} */

#endif
