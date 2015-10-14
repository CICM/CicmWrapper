/*
 * CicmWrapper
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "eview.h"
#include "eobj.h"
#include "ebox.h"
#include "egui.h"
#include "egraphics.h"

#include <m_imp.h>

struct _eview
{
    t_object        v_object;       /*!< The object. */
    t_object*       v_owner;        /*!< The owner. */
    t_symbol*       v_tag;          /*!< The widget tag. */
    
    t_canvas*       v_canvas;       /*!< The canvas of the view. */
    t_rect          v_bounds;       /*!< The bounds of the view. */
    t_rect          v_last;         /*!< The last bounds of the view. */
    char            v_mousedown;    /*!< The mouse down status. */
    char            v_item;         /*!< The selected item. */
    t_elayer**      v_layers;       /*!< The layers. */
    size_t          v_nlayers;      /*!< The number of layers. */
    
    t_mouse_method  v_mouse_enter;
    t_mouse_method  v_mouse_leave;
    t_mouse_method  v_mouse_down;
    t_mouse_method  v_mouse_up;
    t_mouse_method  v_mouse_move;
    t_mouse_method  v_mouse_drag;
    t_mouse_method  v_mouse_dblclick;
    t_mousewheel_method v_mouse_wheel;
    t_key_method    v_key_press;
    t_key_method    v_key_filter;
};

typedef enum view_items
{
    VITEM_NONE    = 0,
    VITEM_OBJ     = 1,
    VITEM_BOTTOM  = 2,
    VITEM_CORNER  = 3,
    VITEM_RIGHT   = 4
} view_items;

static long view_getmodifier(t_float mod)
{
#ifdef __APPLE__
    if(mod >= 256)
    {
        mod -= 256;
    }
#elif _WINDOWS
    
    if(mod >= 131072)
    {
        mod -= 131072;
        mod += EMOD_ALT;
    }
#else
    if (mod == 24)
        mod = EMOD_CMD;
    else if (mod & EMOD_CMD)
    {
        mod ^= EMOD_CMD;
        mod |= EMOD_ALT;
    }
#endif
    return (long)mod;
}

inline static char view_isrunmode(t_eview const* view, long mod)
{
    return !view->v_canvas->gl_edit || mod == EMOD_CMD;
}

inline static char view_isinsubcanvas(t_eview const* view)
{
    return view->v_canvas != eobj_getcanvas(view->v_owner);
}

inline static char view_ignoreclick(t_eview const* view)
{
    int todo;
    return (char)(ebox_getflags((t_ebox *)view->v_owner) & EBOX_IGNORELOCKCLICK); //&& !view->v_owner->b_ignore_click;
}

static void view_mouseenter(t_eview* view, t_float x, t_float y, t_float modifier)
{
    t_pt const pt = {x, y};
    long const mod = view_getmodifier(modifier);
    if(view_isrunmode(view, mod) && view->v_mouse_enter && !view->v_mousedown)
    {
        eview_setcursor(view, ECURSOR_INTERACTION);
        view->v_mouse_enter(view->v_owner, (t_object *)view, pt, mod);
    }
}

static void view_mouseleave(t_eview* view, t_float x, t_float y, t_float modifier)
{
    t_pt const pt = {x, y};
    long const mod = view_getmodifier(modifier);
    if(view_isrunmode(view, mod) && !view->v_mousedown)
    {
        if(view->v_mouse_leave)
        {
            view->v_mouse_leave(view->v_owner, (t_object *)view, pt, mod);
        }
        eview_setcursor(view, ECURSOR_DEFAULT);
    }
    else if(!view_isrunmode(view, mod) && !view->v_mousedown)
    {
        eview_setcursor(view, ECURSOR_SELECTION);
    }
}

