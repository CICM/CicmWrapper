/*
 * CicmWrapper - Pure Data Enhanced
 *
 * A wrapper for Pure Data
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

#include "ecommon.h"

void outlet_int(t_outlet* outlet, int val)
{
    outlet_float(outlet, val);
}

t_outlet* symbolout(void *x)
{
    return outlet_new((t_object *)x, &s_symbol);
}

t_outlet* listout(void *x)
{
    return outlet_new((t_object *)x, &s_list);
}

t_outlet* floatout(void *x)
{
    return outlet_new((t_object *)x, &s_float);
}

t_outlet* bangout(void *x)
{
    return outlet_new((t_object *)x, &s_bang);
}

t_outlet* anythingout(void *x)
{
    return outlet_new((t_object *)x, &s_anything);
}

int obj_isfloatoutlet(t_object *x, int m)
{
    t_outlet *o2;
    for (o2 = x->ob_outlet; o2 && m--; o2 = o2->o_next);
    return (o2 && (o2->o_sym == &s_float));
}

int obj_isfloatinlet(t_object *x, int m)
{
    t_inlet *i;
    if (x->ob_pd->c_firstin)
    {
        if (!m)
            return (x->ob_pd->c_firstin && x->ob_pd->c_floatsignalin);
        else m--;
    }
    for (i = x->ob_inlet; i && m; i = i->i_next, m--)
        ;
    return (i && (i->i_symfrom == &s_float));
}

void canvas_deletelines_for_io(t_canvas *x, t_text *text, t_inlet *inp, t_outlet *outp)
{
    t_linetraverser t;
    t_outconnect *oc;
    linetraverser_start(&t, x);
    while ((oc = linetraverser_next(&t)))
    {
        if ((t.tr_ob == text && t.tr_outlet == outp) || (t.tr_ob2 == text && t.tr_inlet == inp))
        {
            if(glist_isvisible(x))
            {
                sys_vgui(".x%lx.c delete l%lx\n", glist_getcanvas(x), oc);
            }
            obj_disconnect(t.tr_ob, t.tr_outno, t.tr_ob2, t.tr_inno);
        }
    }
}

void* object_method(void* x, t_symbol* s, void* z, method method, long number, void* other)
{
    rmethod nrmethod = (rmethod)getfn((t_pd *)x, s);
    return nrmethod(x, s, z, method, number, other);
}
/*
void* object_method(void* x, t_symbol* s, ...)
{
    va_list arguments;

    va_start(arguments, s);
    while (*vp)
    {
        vp++;
        count++;
        *vp = va_arg(arguments, t_atomtype);
    }
    va_end(ap);

    rmethod nrmethod = (rmethod)getfn((t_pd *)x, s);
    return nrmethod(x, s, ...);
}*/

void object_attr_setvalueof(t_object *x, t_symbol* s, long argc, t_atom* argv)
{
    method setvalue = (method)getfn((t_pd *)x, s);
    setvalue(x, s, argc, argv);
}

void object_attr_getvalueof(t_object *x, t_symbol *s, long *argc, t_atom **argv)
{
    char realname[MAXPDSTRING];
	method getvalue = NULL;
    sprintf(realname, "get%s", s->s_name);
    argc[0] = 0;
    argv[0] = NULL;
    getvalue = (method)getfn((t_pd *)x, gensym(realname));
    if(getvalue)
        getvalue(x, s, argc, argv);
}

t_binbuf* binbuf_via_atoms(long ac, t_atom *av)
{
    t_binbuf* dico = binbuf_new();
    binbuf_add(dico, (int)ac, av);
    return dico;
}

t_symbol* fsymbol_from_symbol(t_symbol* s)
{
    char buffer[MAXPDSTRING];
    if(strchr(s->s_name,' '))
    {
        sprintf(buffer, "'%s'", s->s_name);
        return gensym(buffer);
    }
    else
        return s;
}

t_symbol* symbol_from_fsymbol(t_symbol* s)
{
    char buf[MAXPDSTRING];
    sprintf(buf, "%s",s->s_name);
    return gensym(strtok(buf," '\""));
}

