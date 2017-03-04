# FastenerPattern

![logo_256x256.png](src/icons/logo/logo_256x256.png)

**FastenerPattern** is a solution for designing fastener patterns (i.e. *bolted* and *riveted* joints) in Aerostructures, for Windows, Mac OS X and Linux.

Its core is developed in C++, based on the [Qt framework](https://www.qt.io/ "Qt framework"). It uses the [Boost::Units](http://www.boost.org/ "Boost") module.

## Dimensional Analysis

[Dimensional Analysis](https://en.wikipedia.org/wiki/Dimensional_analysis "https://en.wikipedia.org/wiki/Dimensional_analysis")  is the analysis of relationships between different physical quantities by identifying their fundamental dimensions.

**FastenerPattern** uses the Dimensional Analysis abilities of the *Boost::Units module* to verify the dimensional homogeneity of the mechanical quantities and formulas used by the software.

This enables a better Verification & Validation, since the code is more robust (less permissive) and more reliable.


## Constraint Programming

Optionally, FastenerPattern can use a third-party *Constraint Programming* (CP) solver called GeCode.
GeCode won all gold medals in all categories at the MiniZinc Challenges from 2008 to 2012.

More info [here](http://www.gecode.org/ "http://www.gecode.org/") and [here](https://en.wikipedia.org/wiki/Gecode "https://en.wikipedia.org/wiki/Gecode").

GeCode is downloadable [here](http://www.gecode.org/download.html "http://www.gecode.org/download.html").

Remark:
On Windows, GeCode can be built with MSVC 2013 (32 or 64bits) or MSVC 2015 (32 or 64bits).
At the time of writing those lines, compiling GeCode with MinGW causes problems.
FastenerPattern can still be compiled with MinGW (32bits), however the CP features are disabled.


## Installation

Download the most recent package from the [download](https://github.com/setvisible/fastenerpattern/releases "Last Releases") page, unzip it and run it.

Read the [User's Manual](user-manual/User_Manual.pdf "User_Manual.pdf").


## Build

1. Download `FastenerPattern.tar.gz` [here](https://github.com/setvisible/fastenerpattern/releases "Last Releases").

2. Unzip:

        $ tar zxvf FastenerPattern.tar.gz
        $ cd pch2csv/

3. Build with CMake

     - On Unix / Mac OS X

            $ mkdir -p build
            $ cd build/
            $ cmake ..
            $ make -j
            $ make install

     - On Windows (MinGW) [Warning: CP solver disabled]

            > mkdir build
            > cd build/
            > cmake .. -G "MinGW Makefiles"
            > make -j
            > make install

       (the last command could require admin rights)

     - On Windows (MSVC 2015)

            > mkdir build
            > cd build/
            > cmake .. -G "Visual Studio 14 2015"
            > start .

         Then, double click the Visual Studio project (vcxproj).


## License

The code is released under the GNU **LGPLv3** open source license.

[https://www.gnu.org/licenses/lgpl-3.0.en.html](https://www.gnu.org/licenses/lgpl-3.0.en.html)
