rem 32bit
rem 32bit
rem 32bit
rem debug
copy ..\lib-src\portmidi\pm_dylib\Debug\portmidi-dynamic.dll .\portmidi-dynamic.dll
copy ..\lib-src\portmidi\pm_dylib\Debug\portmidi-dynamic.dll debug\portmidi-dynamic.dll
copy ..\lib-src\portmidi\debug\portmidi.dll .\portmidi.dll
copy ..\lib-src\portmidi\debug\portmidi.dll debug\portmidi.dll
rem release
copy ..\lib-src\portmidi\pm_dylib\release\portmidi-dynamic.dll release\portmidi-dynamic.dll
copy ..\lib-src\portmidi\release\portmidi.dll release\portmidi.dll


rem 64bit
rem 64bit
rem 64bit
rem debug
rem copy ..\lib-src\portmidi(x64)\pm_dylib\x64\Debug\portmidi-dynamic.dll .\portmidi-dynamic.dll
copy ..\lib-src\portmidi(x64)\x64\Debug\portmidi-dynamic.dll .\portmidi-dynamic.dll
copy ..\lib-src\portmidi(x64)\x64\Debug\portmidi-dynamic.dll .\x64\debug\portmidi-dynamic.dll
rem copy ..\lib-src\portmidi(x64)\x64\debug\portmidi.dll .\portmidi.dll
copy ..\lib-src\portmidi(x64)\x64\debug\portmidi.dll .\portmidi.dll
copy ..\lib-src\portmidi(x64)\x64\debug\portmidi.dll .\x64\debug\portmidi.dll
rem release
copy ..\lib-src\portmidi(x64)\x64\release\portmidi-dynamic.dll .\x64\release\portmidi-dynamic.dll
copy ..\lib-src\portmidi(x64)\x64\release\portmidi.dll .\x64\release\portmidi.dll


