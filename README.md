## PD Enhanced
<p>Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8</p>
<p>All rights reserved.</p>

##### Presentation

Pd Enhanced aims to facilitate the creation of objects for Pure Data. It offers an architecture similar to the Max objects that allows to easily create Pure Data objects with a source code for Max objects. One of the main axis is the creation of graphical user interfaces. Pd Chocolate is a small set of externals, not essential but that can be very useful (there’re a vu~ and a nbx~ that don’t have the blink effect of their homologue). The objects are always in developpement so ask if you want a stable version.

- The automatic generation inlets/outlets.
- The background color, border size, border color are easy to set.
- The GUI objects can be resized with click and drag.
- The graphics API use layers and "clip" the drawings within the box boundaries.
- The attributes (size, colors, values and whatever you want) are saved with the patch.
- The inlets/outlets are dynamics.
- The management of mouse motion and keys is simplified.
- The convertion from Max to Pd and from Pd to Max 6 is really easy.
and a lot of other stuff, I can't enumerate everything !

#### Author :

###### Pierre Guillot

#### To compile :

Add the sources files to your project. Include "pd_enhanced.h" file instead of "m_pd.h". Compile like any PD Project. 

###### XCode projects :

The frameworks and headers search paths are : "$SRCROOT/../../../../SDKs/pd-extended/pd/src".
The "pd-extented" folder contains the sources of Pure Data. If you want to contribute to the project, it would better to keep the same path.
Nevertheless, it could be easier to link the project with Pure Data installation in the "Application" folder.

###### Visual Studio projects :

The Visual Studio projects are linked with the Pure Data installation folder : "C:\Program Files (x86)\pd\bin\". 
Note that you need to create a pd.lib that you can create from the pd.dll file.
To create pd.lib :

- Open the "Visual Studio Command Prompt" : "Start/Programs/Microsoft Visual Studio/Tools".
- Change to the pd.dll directory : "cd C:\Program Files (x86)\pd\bin\".
- Tape : "dumpbin /exports C:\yourpath\yourlib.dll"
- Copy only the functions names :
1    0 00017770 function_name_0 -> function_name_0
2    1 00017710 function_name_1 -> function_name_1
ect...
- Name the text file "pd.def" and add "EXPORT" at the first line
- Tape : "lib /def:pd.def /OUT:pd.lib"

You will have a pd.lib in the Pure Data installation folder and be able to compile.

###### Linux projects :

It's "under construction" but it should be similar to the vanilla Pd compilation exept that you have to include all the files of the project.

#### ToDo :
- Add automatic properties window with the attributes. Need to find the best way to avoid tcl files. (important)
- Find the connection with the object to send a "count" (like Max count) in dsp method. (medium)
- Secure the rezize i/o method. (low)

#### License : GPL.