static void view_mousedown(t_eview* view, t_float x, t_float y, t_float modifier)
{
    t_pt const pt = {x, y};
    long const mod = view_getmodifier(modifier);
    if(view_isrunmode(view, mod))
    {
        if(view->v_mouse_down && !view_ignoreclick(view))
        {
            view->v_mouse_down(view->v_owner, (t_object *)view, pt, mod);
        }
    }
    else
    {
        if(view->v_item == VITEM_NONE)
        {
            if(mod == EMOD_SHIFT)
            {
                sys_vgui("pdtk_canvas_mouse .x%lx.c [expr [winfo pointerx .] - [winfo rootx .x%lx.c]] [expr [winfo pointery .] - [winfo rooty .x%lx.c]] 0 1\n", (unsigned long)view->v_canvas, (unsigned long)view->v_canvas, (unsigned long)view->v_canvas);
            }
            else if(mod == EMOD_RIGHT)
            {
                sys_vgui("pdtk_canvas_rightclick .x%lx.c [expr [winfo pointerx .] - [winfo rootx .x%lx.c]] [expr [winfo pointery .] - [winfo rooty .x%lx.c]] 0\n", (unsigned long)view->v_canvas, (unsigned long)view->v_canvas, (unsigned long)view->v_canvas);
            }
            else
            {
                sys_vgui("pdtk_canvas_mouse .x%lx.c [expr [winfo pointerx .] - [winfo rootx .x%lx.c]] [expr [winfo pointery .] - [winfo rooty .x%lx.c]] 0 0\n", (unsigned long)view->v_canvas, (unsigned long)view->v_canvas, (unsigned long)view->v_canvas);
            }
        }
        else
        {
            view->v_last = view->v_bounds;
        }
    }
    view->v_mousedown = 1;
}

static void view_mouseup(t_eview* view, t_float x, t_float y, t_float modifier)
{
    t_pt const pt = {x, y};
    long const mod = view_getmodifier(modifier);
    if(view_isrunmode(view, mod))
    {
        if(view->v_mouse_up && !view_ignoreclick(view))
        {
            view->v_mouse_up(view->v_owner, (t_object *)view, pt, mod);
        }
    }
    else
    {
        sys_vgui("pdtk_canvas_mouseup .x%lx.c [expr [winfo pointerx .] - [winfo rootx .x%lx.c]] [expr [winfo pointery .] - [winfo rooty .x%lx.c]] 1\n", (unsigned long)view->v_canvas, (unsigned long)view->v_canvas, (unsigned long)view->v_canvas);
    }
    view->v_mousedown = 0;
}

static void view_mousemove(t_eview* view, t_float x, t_float y, t_float modifier)
{
    int i; char outlet; float iopos;
    t_pt const pt = {x, y};
    char const bottom = pt.y > view->v_bounds.height - 3.f;
    char const right  = pt.x > view->v_bounds.width - 3.f;
    long const mod = view_getmodifier(modifier);
    int const noutlet = obj_noutlets((t_object *)view->v_owner);

    if(view_isrunmode(view, mod))
    {
        if(view_ignoreclick(view))
        {
            eview_setcursor(view, ECURSOR_DEFAULT);
        }
        else
        {
            eview_setcursor(view, ECURSOR_INTERACTION);
            if(view->v_mouse_move)
            {
                view->v_mouse_move(view->v_owner, (t_object *)view, pt, mod);
            }
        }
    }
    else
    {
        view->v_item = VITEM_NONE;
        if(!view_isinsubcanvas(view) && pt.x >= 0.f && pt.x <= view->v_bounds.width && pt.y >= 0.f && pt.y <= view->v_bounds.height)
        {
            if(right && bottom)
            {
                view->v_item = VITEM_CORNER;
                eview_setcursor(view, ECURSOR_BOTTOM_RIGHT);
            }
            else if(bottom)
            {
                outlet = 0;
                if(noutlet)
                {
                    if(pt.y < 3.f)
                    {
                        if(pt.x <= 7.f)
                        {
                            outlet = 1;
                        }
                        else
                        {
                            for(i = 1; i < noutlet; i++)
                            {
                                iopos = (int)(i / (float)(noutlet - 1) * (view->v_bounds.width));
                                if(pt.x >= iopos && pt.x <= iopos + 7.f)
                                {
                                    outlet = 1;
                                    break;
                                }
                            }
                        }
                    }
                }
                if(!outlet)
                {
                    view->v_item = VITEM_BOTTOM;
                    eview_setcursor(view, ECURSOR_BOTTOM);
                }
                else
                {
                    view->v_item = VITEM_OBJ;
                    eview_setcursor(view, ECURSOR_OUTLET);
                }
            }
            else if(right)
            {
                view->v_item = VITEM_RIGHT;
                eview_setcursor(view, ECURSOR_RIGHT);
            }
            else
            {
                eview_setcursor(view, ECURSOR_SELECTION);
            }
        }
        sys_vgui("pdtk_canvas_motion .x%lx.c [expr [winfo pointerx .] - [winfo rootx .x%lx.c]] [expr [winfo pointery .] - [winfo rooty .x%lx.c]] 0\n", (unsigned long)view->v_canvas, (unsigned long)view->v_canvas, (unsigned long)view->v_canvas);
    }
}

