@echo off
set day=%date:~7,2%
set month=%date:~4,2%
set year=%date:~10,4%
set version=%year%%month%%day%
set archname=UE4_Crescendo_Engine_%version%
set archpath=%cd%\..\Archive\%archname%\Engine
set engpath=%cd%\..\UnrealEngine\Engine
set roboargs=/NJS

rem Copy all relevant engine files
echo Creating new engine archive...
echo Copying files...
Robocopy "%engpath%\Binaries" "%archpath%\Binaries" /E /XF *.pdb *.module %roboargs%
Robocopy "%engpath%\Build" "%archpath%\Build" /E /XF *.pdb %roboargs%
Robocopy "%engpath%\Config" "%archpath%\Config" /E %roboargs%
Robocopy "%engpath%\Content" "%archpath%\Content\Editor\Slate" /E %roboargs%
Robocopy "%engpath%\Content" "%archpath%\Content\EditorBlueprintResources" /E %roboargs%
Robocopy "%engpath%\Content" "%archpath%\Content\EditorEditorKismetResources" /E %roboargs%
Robocopy "%engpath%\Plugins" "%archpath%\Plugins" /E /XF *.pdb %roboargs%
Robocopy "%engpath%\Shaders" "%archpath%\Shaders" /E %roboargs%
echo ...finished copying! Compressing...

rem Compress files in 7-Zip format
%cd%\7zip\7za.exe a -t7z %cd%\..\Archive\%archname%.7z %archpath% -r
echo ...finished compressing! Archive complete: %archname%.7z
echo Updated engine version to %value%.
