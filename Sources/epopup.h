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
 * \fn          void epopupmenu_destroy(t_epopup* popup)
 * \brief       The t_epopup free function.
 * \details     Frees the memory for a t_epopup.
 * \param popup The t_epopup pointer.
 */
void epopupmenu_destroy(t_epopup* popup);

/*!
 * \fn          void epopupmenu_setfont(t_epopup *menu, t_efont *font)
 * \brief       Sets the font of a t_epopup.
 * \param popup The t_epopup pointer.
 * \param font  The t_efont pointer.
 */
void epopupmenu_setfont(t_epopup *popup, t_efont *font);

/*!
 * \fn          void epopupmenu_additem(t_epopup *popup, int itemid, const char *text, char checked, char disabled)
 * \brief       Adds a new item in the popup list.
 * \param popup  The t_epopup pointer.
 * \param itemid The id of the item.
 * \param text   The text of the item.
 * \param checked If the item is marked as checked or not.
 * \param disabled If the item is marked as disabled or not.
 */
void epopupmenu_additem(t_epopup *popup, int itemid, const char *text, char checked, char disabled);

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

/*!
 * \fn          t_etexteditor* etexteditor_create(t_eobj* x, t_symbol* name)
 * \brief       The t_etexteditor creation function.
 * \details     Allocates the memory for a t_etexteditor, intialize the defaults values and link it to the t_eobj.
 * \param x     The t_eobj pointer.
 * \param name  The name of the t_etexteditor.
 * \param bounds The bounds of the t_etexteditor.
 * \return      This function returns a pointer to a new t_etexteditor.
 */
t_etexteditor* etexteditor_create(t_eobj* x, t_symbol* name);

/*!
 * \fn          void etexteditor_destroy(t_etexteditor* editor)
 * \brief       The t_etexteditor free function.
 * \details     Frees the memory for a t_etexteditor.
 * \param editor The t_etexteditor pointer.
 */
void etexteditor_destroy(t_etexteditor* editor);

/*!
 * \fn          void etexteditor_settext(t_etexteditor* editor, const char* text)
 * \brief       Sets the text of the text editor.
 * \param editor The t_etexteditor pointer.
 * \param text   The text to show.
 */
void etexteditor_settext(t_etexteditor* editor, const char* text);

/*!
 * \fn          void etexteditor_clear(t_etexteditor* editor)
 * \brief       Clears the text of the text editor.
 * \param editor The t_etexteditor pointer.
 */
void etexteditor_clear(t_etexteditor* editor);

/*!
 * \fn          void etexteditor_setfont(t_etexteditor *editor, t_efont *font)
 * \brief       Sets the font of a t_etexteditor.
 * \param editor The t_etexteditor pointer.
 * \param font  The t_efont pointer.
 */
void etexteditor_setfont(t_etexteditor *editor, t_efont const* font);

/*!
 * \fn          void etexteditor_setbackgroundcolor(t_etexteditor *editor, t_rgba *color)
 * \brief       Sets the background color of a t_etexteditor.
 * \param editor The t_etexteditor pointer.
 * \param color  The t_rgba pointer.
 */
void etexteditor_setbackgroundcolor(t_etexteditor *editor, t_rgba const* color);

/*!
 * \fn          void etexteditor_settextcolor(t_etexteditor *editor, t_rgba const* color)
 * \brief       Sets the text color of a t_etexteditor.
 * \param editor The t_etexteditor pointer.
 * \param color  The t_rgba pointer.
 */
void etexteditor_settextcolor(t_etexteditor *editor, t_rgba const* color);

/*!
 * \fn          void etexteditor_setwrap(t_etexteditor *editor, char wrap)
 * \brief       Sets if the text shound be wrapped or not.
 * \param editor The t_etexteditor pointer.
 * \param wrap  0 if the text shouldn't be wrapped.
 */
void etexteditor_setwrap(t_etexteditor *editor, char wrap);

/*!
 * \fn          void etexteditor_popup(t_etexteditor *editor, t_rect bounds)
 * \brief       Shows the text editor on the screen.
 * \param editor The t_etexteditor pointer.
 * \param bounds The boundaries relative on the canvas.
 */
void etexteditor_popup(t_etexteditor *editor, t_rect const* bounds);

/*!
 * \fn          void etexteditor_gettext(t_etexteditor *editor, char* text)
 * \brief        Retrieves the text of the text editor.
 * \param editor The t_etexteditor pointer.
 * \param text   The text to fill.
 */
void etexteditor_gettext(t_etexteditor *editor, char* text);


/** @} */

#endif
