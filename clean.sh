#!/bin/bash
# Script to clean buildroot directory.
# Author: Dhruva Koley.

if [ -d "buildroot" ]
then
	cd buildroot
	make distclean
else
	echo "buildroot directory does not exist\n"
fi