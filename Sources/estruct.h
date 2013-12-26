/*
 * PdEnhanced - Pure Data Enhanced 
 *
 * An add-on for Pure Data
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
 * @brief The header that contains all the principals structures of PdEnhanced
 * @details All the principals structures are defined in this file.
 * @author Pierre Guillot
 */
#ifndef DEF_ESTRUCT
#define DEF_ESTRUCT

#include "ecommon/ecommon.h"

/**
 * @struct _pt
 * @brief A point structure.
 * @details It contains the members x and y for abscissa and ordinate.
 */
typedef struct _pt
{
	float x; /*!< The abscissa coordiante. */
	float y; /*!< The ordiante coordiante. */
} t_pt;

/**
 * @struct _rect
 * @brief A rectangle structure.
 * @details It contains the members x, y, width and height for abscissa and ordinate and size.
 */
typedef struct _rect
{
	float x;        /*!< The abscissa coordiante. */
	float y;        /*!< The ordiante coordiante. */
	float width;    /*!< The width of the rectangle */
	float height;   /*!< The height of the rectangle */
} t_rect;

/**
 * @struct _matrix
 * @brief A matrix structure.
 * @details It contains the members xx, yx, xy, yy, x0 and y0
 */
typedef struct _matrix
{
	float xx;   /*!< The abscissa-abscissa translation of the matrix */
	float yx;   /*!< The abscissa-ordiante translation of the matrix */
	float xy;   /*!< The ordiante-abscissa translation of the matrix */
	float yy;   /*!< The ordiante-ordiante translation of the matrix */
	float x0;   /*!< The abscissa origin of the matrix */
	float y0;   /*!< The ordiante origin of the matrix */
} t_matrix;

/**
 * @struct _rgb
 * @brief A rgb color structure.
 * @details It contains the members red, green and blue.
 */
typedef struct _rgb
{
	float red;      /*!< The red value. */
	float green;    /*!< The green value. */
	float blue;     /*!< The blue value. */
} t_rgb;

/**
 * @struct _rgba
 * @brief A rgba color structure.
 * @details It contains the members red, green, blue and alpha.
 */
typedef struct _rgba
{
	float red;      /*!< The red value. */
	float green;    /*!< The green value. */
	float blue;     /*!< The blue value. */
	float alpha;    /*!< The alpha value. */
} t_rgba;

/**
 * @struct _hsl
 * @brief A hsl color structure.
 * @details It contains the members hue, saturation and lightness.
 */
typedef struct _hsl
{
	float hue;          /*!< The hue value. */
	float saturation;   /*!< The saturation value. */
	float lightness;    /*!< The lightness value. */
} t_hsl;

/**
 * @struct _hsla
 * @brief A hsla color structure.
 * @details It contains the members hue, saturation, lightness and alpha.
 */
typedef struct _hsla
{
	float hue;          /*!< The hue value. */
	float saturation;   /*!< The saturation value. */
	float lightness;    /*!< The lightness value. */
    float alpha;        /*!< The alpha value. */
} t_hsla;

typedef struct _ewidget
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
    method          w_deserted;
    method          w_getdrawparameters;
    method          w_save;
    method          w_popup;
    method          w_dsp;
    method          w_object_standat_paint;
    method          w_oksize;
    t_err_method    w_notify;
    method          w_write;
    method          w_read;
} t_ewidget;

typedef struct _eattr
{
    t_object*       obj;
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
    t_symbol*       defvals;
    
} t_eattr;

typedef struct _eclass
{
    t_class     c_class;
    char        c_box;
    t_ewidget   c_widget;
    t_eattr*    c_attr;
    long        c_nattr;
}t_eclass;

typedef enum _emod_flags
{
    EMOD_NONE           = 0,
	EMOD_SHIFT          = 1,
    EMOD_MAJ            = 2,
    EMOD_CTRL           = 4,
    EMOD_CMD            = 8,
    EMOD_ALT            = 16,
} t_emod_flags;

typedef enum _ekey_flags
{
    EKEY_DEL      = 0,
	EKEY_TAB      = 1,
    EKEY_ENTER    = 2,
    EKEY_ESC      = 3,
    
} t_ekey_flags;

