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
 * @file estruct.h
 * @brief The header that contains all the principals structures of CicmWrapper
 * @details All the principals structures are defined in this file.
 * @author Pierre Guillot
 */

#include "ecommon.h"

#ifndef DEF_ESTRUCT
#define DEF_ESTRUCT

typedef enum _emod_flags
{
    EMOD_NONE           = 0,
    EMOD_SHIFT          = 1,
    EMOD_MAJ            = 2,
    EMOD_CTRL           = 4,
    EMOD_CMD            = 8,
    EMOD_ALT            = 16,
    EMOD_RIGHT          = 32
} t_emod_flags;

typedef enum _ekey_flags
{
    EKEY_DEL      = 0,
    EKEY_TAB      = 1,
    EKEY_ENTER    = 2,
    EKEY_ESC      = 3
    
} t_ekey_flags;

typedef enum _eselitem_flags
{
    EITEM_NONE    = 0,
    EITEM_OBJ     = 1,
    EITEM_INLET   = 2,
    EITEM_OUTLET  = 3,
    EITEM_BOTTOM  = 4,
    EITEM_CORNER  = 5,
    EITEM_RIGHT   = 6
    
} t_eselitem_flags;

/**
 * @enum t_elayer_flags
 * @brief The flags that defines the status of a layer.
 * @details It define all possible the status of a layer.
 */
typedef enum t_elayer_flags
{
    EGRAPHICS_OPEN      = 0,  /*!< Open. */
    EGRAPHICS_CLOSE     = -1, /*!< Closed. */
    EGRAPHICS_INVALID   = -2, /*!< Invalid. */
    EGRAPHICS_TODRAW    = -3  /*!< To be drawn. */
} t_elayer_flags;

/**
 * @enum t_etextjustify_flags
 * @brief The flags that defines the text anchor.
 * @details It define all text anchor.
 */
typedef enum t_etextanchor_flags
{
    ETEXT_UP            = 0, /*!< Up and center. */
    ETEXT_UP_LEFT       = 1, /*!< Up anfd left. */
    ETEXT_UP_RIGHT      = 2, /*!< Up anfd right. */
    ETEXT_DOWN          = 3, /*!< Down and center. */
    ETEXT_DOWN_LEFT     = 4, /*!< Down anfd left. */
    ETEXT_DOWN_RIGHT    = 5, /*!< Down anfd right. */
    ETEXT_LEFT          = 6, /*!< Left. */
    ETEXT_RIGHT         = 7, /*!< right. */
    ETEXT_CENTER        = 8  /*!< Center. */
    
} t_etextanchor_flags;

/**
 * @enum t_etextwrap_flags
 * @brief The flags that defines if the text should be wrapped.
 * @details It define true or false.
 */
typedef enum t_etextwrap_flags
{
    ETEXT_NOWRAP    = 0, /*!< False. */
    ETEXT_WRAP      = 1 /*!< True. */
} t_etextwrap_flags;

/**
 * @enum t_etextjustify_flags
 * @brief The flags that define the text justification.
 * @details It define all text justification.
 */
typedef enum t_etextjustify_flags
{
    ETEXT_JLEFT    = 0, /*!< Left. */
    ETEXT_JRIGHT   = 1, /*!< Right. */
    ETEXT_JCENTER  = 2  /*!< Center. */
} t_etextjustify_flags;

/**
 * @enum egraphics_types
 * @brief The types of graphical object.
 * @details It define all the graphical type.
 */
typedef enum
{
    E_GOBJ_INVALID           = 0,   /*!< This type is invalid. */
    E_GOBJ_PATH                 ,   /*!< This is a path. */
    E_GOBJ_RECT                 ,   /*!< This is a rect. */
    E_GOBJ_ARC                  ,   /*!< This is an arc. */
    E_GOBJ_OVAL                 ,   /*!< This is an oval. */
    E_GOBJ_TEXT                 ,   /*!< This is a text. */
    E_GOBJ_IMAG                    /*!< This is an image. */
} egraphics_types;

/**
 * @enum epath_types
 * @brief The types of path.
 * @details It define all the path type.
 */
typedef enum
{
    E_PATH_MOVE    = 0,   /*!< This type is move. */
    E_PATH_CURVE   = 1    /*!< This type is curve. */
} epath_types;

/**
 * @struct t_pt
 * @brief A point structure.
 * @details It contains the members x and y for abscissa and ordinate.
 */
