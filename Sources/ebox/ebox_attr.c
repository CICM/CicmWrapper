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

#include "ebox.h"

//! Change the attributes with an array of atoms
/*
 \ @memberof        ebox
 \ @param x         The object
 \ @param argc      The size of the atoms array
 \ @param argv      The atoms array
 \ @return          Nothing
*/
void atoms_attr_process(void *x, long argc, t_atom *argv)
{
    int i;
    char attr_name[MAXPDSTRING];
    long defc       = 0;
    t_atom* defv    = NULL;
    t_ebox* z       = (t_ebox *)x;
    t_eclass* c     = (t_eclass *)z->e_obj.te_g.g_pd;
    
    for(i = 0; i < c->c_nattr; i++)
    {
        sprintf(attr_name, "@%s", c->c_attr[i].name->s_name);
        atoms_get_attribute(argc, argv, gensym(attr_name), &defc, &defv);
        if(defc && defv)
        {
            object_attr_setvalueof((t_object *)x, c->c_attr[i].name, defc, defv);
            defc = 0;
            free(defv);
            defv = NULL;
        }
    }
}

//! Change the attributes with a binbuf
/*
 \ @memberof        ebox
 \ @param x         The object
 \ @param d         The binbuf
 \ @return          Nothing
*/
void binbuf_attr_process(void *x, t_binbuf *d)
{
    int i;
    char attr_name[256];

    long defc       = 0;
    t_atom* defv    = NULL;
    t_ebox* z       = (t_ebox *)x;
    t_eclass* c     = (t_eclass *)z->e_obj.te_g.g_pd;
    
    for(i = 0; i < c->c_nattr; i++)
    {
        sprintf(attr_name, "@%s", c->c_attr[i].name->s_name);
        binbuf_get_attribute(d, gensym(attr_name), &defc, &defv);
        if(defc && defv)
        {
            object_attr_setvalueof((t_object *)x, c->c_attr[i].name, defc, defv);
            defc = 0;
            free(defv);
            defv = NULL;
        }
    }
}

//! Change the attributes with their defaults values
/*
 \ @memberof        ebox
 \ @param x         The object
 \ @return          Nothing
*/
void default_attr_process(void *x)
{
    int i, j, k;
    long defc       = 0;
    t_atom* defv    = NULL;
    t_ebox* z       = (t_ebox *)x;
    t_eclass* c     = (t_eclass *)z->e_obj.te_g.g_pd;
    
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i].defvals)
        {
            defc = c->c_attr[i].size;
            defv = (t_atom *)calloc(defc, sizeof(t_atom));
            if(defc && defv)
            {
                char check = 0;
                char* str_start = c->c_attr[i].defvals->s_name;
                for(j = 0; j < defc; j++)
                {
                    for(k = 0; k < strlen(str_start); k++)
                    {
                        if(isalpha(str_start[k]))
                            check = 1;
                    }
                    if(check || strpbrk(str_start, "<>()'\""))
                    {
                        atom_setsym(defv+j, gensym(str_start));
                    }
                    else
                    {
                        float val = (float)strtod(str_start, &str_start);
                        atom_setfloat(defv+j, val);
                    }
                }
                object_attr_setvalueof((t_object *)x, c->c_attr[i].name, defc, defv);
                defc = 0;
                free(defv);
                defv = NULL;
            }
        }
    }
}

//! Retrieve the font name of an ebox
/*
 \ @memberof        ebox
 \ @param x         The ebox
 \ @return          The font name
*/
t_symbol* ebox_get_fontname(t_ebox* x)
{
    return x->e_font.c_family;
}

//! Retrieve the font slant of an ebox
/*
 \ @memberof        ebox
 \ @param x         The ebox
 \ @return          The font slant
*/
t_symbol* ebox_font_slant(t_ebox* x)
{
    return x->e_font.c_slant;
}

//! Retrieve the font weight of an ebox
/*
 \ @memberof        ebox
 \ @param x         The ebox
 \ @return          The font weight
*/
t_symbol* ebox_font_weight(t_ebox* x)
{
    return x->e_font.c_weight;
}

//! Retrieve the font size of an ebox
/*
 \ @memberof        ebox
 \ @param x         The ebox
 \ @return          The font size
*/
float ebox_font_size(t_ebox* x)
{
    return x->e_font.c_size;
}

