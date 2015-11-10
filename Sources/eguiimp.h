/*
 * CicmWrapper
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#ifndef DEF_EGUI_IMP
#define DEF_EGUI_IMP

#include "ecommon.h"
#include "epath.h"
#include "etextlayout.h"
#include "ecolor.h"

typedef enum
{
    EPD_LAYER_OPEN,
    EPD_LAYER_CLOSE,
    EPD_LAYER_INVALID,
    EPD_LAYER_READY
}  t_layer_state;

/**
 * @enum epath_maker
 * @brief The types of path.
 * @details It define all the path type.
 */
typedef enum
{
    E_PATH_MOVE     = 0,   /*!< This type is move. */
    E_PATH_LINE     = 1,   /*!< This type is line maker. */
    E_PATH_CURVE    = 2,   /*!< This type is curve. */
    E_PATH_CLOSE    = 3,   /*!< This type is curve. */
    E_PATH_RECT     = 4,   /*!< This type is rectangle. */
    E_PATH_OVAL     = 5,   /*!< This type is oval. */
    E_PATH_ARC      = 6    /*!< This type is arc. */
} epath_maker;


/**
 * @struct _etextlayout
 * @brief The textlayout object.
 * @details It contains the all the informations to be drawn.
 */
struct _etextlayout
{
    char*           c_text;     /*!< The text. */
    char            c_wrap;     /*!< If the text should be wrapped. */
    t_rgba          c_color;    /*!< The color of the text. */
    t_efont         c_font;     /*!< The font of the text. */
    t_rect          c_rect;     /*!< The rectangle of the text. */
    int             c_justify;  /*!< The justification of the graphical object. */
};

/**
 * @struct _epath
 * @brief The path object.
 * @details It contains the all the informations to be drawn.
 */
struct _epath
{
    t_pt*   p_points;   /*!< The points. */
    size_t  p_size;     /*!< The number of valid points. */
    size_t  p_rsize;    /*!< The real number of points. */
};

typedef struct _gpath_impl
{
    t_epath  i_path;
    float    i_width;
    char     i_filled;
    t_rgba   i_color;
} t_gpath_impl;

/**
 * @enum eshape_maker
 * @brief The types of marker.
 * @details It define all the available shape type.
 */
typedef enum
{
    EPD_GATOM_INVALID   = 0,   /*!< This type is invalid. */
    EPD_GATOM_PATH      = 1,   /*!< This type is path. */
    EPD_GATOM_TEXT      = 2,   /*!< This type is text. */
    EPD_GATOM_RECT      = 3,   /*!< This type is rectangle. */
    EPD_GATOM_OVAL      = 4,   /*!< This type is oval. */
    EPD_GATOM_ARC       = 5    /*!< This type is arc. */
} egatom_type;

typedef union _gword
{
    t_gpath_impl    w_path;
    t_etextlayout   w_text;
} t_gword;

typedef struct _gatom
{
    union _gword    a_word;
    egatom_type     a_type;
} t_gatom;

/**
 * @struct t_elayer
 * @brief The drawing layer.
 * @details It contains the all the informations and the graphical objects to be drawn.
 */
struct _elayer
{
    t_symbol*           l_name;             /*!< The name. */
    long                l_state;            /*!< The state. */
    float               l_width;            /*!< The width. */
    float               l_height;           /*!< The height. */
    t_matrix            l_matrix;           /*!< The matrix. */
    char                l_transform;        /*!< If the matrix transformation must be applied. */
    t_gpath_impl        l_path;             /*!< The current path. */
    t_gatom*            l_atoms;            /*!< The graphical atoms. */
    size_t              l_natoms;           /*!< The number of graphical atoms. */
};

/**
 * @struct _eview
 * @brief The view object.
 * @details It contains the members and the methods for view management.
 */
struct _eview
{
    t_object        v_object;       /*!< The object. */
    t_object*       v_owner;        /*!< The owner. */
    t_symbol*       v_tag;          /*!< The widget tag. */

    t_canvas*       v_canvas;       /*!< The canvas of the view. */
    t_edrawparams   v_params;       /*!< The draw parameter of the view. */
    t_rect          v_bounds;       /*!< The bounds of the view. */
    t_rect          v_last;         /*!< The last bounds of the view. */
    char            v_mousedown;    /*!< The mouse down status. */
    char            v_item;         /*!< The selected item. */
    t_elayer**      v_layers;       /*!< The layers. */
    size_t          v_nlayers;      /*!< The number of layers. */
    char            v_selected;     /*!< if the view is selected. */
    
    t_mouse_method          v_mouse_enter;
    t_mouse_method          v_mouse_leave;
    t_mouse_method          v_mouse_down;
    t_mouse_method          v_mouse_up;
    t_mouse_method          v_mouse_move;
    t_mouse_method          v_mouse_drag;
    t_mouse_method          v_mouse_dblclick;
    t_mousewheel_method     v_mouse_wheel;
    t_key_method            v_key_press;
    t_key_method            v_key_filter;
    t_paint_method          v_paint;
    t_drawparameter_method  v_drawparams;
};


#endif
