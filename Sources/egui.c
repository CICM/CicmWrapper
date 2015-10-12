/*
 * CicmWrapper
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "egui.h"
#include "ecommon.h"
#include "egraphics.h"
#include "eobj.h"

struct _egui
{
    t_object        g_object;           /*!< The object. */
    t_object*       g_owner;            /*!< The gui owner. */
    long            g_flags;            /*!< The gui flags. */
    t_object**      g_views;            /*!< The gui view. */
    float           g_size[2];          /*!< The gui size. */
    int             g_nviews;           /*!< The gui number of views. */
    t_symbol*       g_receive;          /*!< The reveive symbol (attribute). */
    t_symbol*       g_send;             /*!< The send send (attribute). */
    char            g_pinned;           /*!< The pinned state (attribute). */
    char            g_ignore_click;     /*!< The igore click state (attribute). */
    char            g_visible;          /*!< The visible state (attribute). */
    t_edrawparams   g_boxparameters;    /*!< The gui parameters. */
};

static void egui_free(t_egui *gui)
{
    if(is_valid_symbol(gui->g_receive))
    {
        pd_unbind((t_pd *)gui->g_owner, gui->g_receive);
    }
}

static void egui_setreceive(t_egui *gui, t_symbol* s)
{
    if(is_valid_symbol(gui->g_receive))
    {
        pd_unbind((t_pd *)gui->g_owner, canvas_realizedollar(eobj_getcanvas(gui->g_owner), gui->g_receive));
    }
    gui->g_receive = s;
    if(is_valid_symbol(gui->g_receive))
    {
        pd_bind((t_pd *)gui->g_owner, canvas_realizedollar(eobj_getcanvas(gui->g_owner), gui->g_receive));
    }
}

static void egui_setsend(t_egui *gui, t_symbol* s)
{
    gui->g_send = s;
}

static void egui_setsize(t_egui *gui, float width, float height)
{
    if(gui->g_flags & EBOX_GROWLINK)
    {
        width  = pd_clip_min(width, 4);
        height = gui->g_size[1];
        gui->g_size[1] += (width - gui->g_size[0]);
        if(gui->g_size[1] < 4)
        {
            gui->g_size[0] += 4 - height;
            gui->g_size[1] = 4;
        }
        else
        {
            gui->g_size[0] =  width;
        }
    }
    else if(gui->g_flags & EBOX_GROWINDI)
    {
        gui->g_size[0] = pd_clip_min(width, 4.f);
        gui->g_size[1] = pd_clip_min(height, 4.f);
    }
}

static void egui_setignoreclick(t_egui *gui, float f)
{
    gui->g_ignore_click = (f == 0.f) ? 0 : 1;
}

static t_class* egui_setup()
{
    t_class* c = NULL;
    t_pd* obj = gensym("egui1572")->s_thing;
    if(!obj)
    {
        c = class_new(gensym("egui"), (t_newmethod)NULL, (t_method)egui_free, sizeof(t_egui), CLASS_PD, A_NULL, 0);
        if(c)
        {
            class_addmethod(c, (t_method)egui_setreceive,       gensym("receive"),      A_SYMBOL, 0);
            class_addmethod(c, (t_method)egui_setsend,          gensym("send"),         A_SYMBOL, 0);
            class_addmethod(c, (t_method)egui_setsize,          gensym("size"),         A_FLOAT, A_FLOAT, 0);
            class_addmethod(c, (t_method)egui_setignoreclick,   gensym("ignoreclick"),  A_FLOAT, 0);
            obj = pd_new(c);
            pd_bind(obj, gensym("egui1572"));
        }
        else
        {
            error("can't initialize flags class.");
        }
        return c;
    }
    else
    {
        return *obj;
    }
}

t_egui* egui_new(t_object* owner, long flags)
{
    t_egui*  x = NULL;
    t_class* c = egui_setup();
    if(c)
    {
        x = (t_egui *)pd_new(c);
        if(x)
        {
            x->g_owner              = owner;
            x->g_flags              = flags;
            x->g_receive            = s_cream_empty;
            x->g_send               = s_cream_empty;
            x->g_visible            = 1;
            x->g_ignore_click       = 0;
            x->g_views              = NULL;
            x->g_nviews             = 0;
            x->g_boxparameters.d_bordercolor        = rgba_black;
            x->g_boxparameters.d_borderthickness    = 1;
            x->g_boxparameters.d_boxfillcolor       = rgba_white;
            x->g_boxparameters.d_cornersize         = 0;
        }
        else
        {
            pd_error(owner, "can't allocate dsp manager.");
        }
    }
    return x;
}

t_egui* egui_findbyname(t_symbol* name)
{
    t_class* c = egui_setup();
    if(c)
    {
        return (t_egui *)pd_findbyclass(name, c);
    }
    return NULL;
}

t_symbol* egui_getreceive_symbol(t_egui const* gui)
{
    return gui->g_receive;
}

long egui_getflags(t_egui const* gui)
{
    return gui->g_flags;
}






