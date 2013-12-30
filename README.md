## PD Enhanced
<p>Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8</p>

##### Presentation

Pd Enhanced aims to facilitate the creation of objects for Pure Data. One of the main axis is the creation of graphical user interfaces and signal processing. Pd Chocolate and Pd Coffee are externals libraries not essential but very useful.

- The inlets/outlets are automatically generated.
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

Add the sources files to your project. Include "pd_enhanced.h" file instead of "m_pd.h". Compile like any Pd Project.
You can use the projects to compile the externals : XCode for Mac Os, Visual Studio for Windows and Code::Block for Linux.
Projects are compiled against Pd Extented 0.43 then the externals are not compatible with older versions of Pd but are compatible
with Pd 0.45. Note that if you compile against Pd 0.45, the externals won't work with Pd 0.43.

###### XCode && Code::Block projects :

The frameworks and headers search paths are : "$SRCROOT/../../../../SDKs/pd-extended/pd/src".
The "pd-extented" folder contains the sources of Pure Data. If you want to contribute to the project, it would be better to keep the same path.
If you want to use one of this projects, just duplicate it and change its name.

###### Visual Studio projects :

The Visual Studio projects are linked with the Pure Data installation folder : "C:\Program Files (x86)\pd\bin\".
This path will change in the futur and will be : "$SRCROOT/../../../../SDKs/pd-extended/pd/src".
Note that you need to create a pd.lib that you can create from the pd.dll file.
To create pd.lib :

- Open the "Visual Studio Command Prompt" : "Start/Programs/Microsoft Visual Studio/Tools".
- Change to the pd.dll directory : "cd C:\Program Files (x86)\pd\bin\".
- Tape : "dumpbin /exports C:\yourpath\yourlib.dll"
- Copy only the functions names :
- - 1    0 00017770 function_name_0 -> function_name_0
- - 2    1 00017710 function_name_1 -> function_name_1
- - ect...
- Name the text file "pd.def" and add "EXPORTS" at the first line
- Tape : "lib /def:pd.def /OUT:pd.lib"
<p>You will have a pd.lib in the Pure Data installation folder and be able to compile.</p>

#### ToDo :
- Update the properties window : attributes order, categories, etc. (low)
- Find the connection with the object to send a "count" (like Max count) in dsp method. (medium)
- Secure the resize i/o method. (low)
- Assit method.

#### License : GPL.


