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
	double x;
	double y;
} t_pt;

typedef struct _rect
{
	float x;
	float y;
	float width;
	float height;
} t_rect;

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
    method          w_mousemove;
    method          w_mousedown;
    method          w_mousedrag;
    method          w_mouseup;
    method          w_getdrawparameters;
    t_err_method    w_notify;
    method          w_save;
    method          w_popup;
    method          w_dsp;
    method          w_object_standat_paint;
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
    bool            save = 0;
    bool            paint = 0;
    bool            invisible = 0;
	long			flags = 0;
    long            offset;
    long            sizemax;
    long            size;
    
	method			getter = NULL;
	method			setter = NULL;
    long            clipped = 0;
	double          minimum;
    double          maximum;
    t_symbol*       defvals = NULL;
    
} t_eattr;

typedef struct _eclass
{
    t_class         c_class;
    bool            c_box;
    t_ewidget       c_widget;
    vector<t_eattr> c_attr;
}t_eclass;

typedef enum
{
	EGRAPHICS_OPEN      = 0,
	EGRAPHICS_CLOSE     = -1,
	EGRAPHICS_INVALID   = -2,
    EGRAPHICS_TODRAW    = -3,
} cicm_graphics_codes;


typedef struct _ergba
{
	double red;
	double green;
	double blue;
	double alpha;
} t_ergba;

typedef struct _ematrix
{
	float xx;
	float yx;
	float xy;
	float yy;
	float x0;
	float y0;
} t_ematrix;

typedef enum
{
    E_GOBJ_INVALID           = 0,
    E_GOBJ_LINE                 ,
    E_GOBJ_PATH                 ,
	E_GOBJ_RECTANGLE            ,
	E_GOBJ_RECTANGLE_ROUNDED    ,
    E_GOBJ_ARC                  ,
    E_GOBJ_OVAL                 ,
} egraphics_types;

typedef struct _egraphics_ob
{
	int             e_type;
    t_symbol*       e_name;
    int             e_filled;
    t_symbol*       e_color;
    float           e_width;
    
	t_pt*           e_points;
    long            e_npoints;
    float           e_roundness;
    float           e_angles[2];
} t_egraphics_obj;

typedef struct _egraphics
{
    t_object*           e_owner;
    t_symbol*           e_name;
    int                 e_state;
    
    t_rect              e_rect;
    t_symbol*           e_color;
    int                 e_width;
    
    t_ematrix*          e_matrix;
    double              e_rotation;
    
    t_egraphics_obj     e_new_objects;
    t_egraphics_obj*    e_objects;
    long                e_number_objects;
    
} t_egraphics;

typedef struct _efont
{
    t_symbol*   c_family;     // times, helvetica, ect...
    t_symbol*   c_slant;     // regular, italic
    t_symbol*   c_weight;   // regular, bold
    double      c_size;
} t_efont;

typedef struct _etextlayout
{
    t_symbol*       c_text;
    t_ergba         c_color;
    t_efont         c_font;
    t_rect          c_rect;
    t_symbol*       c_justification;
} t_etextlayout;

typedef struct _edrawparams
{
	float		d_inletheight;
	float		d_inletvoffset;
	float		d_outletheight;
	float		d_outletvoffset;
	float		d_reserved1;
    
	float		d_cornersize;
	float		d_borderthickness;
	t_ergba	d_bordercolor;
	t_ergba	d_boxfillcolor;
    
} t_edrawparams;

typedef struct _epopupitem
{
    t_ergba c_color;
    long        c_id;
    t_symbol*   c_text;
    bool        c_checked;
    bool        c_disable;
    bool        c_separator;
}t_epopupitem;

typedef struct _epopupmenu
{
    t_object*               c_obj;
    vector<t_epopupitem>    c_items;
    t_efont                 c_font;
    t_rect                  c_rect;
    t_symbol*               c_name;
    
}t_epopupmenu;

typedef struct _ebox
{
    t_object            e_obj;
    t_glist*            e_glist;
    t_rect              e_rect;
    bool                e_selected;
    t_pt                e_mouse;
    bool                e_mouse_down;
    long                e_modifiers;
    bool                e_ready_to_draw;
    t_symbol*           e_box_text;
    char*               e_classname;
    t_edrawparams       e_boxparameters;
    
    t_egraphics*        e_graphics;
    long                e_number_of_graphics;
    
    t_efont             e_font;
    t_epopupmenu*       e_popup = NULL;
    
    vector<t_inlet*>    e_inlets;
    vector<t_outlet*>   e_outlets;
    long                e_nins;
    long                e_nouts;
    
    float               e_float;                // float value for signal
    long                e_dsp_size;
    t_int*              e_dsp_vectors;
    long                e_dsp_flag;
    void*               e_dsp_user_param;
    long                z_misc = 0;     // Special Max for Z_NO_INPLACE
    t_float**           z_sigs_out = NULL;
    method              e_perform_method;
    
    bool                e_no_redraw_box = 0;
    t_object*           b_firstin; // For Max, it doesn't matter !
}t_ebox;





#endif 