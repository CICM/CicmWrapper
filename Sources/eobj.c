/*
 * CicmWrapper
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "eobj.h"
#include "ecommon.h"
#include "egraphics.h"
#include "epopup.h"
#include "eproxy.h"

struct _eobj
{
    t_object            o_obj;              /*!< The Pd object. */
    t_symbol*           o_id;               /*!< The object id. */
    t_canvas*           o_canvas;           /*!< The canvas that own the object. */
    t_eproxy**          o_proxy;            /*!< The array of proxy inlets. */
    int                 o_nproxy;           /*!< The number of proxy inlets. */
    int                 o_cproxy;           /*!< The index of the current proxy inlet used */
    t_symbol*           o_listeners;        /*!< The listeners id. */
};

struct _edspobj
{
    t_eobj d_obj; /*!< The default object. */
};

/*
 * Methods
 */

static t_eproxy* eproxy_new(void *owner, t_symbol* s);
static void eproxy_free(void *owner, t_eproxy* proxy);

extern void eobj_popup(t_eobj* x, t_symbol* s, float itemid);
extern void eobj_save(t_gobj* x, t_binbuf *b);
extern void eobj_write(t_eobj* x, t_symbol* s, int argc, t_atom *argv);
extern void eobj_read(t_eobj* x, t_symbol* s, int argc, t_atom *argv);
extern void eobj_dsp(void *x, t_signal **sp);
extern void eobj_dsp_add(void *x, t_symbol* s, t_object* za, t_typ_method m, long flags, void *userparam);
extern void eobj_properties_window(t_eobj* x, t_glist *glist);

void *eobj_new(t_eclass *c)
{
    t_eobj* x = (t_eobj *)pd_new(c);
    char buffer[MAXPDSTRING];
    if(x)
    {
        x->o_nproxy = 0;
        x->o_proxy = NULL;
        x->o_canvas = canvas_getcurrent();
        sprintf(buffer,"#%s%lx", class_getname(c), (long unsigned int)x);
        x->o_id = gensym(buffer);
        pd_bind(&x->o_obj.ob_pd, x->o_id);
        sprintf(buffer, "%ldcamo", (long unsigned int)x);
        x->o_listeners = gensym(buffer);
    }
    return (x);
}

void eobj_free(void *x)
{
    t_eobj* z = (t_eobj *)x;
    ewindowprop_destroy(z);
    if(z->o_id)
    {
        pd_unbind((t_pd *)x, z->o_id);
        z->o_id = NULL;
    }
    if(z->o_proxy && z->o_nproxy)
    {
        free(z->o_proxy);
        z->o_proxy = NULL;
        z->o_nproxy= 0;
    }
}

char eobj_iscicm(void const* x)
{
    return (char)zgetfn((t_pd *)x, s_cream_iscicm);
}

char eobj_isgui(void const* x)
{
    return (char)zgetfn((t_pd *)x, s_cream_isgui);
}

char eobj_isdsp(void const* x)
{
    return (char)zgetfn((t_pd *)x, s_cream_isdsp);
}

void eobj_proxynew(void* x)
{
    eproxy_new(x, NULL);
}

int eobj_getproxy(void const* x)
{
    return ((t_eobj const*)x)->o_cproxy;
}

t_eclass* eobj_getclass(void const* x)
{
    return (t_eclass *)((t_gobj const*)x)->g_pd;
}

t_symbol* eobj_getclassname(void const* x)
{
    return gensym(class_getname(eobj_getclass(x)));
}

t_canvas* eobj_getcanvas(void const* x)
{
    return ((t_eobj const*)x)->o_canvas;
}

t_symbol* eobj_getid(void const* x)
{
    return ((t_eobj const*)x)->o_id;
}

void eobj_bind(void const* b, void* const l)
{
    if(getfn((t_pd *)l, s_cream_notify))
    {
        pd_bind((t_pd *)l, ((t_eobj const*)b)->o_listeners);
    }
}

