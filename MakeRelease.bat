@echo off

echo Archiving Mercurial repository
call hg archive -tfiles C:\Woopsi

echo Tidying archived directory structure
cd C:\Woopsi\
del .hg_archival.txt
del .hgignore
del .hgtags
del Woopsi.sln
del ToDo.txt
rmdir Woopsi\images /s /q
rmdir Woopsi\Release /s /q
move README.txt Woopsi\

rem Create documentation
mkdir Woopsi\documentation
"C:\Program Files (x86)\doxygen\bin\doxygen" C:\Woopsi\Doxyfile
move Woopsi\documentation\html\** Woopsi\documentation
rmdir Woopsi\documentation\html
del Doxyfile

rem Zip it up
"C:\Program Files\7-Zip\7z" a -r -tzip C:\Woopsi\woopsi-nds-1.x.zip C:\Woopsi\Woopsi
rmdir Woopsi /s /q

del MakeRelease.bat