typedef struct t_pt
{
	float x; /*!< The abscissa coordiante. */
	float y; /*!< The ordiante coordiante. */
} t_pt;

/**
 * @struct t_rect
 * @brief A rectangle structure.
 * @details It contains the members x, y, width and height for abscissa and ordinate and size.
 */
typedef struct t_rect
{
	float x;        /*!< The abscissa coordiante. */
	float y;        /*!< The ordiante coordiante. */
	float width;    /*!< The width of the rectangle */
	float height;   /*!< The height of the rectangle */
} t_rect;

/**
 * @struct t_matrix
 * @brief A matrix structure.
 * @details It contains the members xx, yx, xy, yy, x0 and y0
 */
typedef struct t_matrix
{
	float xx;   /*!< The abscissa-abscissa translation of the matrix */
	float yx;   /*!< The abscissa-ordiante translation of the matrix */
	float xy;   /*!< The ordiante-abscissa translation of the matrix */
	float yy;   /*!< The ordiante-ordiante translation of the matrix */
	float x0;   /*!< The abscissa origin of the matrix */
	float y0;   /*!< The ordiante origin of the matrix */
} t_matrix;

/**
 * @struct t_rgb
 * @brief A rgb color structure.
 * @details It contains the members red, green and blue.
 */
typedef struct t_rgb
{
	float red;      /*!< The red value. */
	float green;    /*!< The green value. */
	float blue;     /*!< The blue value. */
} t_rgb;

/**
 * @struct t_rgba
 * @brief A rgba color structure.
 * @details It contains the members red, green, blue and alpha.
 */
typedef struct t_rgba
{
	float red;      /*!< The red value. */
	float green;    /*!< The green value. */
	float blue;     /*!< The blue value. */
	float alpha;    /*!< The alpha value. */
} t_rgba;

/**
 * @struct t_hsl
 * @brief A hsl color structure.
 * @details It contains the members hue, saturation and lightness.
 */
typedef struct t_hsl
{
	float hue;          /*!< The hue value. */
	float saturation;   /*!< The saturation value. */
	float lightness;    /*!< The lightness value. */
} t_hsl;

/**
 * @struct t_hsla
 * @brief A hsla color structure.
 * @details It contains the members hue, saturation, lightness and alpha.
 */
typedef struct t_hsla
{
	float hue;          /*!< The hue value. */
	float saturation;   /*!< The saturation value. */
	float lightness;    /*!< The lightness value. */
    float alpha;        /*!< The alpha value. */
} t_hsla;

/**
 * @struct t_efont
 * @brief The font structure.
 * @details It contains the informations of a font.
 */
typedef struct t_efont
{
    float       c_size;     /*!< The size of the font. */
    long        c_sizereal; /*!< The plateform's size of the font. */
    t_symbol*   c_family;   /*!< The family of the font (times, helvetica, ect.). */
    t_symbol*   c_slant;    /*!< The slant of the font (regular, italic, etc.). */
    t_symbol*   c_weight;   /*!< The weight of the font (normal, bold). */
} t_efont;

/**
 * @struct t_etext
 * @brief The text structure.
 * @details It contains the all the informations to be drawn.
 */
typedef struct t_etext
{
    t_symbol*       c_text;     /*!< The text. */
    t_rgba          c_color;    /*!< The color of the text. */
    t_efont         c_font;     /*!< The font of the text. */
    t_rect          c_rect;     /*!< The rectangle of the text. */
    t_symbol*       c_anchor;   /*!< The anchor of the text. */
    t_symbol*       c_justify;  /*!< The justification of the text. */
} t_etext;

/**
 * @struct t_egobj
 * @brief The  drawing object.
 * @details It contains the all the informations to be drawn.
 */
typedef struct t_egobj
{
	egraphics_types e_type;         /*!< The type of the graphical object. */
    int             e_filled;       /*!< The filled state of the graphical object. */
    t_symbol*       e_color;        /*!< The color of the graphical object. */
    float           e_width;        /*!< The line width of the graphical object. */
    
	t_pt*           e_points;       /*!< The points of the graphical object. */
    long            e_npoints;      /*!< The number of points of the graphical object. */
    float           e_roundness;    /*!< The roundness of the graphical object. */
    t_efont         e_font;         /*!< The font of the graphical object. */
    t_symbol*       e_anchor;       /*!< The anchor of the graphical object. */
    t_symbol*       e_justify;      /*!< The justification of the graphical object. */
    t_symbol*       e_text;         /*!< The text of the graphical object. */
    
} t_egobj;