void eobj_unbind(void const* b, void* const l)
{
    if(getfn((t_pd *)l, s_cream_notify))
    {
        pd_unbind((t_pd *)l, ((t_eobj const*)b)->o_listeners);
    }
}

extern void eobj_popup(t_eobj* x, t_symbol* s, float itemid)
{
    t_epopup* popup;
    t_eclass* c = eobj_getclass(x);
    if(s && c->c_widget.w_popup)
    {
        popup = epopupmenu_getfromsymbol(s);
        if(popup)
        {
            c->c_widget.w_popup(x, popup, (long)itemid);
        }
    }
}

extern void eobj_save(t_gobj* x, t_binbuf *b)
{
    int i, state;
    int      argc = 0;
    t_atom*  argv    = NULL;
    t_binbuf* d;
    t_object* z = (t_object *)x;
    t_ebox*   y = (t_ebox *)z;
    t_eclass* c = eobj_getclass(x);
    char buffer[MAXPDSTRING];
    if(x && b)
    {
        if(eobj_isbox(x))
        {
            int important_todo;
            /*
            if(c && b)
            {
                state = canvas_suspend_dsp();
                binbuf_addv(b, (char *)"ssiis", &s__X, s_cream_obj, (t_int)z->te_xpix, (t_int)z->te_ypix, eobj_getclassname(x));
                for(i = 0; i < c->c_nattr; i++)
                {
                    if(c->c_attr[i] && c->c_attr[i]->save && c->c_attr[i]->name)
                    {
                        eobj_attr_getvalueof(x, c->c_attr[i]->name, &argc, &argv);
                        if(argc && argv)
                        {
                            if(!(argc == 1 && atom_gettype(argv) == A_SYMBOL && !is_valid_symbol(atom_getsymbol(argv))))
                            {
                                snprintf(buffer, MAXPDSTRING, "@%s", c->c_attr[i]->name->s_name);
                                binbuf_append_attribute(b, gensym(buffer), argc, argv);
                            }
                            argc = 0;
                            free(argv);
                            argv = NULL;
                        }
                    }
                }
                argv = (t_atom *)malloc(3 * sizeof(t_atom));
                if(argv)
                {
                    for(i = 0; i < y->b_nparams; i++)
                    {
                        if(y->b_params[i])
                        {
                            snprintf(buffer, MAXPDSTRING, "@param%i", i);
                            atom_setsym(argv, y->b_params[i]->p_name);
                            atom_setsym(argv+1, y->b_params[i]->p_label);
                            atom_setfloat(argv+2, y->b_params[i]->p_index);
                            binbuf_append_attribute(b, gensym(buffer), 3, argv);
                        }
                    }
                }
                
                if(c->c_widget.w_save != NULL)
                {
                    c->c_widget.w_save(x, b);
                }
                
                binbuf_addv(b, (char *)";");
                canvas_resume_dsp(state);
            }
            */
        }
        else
        {
            binbuf_addv(b, (char *)"ssii", &s__X, s_cream_obj, (t_int)z->te_xpix, (t_int)z->te_ypix);
            d = z->te_binbuf;
            if(d)
            {
                binbuf_addbinbuf(b, d);
            }
            if(c->c_widget.w_save != NULL)
            {
                c->c_widget.w_save(x, b);
            }
            binbuf_addv(b, (char *)";");
        }
    }
}

extern void eobj_write(t_eobj* x, t_symbol* s, int argc, t_atom *argv)
{
    char buf[MAXPDSTRING];
    char* pch;
    t_atom av[1];
    t_eclass* c = eobj_getclass(x);

    // The file name is defined
    if(argc && argv && atom_gettype(argv) == A_SYMBOL)
    {
        pch = strpbrk(atom_getsymbol(argv)->s_name, "/\"");
        // The folder seems defined
        if(pch != NULL)
        {
            atom_setsym(av, atom_getsymbol(argv));
            if(c->c_widget.w_write)
            {
                c->c_widget.w_write(x, s, 1, av);
            }
        }
        // The folder isn't defined so write it in the canvas folder
        else
        {
            sprintf(buf, "%s/%s", canvas_getdir(x->o_canvas)->s_name, atom_getsymbol(argv)->s_name);
            atom_setsym(av, gensym(buf));
            if(c->c_widget.w_write)
            {
                c->c_widget.w_write(x, s, 1, av);
            }
        }
    }
    // The file name is not defined so we popup a window
    else
    {
        sys_vgui("eobj_saveas %s nothing nothing\n", x->o_id->s_name);
    }
}

