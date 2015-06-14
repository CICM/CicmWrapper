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

t_symbol* s_null;
t_symbol* s_atom;
t_symbol* s_obj;
t_symbol* s_attr_modified;
t_symbol* s_eboxbd;
t_symbol* s_eboxio;
t_symbol* s_size;
t_symbol* s_int;
t_symbol* s_long;
t_symbol* s_double;
t_symbol* s_eproxy1572;

void epd_init(void)
{
    t_symbol* epd_symbol = gensym("epd1572");
    if(!epd_symbol->s_thing)
    {
        s_null          = gensym("(null)");
        s_atom          = gensym("atom");
        s_obj           = gensym("obj");
        s_attr_modified = gensym("attr_modified");
        s_eboxbd        = gensym("eboxbd");
        s_eboxio        = gensym("eboxio");
        s_size          = gensym("size");
        s_int           = gensym("int");
        s_long          = gensym("long");
        s_double        = gensym("double");
        s_eproxy1572    = gensym("eproxy1572");
        
        // PATCHER MOUSE MOTION //
        sys_vgui("proc eobj_canvas_motion {patcher val} {\n");
        sys_gui(" set rx [winfo rootx $patcher]\n");
        sys_gui(" set ry [winfo rooty $patcher]\n");
        sys_gui(" set x  [winfo pointerx .]\n");
        sys_gui(" set y  [winfo pointery .]\n");
        sys_vgui(" pdtk_canvas_motion $patcher [expr $x - $rx] [expr $y - $ry] $val\n");
        sys_gui("}\n");
        
        // PATCHER MOUSE DOWN //
        sys_vgui("proc eobj_canvas_down {patcher val} {\n");
        sys_gui(" set rx [winfo rootx $patcher]\n");
        sys_gui(" set ry [winfo rooty $patcher]\n");
        sys_gui(" set x  [winfo pointerx .]\n");
        sys_gui(" set y  [winfo pointery .]\n");
        sys_vgui(" pdtk_canvas_mouse $patcher [expr $x - $rx] [expr $y - $ry] 0 $val\n");
        sys_gui("}\n");
        
        // PATCHER MOUSE UP //
        sys_vgui("proc eobj_canvas_up {patcher} {\n");
        sys_gui(" set rx [winfo rootx $patcher]\n");
        sys_gui(" set ry [winfo rooty $patcher]\n");
        sys_gui(" set x  [winfo pointerx .]\n");
        sys_gui(" set y  [winfo pointery .]\n");
        sys_vgui(" pdtk_canvas_mouseup $patcher [expr $x - $rx] [expr $y - $ry] 0\n");
        sys_gui("}\n");
        
        // PATCHER MOUSE RIGHT //
        sys_vgui("proc eobj_canvas_right {patcher} {\n");
        sys_gui(" set rx [winfo rootx $patcher]\n");
        sys_gui(" set ry [winfo rooty $patcher]\n");
        sys_gui(" set x  [winfo pointerx .]\n");
        sys_gui(" set y  [winfo pointery .]\n");
        sys_vgui(" pdtk_canvas_rightclick $patcher [expr $x - $rx] [expr $y - $ry] 0\n");
        sys_gui("}\n");
    
        epd_symbol->s_thing = (t_class **)1;
    }
}

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

void* object_method(void* x, t_symbol* s, void* z, method method, long number, void* other)
{
    rmethod nrmethod = (rmethod)getfn((t_pd *)x, s);
    return nrmethod(x, s, z, method, number, other);
}

void object_attr_setvalueof(t_object *x, t_symbol* s, int argc, t_atom *argv)
{
    method setvalue = (method)getfn((t_pd *)x, s);
    setvalue(x, s, argc, argv);
}

void object_attr_getvalueof(t_object *x, t_symbol *s, int *argc, t_atom **argv)
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

t_binbuf* binbuf_via_atoms(int ac, t_atom *av)
{
    t_binbuf* dico = binbuf_new();
    binbuf_add(dico, ac, av);
    return dico;
}

static t_symbol* format_symbol(t_symbol* s)
{
    int i, j, lenght = (int)strlen(s->s_name);
    char buffer[MAXPDSTRING];
    buffer[0] = '\"';
    for(i = 0, j = 1; i < lenght && j < MAXPDSTRING - 2; i++, j++)
    {
        if(s->s_name[i] == '"' || s->s_name[i] == '\\')
        {
            buffer[j++] = '\\';
        }
        buffer[j] = s->s_name[i];
    }
    buffer[j++] = '\"';
    buffer[j] = '\0';
    return gensym(buffer);
}

