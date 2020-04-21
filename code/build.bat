@echo off

IF NOT EXIST ..\build mkdir ..\build
pushd ..\build

rem DEBUG COMPILATION
rem cl -nologo -I "..\ext\includes" -MDd -c -FC -Zi "..\ext\libs\glad.c"
rem cl -nologo -EHsc -MDd -c -FC -Zi ..\code\platform.cpp -I "..\ext\includes"
rem LINK -nologo -DEBUG -LIBPATH:"..\ext\libs" platform.obj glfw3.lib glad.obj user32.lib gdi32.lib shell32.lib /ignore:4099

rem RELEASE COMPILATION
cl -nologo -I "..\ext\includes" -MD -c -FC -O2 "..\ext\libs\glad.c"
cl -nologo -MD -c -FC -O2 ..\code\platform.cpp -I "..\ext\includes"
LINK -nologo -OPT:REF -LIBPATH:"..\ext\libs" platform.obj glfw3-release.lib glad.obj user32.lib gdi32.lib shell32.lib /ignore:4099

popd

