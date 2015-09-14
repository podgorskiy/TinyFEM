#!/bin/bash

WorkDir=`dirname "$0"`
cd "$WorkDir"

if [ ! -d ../boost_config ]
then
	mkdir -p ../boost_config
fi

if [ ! -f ../boost_config/boost_config.h ]
then
	cp config/user.hpp.in ../boost_config/boost_config.h
fi

