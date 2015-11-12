/*
 * CicmWrapper - Pure Data Enhanced

 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

/**
 * @file edefine.h
 * @brief The header that contains all the principals structures of CicmWrapper
 * @details All the principals structures are defined in this file.
 * @author Pierre Guillot
 */

#ifndef DEF_ESTRUCT
#define DEF_ESTRUCT

#ifdef _WINDOWS
#include <io.h>
#define _FUNCTION_DEPRECTAED_
#else
#include <unistd.h>
#define _FUNCTION_DEPRECTAED_  __attribute__((deprecated))
#endif

#include <m_pd.h>
#include <g_canvas.h>
#include <s_stuff.h>

#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>

#ifdef _MSC_VER
#define snprintf sprintf_s
#endif


#define PD_INSTANCE (PD_MAJOR_VERSION >= 0) && (PD_MINOR_VERSION) >= 46) && (PD_BUGFIX_VERSION >= 6)
#define CICM_VERSION 1.0

/*! @defgroup groupobj The BASIC Object Part
 * @brief The t_eobj part.
 * @details This part refers to the methods and structures that can be used by all the t_eobj structures.
 */

/*! @addtogroup groupdspobj The DSP Object Part
 * @brief The t_edspobj and t_edspbox part.
 * @details This part refers to the methods and structures that can be used by all the t_edspobj structures.
 */

/*! @addtogroup groupcommon The Common Part
 * @brief The general part.
 * @details This part refers to the general methods and definitions.
 *  @{
 */

//! @cond
#define atom_setfloat(a, f) SETFLOAT(a, f)
#define atom_setlong(a, l)  SETFLOAT(a, (float)l)
#define atom_setsym(a, s)   SETSYMBOL(a, s)
#define atom_setsymbol(a, s)   SETSYMBOL(a, s)
#define atom_getlong(a)     (long)atom_getfloat(a)
#define atom_gettype(a)     (a)[0].a_type
//! @endcond

//! The error type
typedef long t_pd_err;
//! The standart method
typedef void        (*t_typ_method)(void* x, ...);
//! The return method
typedef void*       (*t_ret_method)(void* x, ...);
//! The error method
typedef t_pd_err    (*t_err_method)(void* x, ...);
//! The float method
typedef float       (*t_flt_method)(void* x, ...);
//! The long method
typedef long        (*t_long_method)(void* x, ...);
//! The symbol method
typedef t_symbol*   (*t_sym_method)(void* x, ...);
//! The object method
typedef t_object*   (*t_obj_method)(void* x, ...);
//! The notify method
typedef t_pd_err    (*t_notify_method)(void* x, t_symbol *s, t_symbol *msg, void *sender, void *data);
//! The setter method
typedef t_pd_err    (*t_setter_method)(void* x, t_object *attr, int ac, t_atom *av);
//! The setter method
typedef t_pd_err    (*t_getter_method)(void *x, t_object *attr, int* ac, t_atom **av);
//! The binbuf method
typedef void        (*t_binbuf_method)(void *x, t_binbuf* b);