static t_atom* format_atoms(int ac, t_atom* av)
{
    int i;
    for(i = 0; i < ac; i++)
    {
        if(atom_gettype(av+i) == A_SYMBOL)
        {
            atom_setsym(av+i, format_symbol(atom_getsymbol(av+i)));
        }
        else
        {
            av[i] = av[i];
        }
    }
    return av;
}

static long unformat_symbol(char* text, char* buffer, long size)
{
    int i = 0, j = 0, lenght = (int)strlen(text);
    int end = text[lenght - 1] == '"' || text[lenght - 1] == '\'';
    for(; i < lenght - end && j < size - 1; i++)
    {
        if(text[i] != '\\')
        {
            buffer[j++] = text[i];
        }
    }
    buffer[j] = '\0';
    return !end;
}

static long unformat_atoms(int ac, t_atom* av)
{
    int i, lenght, newize = 0;
    char str = 0;
    char temp[MAXPDSTRING];
    char buffer[MAXPDSTRING];
    t_symbol* s;
    for(i = 0; i < ac; i++)
    {
        if(atom_gettype(av+i) == A_SYMBOL)
        {
            s = atom_getsymbol(av+i);
            if(strcmp(s->s_name, "[") && strcmp(s->s_name, "]"))
            {
                if(!str)
                {
                    if(s->s_name[0] == '"' || s->s_name[0] == '\'')
                    {
                        str = (char)unformat_symbol(s->s_name+1, buffer, MAXPDSTRING);
                    }
                    else
                    {
                        unformat_symbol(s->s_name, buffer, MAXPDSTRING);
                    }
                }
                else
                {
                    lenght = (int)strlen(buffer);
                    strcat(buffer, " ");
                    str = (char)unformat_symbol(s->s_name, buffer+lenght+1, MAXPDSTRING-lenght-1);
                }
                if(!str)
                {
                    atom_setsym(av+newize, gensym(buffer));
                    //sprintf(buffer, "");
                    //buffer[0] = '\0'; //-> is equal to sprintf(buffer, "") but only change the first caractere
                    memset(buffer, '\0', MAXPDSTRING*sizeof(char)); //-> clean all the buffer
                    newize++;
                }
            }
        }
        else if(str)
        {
            sprintf(temp, " %f", atom_getfloat(av+i));
            lenght = (int)strlen(temp);
            while(temp[lenght - 1] == '0')
            {
                temp[lenght - 1] = '\0';
                lenght--;
            }
            if(temp[lenght - 1] == '.')
            {
                temp[lenght - 1] = '\0';
                lenght--;
            }
            strcat(buffer, temp);
        }
        else
        {
            av[newize++] = av[i];
        }
    }

    return newize;
}

t_pd_err binbuf_append_attribute(t_binbuf *d, t_symbol *key, int argc, t_atom *argv)
{
    if(d && key && argc && argv)
    {
        format_atoms(argc, argv);
        binbuf_addv(d, "s", key);
        binbuf_add(d, argc, argv);
        return 0;
    }
    return -1;
}

long atoms_get_attributes_offset(int ac, t_atom* av)
{
    long i;
    for(i = 0; i < ac; i++)
    {
        if(atom_gettype(av+i) == A_SYMBOL && atom_getsymbol(av+i)->s_name[0] == '@')
        {
            break;
        }
    }
    return (long)pd_clip_minmax(i, 0, ac);
}

long binbuf_get_attributes_offset(t_binbuf *d)
{
    return atoms_get_attributes_offset(binbuf_getnatom(d), binbuf_getvec(d));
}

