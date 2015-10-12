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
 * \fn              t_symbol* egui_getreceive_symbol(t_egui const* gui)
 * \brief           Retrieves receive symbol of a gui manager.
 * \param gui       The gui manager pointer.
 * \return          The receive symbol.
 */
t_symbol* egui_getreceive_symbol(t_egui const* gui);

/** @} */

#endif