static void view_mousedrag(t_eview* view, t_float x, t_float y, t_float modifier)
{
    t_atom av[2];
    t_pt const pt = {x, y};
    long const mod = view_getmodifier(modifier);

    if(view_isrunmode(view, mod))
    {
        if(view->v_mouse_drag && !view_ignoreclick(view))
        {
            view->v_mouse_drag(view->v_owner, (t_object *)view, pt, mod);
        }
    }
    else if(!view_isinsubcanvas(view))
    {
        if(view->v_item == VITEM_NONE)
        {
            sys_vgui("pdtk_canvas_motion .x%lx.c [expr [winfo pointerx .] - [winfo rootx .x%lx.c]] [expr [winfo pointery .] - [winfo rooty .x%lx.c]] 0\n", (unsigned long)view->v_canvas, (unsigned long)view->v_canvas, (unsigned long)view->v_canvas);
        }
        else if(!(ebox_getflags((t_ebox *)view->v_owner) & EBOX_GROWNO))
        {
            if(ebox_getflags((t_ebox *)view->v_owner) & EBOX_GROWLINK)
            {
                if(view->v_item == VITEM_BOTTOM)
                {
                    atom_setfloat(av, view->v_last.width + (pt.y - view->v_last.height));
                    atom_setfloat(av+1, pt.y);
                }
                else if(view->v_item == VITEM_RIGHT)
                {
                    atom_setfloat(av, pt.x);
                    atom_setfloat(av+1, view->v_last.height + (pt.x - view->v_last.width));
                }
                else if(view->v_item == VITEM_CORNER)
                {
                    if(pt.y > pt.x)
                    {
                        atom_setfloat(av, pt.y);
                        atom_setfloat(av+1, pt.y);
                    }
                    else
                    {
                        atom_setfloat(av, pt.x);
                        atom_setfloat(av+1, pt.x);
                    }
                }
            }
            else if(ebox_getflags((t_ebox *)view->v_owner) & EBOX_GROWINDI)
            {
                if(view->v_item == VITEM_BOTTOM)
                {
                    atom_setfloat(av,view->v_last.width);
                    atom_setfloat(av+1, pt.y);
                }
                else if(view->v_item == VITEM_RIGHT)
                {
                    atom_setfloat(av, pt.x);
                    atom_setfloat(av+1,view->v_last.height);
                }
                else if(view->v_item == VITEM_CORNER)
                {
                    atom_setfloat(av, pt.x);
                    atom_setfloat(av+1, pt.y);
                }
            }
            pd_typedmess((t_pd *)view->v_owner, s_cream_size, 2, av);
        }
    }
    else
    {
        sys_vgui("pdtk_canvas_motion .x%lx.c [expr [winfo pointerx .] - [winfo rootx .x%lx.c]] [expr [winfo pointery .] - [winfo rooty .x%lx.c]] 1\n", (unsigned long)view->v_canvas, (unsigned long)view->v_canvas, (unsigned long)view->v_canvas);
    }
}