//! The pre-defined (null) t_symbol*
extern t_symbol* s_cream_null;
//! The pre-defined "" t_symbol*
extern t_symbol* s_cream_empty;
extern t_symbol* s_cream_quote;
//! The pre-defined obj t_symbol*
extern t_symbol* s_cream_obj;
//! The pre-defined attr_modified t_symbol*
extern t_symbol* s_cream_attr_modified;
//! The pre-defined value_changed t_symbol*
extern t_symbol* s_cream_value_changed;
//! The pre-defined eboxbd t_symbol*
extern t_symbol* s_cream_eboxbd;
//! The pre-defined eboxio t_symbol*
extern t_symbol* s_cream_eboxio;
//! The pre-defined texteditor t_symbol*
extern t_symbol* s_cream_texteditor;
//! The pre-defined popup t_symbol*
extern t_symbol* s_cream_popup;
//! The pre-defined parameter t_symbol*
extern t_symbol* s_cream_parameter;
//! The pre-defined grabfocus t_symbol*
extern t_symbol* s_cream_grabfocus;
//! The pre-defined repaint t_symbol*
extern t_symbol* s_cream_repaint;
//! The pre-defined size t_symbol*
extern t_symbol* s_cream_size;
//! The pre-defined create t_symbol*
extern t_symbol* s_cream_create;
//! The pre-defined destroy t_symbol*
extern t_symbol* s_cream_destroy;
//! The pre-defined beginchanges t_symbol*
extern t_symbol* s_cream_beginchanges;
//! The pre-defined endchanges t_symbol*
extern t_symbol* s_cream_endchanges;
//! The pre-defined changes t_symbol*
extern t_symbol* s_cream_changes;
//! The pre-defined view t_symbol*
extern t_symbol* s_cream_view;
//! The pre-defined notify t_symbol*
extern t_symbol* s_cream_notify;

//! The pre-defined char t_symbol*
extern t_symbol* s_cream_char;
//! The pre-defined int t_symbol*
extern t_symbol* s_cream_int;
//! The pre-defined long t_symbol*
extern t_symbol* s_cream_long;
//! The pre-defined double t_symbol*
extern t_symbol* s_cream_float;
//! The pre-defined double t_symbol*
extern t_symbol* s_cream_double;
//! The pre-defined atom t_symbol*
extern t_symbol* s_cream_atom;
//! The pre-defined symbol t_symbol*
extern t_symbol* s_cream_symbol;

//! The pre-defined s_pinned t_symbol*
extern t_symbol* s_cream_pinned;
//! The pre-defined ignoreclick t_symbol*
extern t_symbol* s_cream_ignoreclick;
extern t_symbol* s_cream_send;
extern t_symbol* s_cream_save;
extern t_symbol* s_cream_receive;
//! The pre-defined iscicm t_symbol*
extern t_symbol* s_cream_iscicm;
//! The pre-defined isgui t_symbol*
extern t_symbol* s_cream_isgui;
//! The pre-defined isdsp t_symbol*
extern t_symbol* s_cream_isdsp;

//! The pre-defined checkbutton t_symbol*
extern t_symbol* s_cream_checkbutton;
//! The pre-defined onoff t_symbol*
extern t_symbol* s_cream_onoff;
//! The pre-defined color t_symbol*
extern t_symbol* s_cream_color;
//! The pre-defined number t_symbol*
extern t_symbol* s_cream_number;
//! The pre-defined menu t_symbol*
extern t_symbol* s_cream_menu;
//! The pre-defined entry t_symbol*
extern t_symbol* s_cream_entry;
//! The pre-defined font t_symbol*
extern t_symbol* s_cream_font;

//! The pre-defined linear t_symbol*
extern t_symbol* s_cream_linear;
//! The pre-defined exponential t_symbol*
extern t_symbol* s_cream_exponential;
//! The pre-defined logarithmic t_symbol*
extern t_symbol* s_cream_logarithmic;
//! The pre-defined threshold t_symbol*
extern t_symbol* s_cream_threshold;

//! @cond
EXTERN t_namelist *sys_staticpath;
EXTERN t_namelist *sys_searchpath;
EXTERN t_namelist *namelist_append_files(t_namelist *listwas, const char *s);
//! @endcond

/** @} */

/*! @addtogroup groupgraphics The Graphics Part
 * @brief The t_elayer part.
 * @details This part refers to the methods and structures used to paint.
 *  @{
 */

//! Macros that define the a pi
#define EPD_PI  (3.141592653589793238462643383279502884f)
//! Macros that define the a 2pi
#define EPD_2PI (6.283185307179586476925286766559005f)
//! Macros that define the a pi over 2
#define EPD_PI2 (1.57079632679489661923132169163975144f)
//! Macros that define the a pi over 4
#define EPD_PI4 (0.785398163397448309615660845819875721f)