long atoms_get_nattributes(int ac, t_atom* av)
{
    long i, j;
    for(i = 0, j = 0; i < ac; i++)
    {
        if(atom_gettype(av+i) == A_SYMBOL && atom_getsymbol(av+i)->s_name[0] == '@')
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

long atoms_get_keys(int ac, t_atom* av, t_symbol*** s)
{
    long i, j;
    long size = atoms_get_nattributes(ac, av);
    if(size)
    {
        s[0] = getbytes((size_t)size * sizeof(t_symbol *));
        if(s[0])
        {
            for(i = 0, j = 0; i < ac; i++)
            {
                if(atom_gettype(av+i) == A_SYMBOL && atom_getsymbol(av+i)->s_name[0] == '@')
                {
                    s[0][j] = atom_getsymbol(av+i);
                    j++;
                }
            }
            return size;
        }
    }
    return 0;
}

long binbuf_get_keys(t_binbuf *d, t_symbol*** s)
{
    return atoms_get_keys(binbuf_getnatom(d), binbuf_getvec(d), s);
}


t_pd_err atoms_has_attribute(int ac, t_atom* av, t_symbol *key)
{
    int i;
    if(ac && av)
    {
        for(i = 0; i < ac; i++)
        {
            if(atom_gettype(av+i) == A_SYMBOL && atom_getsymbol(av+i) == key)
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

long atoms_get_attribute_index(int ac, t_atom *av, t_symbol *key)
{
    int i;
    if(ac && av)
    {
        for(i = 0; i < ac; i++)
        {
            if(atom_gettype(av+i) == A_SYMBOL && atom_getsymbol(av+i) == key)
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

t_pd_err atoms_get_attribute(int ac, t_atom* av, t_symbol *key, int *argc, t_atom **argv)
{
    long i = 0, index  = 0, newsize = 0;
    argc[0]     = 0;
    argv[0]     = NULL;

    index = atoms_get_attribute_index(ac, av, key) + 1;
    if(index > 0)
    {
        i = index;
        while(i < ac && atom_getsymbol(av+i)->s_name[0] != '@')
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
        argv[0] = (t_atom *)getbytes((size_t)argc[0] * sizeof(t_atom));
        if(argv[0])
        {
            memcpy(argv[0], av+index, (size_t)argc[0] * sizeof(t_atom));
            newsize = unformat_atoms(argc[0], argv[0]);
            if(newsize)
            {
                argv[0] = (t_atom *)resizebytes(argv[0], (size_t)argc[0] * sizeof(t_atom), (size_t)newsize * sizeof(t_atom));
                argc[0] = newsize;
            }
            else
            {
                freebytes(argv[0], (size_t)argc[0] * sizeof(t_atom));
            }
            return 0;
        }
        argc[0] = 0;
        argv[0] = NULL;
        return -1;
    }
    else
    {
        argc[0] = 0;
        argv[0] = NULL;
        return -1;
    }

    return 0;
}

t_pd_err binbuf_get_attribute(t_binbuf *d, t_symbol *key, int *argc, t_atom **argv)
{
    if(d)
        return atoms_get_attribute(binbuf_getnatom(d), binbuf_getvec(d), key, argc, argv);
    else
        return -1;
}

t_pd_err atoms_get_attribute_long(int ac, t_atom* av, t_symbol *key, long *value)
{
    int argc = 0;
    t_atom* argv = NULL;
    if(!atoms_get_attribute(ac, av, key, &argc, &argv))
    {
        if(argc && argv)
        {
            if(atom_gettype(argv) == A_FLOAT)
            {
                value[0] = atom_getlong(argv);
                freebytes(argv, (size_t)argc * sizeof(t_atom));
                return 0;
            }
            freebytes(argv, (size_t)argc * sizeof(t_atom));
        }
        return -1;
    }
    return -1;
}

t_pd_err binbuf_get_attribute_long(t_binbuf *d, t_symbol *key, long *value)
{
    if(d)
        return atoms_get_attribute_long(binbuf_getnatom(d), binbuf_getvec(d), key, value);
    else
        return -1;
}

t_pd_err atoms_get_attribute_float(int ac, t_atom* av, t_symbol *key, float *value)
{
    int argc = 0;
    t_atom* argv = NULL;
    atoms_get_attribute(ac, av, key, &argc, &argv);
    if(argc && argv)
    {
        if(atom_gettype(argv) == A_FLOAT)
        {
            value[0] = atom_getfloat(argv);
            freebytes(argv, (size_t)argc * sizeof(t_atom));
            return 0;
        }
        freebytes(argv, (size_t)argc * sizeof(t_atom));
    }
    return -1;
}

t_pd_err binbuf_get_attribute_float(t_binbuf *d, t_symbol *key, float *value)
{
    if(d)
        return atoms_get_attribute_float(binbuf_getnatom(d), binbuf_getvec(d), key, value);
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
    return radius * sin(angle);
}

double pd_abscissa(double radius, double angle)
{
    return radius * cos(angle);
}

double pd_radius(double x, double y)
{
    return sqrt(x*x + y*y);
}

double pd_angle(double x, double y)
{
    return atan2(y, x);
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
			namelist_append_files(sys_staticpath, path);
			return;
		}
		else if(access(path, O_RDONLY) != -1)
		{
			sprintf(path, "%s/%s/%s", var->nl_string, libraryname, folder);
			namelist_append_files(sys_staticpath, path);
			return;
		}
		var = var->nl_next;
	}
    var = sys_staticpath;
    while (var)
    {
        sprintf(path, "%s/%s",var->nl_string, libraryname);
		if(strncmp(var->nl_string, libraryname, strlen(libraryname)) == 0)
		{
			sprintf(path, "%s/%s", var->nl_string, folder);
			namelist_append_files(sys_staticpath, path);
			return;
		}
		else if(access(path, O_RDONLY) != -1)
		{
			sprintf(path, "%s/%s/%s", var->nl_string, libraryname, folder);
			namelist_append_files(sys_staticpath, path);
			return;
		}
		var = var->nl_next;
    }
}



