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

/*! @addtogroup groupwidget
 *  @{
 */

/*!
 * \fn          t_epopup* epopupmenu_create(t_eobj* x, t_symbol* name)
 * \brief       The t_epopup creation function.
 * \details     Allocates the memory for a t_epopup, intialize the defaults values and link it to the t_eobj.
 * \param x     The t_eobj pointer.
 * \return      This function returns a pointer to a new t_epopup.
 */
t_epopup* epopupmenu_create(t_eobj* x);

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
 * \fn          void epopupmenu_setbackgroundcolor(t_epopup *popup, t_rgba const* color)
 * \brief       Sets the background color of a t_epopup.
 * \param popup The t_epopup pointer.
 * \param color The t_rgba pointer.
 */
void epopupmenu_setbackgroundcolor(t_epopup *popup, t_rgba const* color);

/*!
 * \fn          void epopupmenu_settextcolor(t_epopup *popup, t_rgba const* color)
 * \brief       Sets the text color of a t_epopup.
 * \param popup The t_epopup pointer.
 * \param color The t_rgba pointer.
 */
void epopupmenu_settextcolor(t_epopup *popup, t_rgba const* color);

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
 * \param bounds The bounds on the screen.
 */
void epopupmenu_popup(t_epopup *popup, t_rect const* bounds);

//! @cond

/*!
 * \fn          t_epopup* epopupmenu_getfromsymbol(t_symbol* name)
 * \brief       Retrieves a popup from a symbol.
 * \param name  The binding symbol of the texteditor.
 */
t_epopup* epopupmenu_getfromsymbol(t_symbol* name);

//! @endcond

/*!
 * \fn          t_etexteditor* etexteditor_create(t_eobj* x, t_symbol* name)
 * \brief       The t_etexteditor creation function.
 * \details     Allocates the memory for a t_etexteditor, intialize the defaults values and link it to the t_eobj.
 * \param x     The t_ebox pointer.
 * \return      This function returns a pointer to a new t_etexteditor.
 */
t_etexteditor* etexteditor_create(t_ebox* x);

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
 * \fn          void etexteditor_gettext(t_etexteditor *editor, char* text)
 * \brief        Retrieves the text of the text editor.
 * \param editor The t_etexteditor pointer.
 * \param text   The text to fill.
 */
void etexteditor_gettext(t_etexteditor *editor, char** text);

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
 * \fn          void etexteditor_grabfocus(t_etexteditor *editor)
 * \brief        Forces the focus on the text editor.
 * \param editor The t_etexteditor pointer.
 */
void etexteditor_grabfocus(t_etexteditor *editor);

//! @cond

/*!
 * \fn          t_etexteditor* etexteditor_getfromsymbol(t_symbol* name)
 * \brief       Retrieves a texteditor from a symbol.
 * \param name  The binding symbol of the texteditor.
 */
t_etexteditor* etexteditor_getfromsymbol(t_symbol* name);

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
 * \fn              void ebox_parameter_flags(t_ebox* x, int index, long flags)
 * \brief           Sets the flags of the parameter.
 * \param x         The t_ebox pointer.
 * \param index     The index of the parameter.
 * \param flags     The flags the parameter.
 */
void ebox_parameter_flags(t_ebox* x, int index, long flags);

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



//! @cond

void eobj_create_properties_window(t_eobj* x, t_glist *glist);

void eobj_destroy_properties_window(t_eobj* x);

void tcltk_create_methods(void);

//! @endcond



/** @} */

#endif