static void view_mousemotion(t_eview* view, t_float x, t_float y, t_float modifier)
{
    if(!view->v_mousedown)
    {
        view_mousemove(view, x, y, modifier);
    }
    else
    {
        view_mousedrag(view, x, y, modifier);
    }
}

static void view_mousedblclick(t_eview* view, t_symbol* s, int argc, t_atom *argv)
{
    t_pt const pt = {atom_getfloatarg(0, argc, argv), atom_getfloatarg(1, argc, argv)};
    long const mod = view_getmodifier(atom_getfloatarg(2, argc, argv));
    if(view->v_mouse_dblclick && !view_ignoreclick(view))
    {
        if(view_isrunmode(view, mod) && !(ebox_getflags((t_ebox *)view->v_owner) & EBOX_DBLCLICK_EDIT))
        {
            view->v_mouse_dblclick(view->v_owner, (t_object *)view, pt, mod);
        }
        else if(!view_isrunmode(view, mod) && (ebox_getflags((t_ebox *)view->v_owner) & EBOX_DBLCLICK_EDIT))
        {
            sys_vgui("pdtk_canvas_mouse .x%lx.c [expr [winfo pointerx .] - [winfo rootx .x%lx.c]] [expr [winfo pointery .] - [winfo rooty .x%lx.c]] 0 1\n", (unsigned long)view->v_canvas, (unsigned long)view->v_canvas, (unsigned long)view->v_canvas);
            sys_vgui("pdtk_canvas_mouseup .x%lx.c [expr [winfo pointerx .] - [winfo rootx .x%lx.c]] [expr [winfo pointery .] - [winfo rooty .x%lx.c]] 0\n", (unsigned long)view->v_canvas, (unsigned long)view->v_canvas, (unsigned long)view->v_canvas);            
            view->v_mouse_dblclick(view->v_owner, (t_object *)view, pt, mod);
        }
    }
}

static void view_mousewheel(t_eview* view, t_symbol* s, int argc, t_atom *argv)
{
    t_pt const pt = {atom_getfloatarg(0, argc, argv), atom_getfloatarg(1, argc, argv)};
    float const delta = atom_getfloatarg(2, argc, argv);
    long const mod = view_getmodifier(atom_getfloatarg(3, argc, argv));
    if(view_isrunmode(view, mod) && view->v_mouse_wheel && !view_ignoreclick(view))
    {
        view->v_mouse_wheel(view->v_owner, (t_object *)view, pt, mod, delta, delta);
    }
}

static void view_key(t_eview* view, t_float key, t_float modifier)
{
    long const kchar = (char)key;
    long const mod = view_getmodifier(modifier);
    if(view_isrunmode(view, mod))
    {
        if(kchar == 65288)
        {
            if(view->v_key_filter)
            {
                view->v_key_filter(view->v_owner, (t_object *)view, EKEY_DEL, mod);
            }
            else if(view->v_key_press)
            {
                view->v_key_press(view->v_owner, (t_object *)view, EKEY_DEL, mod);
            }
        }
        else if(kchar == 65289)
        {
            if(view->v_key_filter)
            {
                view->v_key_filter(view->v_owner, (t_object *)view, EKEY_TAB, mod);
            }
            else if(view->v_key_press)
            {
                view->v_key_press(view->v_owner, (t_object *)view, EKEY_TAB, mod);
            }
        }
        else if(kchar == 65293)
        {
            if(view->v_key_filter)
            {
                view->v_key_filter(view->v_owner, (t_object *)view, EKEY_RETURN, mod);
            }
            else if(view->v_key_press)
            {
                view->v_key_press(view->v_owner, (t_object *)view, EKEY_RETURN, mod);
            }
        }
        else if(kchar == 65307)
        {
            if(view->v_key_filter)
            {
                view->v_key_filter(view->v_owner, (t_object *)view, EKEY_ESC, mod);
            }
            else if(view->v_key_press)
            {
                view->v_key_press(view->v_owner, (t_object *)view, EKEY_ESC, mod);
            }
        }
        else
        {
            if(view->v_key_press)
            {
                view->v_key_press(view->v_owner, (t_object *)view, (char)kchar, mod);
            }
        }
    }
}

