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
    
    struct _canvasenvironment
    {
        t_symbol *ce_dir;      /* directory patch lives in */
        int ce_argc;           /* number of "$" arguments */
        t_atom *ce_argv;       /* array of "$" arguments */
        int ce_dollarzero;     /* value of "$0" */
        t_namelist *ce_path;   /* search path */
    };
    
    extern t_namelist *sys_externlist;
    extern t_namelist *sys_searchpath;
    extern t_namelist *sys_staticpath;
    extern t_namelist *sys_helppath;
    extern t_namelist *namelist_append_files(t_namelist *listwas, const char *s);
    extern t_namelist *namelist_append(t_namelist *listwas, const char *s, int allowdup);
    extern t_symbol* sys_libdir;
	
	void setup_c0x2elibrary(void)
	{
		char path[MAXPDSTRING];
#ifdef _WINDOWS
		t_canvasenvironment *e = NULL;
#endif
		t_namelist* var = NULL;
    
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
        //setup_c0x2epatchermess();
		setup_c0x2eprepend();

#ifdef _WINDOWS

        
		if(!canvas_getcurrent())
			return;
		
		e = canvas_getenv(canvas_getcurrent());
		if(e)
		{
			var = e->ce_path;
			while (var)
			{
				sprintf(path, "%s/CreamLibrary",var->nl_string);
				if(strncmp(var->nl_string, "CreamLibrary", 10) == 0)
				{
					sprintf(path, "%s/misc", var->nl_string);
					namelist_append(e->ce_path, path, 1);
					return;
				}
				else if(access(path, O_RDONLY) != -1)
				{
					sprintf(path, "%s/CreamLibrary/misc", var->nl_string);
					namelist_append(e->ce_path, path, 1);
					return;
				}
				var = var->nl_next;
			}
		}
#else
        
		var = sys_searchpath;
		while (var)
		{
			sprintf(path, "%s/CreamLibrary",var->nl_string);
			if(strncmp(var->nl_string, "CreamLibrary", 10) == 0)
			{
				sprintf(path, "%s/misc", var->nl_string);
				namelist_append(sys_searchpath, path, 1);
				return;
			}
			else if(access(path, O_RDONLY) != -1)
			{
				sprintf(path, "%s/CreamLibrary/misc", var->nl_string);
				namelist_append(sys_searchpath, path, 1);
				return;
			}
			var = var->nl_next;
		}
#endif
	}
    
}

