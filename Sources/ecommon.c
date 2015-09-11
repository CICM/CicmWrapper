/*
 * CicmWrapper - Pure Data Enhanced
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "ecommon.h"
#include "egraphics.h"
#include "epopup.h"

t_symbol* s_cream_null;
t_symbol* s_cream_empty;
t_symbol* s_cream_obj;
t_symbol* s_cream_attr_modified;
t_symbol* s_cream_value_changed;
t_symbol* s_cream_eboxbd;
t_symbol* s_cream_eboxio;
t_symbol* s_cream_texteditor;
t_symbol* s_cream_popup;
t_symbol* s_cream_parameter;
t_symbol* s_cream_param;
t_symbol* s_cream_grabfocus;
t_symbol* s_cream_repaint;
t_symbol* s_cream_size;
t_symbol* s_cream_create;
t_symbol* s_cream_destroy;
t_symbol* s_cream_beginchanges;
t_symbol* s_cream_endchanges;
t_symbol* s_cream_changes;
t_symbol* s_pinned;
t_symbol* s_iscicm;

t_symbol* s_cream_char;
t_symbol* s_cream_int;
t_symbol* s_cream_long;
t_symbol* s_cream_float;
t_symbol* s_cream_double;
t_symbol* s_cream_atom;
t_symbol* s_cream_symbol;

t_symbol* s_cream_checkbutton;
t_symbol* s_cream_onoff;
t_symbol* s_cream_color;
t_symbol* s_cream_number;
t_symbol* s_cream_menu;
t_symbol* s_cream_entry;
t_symbol* s_cream_font;

t_symbol* s_cream_linear;
t_symbol* s_cream_exponential;
t_symbol* s_cream_logarithmic;
t_symbol* s_cream_threshold;

void epd_init(void)
{
    s_cream_null          = gensym("(null)");
    s_cream_empty         = gensym("");
    s_cream_obj           = gensym("obj");
    s_cream_attr_modified = gensym("attr_modified");
    s_cream_value_changed = gensym("value_changed");
    s_cream_eboxbd        = gensym("eboxbd");
    s_cream_eboxio        = gensym("eboxio");
    s_cream_texteditor    = gensym("texteditor");
    s_cream_repaint       = gensym("repaint");
    s_cream_popup         = gensym("popup");
    s_cream_parameter     = gensym("parameter");
    s_cream_param         = gensym("param");
    s_cream_grabfocus     = gensym("grabfocus");
    s_cream_size          = gensym("size");
    s_cream_create        = gensym("create");
    s_cream_destroy       = gensym("destroy");
    s_cream_beginchanges  = gensym("beginchanges");
    s_cream_endchanges    = gensym("endchanges");
    s_cream_changes       = gensym("changes");
    
    s_cream_char            = gensym("char");
    s_cream_int             = gensym("int");
    s_cream_long            = gensym("long");
    s_cream_float           = gensym("float");
    s_cream_double          = gensym("double");
    s_cream_atom            = gensym("atom");
    s_cream_symbol          = gensym("symbol");
    
    s_pinned        = gensym("pinned");
    s_iscicm        = gensym("iscicm");
    
    s_cream_linear      = gensym("linear");
    s_cream_exponential = gensym("exponential");
    s_cream_logarithmic = gensym("logarithmic");
    s_cream_threshold   = gensym("threshold");
    
    s_cream_checkbutton = gensym("checkbutton");
    s_cream_onoff       = gensym("onoff");
    s_cream_color       = gensym("color");
    s_cream_number      = gensym("number");
    s_cream_menu        = gensym("menu");
    s_cream_entry       = gensym("entry");
    s_cream_font        = gensym("font");
    
    tcltk_create_methods();
}

static const char* estrchrn(const char* s, const char* delim)
{
    char state = 0;
    const char *temp = NULL;
    while(s[0] != '\0')
    {
        temp    = delim;
        state   = 0;
        while(temp[0] != '\0')
        {
            if(s[0] == temp[0])
            {
                state = 1;
                break;
            }
            temp++;
        }
        if(!state)
        {
            return s;
        }
        s++;
    }
    return NULL;
}

static const char* estrchr(const char* s, const char* delim)
{
    const char *temp = NULL;
    while(s[0] != '\0')
    {
        temp = delim;
        while(temp[0] != '\0')
        {
            if(s[0] == temp[0])
            {
                return s;
            }
            temp++;
        }
        s++;
    }
    return s;
}

char* estrtok(const char** str, const char* delim, char* token)
{
    char* mem = NULL;
    const char *end;
    const char* start = estrchrn(str[0], delim);
    if(start)
    {
        end = estrchr(start+1, delim);
        if(token)
        {
            mem = (char *)realloc(token, (size_t)(end - start + 1) * sizeof(char));
            if(mem)
            {
                memset(mem, 0, (size_t)(end - start + 1) * sizeof(char));
                strncpy(mem, start, (size_t)(end - start));
                str[0] = end;
                return mem;
            }
            else
            {
                free(token);
                str[0] = NULL;
                return NULL;
            }
        }
        else
        {
            mem = (char *)malloc((size_t)(end - start + 1) * sizeof(char));
            if(mem)
            {
                memset(mem, 0, (size_t)(end - start + 1) * sizeof(char));
                strncpy(mem, start, (size_t)(end - start));
                str[0] = end;
                return mem;
            }
            else
            {
                str[0] = NULL;
                return NULL;
            }
        }
    }
    else
    {
        if(token)
        {
            free(token);
        }
        str[0] = NULL;
        return NULL;
    }
    
    
}

float interpolation_bezier(const float f1, const float f2, const float delta, const float node)
{
    const float t = pd_clip(delta, 0.f, 1.f);
    return interpolation_linear(f1, f2, pd_clip(t * t + 2.f * t * (1.f - t) * node, 0.f, 1.f));
}

float interpolation_linear(const float f1, const float f2, const float delta)
{
    const float delta2 = pd_clip(delta, 0., 1.);
    return f1 * (1.f - delta2) + f2 * delta;
}

void object_method(void* x, t_symbol* s, void* z, t_typ_method method, long number, void* other)
{
    t_ret_method nrmethod = (t_ret_method)getfn((t_pd *)x, s);
    nrmethod(x, s, z, method, number, other);
}

t_binbuf* binbuf_via_atoms(int ac, t_atom *av)
{
    t_binbuf* dico = binbuf_new();
    if(dico)
    {
        binbuf_add(dico, ac, av);
    }
    return dico;
}

static t_symbol* format_symbol(t_symbol* s)
{
    int i, j, lenght;
    char buffer[MAXPDSTRING];
    s = get_valid_symbol(s);
    lenght = (int)strlen(s->s_name);
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
    return gensym("");
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
    char temp[256];
    char buffer[MAXPDSTRING];
    t_symbol* s;
    for(i = 0; i < ac; i++)
    {
        if(atom_gettype(av+i) == A_SYMBOL)
        {
            s = get_valid_symbol(atom_getsymbol(av+i));
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
                    strncat(buffer, " ", 1);
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
            strncat(buffer, temp, lenght);
        }
        else
        {
            av[newize++] = av[i];
        }
    }

    return newize;
}

t_symbol* get_valid_symbol(t_symbol* s)
{
    if(s && s != s_cream_null)
    {
        return s;
    }
    else
    {
        return s_cream_empty;
    }
}

char is_valid_symbol(t_symbol* s)
{
    if(s && s != s_cream_null && s != s_cream_empty)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void unparse_atoms(int argc, t_atom* argv, int* ac, t_atom** av)
{
    char text[MAXPDSTRING], temp[256];
    int i; char str; t_symbol* s; size_t l;
    *ac = 0;
    *av = (t_atom*)malloc((size_t)argc * sizeof(t_atom));
    if(*av)
    {
        str = 0;
        for(i = 0; i < argc; i++)
        {
            if(atom_gettype(argv+i) == A_SYMBOL)
            {
                s = atom_getsymbol(argv+i);
                if(!str) // We are not in a string
                {
                    if(s->s_name[0] == '{' || s->s_name[0] == '\'' || s->s_name[0] == '"' || s->s_name[0] == '[') // We enter a string
                    {
                        l = strlen(s->s_name);
                        memset(text, 0, MAXPDSTRING);
                        if(s->s_name[l-1] == '}' || s->s_name[l-1] == '\'' || s->s_name[l-1] == '"') // We leave the string
                        {
                            strncpy(text, s->s_name+1, l-2);
                            atom_setsym(av[0]+ac[0], gensym(text));
                            ac[0]++;
                        }
                        else // We continue the string
                        {
                            strncpy(text, s->s_name+1, l-1);
                            str = 1;
                        }
                    }
                    else // The string is good
                    {
                        atom_setsym(av[0]+ac[0], s);
                        ac[0]++;
                    }
                }
                else
                {
                    l = strlen(s->s_name);
                    if(s->s_name[l-1] == '}' || s->s_name[l-1] == '\'' || s->s_name[l-1] == '"' || s->s_name[0] == ']') // We leave the string
                    {
                        strncat(text, " ", 1);
                        strncat(text, s->s_name, l-1);
                        atom_setsym(av[0]+ac[0], gensym(text));
                        ac[0]++;
                        str = 0;
                    }
                    else // We are still in the string
                    {
                        strncat(text, " ", 1);
                        strncat(text, s->s_name, l);
                    }
                }
                
            }
            else if(atom_gettype(argv+i) == A_FLOAT)
            {
                if(str)
                {
                    memset(temp, 0, 256);
                    sprintf(temp, " %g", atom_getfloat(argv+i));
                    strncat(text, temp, 256);
                }
                else
                {
                    atom_setfloat(av[0]+ac[0], atom_getfloat(argv+i));
                    ac[0]++;
                }
            }
        }
        if(str)
        {
            atom_setsym(av[0]+ac[0], gensym(text));
            ac[0]++;
        }
    }
}

void parse_atoms(int argc, t_atom* argv, int* ac, t_atom** av)
{
    char text[MAXPDSTRING];
    *ac = argc;
    *av = (t_atom*)malloc((size_t)argc * sizeof(t_atom));
    int i;
    if(*av)
    {
        memcpy(*av, argv, (size_t)argc * sizeof(t_atom));
        for(i = 0; i < ac[0]; i++)
        {
            if(atom_gettype(av[0]+i) == A_SYMBOL && strchr(atom_getsymbol(av[0]+i)->s_name, ' ') != NULL)
            {
                sprintf(text, "\"%s\"", atom_getsymbol(av[0]+i)->s_name);
                atom_setsym(av[0]+i, gensym(text));
            }
        }
    }
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

int atoms_get_attributes_offset(int argc, t_atom* argv)
{
    int i;
    for(i = 0; i < argc; i++)
    {
        if(atom_gettype(argv+i) == A_SYMBOL && atom_getsymbol(argv+i)->s_name[0] == '@')
        {
            break;
        }
    }
    return (int)pd_clip(i, 0, argc);
}

int binbuf_get_attributes_offset(t_binbuf *d)
{
    return atoms_get_attributes_offset(binbuf_getnatom(d), binbuf_getvec(d));
}

int atoms_get_nattributes(int argc, t_atom* argv)
{
    int i, j;
    for(i = 0, j = 0; i < argc; i++)
    {
        if(atom_gettype(argv+i) == A_SYMBOL && atom_getsymbol(argv+i)->s_name[0] == '@')
        {
            j++;
        }
    }
    return j;
}

int binbuf_get_nattributes(t_binbuf *d)
{
    return atoms_get_nattributes(binbuf_getnatom(d), binbuf_getvec(d));
}

int atoms_get_keys(int ac, t_atom* av, t_symbol*** keys)
{
    int i, j;
    int size = atoms_get_nattributes(ac, av);
    if(size)
    {
        keys[0] = (t_symbol**)malloc((size_t)size * sizeof(t_symbol *));
        if(keys[0])
        {
            for(i = 0, j = 0; i < ac; i++)
            {
                if(atom_gettype(av+i) == A_SYMBOL && atom_getsymbol(av+i)->s_name[0] == '@')
                {
                    keys[0][j] = atom_getsymbol(av+i);
                    j++;
                }
            }
            return size;
        }
    }
    return 0;
}

int binbuf_get_keys(t_binbuf *d, t_symbol*** keys)
{
    return atoms_get_keys(binbuf_getnatom(d), binbuf_getvec(d), keys);
}


t_pd_err atoms_has_attribute(int argc, t_atom* argv, t_symbol *key)
{
    int i;
    if(argc && argv)
    {
        for(i = 0; i < argc; i++)
        {
            if(atom_gettype(argv+i) == A_SYMBOL && atom_getsymbol(argv+i) == key)
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

int atoms_get_attribute_index(int argc, t_atom *argv, t_symbol *key)
{
    int i;
    if(argc && argv)
    {
        for(i = 0; i < argc; i++)
        {
            if(atom_gettype(argv+i) == A_SYMBOL && atom_getsymbol(argv+i) == key)
            {
                return i;
                break;
            }
        }
    }
    return -1;
}

int binbuf_get_attribute_index(t_binbuf *d, t_symbol *key)
{
    return atoms_get_attribute_index(binbuf_getnatom(d), binbuf_getvec(d), key);
}

t_pd_err atoms_get_attribute(int ac, t_atom* av, t_symbol *key, int *argc, t_atom **argv)
{
    t_atom* temp;
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
        argv[0] = (t_atom *)malloc((size_t)argc[0] * sizeof(t_atom));
        if(argv[0])
        {
            memcpy(argv[0], av+index, (size_t)argc[0] * sizeof(t_atom));
            newsize = unformat_atoms(argc[0], argv[0]);
            if(newsize)
            {
                temp = (t_atom *)realloc(argv[0], (size_t)newsize * sizeof(t_atom));
                if(temp)
                {
                    argv[0] = temp;
                    argc[0] = (int)newsize;
                    return 0;
                }
                else
                {
                    free(argv[0]);
                    argv[0] = NULL;
                    argc[0] = 0;
                }
            }
            else
            {
                free(argv[0]);
            }
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

t_pd_err atoms_get_attribute_int(int ac, t_atom* av, t_symbol *key, int *value)
{
    int argc = 0;
    t_atom* argv = NULL;
    if(!atoms_get_attribute(ac, av, key, &argc, &argv))
    {
        if(argc && argv)
        {
            if(atom_gettype(argv) == A_FLOAT)
            {
                value[0] = (int)atom_getfloat(argv);
                free(argv);
                return 0;
            }
            free(argv);
        }
        return -1;
    }
    return -1;
}

t_pd_err binbuf_get_attribute_int(t_binbuf *d, t_symbol *key, int *value)
{
    if(d)
        return atoms_get_attribute_int(binbuf_getnatom(d), binbuf_getvec(d), key, value);
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
                free(argv);
                return 0;
            }
            free(argv);
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
    if(!atoms_get_attribute(ac, av, key, &argc, &argv))
    {
        if(argc && argv)
        {
            if(atom_gettype(argv) == A_FLOAT)
            {
                value[0] = atom_getfloat(argv);
                free(argv);
                return 0;
            }
            free(argv);
        }
        return -1;
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

t_pd_err atoms_get_attribute_symbol(int ac, t_atom* av, t_symbol *key, t_symbol **value)
{
    int argc = 0;
    t_atom* argv = NULL;
    if(!atoms_get_attribute(ac, av, key, &argc, &argv))
    {
        if(argc && argv)
        {
            if(atom_gettype(argv) == A_SYMBOL)
            {
                value[0] = atom_getsymbol(argv);
                free(argv);
                return 0;
            }
            free(argv);
        }
        return -1;
    }
    return -1;
}

t_pd_err binbuf_get_attribute_symbol(t_binbuf *d, t_symbol *key, t_symbol **value)
{
    if(d)
        return atoms_get_attribute_symbol(binbuf_getnatom(d), binbuf_getvec(d), key, value);
    else
        return -1;
}

void epd_add_folder(const char* name, const char* folder)
{
	char path[MAXPDSTRING];
	t_namelist* var = sys_searchpath;
	while (var)
	{
		sprintf(path, "%s/%s",var->nl_string, name);
		if(strncmp(var->nl_string, name, strlen(name)) == 0)
		{
			sprintf(path, "%s/%s", var->nl_string, folder);
			namelist_append_files(sys_staticpath, path);
			return;
		}
		else if(access(path, O_RDONLY) != -1)
		{
			sprintf(path, "%s/%s/%s", var->nl_string, name, folder);
			namelist_append_files(sys_staticpath, path);
			return;
		}
		var = var->nl_next;
	}
    var = sys_staticpath;
    while (var)
    {
        sprintf(path, "%s/%s",var->nl_string, name);
		if(strncmp(var->nl_string, name, strlen(name)) == 0)
		{
			sprintf(path, "%s/%s", var->nl_string, folder);
			namelist_append_files(sys_staticpath, path);
			return;
		}
		else if(access(path, O_RDONLY) != -1)
		{
			sprintf(path, "%s/%s/%s", var->nl_string, name, folder);
			namelist_append_files(sys_staticpath, path);
			return;
		}
		var = var->nl_next;
    }

}



