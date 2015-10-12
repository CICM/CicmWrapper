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

struct _egui
{
    t_object        g_object;           /*!< The object. */
    t_object*       g_owner;            /*!< The gui owner. */
    long            g_flags;            /*!< The gui flags. */
    t_object**      g_views;            /*!< The gui view. */
    float           g_size[2];          /*!< The gui size. */
    int             g_nviews;           /*!< The gui number of views. */
    t_symbol*       g_receive_id;       /*!< The reveive symbol (attribute). */
    t_symbol*       g_send_id;          /*!< The send send (attribute). */
    char            g_pinned;           /*!< The pinned state (attribute). */
    char            g_ignore_click;     /*!< The igore click state (attribute). */
    char            g_visible;          /*!< The visible state (attribute). */
    t_edrawparams   g_boxparameters;    /*!< The gui parameters. */
};

static void egui_free(t_egui *gui)
{
    ;
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
            x->g_receive_id         = s_cream_empty;
            x->g_send_id            = s_cream_empty;
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






