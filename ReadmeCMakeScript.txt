CMakeScript is an extension to CMake, a cross-platform, open-source build system. 
CMakeScript adds Java Script as alternative script language to CMake's home-grown macro language. 
CMakeScript comes with a visual debugger for Java Script.

Hosted at:
http://sourceforge.net/projects/cmakescript/



Build instructions:

Qt 4.5 is needed 
(it also build with older Qt version but then no debugger is available.)

Build as usual, e.g. in the folder "build" on the same level as the cmakescript checkout:
cmake ../cmakescript

Test with Example/Hello/CMakeLists.js in a new folder:
../build/bin/cmake../cmakescript/Example/Hello

To start the debugger add "debugger" in the script.