/**
 * @enum etextwrap_flags
 * @brief The flags that defines if the text should be wrapped.
 * @details It define true or false.
 */
typedef enum etextwrap_flags
{
    ETEXT_NOWRAP    = 0, /*!< False. */
    ETEXT_WRAP      = 1  /*!< True. */
} etextwrap_flags;

/**
 * @enum etextjustify_flags
 * @brief The flags that define the text justification.
 * @details It define all text justification.
 */
typedef enum etextjustify_flags
{
    ETEXT_LEFT                      = 1,
    ETEXT_RIGHT                     = 2,
    ETEXT_HORINTALLYCENTRED         = 4,
    ETEXT_TOP                       = 8,
    ETEXT_TOPLEFT                   = 9,
    ETEXT_TOPRIGHT                  = 10,
    ETEXT_CENTREDTOP                = 12,
    ETEXT_BOTTOM                    = 16,
    ETEXT_BOTTOMLEFT                = 17,
    ETEXT_BOTTOMRIGHT               = 18,
    ETEXT_CENTREDBOTTOM             = 20,
    ETEXT_VERTICALLYCENTRER         = 32,
    ETEXT_CENTREDLEFT               = 33,
    ETEXT_CENTREDRIGHT              = 34,
    ETEXT_CENTRED                   = 36
} etextjustify_flags;

/**
 * @enum elayer_types
 * @brief The types of graphical object.
 * @details It define all the graphical type.
 */

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

//! The pre-defined black t_rgba
extern const t_rgba rgba_black;
//! The pre-defined grey dark t_rgba
extern const t_rgba rgba_greydark;
//! The pre-defined grey t_rgba
extern const t_rgba rgba_grey;
//! The pre-defined grey light t_rgba
extern const t_rgba rgba_greylight;
//! The pre-defined white t_rgba
extern const t_rgba rgba_white;
//! The pre-defined blue t_rgba
extern const t_rgba rgba_blue;
//! The pre-defined green t_rgba
extern const t_rgba rgba_green;
//! The pre-defined red t_rgba
extern const t_rgba rgba_red;

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
 * @struct t_efont
 * @brief The font structure.
 * @details It contains the informations of a font.
 */
struct _efont
{
    t_symbol*   family;   /*!< The family of the font (times, helvetica, ect.). */
    float       size;     /*!< The size of the font. */
    t_symbol*   slant;    /*!< The slant of the font (regular, italic, etc.). */
    t_symbol*   weight;   /*!< The weight of the font (normal, bold). */
};
typedef struct _efont t_efont;

EXTERN_STRUCT _eguicontext;
typedef struct _eguicontext t_eguicontext;

EXTERN_STRUCT _elayer;
typedef struct _elayer t_elayer;

EXTERN_STRUCT _epath;
typedef struct _epath t_epath;

/**
 * @struct t_etextlayout
 * @brief The text structure.
 * @details It contains the all the informations to be drawn.
 */
EXTERN_STRUCT _etextlayout;
typedef struct _etextlayout t_etextlayout;

/** @} */


typedef void (*t_mouse_method)(void *x, t_object *view, t_pt pt, long modifiers);
typedef void (*t_mousewheel_method)(void *x, t_object *view, t_pt pt, long modifiers, float deltax, float deltay);
typedef void (*t_key_method)(void *x, t_object *view, char textcharacter, long modifiers);
typedef void (*t_popup_method)(void *x, t_object *view, long itemid);
typedef void (*t_drawparameter_method)(void *x, t_object *view, t_edrawparams* params);
typedef void (*t_oksize_method)(void *x, t_object *view, t_rect *newrect);
typedef void (*t_paint_method)(void *x, t_object *view);

/**
 * @struct t_eobj
 * @brief The default object.
 * @details It contains the native object, the canvas pointer and members for proxy inlets.
 * @details This should be used for no graphical object that don't have signal processing methods.
 * @ingroup groupobj
 */