static void eview_free(t_eview* view)
{
    pd_unbind((t_pd *)view, view->v_tag);
    sys_vgui("destroy .x%lx.c.canvas%lx\n", (unsigned long)view->v_canvas, (unsigned long)view);
    canvas_deletelinesfor(view->v_canvas, (t_text *)view->v_owner);
}

static t_class* eview_setup()
{
    t_class* eclass = NULL;
    t_pd* obj = gensym("eview1572")->s_thing;
    if(!obj)
    {
        eclass = class_new(gensym("eview"), NULL, (t_method)eview_free, sizeof(t_eview), CLASS_PD, A_GIMME, 0);
        
        class_addmethod(eclass, (t_method)view_mouseenter,      gensym("mouseenter"),   A_FLOAT, A_FLOAT, A_FLOAT);
        class_addmethod(eclass, (t_method)view_mouseleave,      gensym("mouseleave"),   A_FLOAT, A_FLOAT, A_FLOAT);
        class_addmethod(eclass, (t_method)view_mousedown,       gensym("mousedown"),    A_FLOAT, A_FLOAT, A_FLOAT);
        class_addmethod(eclass, (t_method)view_mouseup,         gensym("mouseup"),      A_FLOAT, A_FLOAT, A_FLOAT);
        class_addmethod(eclass, (t_method)view_mousemotion,     gensym("mousemotion"),  A_FLOAT, A_FLOAT, A_FLOAT);
        class_addmethod(eclass, (t_method)view_mousewheel,      gensym("mousewheel"),   A_GIMME);
        class_addmethod(eclass, (t_method)view_mousedblclick,   gensym("dblclick"),     A_GIMME);
        class_addmethod(eclass, (t_method)view_key,             gensym("key"),          A_FLOAT, A_FLOAT);
        
        obj = pd_new(eclass);
        pd_bind(obj, gensym("eview1572"));
        return eclass;
    }
    else
    {
        return *obj;
    }
}

t_eview* eview_findbyname(t_symbol* name)
{
    t_eview* x = NULL;
    t_class*  c = eview_setup();
    if(c)
    {
        x = (t_eview *)pd_findbyclass(name, c);
    }
    return x;
}

