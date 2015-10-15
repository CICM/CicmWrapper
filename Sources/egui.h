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
 * \file eattr.h
 * \brief The header that declares all the methods for the t_eattr.
 * \author Pierre Guillot
 */

#ifndef DEF_EGUI
#define DEF_EGUI

#include "edefine.h"

/*! @defgroup groupguipart The gui part of the library.
 * @brief The gui part of the library.
 * @details This part refers to the methods and structures that can be used
 * @details to manage graphical user interfaces.
 */

/*! @addtogroup groupguipart
 *  @{
 */

/*!< The width and the height can't be modified. */
#define EBOX_GROWNO          (1<<4)
/*!< The width and the height are linked. */
#define EBOX_GROWLINK        (1<<5)
/*!< The width and the height are independant. */
#define EBOX_GROWINDI        (1<<6)
/*!< The t_ebox ignore the mouse events. */
#define EBOX_IGNORELOCKCLICK (1<<7)
/*!< The t_ebox supports text fields. */
#define EBOX_TEXTFIELD       (1<<11)
/*!< The t_ebox font attr changes the size. */
#define EBOX_FONTSIZE        (1<<18)
/*!< The double-click is received in edition. */
#define EBOX_DBLCLICK_EDIT   (1<<26)

/**
 * @struct _egui
 * @brief The gui manager object.
 * @details It contains the members and the methods for gui management.
 */
EXTERN_STRUCT _egui;
typedef struct _egui t_egui;

/*!
 * \fn              t_egui* egui_new(t_object* owner)
 * \brief           Allocates and initializes a new gui manager.
 * \param owner     The owner of the gui manager.
 * \param flags     The flags of the gui.
 * \return          The new gui manager pointer.
 */
t_egui* egui_new(t_object* owner, long flags);

/*!
 * \fn              t_egui* egui_findbyname(t_symbol* name)
 * \brief           Retrieves the gui manager that matchs to the name.
 * \param name      The name of the gui manager.
 * \return          The dspguimanager that matchs to the name if it exists, othersise NULL.
 */
t_egui* egui_findbyname(t_symbol* name);

/*!
 * \fn              float egui_getwidth(t_egui const* gui)
 * \brief           Retrieves if the width of a gui manager.
 * \param gui       The gui manager pointer.
 * \return          The width.
 */
float egui_getwidth(t_egui const* gui);

/*!
 * \fn              float egui_getheight(t_egui const* gui)
 * \brief           Retrieves if the height of a gui manager.
 * \param gui       The gui manager pointer.
 * \return          The height.
 */
float egui_getheight(t_egui const* gui);

/*!
 * \fn              char egui_ispinned(t_egui const* gui)
 * \brief           Retrieves if the gui manager is pinned.
 * \param gui       The gui manager pointer.
 * \return          A non-null value if the gui manager is pinned, otherwise 0.
 */
char egui_ispinned(t_egui const* gui);

/*!
 * \fn              char egui_ignoreclick(t_egui const* gui)
 * \brief           Retrieves if the gui manager ignores clicks.
 * \param gui       The gui manager pointer.
 * \return          A non-null value if the gui manager ignores clicks, otherwise 0.
 */
char egui_ignoreclick(t_egui const* gui);

/*!
 * \fn              t_symbol* egui_getreceive(t_egui const* gui)
 * \brief           Retrieves receive symbol of a gui manager.
 * \param gui       The gui manager pointer.
 * \return          The receive symbol.
 */
t_symbol* egui_getreceive(t_egui const* gui);

/*!
 * \fn              t_symbol* egui_getsend(t_egui const* gui)
 * \brief           Retrieves send symbol of a gui manager.
 * \param gui       The gui manager pointer.
 * \return          The send symbol.
 */
t_symbol* egui_getsend(t_egui const* gui);

/*!
 * \fn              long egui_getflags(t_egui const* gui)
 * \brief           Retrieves the flags of a gui manager.
 * \param gui       The gui manager pointer.
 * \return          The flags of the gui manager.
 */
