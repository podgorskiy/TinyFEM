@echo off
pushd %~dp0
if not exist ..\boost_config mkdir ..\boost_config
if not exist ..\boost_config\boost_config.h call :copy_config

popd
goto :eof

:copy_config
echo copy config\user.hpp.in to ..\boost_config\boost_config.h
copy config\user.hpp.in ..\boost_config\boost_config.h>nul
goto :eof