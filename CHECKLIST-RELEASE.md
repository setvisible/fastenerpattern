# Checklist for Release

### Testing

 - Make sure FastenerPattern works
 - Run auto tests
 - Run manual & performance tests
 - Ensure they all passed


### Pre-Building

 - Change `APP_VERSION` in `version.pri`
 - Change `MAJOR.MINOR.PATCH` in `CMakeLists.txt`
 - List the changes with `git log --pretty=oneline` and added them to `CHANGELOG`
 - Make sure `CHANGELOG` has a proper date set
 - Commit (use **Git GUI**)
 - Tag the commit with a new version number: `git tag -a vXXX -m "FastenerPattern version XXX"` where XXX is the version
 - Push with `git push origin vXXX`
 - Verify with `git show vXXX`


### Building the targets

- Build the 32 bits target as 'Release'
- Build the 64 bits target as 'Release'

### Post-Building

#### [Unix only]

- Check the version in both targets with `$ FastenerPattern --version`
- Tar the target with:
 * the GLIDER examples

--> Target name : `FastenerPattern-vXXXX-unix32.zip` where XXX is the version


#### [Windows only]

- Check the version in both targets  with `> FastenerPattern.exe --version`
- Make sure you copy the relevant Qt DLLs and the Windows platform plugin (qwindows.dll):

     platforms\qwindows.dll	The Qt Windows platform plugin  
     Qt5Core.dll               The Qt Core module  
     Qt5Gui.dll                The Qt GUI module  
     Qt5Widgets.dll            The Qt Widgets module  
     LIBWINPTHREAD-1.DLL       MinGW
     LIBGCC_S_DW2-1.DLL        MinGW
     LIBSTDC++-6.DLL           MinGW

- Using the **Dependency Walker** tool, command is `depends <application executable>`. This will provide a list of the libraries that your application depends on and other information.

- Finally, use `windeployqt --release`command.

- Zip the target with:
 * the CPFP solver
 * the User's Manual (from ./user-manual/*.pdf)
 * the LICENSE file
 * the DLLs

--> Target name : `FastenerPattern-vXXXX-win32.zip` where XXX is the version


### Releasing on Github.io

- Go to the [releases](https://github.com/setvisible/fastenerpattern/releases) page
- Edit the tag X.Y.Z
- Upload the targets
- Publish as Pre-Release and write the announce:
 * Title: "FastenerPattern version X.Y"
 * Description: the list of changes


That's all folks