extern void eobj_read(t_eobj* x, t_symbol* s, int argc, t_atom *argv)
{
    char buf[MAXPDSTRING];
    char* pch;
    t_atom av[1];
    t_namelist* var;
    t_eclass* c = eobj_getclass(x);
    
    // Name
    if(argc && argv && atom_gettype(argv) == A_SYMBOL)
    {
        // Valid path
        if((access(atom_getsymbol(argv)->s_name, O_RDONLY) != -1))
        {
            if(c->c_widget.w_read)
                c->c_widget.w_read(x, s, 1, argv);
        }
        // Invalid path or no path
        else
        {
            // Wrong path but we don't care
            pch = strpbrk(atom_getsymbol(argv)->s_name, "/\"");
            if(pch != NULL)
            {
                if(c->c_widget.w_read)
                    c->c_widget.w_read(x, s, 1, argv);
            }
            else
            {
                // Look in the canvas folder
                sprintf(buf, "%s/%s", canvas_getdir(x->o_canvas)->s_name, atom_getsymbol(argv)->s_name);
                if((access(buf, O_RDONLY) != -1))
                {
                    atom_setsym(av, gensym(buf));
                    if(c->c_widget.w_read)
                        c->c_widget.w_read(x, s, 1, av);
                    return;
                }
                // Look in the search path
                var = sys_searchpath;
                while (var)
                {
                    sprintf(buf, "%s/%s", var->nl_string, atom_getsymbol(argv)->s_name);
                    if((access(buf, O_RDONLY) != -1))
                    {
                        atom_setsym(av, gensym(buf));
                        if(c->c_widget.w_read)
                            c->c_widget.w_read(x, s, 1, av);
                        return;
                    }
                    var = var->nl_next;
                }
                // Nothing work but we don't care
                if(c->c_widget.w_read)
                    c->c_widget.w_read(x, s, 1, av);
                return;
            }
        }
    }
    // No name so we popup a window
    else
    {
        sys_vgui("eobj_openfrom %s\n", x->o_id->s_name);
    }
}

void eobj_attrprocess_viatoms(void *x, int argc, t_atom *argv)
{
    int     i;
    char    buffer[MAXPDSTRING];
    int     defc        = 0;
    t_atom* defv        = NULL;
    t_eclass* c         = eobj_getclass(x);

    for(i = 0; i < c->c_nattr; i++)
    {
        sprintf(buffer, "@%s", c->c_attr[i]->name->s_name);
        atoms_get_attribute(argc, argv, gensym(buffer), &defc, &defv);
        if(defc && defv)
        {
            eobj_attr_setvalueof(x, c->c_attr[i]->name, defc, defv);
            defc = 0;
            free(defv);
            defv = NULL;
        }
    }
}

void eobj_attrprocess_viabinbuf(void *x, t_binbuf *d)
{
    int i;
    char attr_name[MAXPDSTRING];

    int defc       = 0;
    t_atom* defv    = NULL;
    t_eclass* c     = eobj_getclass(x);
    for(i = 0; i < c->c_nattr; i++)
    {
        sprintf(attr_name, "@%s", c->c_attr[i]->name->s_name);
        binbuf_get_attribute(d, gensym(attr_name), &defc, &defv);
        if(defc && defv)
        {
            eobj_attr_setvalueof(x, c->c_attr[i]->name, defc, defv);
            defc = 0;
            free(defv);
            defv = NULL;
        }
    }
}

