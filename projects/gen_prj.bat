@echo off 
SET MAKETOOL=..\tools\premake4.exe 
%MAKETOOL% vs2012
%MAKETOOL% gmake