typedef enum _eselitem_flags
{
    EITEM_NONE    = 0,
    EITEM_OBJ     = 1,
	EITEM_INLET   = 2,
    EITEM_OUTLET  = 3,
    EITEM_BOTTOM  = 4,
    EITEM_CORNER  = 5,
    EITEM_RIGHT   = 6,
    
} t_eselitem_flags;

typedef enum _elayer_flags
{
	EGRAPHICS_OPEN      = 0,
	EGRAPHICS_CLOSE     = -1,
	EGRAPHICS_INVALID   = -2,
    EGRAPHICS_TODRAW    = -3,
} t_elayer_flags;

typedef enum _etextanchor_flags
{
    ETEXT_UP            = 0,
    ETEXT_UP_LEFT       = 1,
    ETEXT_UP_RIGHT      = 2,
    ETEXT_DOWN          = 3,
    ETEXT_DOWN_LEFT     = 4,
    ETEXT_DOWN_RIGHT    = 5,
	ETEXT_LEFT          = 6, // first element of text = x
    ETEXT_RIGHT         = 7, // last element of text = x
	ETEXT_CENTER        = 8  // center element of text = x
    
} t_etextanchor_flags;

typedef enum _etextwrap_flags
{
	ETEXT_NOWRAP    = 0,
	ETEXT_WRAP      = 1
} t_etextwrap_flags;

typedef enum _etextjustify_flags
{
	ETEXT_JLEFT    = 0,
	ETEXT_JRIGHT   = 1,
    ETEXT_JCENTER  = 2
} t_etextjustify_flags;

typedef enum
{
    E_GOBJ_INVALID           = 0,
    E_GOBJ_PATH                 ,
    E_GOBJ_RECT                 ,
    E_GOBJ_ARC                  ,
    E_GOBJ_OVAL                 ,
    E_GOBJ_TEXT                 ,
    E_GOBJ_IMAG                 ,
} egraphics_types;

typedef struct _efont
{
    t_symbol*   c_family;   // times, helvetica, ect...
    t_symbol*   c_slant;    // regular, italic
    t_symbol*   c_weight;   // normal, bold
    float       c_size;
} t_efont;

typedef struct _epopup
{
    t_symbol*   c_name;
    t_symbol*   c_send;
}t_epopup;

typedef struct _etextfield
{
    t_symbol*   c_name;
    t_symbol*   c_send;
}t_etextfield;

typedef struct _etext
{
    t_symbol*       c_text;
    t_rgba          c_color;
    t_efont         c_font;
    t_rect          c_rect;
    t_symbol*       c_anchor;
    t_symbol*       c_justify;
} t_etext;

typedef struct _egobj
{
	int             e_type;
    int             e_filled;
    t_symbol*       e_color;
    float           e_width;
    
	t_pt*           e_points;
    long            e_npoints;
    float           e_roundness;
    t_efont         e_font;
    t_symbol*       e_anchor;
    t_symbol*       e_justify;
    t_symbol*       e_text;

} t_egobj;

typedef struct _elayer
{
    t_object*           e_owner;
    t_symbol*           e_name;
    t_symbol*           e_id;
    int                 e_state;
    
    t_rect              e_rect;
    t_symbol*           e_color;
    int                 e_width;
    
    t_matrix            e_matrix;
    t_egobj             e_new_objects;
    t_egobj*            e_objects;
    long                e_number_objects;
} t_elayer;

typedef struct _eview
{
    t_object*   e_owner;
    t_symbol*   e_name;
} t_eview;

typedef struct _edrawparams
{    
	float		d_cornersize;
	float       d_borderthickness;
	t_rgba      d_bordercolor;
	t_rgba      d_boxfillcolor;
    
} t_edrawparams;

t_class *eproxy_class;

typedef struct _eproxy {
	t_pd        p_pd;
	t_object*   p_owner;
    int         p_index;
} t_eproxy;


/**
 * @struct _eobj
 * @brief The Pd Enhanced Box object.
 * @details It contains the Pd object, the canvas pointer and members for proxy inlets.
 * This should be used for no graphical object that don't have signal processing methods.
 */
