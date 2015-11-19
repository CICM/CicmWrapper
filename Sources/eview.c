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
#include "elayer.h"
#include "eguiimp.h"
#include "eguicontext.h"

#include <m_imp.h>

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
    return (char)(ebox_getflags((t_ebox *)view->v_owner) & EBOX_IGNORELOCKCLICK) && !ebox_ignoreclick((t_ebox *)view->v_owner);
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
#ifdef CICM_WRAPPER_TCL
        if(view->v_item == VITEM_NONE)
        {
            eguicontext_canvas_mousedown(eguicontext_get(), view->v_canvas, mod);
        }
#endif
        view->v_last = view->v_bounds;
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
#ifdef CICM_WRAPPER_TCL
        eguicontext_canvas_mouseup(eguicontext_get(), view->v_canvas, mod);
#endif
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
                if(!outlet)
                {
                    view->v_item = VITEM_BOTTOM;
                    eview_setcursor(view, ECURSOR_BOTTOM);
                }
                else
                {
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
    size_t i;
    for(i = 0; i < view->v_nlayers; i++)
    {
        elayer_free(view->v_layers[i]);
    }
    if(view->v_nlayers && view->v_layers)
    {
        free(view->v_layers);
    }
    view->v_layers  = NULL;
    view->v_nlayers = 0;
    pd_unbind((t_pd *)view, view->v_tag);
    eguicontext_view_remove(eguicontext_get(), view);
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
            v->v_params.d_borderthickness = 1.f;
            v->v_params.d_cornersize      = 0.f;
            v->v_params.d_bordercolor     = rgba_black;
            v->v_params.d_boxfillcolor    = rgba_white;
            
            v->v_bounds.x       = bounds->x;
            v->v_bounds.y       = bounds->y;
            v->v_bounds.width   = bounds->width;
            v->v_bounds.height  = bounds->height;
            v->v_mousedown      = 0;
            v->v_item           = VITEM_NONE;
            v->v_nlayers        = 0;
            v->v_layers         = NULL;
            v->v_selected       = 0;
            
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
            v->v_paint          = (t_paint_method)zgetfn((t_pd *)x, gensym("paint"));
            v->v_drawparams     = (t_drawparameter_method)zgetfn((t_pd *)x, gensym("getdrawparams"));
            
            sprintf(buffer, "tag%lx", (unsigned long)v);
            v->v_tag = gensym(buffer);
            pd_bind((t_pd *)v, v->v_tag);
            if(v->v_drawparams)
            {
                v->v_drawparams(v->v_owner, (t_object *)v, &v->v_params);
            }
            eguicontext_view_add(eguicontext_get(), v);
        }
        else
        {
            pd_error(x, "can't allocate a new view.");
        }
    }
    else
    {
        pd_error(x, "can't find the view class.");
    }
    return v;
}

t_canvas* eview_getcanvas(t_eview const* view)
{
    return view->v_canvas;
}

void eview_getdrawparams(t_eview const* view, t_edrawparams* params)
{
    memcpy(params, &view->v_params, sizeof(t_edrawparams));
}

void eview_getdrawbounds(t_eview const* view, t_rect* bounds)
{
    bounds->x = view->v_bounds.x + view->v_params.d_borderthickness;
    bounds->y = view->v_bounds.y + view->v_params.d_borderthickness;
    bounds->width = view->v_bounds.width -  2.f * view->v_params.d_borderthickness;
    bounds->height = view->v_bounds.height -  2.f * view->v_params.d_borderthickness;
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
    eguicontext_view_boundschanged(eguicontext_get(), view);
}

void eview_setsize(t_eview* view, t_pt const* size)
{
    view->v_bounds.width    = size->x;
    view->v_bounds.height   = size->y;
    eguicontext_view_boundschanged(eguicontext_get(), view);
    eview_layer_invalidate(view, NULL);
    eview_draw(view);
}

