/*
 * CicmWrapper
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "eparam.h"

/*
void ebox_texteditor_keypress(t_ebox *x, t_symbol *s, float f)
{
    t_etextlayouteditor* editor;
    const t_eclass* c = eobj_getclass(x);
    if(c && c->c_widget.w_texteditor_keypress)
    {
        editor = etexteditor_getfromsymbol(s);
        if(editor)
        {
            c->c_widget.w_texteditor_keypress(x, editor, (int)f);
        }
    }
}

void ebox_texteditor_keyfilter(t_ebox *x, t_symbol *s, float f)
{
    t_etextlayouteditor* editor;
    const t_eclass* c = eobj_getclass(x);
    if(c && c->c_widget.w_texteditor_keyfilter)
    {
        editor = etexteditor_getfromsymbol(s);
        if(editor)
        {
            c->c_widget.w_texteditor_keyfilter(x, editor, (int)f);
        }
    }
}

void ebox_texteditor_focus(t_ebox *x, t_symbol *s, float f)
{
    t_etextlayouteditor* editor;
    const t_eclass* c = eobj_getclass(x);
    if(c && c->c_widget.w_texteditor_focus)
    {
        editor = etexteditor_getfromsymbol(s);
        if(editor)
        {
            c->c_widget.w_texteditor_focus(x, editor, (int)f);
        }
    }
}


static void stringifyJustification(int justification, char* text)
{
    if(justification & ETEXT_TOP)
        sprintf(text, "-anchor n");
    else if(justification & ETEXT_BOTTOM)
        sprintf(text, "-anchor s");
    else
        sprintf(text, "-anchor ");
    
    if(justification & ETEXT_LEFT)
        strncat(text, "w", 1);
    else if(justification & ETEXT_RIGHT)
        strncat(text, "e", 1);
    else if(justification & ETEXT_CENTRED)
        strncat(text, "center", 6);
}

static t_pt recomputeRectangle(int justification, t_pt const* p1, t_pt const* p2)
{
    t_pt pt;
    if(justification & ETEXT_TOP)
        pt.y = p1->y;
    else if(justification & ETEXT_BOTTOM)
        pt.y = p1->y + p2->y;
    else
        pt.y = p1->y + p2->y * 0.5f;
    
    if(justification & ETEXT_LEFT)
        pt.x = p1->x;
    else if(justification & ETEXT_RIGHT)
        pt.x = p1->x + p2->x;
    else
        pt.x = p1->x + p2->x * 0.5f;
    
    return pt;
}
*/





void ebox_set_parameter_attribute(t_ebox *x, t_symbol *s, int argc, t_atom* argv)
{
    /*
    t_eparam* param;
    int ac = 0; t_atom* av = NULL;
    if(argc > 2 && argv && atom_gettype(argv) == A_FLOAT && atom_gettype(argv+1) == A_SYMBOL)
    {
        param = eparameter_getbyindex(x, (int)atom_getfloat(argv));
        if(param)
        {
            if(atom_getsymbol(argv+1) == gensym("name"))
            {
                unparse_atoms(argc-2, argv+2, &ac, &av);
                if(ac && av && atom_gettype(av) == A_SYMBOL)
                {
                    eparameter_setname(param, atom_getsymbol(av));
                    free(av);
                }
            }
            else if(atom_getsymbol(argv+1) == gensym("label"))
            {
                unparse_atoms(argc-2, argv+2, &ac, &av);
                if(ac && av && atom_gettype(av) == A_SYMBOL)
                {
                    eparameter_setlabel(param, atom_getsymbol(av));
                    free(av);
                }
            }
            else if(atom_getsymbol(argv+1) == gensym("index") && atom_gettype(argv+2) == A_FLOAT)
            {
                eparameter_setindex(param, atom_getfloat(argv+2));
            }
        }
    }
     */
}

static void ebox_parameter_notify(t_eparam* param, t_symbol* action)
{
    /*
    t_atom av[3];
    if(is_valid_symbol(eobj_getid(param->p_owner)) && eobj_getid(param->p_owner)->s_thing)
    {
        atom_setsym(av, param->p_bind);
        atom_setsym(av+1, action);
        atom_setfloat(av+2, (float)param->p_index);
        pd_typedmess(param->p_owner->b_obj.o_camo_id->s_thing, s_cream_parameter, 3, av);
    }
     */
}

static t_class* eparameter_setup()
{
    t_class* eparameter_class = NULL;
    t_symbol* eparameter1572_sym = gensym("eparameter1572");
    if(!eparameter1572_sym->s_thing)
    {
        eparameter_class = class_new(gensym("eparameter"), NULL, (t_method)NULL, sizeof(t_etextlayouteditor), CLASS_PD, A_GIMME, 0);
        int aki;
        eparameter1572_sym->s_thing = (t_class **)eparameter_class;
        return eparameter_class;
    }
    else
    {
        return (t_class *)eparameter1572_sym->s_thing;
    }
}


