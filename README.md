# FastenerPattern

![logo_256x256.png](src/icons/logo/logo_256x256.png)

**FastenerPattern** is a solution for designing fastener patterns
(i.e. *bolted* and *riveted* joints) in Aerostructures, for Windows, Mac OS X and Linux.

Its core is developed in C++, based on the [Qt framework](https://www.qt.io/ "Qt framework").
It uses the [Boost.Units](http://boost.org/libs/units "http://boost.org/libs/units") module.


## Boost.Units (Dimensional Analysis)

[Boost.Units](http://boost.org/libs/units "http://boost.org/libs/units")
is the kind of library which could save a rocket.
The idea is to no longer manipulate simple numerical values, but physical quantities.
Indeed, we don't want to add masses to lengths.
We want to add masses with different orders of magnitude.

In short, we strongly type all numerical quantities as multiples of the
[International System](https://en.wikipedia.org/wiki/International_system
"https://en.wikipedia.org/wiki/International_system") units.

[Dimensional Analysis](https://en.wikipedia.org/wiki/Dimensional_analysis
"https://en.wikipedia.org/wiki/Dimensional_analysis")
is the analysis of relationships between different physical quantities
by identifying their fundamental dimensions.

This software uses the *Dimensional Analysis* abilities of
[Boost.Units](http://boost.org/libs/units "http://boost.org/libs/units")
to verify the dimensional homogeneity of the mechanical quantities and formulas used by the software.
It makes the code of this software more robust (=less permissive) and more reliable.


## Computational Geometry

### Triangle (Delaunay triangulation)

[Triangle](http://www.cs.cmu.edu/~quake/triangle.html "http://www.cs.cmu.edu/~quake/triangle.html"),
by Jonathan Shewchuk, is an award-winning C language code that generates Delaunay triangulations,
constrained Delaunay triangulations (forced to have certain edges), and quality-conforming
Delaunay triangulations (which avoid small angles by inserting extra points).
It has been widely used for finite element analysis and is fast and robust.

It is available at [http://www.cs.cmu.edu/~quake/triangle.html](http://www.cs.cmu.edu/~quake/triangle.html "http://www.cs.cmu.edu/~quake/triangle.html").



## Installation

The following intructions are in the
[manual](user-manual/User_Manual.pdf "User_Manual.pdf").

1. Download the most recent package
[here](https://github.com/setvisible/fastenerpattern/releases "Last Releases").

2. Unzip:

        $ tar zxvf FastenerPattern.tar.gz
        $ cd FastenerPattern/

3. Build

     - On Unix or Mac OS X

            $ mkdir -p build
            $ cd build/
            $ cmake ..              # Perform a system check.

            Then
            $ make -j               # Build the program(s).
            $ make strip            # Remove unneeded symbols.

            Finally
            $ sudo make install     # Install the program(s).

        Enjoy FastenerPattern using `fastenerpattern`.

        Rem: On MacOSX, you may must type
        `$ chmod +x install.sh`
        after `$ make` for installing.


     - On Windows (with MinGW)

            > mkdir build
            > cd build/
            > cmake .. -G "MinGW Makefiles"

            Then
            > make -j
            > make install/strip

            Finally (could require admin rights)
            > make install



     - On Windows (with MSVC 2015)

            > mkdir build
            > cd build/
            > cmake .. -G "Visual Studio 14 2015"

            > start .
            Then, double click the Visual Studio project (vcxproj).

        Enjoy FastenerPattern running `fastenerpattern.exe`.


## License

Copyright 2016-2017 FastenerPattern Contributors, some rights reserved.

FastenerPattern is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License - GNU LGPL -
as published by the Free Software Foundation, either version 3 of the License,
or (at your option) any later version.

FastenerPattern is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with FastenerPattern. If not, see
<https://www.gnu.org/licenses/lgpl-3.0.en.html>.