/**
 * @struct t_epopup
 * @brief The popup structure.
 * @details It contains the informations to show and retieve a popup.
 */
typedef struct t_epopup
{
    t_symbol*   c_name; /*!< The name of the popup. */
    t_symbol*   c_send; /*!< The name of the owner. */
}t_epopup;

/**
 * @struct t_elayer
 * @brief The  drawing layer.
 * @details It contains the all the informations and the graphical objects to be drawn.
 */
typedef struct t_elayer
{
    t_object*           e_owner;            /*!< The layer owner. */
    t_symbol*           e_name;             /*!< The layer name. */
    t_symbol*           e_id;               /*!< The layer canvas ID. */
    int                 e_state;            /*!< The layer state. */
    t_rect              e_rect;             /*!< The layer size. */
    
    t_symbol*           e_color;            /*!< The layer color. */
    int                 e_line_width;       /*!< The layer line width. */
    t_matrix            e_matrix;           /*!< The layer matrix. */
    t_egobj             e_new_objects;      /*!< The layer new object. */
    t_egobj*            e_objects;          /*!< The layer objects. */
    long                e_number_objects;   /*!< The number of layer objects. */
} t_elayer;

/**
 * @struct t_edrawparams
 * @brief The  drawing parameters.
 * @details It contains the default parameters of a ebox.
 */
typedef struct t_edrawparams
{
	float		d_cornersize;       /*!< The corner roundness. */
	float       d_borderthickness;  /*!< The border size. */
	t_rgba      d_bordercolor;      /*!< The border color. */
	t_rgba      d_boxfillcolor;     /*!< The background color. */
    
} t_edrawparams;

/**
 * @struct t_eproxy
 * @brief The  Proxy object.
 * @details It contains the proxy class, the eobj owner and the index of the proxy.
 */
typedef struct t_eproxy
{
	t_pd        p_pd;       /*!< The class object. */
	t_object*   p_owner;    /*!< The pointer to the eobj owner. */
    t_inlet*    p_inlet;    /*!< The pointer to the inlet. */
    int         p_index;    /*!< The index of the proxy. */
} t_eproxy;

typedef struct t_ewidget
{
    t_getrectfn     w_getrectfn;
    t_displacefn    w_displacefn;
    t_selectfn      w_selectfn;
    t_activatefn    w_activatefn;
    t_deletefn      w_deletefn;
    t_visfn         w_visfn;
    t_clickfn       w_clickfn;
    method          w_assist;
    method          w_paint;
    method          w_mouseenter;
    method          w_mouseleave;
    method          w_mousemove;
    method          w_mousedown;
    method          w_mousedrag;
    method          w_mouseup;
    method          w_mousewheel;
    method          w_dblclick;
    method          w_key;
    method          w_keyfilter;
    method          w_getdrawparameters;
    method          w_save;
    method          w_dosave;
    method          w_popup;
    method          w_dsp;
    method          w_object_standat_paint;
    method          w_oksize;
    t_err_method    w_notify;
    method          w_write;
    method          w_read;
} t_ewidget;

typedef struct t_eattr
{
    t_object        obj;
    t_symbol*       name;
    t_symbol*       type;
    t_symbol*       category;
    t_symbol*       label;
    t_symbol*       style;
    long            order;
    char            save;
    char            paint;
    char            invisible;
    long			flags;
    long            offset;
    long            sizemax;
    long            size;
    
    method			getter;
    method			setter;
    long            clipped;
    double          minimum;
    double          maximum;
    double          step;
    t_symbol*       defvals;
    t_symbol**      itemslist;
    long            itemssize;
} t_eattr;


/**
 * @struct t_eclass
 * @brief The  class.
 * @details It contains the Pure Data default class with extra methods and attributes.
 */
typedef struct t_eclass
{
    t_class     c_class;    /*!< The default class. */
    char        c_box;      /*!< The marker if the class is GUI. */
    char        c_dsp;      /*!< The marker if the class is DSP. */
    t_ewidget   c_widget;   /*!< The extra widget methods. */
    t_eattr**   c_attr;     /*!< The attributes. */
    long        c_nattr;    /*!< The number of attributes. */
}t_eclass;

/*! @addtogroup groupobj The BASIC Object Part
 * @brief The eobj part.
 * @details This part refers to the methods and structures that can be used by all the t_eobj structures.
 *  @{
 */