t_atom* fatoms_from_atoms(long ac, t_atom* av)
{
    int i;
    for(i = 0; i < ac; i++)
    {
        if(atom_gettype(av+i) == A_SYM)
            atom_setsym(av+i, fsymbol_from_symbol(atom_getsym(av+i)));
        else
            av[i] = av[i];
    }
    return av;
}

long atoms_from_fatoms(long ac, t_atom* av)
{
    int i, j;
    char buffer[MAXPDSTRING];
    char temp[MAXPDSTRING];
    char *pch;
    t_symbol* s;
    j = 0;
    for(i = 0; i < ac; i++)
    {
        if(atom_gettype(av+i) == A_SYM)
        {
            s = atom_getsym(av+i);
            pch = strpbrk(s->s_name,"'");
            if(pch != NULL)
            {
                sprintf(buffer, "%s", strtok(pch," '\""));
                if (i+1 < ac)
                {
                    do
                    {
                        i++;
                        strcat(buffer, " ");
                        atom_string(av+i, temp, MAXPDSTRING);
                        pch = strpbrk(temp,"'");
                        strcat(buffer, strtok(temp," '\""));
                    }
                    while(pch == NULL && i < ac);
                }
                atom_setsym(av+j, gensym(buffer));
                j++;

            }
            else if(!strcmp(s->s_name, "[") || !strcmp(s->s_name, "]"))
            {
                ;
            }
            else
            {
                atom_setsym(av+j, symbol_from_fsymbol(s));
                j++;
            }
        }
        else
        {
            av[j] = av[i];
            j++;
        }
    }
    return j;
}


t_pd_err binbuf_append_attribute(t_binbuf *d, t_symbol *key, long argc, t_atom *argv)
{
    int i;

    long ac = argc+1;
    t_atom* av = (t_atom *)calloc(ac, sizeof(t_atom));
    atom_setsym(av, key);
    argv = fatoms_from_atoms(argc, argv);
    for(i = 0; i < argc; i++)
    {
        av[i+1] = argv[i];
    }

    binbuf_add(d, (int)ac, av);
    return 0;
}

long atoms_get_attributes_offset(long ac, t_atom* av)
{
    long i;
    for(i = 0; i < ac; i++)
    {
        if(atom_gettype(av+i) == A_SYM && atom_getsym(av+i)->s_name[0] == '@')
        {
            break;
        }
    }
    return pd_clip_minmax(i, 0, ac);
}

long binbuf_get_attributes_offset(t_binbuf *d)
{
    return atoms_get_attributes_offset(binbuf_getnatom(d), binbuf_getvec(d));
}

long atoms_get_nattributes(long ac, t_atom* av)
{
    long i, j;
    for(i = 0, j = 0; i < ac; i++)
    {
        if(atom_gettype(av+i) == A_SYM && atom_getsym(av+i)->s_name[0] == '@')
        {
            j++;
        }
    }
    return j;
}

long binbuf_get_nattributes(t_binbuf *d)
{
    return atoms_get_nattributes(binbuf_getnatom(d), binbuf_getvec(d));
}

long atoms_get_keys(long ac, t_atom* av, t_symbol*** s)
{
    long i, j;
    long size = atoms_get_nattributes(ac, av);
    s[0] = malloc(size * sizeof(t_symbol *));

    for(i = 0, j = 0; i < ac; i++)
    {
        if(atom_gettype(av+i) == A_SYM && atom_getsym(av+i)->s_name[0] == '@')
        {
            s[0][j] = atom_getsym(av+i);
            j++;
        }
    }
    return size;
}

long binbuf_get_keys(t_binbuf *d, t_symbol*** s)
{
    return atoms_get_keys(binbuf_getnatom(d), binbuf_getvec(d), s);
}


t_pd_err atoms_has_attribute(long ac, t_atom* av, t_symbol *key)
{
    int i;
    if(ac && av)
    {
        for(i = 0; i < ac; i++)
        {
            if(atom_gettype(av+i) == A_SYM && atom_getsym(av+i) == key)
            {
                return 1;
            }
        }
    }
    return 0;
}