long egui_getflags(t_egui const* gui);

/*!
 * \fn          void egui_redraw(t_egui* gui)
 * \brief       Notifies the gui that it should be redrawn.
 * \param gui   The gui manager pointer.
 */
void egui_redraw(t_egui* gui);

/*!
 * \fn          void egui_setreceive(t_egui *gui, t_symbol* s)
 * \brief       Sets the receive symbol of the gui manager.
 * \param gui   The gui manager pointer.
 * \param s     The symbol.
 */
void egui_setreceive(t_egui *gui, t_symbol* s);

/*!
 * \fn          void egui_setsend(t_egui *gui, t_symbol* s)
 * \brief       Sets the send symbol of the gui manager.
 * \param gui   The gui manager pointer.
 * \param s     The symbol.
 */
void egui_setsend(t_egui *gui, t_symbol* s);

/*!
 * \fn              void egui_setsize(t_egui *gui, float width, float height)
 * \brief           Sets the global size of the gui manager.
 * \param gui       The gui manager pointer.
 * \param width     The width.
 * \param height    The height.
 */
void egui_setsize(t_egui *gui, float width, float height);

/*!
 * \fn          void egui_setignoreclick(t_egui *gui, char ignore)
 * \brief       Sets if the gui should ignore mouse click.
 * \param gui   The gui manager pointer.
 * \param state A positive value to ignore, otherwise a non-null value.
 */
void egui_setignoreclick(t_egui *gui, char state);

/*!
 * \fn          void egui_setpinned(t_egui *gui, char state)
 * \brief       Sets if the gui should be pinned to the canvas.
 * \param gui   The gui manager pointer.
 * \param state A positive value to pinne, otherwise a non-null value.
 */
void egui_setpinned(t_egui *gui, char state);




/*!
 * \fn          void egui_view_add(t_egui *gui, t_glist* glist)
 * \brief       Displays a view of the gui for a canvas.
 * \param gui   The gui pointer.
 * \param glist The canvas pointer.
 */
void egui_view_add(t_egui *gui, t_glist* glist);

/*!
 * \fn          void ebox_view_remove(t_egui *gui, t_glist *glist)
 * \brief       Removes a view of the gui.
 * \param gui   The gui pointer.
 * \param glist The canvas pointer.
 */
void egui_view_remove(t_egui *gui, t_glist *glist);

/*!
 * \fn          void egui_view_getposition(t_egui const* gui, t_glist const* glist, t_pt* pos)
 * \brief       Retrieves the position of a view.
 * \param gui   The gui pointer.
 * \param glist The canvas pointer.
 * \param pos   The position of the view.
 */
void egui_view_getposition(t_egui const* gui, t_glist const* glist, t_pt* pos);

/*!
 * \fn          void egui_view_getsize(t_egui const* gui, t_glist const* glist, t_pt* size)
 * \brief       Retrieves the size of a view.
 * \param gui   The gui pointer.
 * \param glist The canvas pointer.
 * \param size  The size of the view.
 */
void egui_view_getsize(t_egui const* gui, t_glist const* glist, t_pt* size);

/*!
 * \fn              void egui_view_getbounds(t_egui const* gui, t_glist const* glist, t_rect* bounds)
 * \brief           Retrieves the bounds of a view.
 * \param gui       The gui pointer.
 * \param glist     The canvas pointer.
 * \param bounds    The bounds of the view.
 */
void egui_view_getbounds(t_egui const* gui, t_glist const* glist, t_rect* bounds);

/*!
 * \fn          void egui_view_setposition(t_egui *gui, t_glist *glist, t_pt const* pos)
 * \brief       Sets the position of a view.
 * \param gui   The gui pointer.
 * \param glist The canvas pointer.
 * \param pos   The position of the view.
 */
void egui_view_setposition(t_egui *gui, t_glist *glist, t_pt const* pos);