/**
 * @struct t_eobj
 * @brief The  object.
 * @details It contains the Pd object, the canvas pointer and members for proxy inlets.
 * This should be used for no graphical object that don't have signal processing methods.
 */
typedef struct t_eobj
{
    t_object            o_obj;              /*!< The Pd object. */
    t_symbol*           o_id;               /*!< The object ID. */
    t_canvas*           o_canvas;           /*!< The canvas that own the object. */
    t_eproxy**          o_proxy;            /*!< The array of proxy inlets. */
    int                 o_nproxy;           /*!< The number of proxy inlets. */
    int                 o_current_proxy;    /*!< The index of the current proxy inlet used */
}t_eobj;

/** @} */

/*! @addtogroup groupdspobj The DSP Object Part
 * @brief The edspobj part.
 * @details This part refers to the methods and structures that can be used by all the t_edspobj structures.
 *  @{
 */

/**
 * @enum edsp_flags
 * @brief The flags that discribe the behavior of a t_edspobj.
 * @details It define all the behavior of a t_edspobj.
 */
typedef enum
{
    E_INPLACE    = 0,    /*!< The signal processing can be inplace. */
    E_NO_INPLACE = 1     /*!< The signal processing can't be inplace. */
} edsp_flags;

/**
 * @struct t_edspobj
 * @brief The  DSP object.
 * @details It contains the  object with all the members for signal processing.
 * This should be used for no graphical object that have signal processing methods.
 */
typedef struct t_edspobj
{
    t_eobj              d_obj;              /*!< The  object. */
    float               d_float;            /*!< The float member to initialize the signal method. */
    long                d_dsp_size;         /*!< The number of signal inlets and outlets. */
    t_int*              d_dsp_vectors;      /*!< The vector that contains all the pointers for the perform method. */
    long                d_dsp_flag;         /*!< The flags to initialize the perform method. */
    void*               d_dsp_user_param;   /*!< The user parameters to pass through the perform method. */
    t_float**           d_sigs_out;         /*!< The array of signal vectors. */
    t_float*            d_sigs_real;        /*!< The real array of signal. */
    method              d_perform_method;   /*!< The user perform method. */
    long                d_misc;             /*!< The flag that could be inplace or not. */
}t_edspobj;

/** @} */

/*! @addtogroup groupbox The GUI Object Part
 * @brief The ebox part.
 * @details This part refers to the methods and structures that can be used by all the t_ebox structures.
 *  @{
 */

/**
 * @enum ebox_flags
 * @brief The flags that discribe the behavior of a t_ebox.
 * @details It define all the behavior of a t_ebox.
 */
typedef enum
{
    EBOX_GROWNO          = (1<<4),    /*!< The width and the height can't be modified. */
    EBOX_GROWLINK        = (1<<5),    /*!< The width and the height are linked. */
    EBOX_GROWINDI        = (1<<6),    /*!< The width and the height are independant. */
    EBOX_IGNORELOCKCLICK = (1<<7)     /*!< The t_ebox ignore the mouse events. */
} ebox_flags;

/**
 * @enum ebox_cursors
 * @brief The cursors that can be used.
 * @details It define all the available cursors.
 */
typedef enum
{
    ECURSOR_LEFT_PTR     = 0, /*!< The left_ptr string. */
    ECURSOR_CENTER_PTR   = 1, /*!< The center_ptr string. */
    ECURSOR_SDOUBLE_ARROW= 2, /*!< The sb_v_double_arrow string. */
    ECURSOR_PLUS         = 3, /*!< The plus string. */
    ECURSOR_HAND         = 4, /*!< The hand2 string. */
    ECURSOR_CIRCLE       = 5, /*!< The circle string. */
    ECURSOR_X            = 6, /*!< The X_cursor string. */
    ECURSOR_BOTTOM       = 7, /*!< The bottom_side string. */
    ECURSOR_RIGHT_CORNER = 8, /*!< The bottom_right_corner string. */
    ECURSOR_RIGHT_SIDE   = 9, /*!< The right_side string. */
    ECURSOR_DOUBLE_ARROW = 10, /*!< The double_arrow string. */
    ECURSOR_EXCHANGE     = 11, /*!< The exchange string. */
    ECURSOR_XTERM        = 12  /*!< The xterm string. */
} ebox_cursors;

/**
 * @struct t_ebox
 * @brief The  GUI object.
 * @details It contains the  object with all the members for graphical behavior.
 * This should be used for graphical object that don't have signal processing methods.
 */