t_pd_err binbuf_has_attribute(t_binbuf *d, t_symbol *key)
{
    if(d)
        return atoms_has_attribute(binbuf_getnatom(d), binbuf_getvec(d), key);
    else
        return -1;
}

long atoms_get_attribute_index(long ac, t_atom *av, t_symbol *key)
{
    int i;
    if(ac && av)
    {
        for(i = 0; i < ac; i++)
        {
            if(atom_gettype(av+i) == A_SYM && atom_getsym(av+i) == key)
            {
                return i;
                break;
            }
        }
    }
    return -1;
}

long binbuf_get_attribute_index(t_binbuf *d, t_symbol *key)
{
    return atoms_get_attribute_index(binbuf_getnatom(d), binbuf_getvec(d), key);
}

t_pd_err atoms_get_attribute(long ac, t_atom* av, t_symbol *key, long *argc, t_atom **argv)
{
    long i = 0, index  = 0;
    argc[0]     = 0;
    argv[0]     = NULL;

    index = atoms_get_attribute_index(ac, av, key) + 1;
    if(index > 0)
    {
        i = index;
        while(i < ac && atom_getsym(av+i)->s_name[0] != '@')
        {
            i++;
            argc[0]++;
        }
    }
    else
    {
        argc[0] = 0;
        argv[0] = NULL;
        return -1;
    }

    if(argc[0])
    {
        argv[0] = (t_atom *)calloc(argc[0], sizeof(t_atom));
        for (i = 0; i < argc[0]; i++)
        {
            argv[0][i] = av[i+index];
        }
        argc[0] = atoms_from_fatoms(argc[0], argv[0]);
        argv[0] = (t_atom *)realloc(argv[0], argc[0] * sizeof(t_atom));

        if(!argv[0])
        {
            argc[0] = 0;
            argv[0] = NULL;
            return -1;
        }
    }
    else
    {
        argc[0] = 0;
        argv[0] = NULL;
        return -1;
    }

    return 0;
}

t_pd_err binbuf_get_attribute(t_binbuf *d, t_symbol *key, long *argc, t_atom **argv)
{
    if(d)
        return atoms_get_attribute(binbuf_getnatom(d), binbuf_getvec(d), key, argc, argv);
    else
        return -1;
}

double pd_clip_min(double aValue, double aMinimum)
{
    if(aValue < aMinimum)
        return aMinimum;
    else
        return aValue;
}

double pd_clip_max(double aValue, double aMaximum)
{
    if(aValue > aMaximum)
        return aMaximum;
    else
        return aValue;
}

double pd_ordinate(double radius, double angle)
{
    return radius * sinf(angle);
}

double pd_abscissa(double radius, double angle)
{
    return radius * cosf(angle);
}

double pd_radius(double x, double y)
{
    return sqrt(x*x + y*y);
}

double pd_angle(double x, double y)
{
    //if(x > 0. && y >= 0.)
        return atan2(y, x);
    /*
    else if(x > 0. && y < 0.)
        return atan2(y, x) + EPD_2PI;
    else if(x < 0.)
        return atan2(y, x) + EPD_PI;
    else if(x == 0 && y < 0)
        return EPD_PI2;
    else
        return EPD_PI + EPD_PI2;
    */
}

double pd_clip_minmax(double aValue, double aMinimum, double aMaximum)
{
    if(aValue < aMinimum)
        return aMinimum;
    else if(aValue > aMaximum)
        return aMaximum;
    else
        return aValue;
}

void pd_library_add_folder(char* libraryname, char* folder)
{
	char path[MAXPDSTRING];
	t_namelist* var = sys_searchpath;
	while (var)
	{
		sprintf(path, "%s/%s",var->nl_string, libraryname);
		if(strncmp(var->nl_string, libraryname, strlen(libraryname)) == 0)
		{
			sprintf(path, "%s/%s", var->nl_string, folder);
			namelist_append_files(sys_searchpath, path);
			return;
		}
		else if(access(path, O_RDONLY) != -1)
		{
			sprintf(path, "%s/%s/%s", var->nl_string, libraryname, folder);
			namelist_append_files(sys_searchpath, path);
			return;
		}
		var = var->nl_next;
	}
}



