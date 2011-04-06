@echo off

call hg archive -tfiles C:\Woopsi

rem Tidy up directory structure
cd C:\Woopsi\
del .hg_archival.txt
del .hgignore
del .hgtags
del Woopsi.sln
del ToDo.txt
rmdir Woopsi\images /s /q
rmdir Woopsi\Release /s /q
del Woopsi\Woopsi.vcproj
move README.txt Woopsi\

rem Create documentation
mkdir Woopsi\documentation
"C:\Program Files (x86)\doxygen\bin\doxygen" C:\Woopsi\Doxyfile
move Woopsi\documentation\html\** Woopsi\documentation
rmdir Woopsi\documentation\html
del Doxyfile

del MakeRelease.bat