typedef struct t_ebox
{
    t_eobj              b_obj;              ///<The  object.
    
    t_symbol*           b_receive_id;       /*!< The object user ID. */
    t_symbol*           b_send_id;          /*!< The object send ID. */
    t_symbol*           b_objpreset_id;     /*!< The object preset ID. */
    
    t_symbol*           b_canvas_id;        /*!< The canvas ID. */
    t_symbol*           b_drawing_id;       /*!< The drawing ID. */
    t_symbol*           b_window_id;        /*!< The window ID. */
    t_symbol*           b_all_id;           /*!< The global ID. */
    
    long                b_flags;            /*!< The ebox flags. */
    t_rect              b_rect;             /*!< The ebox rectangle. */
    t_rect              b_rect_last;        /*!< The ebox previous rectangle. */
    t_efont             b_font;             /*!< The ebox font. */
    int                 b_selected_box;     /*!< The selected state */
    int                 b_selected_item;    /*!< The items selected. */
    int                 b_selected_inlet;   /*!< The inlet selected. */
    int                 b_selected_outlet;  /*!< The outlet selected. */
    
    char                b_mouse_down;       /*!< The mouse state. */
    
    char                b_visible;          /*!< The visible state. */
    char                b_ready_to_draw;    /*!< The ebox state for drawing. */
    char                b_have_window;      /*!< The ebox window state. */
    char                b_isinsubcanvas;    /*!< If the box is in a sub canvas. */
    t_edrawparams       b_boxparameters;    /*!< The ebox parameters. */
    
    t_elayer*           b_layers;           /*!< The ebox layers. */
    long                b_number_of_layers; /*!< The ebox number of layers. */
}t_ebox;

/** @} */


/**
 * @struct t_edspbox
 * @brief The  GUI DSP object.
 * @details It contains the  object with all the members for graphical behavior and signal processing.
 * This should be used for graphical object that have signal processing methods.
 * @ingroup groupbox groupdspobj
 */
typedef struct t_edspbox
{
    t_eobj              b_obj;              /*!< The  DSP object. */
    
    t_symbol*           b_receive_id;       /*!< The object user ID. */
    t_symbol*           b_send_id;          /*!< The object send ID. */
    t_symbol*           b_objpreset_id;     /*!< The object preset ID. */
    
    t_symbol*           b_canvas_id;        /*!< The canvas ID. */
    t_symbol*           b_drawing_id;       /*!< The drawing ID. */
    t_symbol*           b_window_id;        /*!< The window ID. */
    t_symbol*           b_all_id;           /*!< The global ID. */
    
    long                b_flags;            /*!< The ebox flags. */
    t_rect              b_rect;             /*!< The ebox rectangle. */
    t_rect              b_rect_last;        /*!< The ebox previous rectangle. */
    t_efont             b_font;             /*!< The ebox font. */
    int                 b_selected_box;     /*!< The selected state */
    int                 b_selected_item;    /*!< The items selected. */
    int                 b_selected_inlet;   /*!< The inlet selected. */
    int                 b_selected_outlet;  /*!< The outlet selected. */
    
    char                b_mouse_down;       /*!< The mouse state. */
    
    char                b_visible;          /*!< The visible State. */
    char                b_ready_to_draw;    /*!< The ebox state for drawing. */
    char                b_have_window;      /*!< The ebox window state. */
    char                b_isinsubcanvas;    /*!< If the box is in a sub canvas. */
    t_edrawparams       b_boxparameters;    /*!< The ebox parameters. */
    
    t_elayer*           b_layers;           /*!< The ebox layers. */
    long                b_number_of_layers; /*!< The ebox number of layers. */
    
    float               d_float;            /*!< The float member to initialize the signal method. */
    long                d_dsp_size;         /*!< The number of signal inlets and outlets. */
    t_int*              d_dsp_vectors;      /*!< The vector that contains all the pointers for the perform method. */
    long                d_dsp_flag;         /*!< The flags to initialize the perform method. */
    void*               d_dsp_user_param;   /*!< The user parameters to pass through the perform method. */
    t_float**           d_sigs_out;         /*!< The array of signal vectors. */
    t_float*            d_sigs_real;        /*!< The real array of signal. */
    method              d_perform_method;   /*!< The user perform method. */
    long                d_misc;             /*!< The flag that could be inplace or not. */
}t_edspbox;



#endif