t_eparam* ebox_parameter_create(t_ebox *x, int index)
{
    int i;
    char text[MAXPDSTRING];
    t_eparam **temp  = NULL, *param = NULL;
    t_class* c = eparameter_setup();
    if(c)
    {
        /*
        index--;
        if(index < x->b_nparams && x->b_params[index])
        {
            return x->b_params[index];
        }
        else
        {
            param = (t_eparam *)pd_new(c);
            if(param)
            {
                sprintf(text, "param%ld", (unsigned long)(param));
                param->p_bind       = gensym(text);
                param->p_name       = s_cream_empty;
                param->p_label      = s_cream_empty;
                param->p_owner      = x;
                param->p_index      = index+1;
                param->p_value      = 0.f;
                param->p_min        = 0.f;
                param->p_max        = 1.f;
                param->p_nstep      = (int)(1.f / FLT_EPSILON) + 1;
                param->p_getter     = (t_param_getter)NULL;
                param->p_setter     = (t_param_setter)NULL;
                param->p_getter_t   = (t_param_getter_t)NULL;
                param->p_setter_t   = (t_param_setter_t)NULL;
                param->p_auto       = 1;
                param->p_meta       = 0;
                param->p_enable     = 1;
                param->p_flags      = 0;
                pd_bind((t_pd *)param, param->p_bind);
                if(x->b_params && index < x->b_nparams)
                {
                    x->b_params[index] = param;
                    ebox_parameter_notify(param, s_cream_create);
                    return param;
                }
                else if(x->b_params && index >= x->b_nparams)
                {
                    temp = (t_eparam **)realloc(x->b_params, sizeof(t_eparam *) * (size_t)(index + 1));
                    if(temp)
                    {
                        x->b_params = temp;
                        for(i = (int)x->b_nparams; i < index; i++)
                        {
                            x->b_params[i] = NULL;
                        }
                        x->b_params[index] = param;
                        x->b_nparams = index+1;
                        ebox_parameter_notify(param, s_cream_create);
                        return param;
                    }
                    else
                    {
                        return NULL;
                    }
                }
                else if(!x->b_params || !x->b_nparams)
                {
                    x->b_params = (t_eparam **)malloc(sizeof(t_eparam *) * (size_t)(index + 1));
                    if(x->b_params)
                    {
                        for(i = 0; i < index; i++)
                        {
                            x->b_params[i] = NULL;
                        }
                        x->b_params[index] = param;
                        x->b_nparams = index+1;
                        ebox_parameter_notify(param, s_cream_create);
                        return param;
                    }
                    else
                    {
                        x->b_nparams = 0;
                        return NULL;
                    }
                }
            }
        }
         */
    }
    return NULL;
}

void ebox_parameter_destroy(t_ebox* x, int index)
{
    int i = 0;
    /*
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            ebox_parameter_notify(x->b_params[index], s_cream_destroy);
            pd_unbind((t_pd *)x->b_params[index], x->b_params[index]->p_bind);
            free(x->b_params[index]);
            x->b_params[index] = NULL;
            for(i = index; i < x->b_nparams - 1; i++)
            {
                x->b_params[i] = x->b_params[i+1];
            }
        }
    }
     */
}

float ebox_parameter_getvalue(t_ebox* x, int index)
{
    /*
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            if(x->b_params[index]->p_getter)
            {
                return x->b_params[index]->p_getter(x->b_params[index]->p_owner, x->b_params[index]);
            }
            return x->b_params[index]->p_value;
        }
    }
     */
    return 0;
}

void ebox_parameter_setvalue(t_ebox* x, int index, float value, char notify)
{
    /*
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            if(x->b_params[index]->p_setter)
            {
                x->b_params[index]->p_setter(x->b_params[index]->p_owner, x->b_params[index], value);
            }
            else
            {
                if(x->b_params[index]->p_min < x->b_params[index]->p_max)
                {
                    x->b_params[index]->p_value = pd_clip(value, x->b_params[index]->p_min, x->b_params[index]->p_max);
                }
                else
                {
                    x->b_params[index]->p_value  = pd_clip(value, x->b_params[index]->p_max, x->b_params[index]->p_min);
                }
            }
            if(notify)
            {
                ebox_parameter_notify(x->b_params[index], s_cream_value_changed);
            }
        }
    }
     */
}

float ebox_parameter_getvalue_normalized(t_ebox* x, int index)
{
    /*
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            if(x->b_params[index]->p_max == x->b_params[index]->p_min)
            {
                return 1.;
            }
            else if(x->b_params[index]->p_min < x->b_params[index]->p_max)
            {
                return (ebox_parameter_getvalue(x, index+1) - x->b_params[index]->p_min) / (x->b_params[index]->p_max - x->b_params[index]->p_min);
            }
            else
            {
                return (ebox_parameter_getvalue(x, index+1) - x->b_params[index]->p_max) / (x->b_params[index]->p_min - x->b_params[index]->p_max);
            }
        }
    }
     */
    return 0.f;
}

