/*
// Copyright (c) 2012-2014 Eliott Paris, Julien Colafrancesco & Pierre Guillot, CICM, Universite Paris 8.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "c.library.h"

extern "C"
{
    typedef struct _namelist    /* element in a linked list of stored strings */
    {
        struct _namelist *nl_next;  /* next in list */
        char *nl_string;            /* the string */
    } t_namelist;
    
    extern t_namelist *sys_externlist;
    extern t_namelist *sys_searchpath;
    extern t_namelist *sys_staticpath;
    extern t_namelist *sys_helppath;
    extern t_namelist *namelist_append_files(t_namelist *listwas, const char *s);
}

extern "C" void setup_c0x2elibrary(void)
{
    char path[MAXPDSTRING];
    t_namelist* var;
    
    post("Cream Library by Pierre Guillot");
    post("© 2013 - 2014  CICM | Paris 8 University");
    post("Version 0.2");
    
    // Caramel
    setup_c0x2econvolve_tilde();
    setup_c0x2efreeverb_tilde();
    
    // Chocolate
    setup_c0x2ebang();
    setup_c0x2eblackboard();
    setup_c0x2ebreakpoints();
    setup_c0x2ecolorpanel();
    setup_c0x2egain_tilde();
    setup_c0x2eincdec();
    setup_c0x2eknob();
    setup_c0x2emenu();
    setup_c0x2emeter_tilde();
    setup_c0x2enumber();
    setup_c0x2enumber_tilde();
    setup_c0x2eplane();
    setup_c0x2epreset();
    setup_c0x2eradio();
    setup_c0x2erslider();
    setup_c0x2escope_tilde();
    setup_c0x2eslider();
    setup_c0x2etab();
    setup_c0x2etoggle();
    
    // Coffee
    setup_c0x2eloadmess();
    setup_c0x2epak();
    setup_c0x2epatcherargs();
    setup_c0x2epatcherinfos();
    setup_c0x2eprepend();

#ifndef _WINDOWS
    var = sys_searchpath;
    while (var)
    {
        sprintf(path, "%s/CreamLibrary",var->nl_string);
        if(strncmp(var->nl_string, "CreamLibrary", 10) == 0)
        {
            sprintf(path, "%s/misc", var->nl_string);
            namelist_append_files(sys_searchpath, path);
            return;
        }
        else if(access(path, O_RDONLY) != -1)
        {
            sprintf(path, "%s/CreamLibrary/misc", var->nl_string);
            namelist_append_files(sys_searchpath, path);
            return;
        }
        var = var->nl_next;
    }
#endif
}

