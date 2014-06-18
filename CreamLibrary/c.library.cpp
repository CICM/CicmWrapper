/*
// Copyright (c) 2012-2014 Eliott Paris, Julien Colafrancesco & Pierre Guillot, CICM, Universite Paris 8.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "c.library.h"

extern "C" void setup_c0x2elibrary(void)
{
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
	//setup_c0x2evoronoy();

	// Coffee
	setup_c0x2eloadmess();
	setup_c0x2epak();
	setup_c0x2epatcherargs();
	setup_c0x2epatcherinfos();
    //setup_c0x2epatchermess();
	setup_c0x2eprepend();

    pd_library_add_folder("CreamLibrary", "misc");
}