void ebox_parameter_setvalue_normalized(t_ebox* x, int index, float value, char notify)
{
    /*
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            if(x->b_params[index]->p_min < x->b_params[index]->p_max)
            {
                ebox_parameter_setvalue(x, index+1, (value * (x->b_params[index]->p_max - x->b_params[index]->p_min) + x->b_params[index]->p_min), notify);
            }
            else
            {
                ebox_parameter_setvalue(x, index+1, (value * (x->b_params[index]->p_min - x->b_params[index]->p_max) + x->b_params[index]->p_max), notify);
            }
        }
    }
     */
}

float ebox_parameter_getmin(t_ebox* x, int index)
{
    /*
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            return x->b_params[index]->p_min;
        }
    }
     */
    return 1.f;
}

float ebox_parameter_getmax(t_ebox* x, int index)
{
    /*
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            return x->b_params[index]->p_max;
        }
    }
     */
    return 1.f;
}

int ebox_parameter_getnstep(t_ebox* x, int index)
{
    /*
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            return x->b_params[index]->p_nstep;
        }
    }
     */
    return 1;
}

t_symbol* ebox_parameter_getbind(t_ebox* x, int index)
{
    /*
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            return x->b_params[index]->p_bind;
        }
    }
     */
    return NULL;
}

char ebox_parameter_isinverted(t_ebox* x, int index)
{
    /*
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            return (char)(x->b_params[index]->p_max < x->b_params[index]->p_min);
        }
    }
     */
    return 0;
}

void ebox_parameter_notify_changes(t_ebox *x, int index)
{
    /*
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            ebox_parameter_notify(x->b_params[index], s_cream_value_changed);
        }
    }
     */
}

void ebox_parameter_begin_changes(t_ebox *x, int index)
{
    /*
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            ebox_parameter_notify(x->b_params[index], s_cream_beginchanges);
        }
    }
     */
}

void ebox_parameter_end_changes(t_ebox *x, int index)
{
    /*
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            ebox_parameter_notify(x->b_params[index], s_cream_endchanges);
        }
    }
     */
}

void ebox_parameter_setname(t_ebox* x, int index, t_symbol* name)
{
    /*
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            x->b_params[index]->p_name = get_valid_symbol(name);
            ebox_parameter_notify(x->b_params[index], s_cream_attr_modified);
        }
    }
     */
}

void ebox_parameter_setlabel(t_ebox* x, int index, t_symbol* label)
{
    /*
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            x->b_params[index]->p_label = get_valid_symbol(label);
            ebox_parameter_notify(x->b_params[index], s_cream_attr_modified);
        }
    }
     */
}

void ebox_parameter_setminmax(t_ebox* x, int index, float min, float max)
{
    /*
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            x->b_params[index]->p_min = min;
            x->b_params[index]->p_max = max;
            ebox_parameter_notify(x->b_params[index], s_cream_attr_modified);
            ebox_parameter_setvalue(x, index+1, x->b_params[index]->p_value, 1);
        }
    }
     */
}

void ebox_parameter_setnstep(t_ebox* x, int index, int nstep)
{
    /*
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            x->b_params[index]->p_nstep = nstep > 1 ? (int)nstep : 1;
            ebox_parameter_notify(x->b_params[index], s_cream_attr_modified);
        }
    }
     */
}

void ebox_parameter_setindex(t_ebox* x, int index, int pindex)
{
    /*
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            x->b_params[index]->p_index = pindex;
            ebox_parameter_notify(x->b_params[index], s_cream_attr_modified);
        }
    }
     */
}

void ebox_parameter_setflags(t_ebox* x, int index, long flags)
{
    /*
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            x->b_params[index]->p_flags = flags;
            ebox_parameter_notify(x->b_params[index], s_cream_attr_modified);
        }
    }
     */
}

void ebox_parameter_enable(t_ebox* x, int index)
{
    /*
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            x->b_params[index]->p_enable = 1;
            ebox_parameter_notify(x->b_params[index], s_cream_attr_modified);
        }
    }
     */
}

void ebox_parameter_disable(t_ebox* x, int index)
{
    /*
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            x->b_params[index]->p_enable = 0;
            ebox_parameter_notify(x->b_params[index], s_cream_attr_modified);
        }
    }
     */
}

void ebox_parameter_setsettergetter(t_ebox* x, int index, t_param_setter setter, t_param_getter getter)
{
    /*
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            x->b_params[index]->p_setter = setter;
            x->b_params[index]->p_getter = getter;
            ebox_parameter_notify(x->b_params[index], s_cream_attr_modified);
        }
    }
     */
}

void ebox_parameter_setsettergetter_text(t_ebox* x, int index, t_param_setter_t setter, t_param_getter_t getter)
{
    /*
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            x->b_params[index]->p_setter_t = setter;
            x->b_params[index]->p_getter_t = getter;
            ebox_parameter_notify(x->b_params[index], s_cream_attr_modified);
        }
    }
     */
}


t_eparam* eparameter_getbyindex(t_ebox* x, int index)
{
    /*
    int tod_chaned;
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            return x->b_params[index];
        }
    }
     */
    return NULL;
}