typedef struct _eobj
{
    t_object            o_obj;              /*!< The Pd object. */
    t_symbol*           o_id;               /*!< The object id. */
    t_canvas*           o_canvas;           /*!< The canvas that own the object. */
    t_object**          o_proxy;            /*!< The array of proxy inlets. */
    size_t              o_nproxy;           /*!< The number of proxy inlets. */
    size_t              o_cproxy;           /*!< The index of the current proxy inlet used */
    t_symbol*           o_listeners;        /*!< The listeners id. */
    long                o_dummy;            /*!< The dummy member. */
} t_eobj;

/**
 * @brief The GUI object.
 * @details It contains the t_eobj with all the members for graphical behavior.
 * This should be used for graphical object that don't have signal processing methods.
 * @ingroup groupbox
 */
typedef t_eobj t_ebox;

/**
 * @brief The dsp object.
 * @details It contains all the members for signal processing.
 * @details This should be used for no graphical object that have signal processing methods.
 * @ingroup groupdspobj
 */
typedef t_eobj t_edspobj;

/**
 * @brief The GUI DSP object.
 * @details It contains the t_eobj with all the members for graphical behavior and signal processing.
 * This should be used for graphical object that have signal processing methods.
 * @ingroup groupbox groupdspobj
 */
typedef t_ebox t_edspbox;

/**
 * @brief The class.
 * @details The native class.
 */
typedef struct _class t_eclass;

/** @} */

/*! @addtogroup groupbox The GUI Object Part
 * @brief The t_ebox and t_edspbox part.
 * @details This part refers to the methods and structures that can be used by all the t_ebox structures.
 *  @{
 */

/**
 * @enum emod_flags
 * @brief The mouse modifiers.
 * @details It define mouse modifiers.
 */
typedef enum emod_flags
{
    EMOD_NONE           = 0, /*!< Nothing. */
    EMOD_SHIFT          = 1, /*!< Shift. */
    EMOD_MAJ            = 2, /*!< Maj. */
    EMOD_CTRL           = 4, /*!< Control. */
    EMOD_CMD            = 8, /*!< Command. */
    EMOD_ALT            = 16,/*!< Alt. */
    EMOD_RIGHT          = 32 /*!< Right click. */
} emod_flags;

/**
 * @enum ekey_flags
 * @brief The system keys.
 * @details It define the system keys.
 */
typedef enum ekey_flags
{
    EKEY_DEL      = 0, /*!< The delete key. */
    EKEY_TAB      = 1, /*!< The tabulation key. */
    EKEY_RETURN   = 2, /*!< The return key. */
    EKEY_ESC      = 3  /*!< The escape key. */
    
} ekey_flags;

//! @cond
typedef enum eselitem_flags
{
    EITEM_NONE    = 0,
    EITEM_OBJ     = 1,
    EITEM_INLET   = 2,
    EITEM_OUTLET  = 3,
    EITEM_BOTTOM  = 4,
    EITEM_CORNER  = 5,
    EITEM_RIGHT   = 6
} eselitem_flags;
//! @endcond


/**
 * @enum ecursor_types
 * @brief The cursors that can be used.
 * @details It define all the available cursors.
 */
typedef enum
{
    ECURSOR_DEFAULT      = 0, /*!< The default pointer. */
    ECURSOR_INTERACTION  = 1, /*!< The interaction pointer. */
    ECURSOR_SDOUBLE_ARROW= 2, /*!< The sb_v_double_arrow string. */
    ECURSOR_PLUS         = 3, /*!< The plus string. */
    ECURSOR_SELECTION    = 4, /*!< The selection pointer. */
    ECURSOR_OUTLET       = 5, /*!< The outlet pointer. */
    ECURSOR_X            = 6, /*!< The X_cursor string. */
    ECURSOR_BOTTOM       = 7, /*!< The bottom border pointer. */
    ECURSOR_BOTTOM_RIGHT = 8, /*!< The bottom right corner pointer. */
    ECURSOR_RIGHT        = 9, /*!< The right border pointer. */
    ECURSOR_DOUBLE_ARROW = 10, /*!< The double_arrow string. */
    ECURSOR_EXCHANGE     = 11, /*!< The exchange string. */
    ECURSOR_XTERM        = 12  /*!< The xterm string. */
} ecursor_types;