t_eview* eview_new(t_object* x, t_canvas* cnv, t_rect const* bounds)
{
    char buffer[MAXPDSTRING];
    t_eview* v = NULL;
    t_class* c = eview_setup();
    if(c)
    {
        v = (t_eview *)pd_new(c);
        if(v)
        {
            v->v_canvas         = glist_getcanvas(cnv);
            v->v_owner          = x;
            v->v_bounds.x       = bounds->x;
            v->v_bounds.y       = bounds->y;
            v->v_bounds.width   = bounds->width;
            v->v_bounds.height  = bounds->height;
            v->v_mousedown      = 0;
            v->v_item           = VITEM_NONE;
            v->v_nlayers        = 0;
            v->v_layers         = NULL;
            
            v->v_mouse_enter    = (t_mouse_method)zgetfn((t_pd *)x, gensym("mouseenter"));
            v->v_mouse_leave    = (t_mouse_method)zgetfn((t_pd *)x, gensym("mouseleave"));
            v->v_mouse_down     = (t_mouse_method)zgetfn((t_pd *)x, gensym("mousedown"));
            v->v_mouse_up       = (t_mouse_method)zgetfn((t_pd *)x, gensym("mouseup"));
            v->v_mouse_move     = (t_mouse_method)zgetfn((t_pd *)x, gensym("mousemove"));
            v->v_mouse_drag     = (t_mouse_method)zgetfn((t_pd *)x, gensym("mousedrag"));
            v->v_mouse_dblclick = (t_mouse_method)zgetfn((t_pd *)x, gensym("dblclick"));
            v->v_mouse_wheel    = (t_mousewheel_method)zgetfn((t_pd *)x, gensym("mousewheel"));
            v->v_key_press      = (t_key_method)zgetfn((t_pd *)x, gensym("key"));
            v->v_key_filter     = (t_key_method)zgetfn((t_pd *)x, gensym("keyfilter"));
            
            sprintf(buffer, "tag%lx", (unsigned long)v);
            v->v_tag = gensym(buffer);
            pd_bind((t_pd *)v, v->v_tag);
            
            sys_vgui("canvas .x%lx.c.canvas%lx -width %i -height %i -bd 0 -takefocus 1 -bg grey\n",
                     (unsigned long)v->v_canvas, (unsigned long)v, (int)v->v_bounds.width, (int)v->v_bounds.height);
            
            sys_vgui("bind .x%lx.c.canvas%lx <Enter> {+pdsend {%s mouseenter %%x %%y %%s}}\n",
                     (unsigned long)v->v_canvas, (unsigned long)v, v->v_tag->s_name);
            sys_vgui("bind .x%lx.c.canvas%lx <Leave> {+pdsend {%s mouseleave %%x %%y %%s}}\n",
                     (unsigned long)v->v_canvas, (unsigned long)v, v->v_tag->s_name);
            sys_vgui("bind .x%lx.c.canvas%lx <Button-3> {+pdsend {%s mousedown %%x %%y %i}}\n",
                     (unsigned long)v->v_canvas, (unsigned long)v, v->v_tag->s_name, EMOD_RIGHT);
            sys_vgui("bind .x%lx.c.canvas%lx <Button-2> {+pdsend {%s mousedown %%x %%y %i}}\n",
                     (unsigned long)v->v_canvas, (unsigned long)v, v->v_tag->s_name, EMOD_RIGHT);
            sys_vgui("bind .x%lx.c.canvas%lx <Button-1> {+pdsend {%s mousedown %%x %%y %%s}}\n",
                     (unsigned long)v->v_canvas, (unsigned long)v, v->v_tag->s_name);
            sys_vgui("bind .x%lx.c.canvas%lx <ButtonRelease> {+pdsend {%s mouseup %%x %%y %%s}}\n",
                     (unsigned long)v->v_canvas, (unsigned long)v, v->v_tag->s_name);
            sys_vgui("bind .x%lx.c.canvas%lx <Motion> {+pdsend {%s mousemotion %%x %%y %%s}}\n",
                     (unsigned long)v->v_canvas, (unsigned long)v, v->v_tag->s_name);
            
            if(v->v_mouse_dblclick)
            {
                sys_vgui("bind .x%lx.c.canvas%lx <Double-Button-1> {+pdsend {%s dblclick %%x %%y %%s}}\n",
                         (unsigned long)v->v_canvas, (unsigned long)v, v->v_tag->s_name);
            }
            if(v->v_mouse_wheel)
            {
                sys_vgui("bind .x%lx.c.canvas%lx <MouseWheel> {+pdsend {%s mousewheel  %%x %%y %%D %%s}}\n",
                         (unsigned long)v->v_canvas, (unsigned long)v, v->v_tag->s_name);
            }
            if(v->v_key_press || v->v_key_filter)
            {
                sys_vgui("bind .x%lx.c.canvas%lx <KeyPress>  {+pdsend {%s key %%N %%s}} \n",
                         (unsigned long)v->v_canvas, (unsigned long)v, v->v_tag->s_name);
            }
            
            int todo_position;
            sys_vgui(".x%lx.c create window %i %i -anchor nw -window .x%lx.c.canvas%lx -tags win%lx -width %i -height %i\n",
                     (unsigned long)v->v_canvas,
                     (int)v->v_bounds.x, (int)v->v_bounds.y,
                     (unsigned long)v->v_canvas, (unsigned long)v, (unsigned long)v,
                     (int)v->v_bounds.width, (int)v->v_bounds.height);
        }
    }
    return v;
}