void eview_setbounds(t_eview* view, t_rect const* bounds)
{
    view->v_bounds.x = bounds->x;
    view->v_bounds.y = bounds->y;
    view->v_bounds.width    = bounds->width;
    view->v_bounds.height   = bounds->height;
    eguicontext_view_boundschanged(eguicontext_get(), view);
    eview_layer_invalidate(view, NULL);
    eview_draw(view);
}

void eview_select(t_eview* view)
{
    if(!view->v_selected)
    {
        view->v_selected = 1;
        int todo;
        eguicontext_view_boundschanged(eguicontext_get(), view);
    }
}

void eview_deselect(t_eview* view)
{
    if(view->v_selected)
    {
        view->v_selected = 0;
        int todo;
        eguicontext_view_boundschanged(eguicontext_get(), view);
    }
}

void eview_setcursor(t_eview* view, ecursor_types cursor)
{
    eguicontext_view_setcursor(eguicontext_get(), view, cursor);
}

static t_elayer* eview_getlayer(t_eview const* view, t_symbol const* name)
{
    size_t i;
    for(i = 0; i < view->v_nlayers; i++)
    {
        if(elayer_getname(view->v_layers[i]) == name)
        {
            return view->v_layers[i];
        }
    }
    return NULL;
}

void eview_draw(t_eview* view)
{
    if(view->v_drawparams)
    {
        view->v_drawparams(view->v_owner, (t_object *)view, &view->v_params);
    }
    if(view->v_paint)
    {
        (view->v_paint)(view->v_owner, (t_object *)view);
    }
}

t_elayer* eview_layer_start(t_eview* view, t_symbol *name, float width, float height)
{
    t_elayer** temp = NULL;
    t_elayer* l = eview_getlayer(view, name);
    if(!l)
    {
        l = elayer_new(name, width, height);
        if(l)
        {
            if(view->v_nlayers && view->v_layers)
            {
                temp = (t_elayer **)realloc(view->v_layers, (view->v_nlayers + 1) * sizeof(t_elayer *));
                if(temp)
                {
                    view->v_layers                  = temp;
                    view->v_layers[view->v_nlayers] = l;
                    view->v_nlayers                 = view->v_nlayers + 1;
                    return l;
                }
                else
                {
                    pd_error(view, "can't increase the number of layers.");
                    elayer_free(l);
                    return NULL;
                }
            }
            else
            {
                view->v_layers = (t_elayer **)malloc(sizeof(t_elayer *));
                if(view->v_layers)
                {
                    view->v_nlayers     = 1;
                    view->v_layers[0]   = l;
                    return l;
                }
                else
                {
                    pd_error(view, "can't allocate any layer.");
                    elayer_free(l);
                    return NULL;
                }
            }
        }
        return NULL;
    }
    return elayer_reopen(l);
}

t_pd_err eview_layer_invalidate(t_eview* view, t_symbol *name)
{
    size_t i = 0;
    t_pd_err err = 0;
    t_elayer* l = NULL;
    if(name)
    {
        l = eview_getlayer(view, name);
        if(l)
        {
            return elayer_invalidate(l);
        }
        return -1;
    }
    else
    {
        for(i = 0; i < view->v_nlayers; i++)
        {
            err = (elayer_invalidate(view->v_layers[i]) != 0) ? -1 : err;
        }
        return err;
    }
}

t_pd_err eview_layer_end(t_eview* view, t_symbol *name)
{
    t_elayer* l = eview_getlayer(view, name);
    if(l)
    {
        return elayer_end(l);
    }
    return -1;
}

t_pd_err eview_layer_paint(t_eview* view, t_symbol *name, const float xoffset, const float yoffset)
{
    t_elayer* l = eview_getlayer(view, name);
    if(l)
    {
        if(!eguicontext_view_paint_layer(eguicontext_get(), view, l,
                                     xoffset + view->v_params.d_borderthickness,
                                     yoffset + view->v_params.d_borderthickness))
        {
            return elayer_painted(l);
        }
        return -1;
    }
    return -1;
}