/*!
 * \fn          void egui_view_setsize(t_egui const* gui, t_glist const* glist, t_pt const* size)
 * \brief       Sets the size of a view.
 * \param gui   The gui pointer.
 * \param glist The canvas pointer.
 * \param size  The size of the view.
 */
void egui_view_setsize(t_egui* gui, t_glist const* glist, t_pt const* size);

/*!
 * \fn              void egui_view_setbounds(t_egui const* gui, t_glist const* glist, t_rect const* bounds)
 * \brief           Sets the bounds of a view.
 * \param gui       The gui pointer.
 * \param glist     The canvas pointer.
 * \param bounds    The bounds of the view.
 */
void egui_view_setbounds(t_egui* gui, t_glist const* glist, t_rect const* bounds);

/*!
 * \fn              void egui_view_select(t_egui *gui, t_glist *glist)
 * \brief           Selects a view.
 * \param gui       The gui pointer.
 * \param glist     The canvas pointer.
 */
void egui_view_select(t_egui *gui, t_glist *glist);

/*!
 * \fn              void egui_view_deselect(t_egui *gui, t_glist *glist)
 * \brief           Deselects a view.
 * \param gui       The gui pointer.
 * \param glist     The canvas pointer.
 */
void egui_view_deselect(t_egui *gui, t_glist *glist);

/*!
 * \fn              void egui_view_setcursor(t_egui *gui, t_glist *glist, ebox_cursors cursor)
 * \brief           Changes the cursor of the mouse.
 * \param gui       The gui pointer.
 * \param glist     The canvas pointer.
 * \param cursor    The type of cursor.
 */
void egui_view_setcursor(t_egui *gui, t_glist *glist, ebox_cursors cursor);

/*!
 * \fn              t_pd_err egui_view_invalidate_layer(t_egui *gui, t_glist *glist, t_symbol *name)
 * \brief           Invalidates a layer in a view
 * \param gui       The gui pointer.
 * \param glist     The canvas pointer.
 * \param name      The name of the layer.
 * \return A null value if nothing goes wrong, otherwise a non-null value.
 */
t_pd_err egui_view_invalidate_layer(t_egui *gui, t_glist *glist, t_symbol *name);

/*!
 * \fn              t_elayer* egui_view_start_layer(t_egui *gui, t_glist *glist, t_symbol *name,
                                                                 const float width, const float height)
 * \brief           Starts a new layer in a view
 * \param gui       The gui pointer.
 * \param glist     The canvas pointer.
 * \param name      The name of the layer.
 * \param width     The width of the layer.
 * \param height    The height of the layer.
 * \return the pointer of the layer if nothing goes wrong, otherwise a NULL.
 */
t_elayer* egui_view_start_layer(t_egui *gui, t_glist *glist, t_symbol *name,
                                             const float width, const float height);
/*!
 * \fn              t_pd_err egui_view_end_layer(t_egui *gui, t_glist *glist, t_symbol *name)
 * \brief           Ends a layer in a view
 * \param gui       The gui pointer.
 * \param glist     The canvas pointer.
 * \param name      The name of the layer.
 * \return A null value if nothing goes wrong, otherwise a non-null value.
 */
t_pd_err egui_view_end_layer(t_egui *gui, t_glist *glist, t_symbol *name);

/*!
 * \fn              t_pd_err egui_view_paint_layer(t_egui *gui, t_glist *glist, t_symbol *name,
                                                                const float xoffset, const float yoffset)
 * \brief           Paints a layer in a view
 * \param gui       The gui pointer.
 * \param glist     The canvas pointer.
 * \param name      The name of the layer.
 * \param xoffset   The abscissa offset of the layer.
 * \param yoffset   The ordinate yoffset of the layer.
 * \return A null value if nothing goes wrong, otherwise a non-null value.
 */
t_pd_err egui_view_paint_layer(t_egui *gui, t_glist *glist, t_symbol *name,
                               const float xoffset, const float yoffset);




/** @} */

#endif