typedef struct _eobj
{
    t_object            o_obj;              /*!< The Pd object. */
    t_canvas*           o_canvas;           /*!< The canvas that own the object. */
    t_eproxy            o_proxy[256];       /*!< The array of proxy inlets. */
    long                o_nproxy;           /*!< The number of proxy inlets. */
    long                o_current_proxy;    /*!< The index of the current proxy inlet used */
}t_eobj;

/**
 * @struct _edspobj
 * @brief The Pd Enhanced Box DSP object.
 * @details It contains the Pd Enhanced object with all the members for signal processing.
 * This should be used for no graphical object that have signal processing methods.
 */
typedef struct _edspobj
{
    t_eobj              d_obj;              /*!< The Pd Enhanced object. */

    t_inlet*            d_inlets[256];      /*!< The array of signal inlets. */
    t_outlet*           d_outlets[256];     /*!< The array of signal outlets. */
    float               d_float;            /*!< The float member to initialize the signal method. */
    long                d_dsp_size;         /*!< The number of signal inlets and outlets. */
    t_int               d_dsp_vectors[262]; /*!< The vector that contains all the pointers for the perform method. */
    long                d_dsp_flag;         /*!< The flags to initialize the perform method. */
    void*               d_dsp_user_param;   /*!< The user parameters to pass through the perform method. */
    t_float*            d_sigs_out[256];    /*!< The array of signal vectors. */
    method              d_perform_method;   /*!< The user perform method. */
    long                d_misc;
}t_edspobj;

/**
 * @struct _ebox
 * @brief The Pd Enhanced GUI object.
 * @details It contains the Pd Enhanced object with all the members for graphical behavior.
 * This should be used for graphical object that don't have signal processing methods.
 */
typedef struct _ebox
{
    t_eobj              b_obj;              /*!< The Pd Enhanced object. */
    
    t_symbol*           b_object_id;        /*!< The object intern ID. */
    t_symbol*           b_objuser_id;       /*!< The object user ID. */
    t_symbol*           b_objpreset_id;     /*!< The object preset ID. */
    
    t_symbol*           b_canvas_id;        /*!< The canvas ID. */
    t_symbol*           b_drawing_id;       /*!< The drawing ID. */
    t_symbol*           b_editor_id;        /*!< The editor ID. */
    t_symbol*           b_window_id;        /*!< The window ID. */
    t_symbol*           b_all_id;           /*!< The global ID. */
    
    long                b_flags;
    t_rect              b_rect;
    t_efont             b_font;
    t_rect              b_rect_last;
    int                 b_selected_box;
    int                 b_selected_item;
    int                 b_selected_inlet;
    int                 b_selected_outlet;
    
    t_pt                b_mouse;
    t_pt                b_move_box;
    char                b_mouse_down;
    long                b_modifiers;
    
    char                b_ready_to_draw;
    t_edrawparams       b_boxparameters;
    
    t_elayer*           b_layers;
    long                b_number_of_layers;
}t_ebox;

/**
 * @struct _edspbox
 * @brief The Pd Enhanced GUI DSP object.
 * @details It contains the Pd Enhanced object with all the members for graphical behavior and signal processing.
 * This should be used for graphical object that have signal processing methods.
 */
typedef struct _edspbox
{
    t_edspobj           b_obj;              /*!< The Pd Enhanced DSP object. */
    
    t_symbol*           b_object_id;
    t_symbol*           b_objuser_id;
    t_symbol*           b_objpreset_id;
    t_symbol*           b_canvas_id;
    t_symbol*           b_drawing_id;
    t_symbol*           b_editor_id;
    t_symbol*           b_window_id;
    t_symbol*           b_all_id;
    
    long                b_flags;
    t_rect              b_rect;
    t_efont             b_font;
    t_rect              b_rect_last;
    int                 b_selected_box;
    int                 b_selected_item;
    int                 b_selected_inlet;
    int                 b_selected_outlet;
    
    t_pt                b_mouse;
    t_pt                b_move_box;
    char                b_mouse_down;
    long                b_modifiers;
    
    char                b_ready_to_draw;
    t_edrawparams       b_boxparameters;
    
    t_elayer*           b_layers;
    long                b_number_of_layers;
}t_edspbox;





#endif 