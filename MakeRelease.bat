@echo off

echo Archiving Mercurial repository
call hg archive -tfiles C:\Woopsi

echo Tidying archived directory structure
del C:\Woopsi\.hg_archival.txt
del C:\Woopsi\.hgignore
del C:\Woopsi\.hgtags
del ToDo.txt
rmdir C:\Woopsi\Woopsi\images /s /q
rmdir C:\Woopsi\Woopsi\Release /s /q
move C:\Woopsi\README.txt Woopsi\

echo Creating documentation
mkdir C:\Woopsi\Woopsi\documentation
"C:\Program Files (x86)\doxygen\bin\doxygen" C:\Woopsi\Doxyfile
move C:\Woopsi\Woopsi\documentation\html\** C:\Woopsi\Woopsi\documentation
rmdir C:\Woopsi\Woopsi\documentation\html
del C:\Woopsi\Doxyfile

echo Zipping it up
"C:\Program Files\7-Zip\7z" a -r -tzip C:\Woopsi\woopsi-nds-1.x.zip C:\Woopsi\Woopsi
rmdir C:\Woopsi\Woopsi /s /q

del C:\Woopsi\MakeRelease.bat