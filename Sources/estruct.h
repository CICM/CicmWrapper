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

#ifndef DEF_ESTRUCT
#define DEF_ESTRUCT

#include "ecommon/ecommon.h"

typedef struct _pt
{
	float x;
	float y;
} t_pt;

typedef struct _rect
{
	float x;
	float y;
	float width;
	float height;
} t_rect;

typedef struct _rgb
{
	float red;
	float green;
	float blue;
} t_rgb;

typedef struct _rgba
{
	float red;
	float green;
	float blue;
	float alpha;
} t_rgba;

typedef struct _matrix
{
	float xx;
	float yx;
	float xy;
	float yy;
	float x0;
	float y0;
} t_matrix;

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
    method          w_dblclick;
    method          w_dblclicklong;
    method          w_key;
    method          w_keyfilter;
    method          w_deserted;
    method          w_getdrawparameters;
    t_err_method    w_notify;
    method          w_save;
    method          w_popup;
    method          w_dsp;
    method          w_object_standat_paint;
    method          w_oksize;
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
/*
"",
"Mod1-",
"Mod2-",
"Control-",
"Shift-",

"Mod1-Mod2-",
"Mod1-Control-",
"Mod1-Shift-",
"Mod2-Control-",
"Mod2-Shift-"
"Control-Shift-",

"Mod1-Mod2-Control-",
"Mod1-Mod2-Shift-",
"Mod2-Control-Shift-",

"Mod1-Mod2-Shift-Control-",
*/

typedef enum _emod_flags
{
    EMOD_NONE     = 0,
	EMOD_CMD      = 1,
    EMOD_ALT      = 2,
    EMOD_CTRL     = 3,
    EMOD_SHIFT    = 4,
	EMOD_CMDALT   = 5,
    EMOD_CMDCTRL  = 6,
    EMOD_CMDSHIFT = 7,
    EMOD_ALTCTRL  = 8,
	EMOD_ALTSHIFT = 9,
    EMOD_CTRLSHIFT  = 10,
    EMOD_CMDALTCTRL = 11,
    EMOD_CMDALTSHIFT = 12,
    EMOD_ALTCTRLSHIFT = 13,
    EMOD_CMDALTCTRLSHIFT = 14,
} t_emod_flags;

typedef enum _ekey_flags
{
    EKEY_DEL      = 8,
	EKEY_TAB      = 9,
    EKEY_ENTER    = 10,
    EKEY_ESC      = 27,
    
} t_ekey_flags;

typedef enum _elayer_flags
{
	EGRAPHICS_OPEN      = 0,
	EGRAPHICS_CLOSE     = -1,
	EGRAPHICS_INVALID   = -2,
    EGRAPHICS_TODRAW    = -3,
} t_elayer_flags;

typedef enum _etextjustify_flags
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
    
} t_etextjustify_flags;

typedef enum _etextwrap_flags
{
	ETEXT_NOWRAP    = 0,
	ETEXT_WRAP      = 1
} t_etextwrap_flags;

typedef enum
{
    E_GOBJ_INVALID           = 0,
    E_GOBJ_PATH                 ,
    E_GOBJ_RECT                 ,
    E_GOBJ_ARC                  ,
    E_GOBJ_OVAL                 ,
    E_GOBJ_TEXT                 ,
} egraphics_types;

typedef struct _efont
{
    t_symbol*   c_family;   // times, helvetica, ect...
    t_symbol*   c_slant;    // regular, italic
    t_symbol*   c_weight;   // regular, bold
    float       c_size;
} t_efont;

typedef struct _etext
{
    t_symbol*       c_text;
    t_rgba          c_color;
    t_efont         c_font;
    t_rect          c_rect;
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

typedef struct _ebox
{
    t_object            e_obj;
    t_canvas*           e_canvas;
    
    char*               e_classname;
    t_symbol*           e_name_tcl;
    t_symbol*           e_name_rcv;
    
    t_symbol*           e_canvas_id;
    t_symbol*           e_drawing_id;
    t_symbol*           e_frame_id;
    t_symbol*           e_handle_id;
    t_symbol*           e_window_id;
    t_symbol*           e_all_id;
    
    t_rect              e_rect;
    t_efont             e_font;
    
    char                e_selected;
    t_pt                e_mouse;
    char                e_mouse_down;
    long                e_modifiers;
    
    char                e_ready_to_draw;
    t_edrawparams       e_boxparameters;
    float               e_lastclick;
    float               e_dblclick_time;
    float               e_dblclicklong_time;
    
    float               e_deserted_time;
    t_clock*            e_deserted_clock;
    
    t_elayer*           e_layers;
    long                e_number_of_layers;
    
    
    //t_epopupmenu*       e_popup;
    
    t_inlet*            e_inlets[256];
    t_outlet*           e_outlets[256];
    
    float               e_float;                // float value for signal
    long                e_dsp_size;
    t_int               e_dsp_vectors[262];
    long                e_dsp_flag;
    void*               e_dsp_user_param;
    long                z_misc;     // Special Max for Z_NO_INPLACE
    t_float*            z_sigs_out[256];
    method              e_perform_method;
    
    t_object*           b_firstin; // For Max, it doesn't matter !
}t_ebox;





#endif 