/**
 * @enum elayer_flags
 * @brief The flags that defines the status of a layer.
 * @details It define all possible the status of a layer.
 */
typedef enum eparam_flags
{
    EPARAM_STATIC_NAME      = (1<<1),   /*!< If the name of the parameter can't be changed by the users. */
    EPARAM_STATIC_LABEL     = (1<<2),   /*!< If the name of the parameter can't be changed by the users. */
    EPARAM_STATIC_MIN       = (1<<3),   /*!< If the minimum value of the parameter can't be changed by the users. */
    EPARAM_STATIC_MAX       = (1<<4),   /*!< If the maximum value of the parameter can't be changed by the users. */
    EPARAM_STATIC_INVERTED  = (1<<5),   /*!< If the maximum and minimum values of the parameter can't be inverted by the users. */
    EPARAM_STATIC_NSTEPS    = (1<<6),    /*!< If the number of steps can't be changed by the users. */
    EPARAM_STATIC_INDEX     = (1<<7)    /*!< If the number of steps can't be changed by the users. */
} eparam_flags;


/** @} */



/*! @addtogroup groupwidget The Widget Part
 * @brief The t_epopup and t_etextlayouteditor part.
 * @details This part refers to the methods and structures that can be used by all the t_epopup and t_etextlayouteditor structures.
 *  @{
 */

/**
 * @struct t_epopup_item
 * @brief The popup's item structure.
 * @details It contains the informations to show an item of a popup.
 */
typedef struct t_epopup_item
{
    int         c_id;                   /*!< The id of the item. */
    char        c_label[MAXPDSTRING];   /*!< The name of the popup. */
    char        c_checked;              /*!< If the item is checked. */
    char        c_disable;              /*!< If the item is disable. */
    char        c_separator;            /*!< If the item is a seprator. */
}t_epopup_item;

/**
 * @struct t_epopup
 * @brief The popup structure.
 * @details It contains the informations to show and retrieve a popup.
 */
typedef struct t_epopup
{
    t_object        c_obj;      /*!< The object. */
    t_object*       c_owner;    /*!< The owner. */
    t_symbol*       c_popup_id; /*!< The popup id. */
    t_symbol*       c_canvas_id;/*!< The canvas id. */
    t_symbol*       c_name;     /*!< The name of the popup. */
    t_efont*        c_font;     /*!< The font of the popup. */
    t_rgba          c_bgcolor;  /*!< The background color of the popup. */
    t_rgba          c_txtcolor; /*!< The text color of the popup. */
    int             c_size;     /*!< The number of items. */
    t_epopup_item*  c_items;    /*!< The items. */
    t_rect          c_bounds;   /*!< The bounds of the popup. */
}t_epopup;


/**
 * @struct t_etextlayouteditor
 * @brief The text editor structure.
 * @details It contains the informations to show and retrieve a popup.
 */
typedef struct t_etextlayouteditor
{
    t_object    c_obj;          /*!< The object. */
    t_object*   c_owner;
    t_symbol*   c_editor_id;
    t_symbol*   c_canvas_id;
    t_symbol*   c_frame_id;
    t_symbol*   c_window_id;
    t_symbol*   c_name;
    char*       c_text;
    int         c_size;
    t_efont*    c_font;
    t_rgba      c_bgcolor;
    t_rgba      c_txtcolor;
    char        c_wrap;
    t_rect      c_bounds;
}t_etextlayouteditor;

/**

/** @} */



#endif
