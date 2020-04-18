@echo off

IF NOT EXIST ..\build mkdir ..\build
pushd ..\build
rem Compile Glad into Object File
rem cl -nologo -I "..\ext\includes" -MDd -c -FC -Zi "..\ext\libs\glad.c"

cl -nologo -EHsc -MDd -c -FC -Zi ..\code\platform.cpp -I "..\ext\includes"
LINK -nologo -DEBUG -LIBPATH:"..\ext\libs" platform.obj glfw3.lib glad.obj user32.lib gdi32.lib shell32.lib /ignore:4099

popd