extern void eobj_properties_window(t_eobj* x, t_glist *glist)
{
    ewindowprop_create(x);
}

void eobj_attr_setvalueof(void *x, t_symbol* s, int argc, t_atom *argv)
{
    t_typ_method setvalue = (t_typ_method)getfn((t_pd *)x, s);
    if(setvalue)
    {
        setvalue(x, s, argc, argv);
    }
}

void eobj_attr_getvalueof(void *x, t_symbol *s, int *argc, t_atom **argv)
{
    char realname[MAXPDSTRING];
    t_typ_method getvalue = NULL;
    sprintf(realname, "get%s", s->s_name);
    argc[0] = 0;
    argv[0] = NULL;
    getvalue = (t_typ_method)getfn((t_pd *)x, gensym(realname));
    if(getvalue)
    {
        getvalue(x, s, argc, argv);
    }
}







static t_edsp* eobj_getdsp(void *x)
{
    char text[MAXPDSTRING];
    sprintf(text, "%lxdsp", (unsigned long)x);
    return edsp_findbyname(gensym(text))
}

void eobj_dspsetup(void *x, long nins, long nouts)
{
    char text[MAXPDSTRING];
    t_edsp* dsp = edsp_new();
    if(dsp)
    {
        sprintf(text, "%lxdsp", (unsigned long)x);
        pd_bind((t_pd *)dsp, gensym(text));
    }
}

void eobj_dspflags(void *x, long flags)
{
    t_edsp* dsp = eobj_getdsp(x);
    if(dsp)
    {
        edsp_setflags(dsp, flags);
    }
}

void eobj_dspfree(void *x)
{
    t_edsp* dsp = eobj_getdsp(x);
    if(dsp)
    {
        edsp_free(dsp);
    }
    eobj_free(x);
}

static void eobj_dsp(void *x, t_signal **sp)
{
    t_edsp* dsp = eobj_getdsp(x);
    if(dsp)
    {
        edsp_prepare(dsp, sp);
    }
}

static void eobj_dspadd(void *x, t_symbol* s, t_object* za, t_typ_method m, long flags, void *userparam)
{
    t_edsp* dsp = eobj_getdsp(x);
    if(dsp)
    {
        edsp_add(dsp, m, flags, userparam);
    }
}

void eobj_resize_inputs(void *x, long nins)
{
    int i, cinlts;
    t_eobj* obj = (t_eobj *)x;
    nins = (long)pd_clip_min(nins, 1);
    cinlts = obj_nsiginlets((t_object *)x);
    if(nins > cinlts)
    {
        for(i = cinlts; i < nins; i++)
        {
            eproxy_new(obj, &s_signal);
        }
    }
    else if(nins < cinlts)
    {
        for(i = obj->o_nproxy - 1; i >= nins; --i)
        {
            eproxy_free(obj, obj->o_proxy[i]);
        }
    }
    canvas_fixlinesfor(eobj_getcanvas(obj), (t_text *)x);
}


t_sample* eobj_getsignalinput(void *x, long index)
{
    t_edsp* dsp = eobj_getdsp(x);
    if(dsp)
    {
        if(index < obj_nsiginlets((t_object *)x) && index >= 0 && dsp->d_vectors )
        {
            return (t_sample *)dsp->d_vectors[7 + index];
        }
    }
    return NULL;
}

t_sample* eobj_getsignaloutput(void *x, long index)
{
    t_edsp* dsp = eobj_getdsp(x);
    if(dsp)
    {
        if(index < obj_nsigoutlets((t_object *)x) && index >= 0 && dsp->d_vectors)
        {
            if(dsp->d_misc == E_NO_INPLACE)
                return dsp->d_sigs_out[index];
            else
                return (t_sample *)dsp->d_vectors[index + 7 + obj_nsiginlets((t_object *)x)];
        }
    }
    return NULL;
}

//! @endcond