//! The default notify method of ebox called when an attribute has changed // PRIVATE
/*
 \ @memberof        ebox
 \ @param x         The ebox
 \ @param s         The name of the attribute
 \ @param msg       Nothing (for Max 6 compatibility)
 \ @param sender    Nothing (for Max 6 compatibility)
 \ @param data      Nothing (for Max 6 compatibility)
 \ @return          Always 0 (for the moment)
*/
t_pd_err ebox_notify(t_ebox *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	int i;
    float bdsize;
    t_eclass* c = (t_eclass *)x->e_obj.te_g.g_pd;
    if(s == gensym("size"))
    {
        if(c->c_widget.w_oksize != NULL)
            c->c_widget.w_oksize(x, &x->e_rect);
        for(i = 0; i < x->e_number_of_layers; i++)
        {
            x->e_layers[i].e_state = EGRAPHICS_INVALID;
        }
        if(x->e_canvas && x->e_ready_to_draw && c->c_box == 0)
        {
            bdsize = x->e_boxparameters.d_borderthickness;
            
            sys_vgui("%s itemconfigure %s -width %d -height %d\n", x->e_canvas_id->s_name, x->e_window_id->s_name, (int)(x->e_rect.width + bdsize * 2.), (int)(x->e_rect.height + bdsize * 2.));
            canvas_fixlinesfor(x->e_canvas, (t_text *)x);
        }
        
        ebox_redraw(x);
    }
    return 0;
}

//! The default size attribute method of ebox called when an size attribute has changed. This function restrains the width and the height depending of the ebox flags EBOX_GROWNO, EBOX_GROWLINK and EBOX_GROWINDI // PRIVATE
/*
 \ @memberof        ebox
 \ @param x         The ebox
 \ @param attr      Nothing (for Max 6 compatibility)
 \ @param argc      The size of the array of atoms
 \ @param argv      The array of atoms that contains the new width and the new height
 \ @return          Always 0 (for the moment)
*/
t_pd_err ebox_size_set(t_ebox *x, t_object *attr, long argc, t_atom *argv)
{
    float width, height;
    if(argc && argv)
    {
        if(x->e_flags & EBOX_GROWNO)
            return 0;
        else if(x->e_flags & EBOX_GROWLINK)
        {
            if(atom_gettype(argv) == A_FLOAT)
            {
                width  = pd_clip_min(atom_getfloat(argv), 4);
                height = x->e_rect.height;
                x->e_rect.height += width - x->e_rect.width;
                if(x->e_rect.height < 4)
                {
                    x->e_rect.width += 4 - height;
                    x->e_rect.height = 4;
                }
                else
                {
                    x->e_rect.width  =  width;
                }
            }
        }
        else if (x->e_flags & EBOX_GROWINDI)
        {
            if(atom_gettype(argv) == A_FLOAT)
                x->e_rect.width = pd_clip_min(atom_getfloat(argv), 4);
            if(atom_gettype(argv+1) == A_FLOAT)
                x->e_rect.height = pd_clip_min(atom_getfloat(argv+1), 4);
        }
    }
    
	return 0;
}

//! The attribute print method that post all the attributes characteristics in the PD console // PRIVATE
/*
 \ @memberof        ebox
 \ @param x         The ebox
 \ @return          Nothing
*/
void ebox_attrprint(t_ebox* x)
{
    int i;
    t_eclass* c     = (t_eclass *)x->e_obj.te_g.g_pd;
    post("%s attributes :", c->c_class.c_name->s_name);
    for(i = 0; i < c->c_nattr; i++)
    {
        
        post("Label : \"%s\" Name : \"%s\" Type : \"%s\" Size : \"%i\"", c->c_attr[i].label->s_name, c->c_attr[i].name->s_name, c->c_attr[i].type->s_name, c->c_attr[i].size);
    }
}

//! Open the properties window (PRIVATE)
/*
 \ @memberof        ebox
 \ @param z         The gobj object
 \ @param glist     The canvas
 \ @return          Nothing
*/
void ebox_properties(t_gobj *z, t_glist *glist)
{
    int i, j;
    t_atom *argv;
    long    argc;
    t_ebox *x = (t_ebox *)z;
    t_eclass* c = (t_eclass *)x->e_obj.te_g.g_pd;
    char buffer[MAXPDSTRING];
    char temp[MAXPDSTRING];
    
    sprintf(buffer, "pdtk_%s_dialog %%s ", c->c_class.c_name->s_name);
    for(i = 0; i < c->c_nattr; i++)
    {
        object_attr_getvalueof((t_object *)x, c->c_attr[i].name, &argc, &argv);
        strcat(buffer, "\"");
        if(argc && argv)
        {
            for(j = 0; j < argc; j++)
            {
                atom_string(argv+j, temp, MAXPDSTRING);
                if(c->c_attr[i].type == gensym("symbol") && strchr(temp, ' '))
                {
                    
                    strcat(buffer, "'");
                    strcat(buffer, temp);
                    strcat(buffer, "'");
                }
                else
                {
                    strcat(buffer, temp);
                }
                strcat(buffer, " ");
            }
        }
        strcat(buffer, "\"");
        strcat(buffer, " ");
    }
    strcat(buffer, "\n");
    gfxstub_new(&x->e_obj.ob_pd, x, buffer);
}

//! Receive the properties window messages and change the attributes values (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The object
 \ @param s         Nothing (for Max 6 compatibility)
 \ @return          Nothing
 */
void ebox_dialog(t_object *x, t_symbol *s, long argc, t_atom* argv)
{
    if(argc && argv)
    {
        atoms_attr_process(x, argc, argv);
        ebox_properties((t_gobj *)x, NULL);
    }
}