t_canvas* eview_getcanvas(t_eview const* view)
{
    return view->v_canvas;
}

void eview_getposition(t_eview const* view, t_pt* pos)
{
    pos->x = view->v_bounds.x;
    pos->y = view->v_bounds.y;
}

void eview_getsize(t_eview const* view, t_pt* size)
{
    size->x = view->v_bounds.width;
    size->y = view->v_bounds.height;
}

void eview_getbounds(t_eview const* view, t_rect* bounds)
{
    bounds->x = view->v_bounds.x;
    bounds->y = view->v_bounds.y;
    bounds->width = view->v_bounds.width;
    bounds->height = view->v_bounds.height;
}

void eview_setposition(t_eview* view, t_pt const* pos)
{
    view->v_bounds.x = pos->x;
    view->v_bounds.y = pos->y;
    sys_vgui(".x%lx.c coords win%lx %d %d\n", (unsigned long)view->v_canvas, (unsigned long)view,
             (int)view->v_bounds.x, (int)view->v_bounds.y);
    
    canvas_fixlinesfor(view->v_canvas, (t_text*)view->v_owner);
}

void eview_setsize(t_eview* view, t_pt const* size)
{
    view->v_bounds.width    = size->x;
    view->v_bounds.height   = size->y;
    sys_vgui(".x%lx.c itemconfigure win%lx -width %d -height %d\n", (unsigned long)view->v_canvas, (unsigned long)view,
             (int)view->v_bounds.width, (int)view->v_bounds.height);
    
    canvas_fixlinesfor(view->v_canvas, (t_text*)view->v_owner);
}

void eview_setbounds(t_eview* view, t_rect const* bounds)
{
    view->v_bounds.x = bounds->x;
    view->v_bounds.y = bounds->y;
    view->v_bounds.width    = bounds->width;
    view->v_bounds.height   = bounds->height;
    sys_vgui(".x%lx.c coords win%lx %d %d\n", (unsigned long)view->v_canvas, (unsigned long)view,
             (int)view->v_bounds.x, (int)view->v_bounds.y);
    sys_vgui(".x%lx.c itemconfigure win%lx -width %d -height %d\n", (unsigned long)view->v_canvas, (unsigned long)view,
             (int)view->v_bounds.width, (int)view->v_bounds.height);
    
    canvas_fixlinesfor(view->v_canvas, (t_text*)view->v_owner);
}

static char *my_cursorlist[] =
{
    "left_ptr",
    "center_ptr",
    "sb_v_double_arrow",
    "plus",
    "hand2",
    "circle",
    "X_cursor",
    "bottom_side",
    "bottom_right_corner",
    "right_side",
    "double_arrow",
    "exchange",
    "xterm"
};

void eview_setcursor(t_eview* view, ebox_cursors cursor)
{
    sys_vgui(".x%lx.c.canvas%lx configure -cursor %s\n",
             (unsigned long)view->v_canvas, (unsigned long)view, my_cursorlist[cursor]);
}

void eview_layers_update(t_eview* view)
{
    size_t i;
    for(i = 0; view->v_nlayers; i++)
    {
        if(egraphics_get_state(view->v_layers[i]) == EPD_LAYER_INVALID)
        {
            int todo;
            //sys_vgui(".x%lx.c.canvas%lx delete %s\n", (unsigned long)view->v_canvas, (unsigned long)view, x->b_layers[i].e_id->s_name);
            
        }
    }
}

void eview_layer_start(t_eview* view, t_symbol *name, float width, float height)
{
    size_t i;
    for(i = 0; view->v_nlayers; i++)
    {
        int todo;
    }
}







