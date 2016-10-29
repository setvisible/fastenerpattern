# FastenerPattern

FastenerPattern is a solution for designing Fastener Patterns (i.e. Bolted and Riveted Joints) in Aerostructures, for Windows, Mac OS X and Linux.

Its core is developed in C++, based on the Qt framework.


## Download

Go to the [download](https://github.com/setvisible/fastenerpattern/releases "Download") page

**NOTE: Work in progress**

## Build

1. Download `FastenerPattern.tar.gz` [here](https://github.com/setvisible/fastenerpattern/releases "Download").

2. Unzip:

        $ tar zxvf FastenerPattern.tar.gz
        $ cd pch2csv/

3. Build with CMake

     - On Unix / Mac OS X

            $ mkdir -p build
            $ cd build/
            $ cmake ..
            $ make -j64
            $ make install

     - On Windows (MinGW)

            > mkdir build
            > cd build/
            > cmake .. -G "MinGW Makefiles"
            > make -j64
            > make install

       (the last command could require admin rights)

     - On Windows (MSVC)

            > mkdir build
            > cd build/
            > cmake .. -G "Visual Studio 14 2015"
            > start .

         Then, double click the Visual Studio project (vcxproj).


## Usage

Just run the executable.

The user's manual is [here](https://github.com/setvisible/fastenerpattern/user-manual "User's manual directory").

## License

The code is released under the GNU **LGPLv3** open source license.

[https://www.gnu.org/licenses/lgpl-3.0.en.html](https://www.gnu.org/licenses/lgpl-3.0.en.html)
