#!/bin/bash

FILE=$1

# See if the third argument is to create or remove a user
if [ $3 = "-a" ] || [ $3 = "-r" ] ; then
	# Check if group exists
	egrep -i "^$2" /etc/group; 
	if [ $? -eq 0 ] ; then
		echo "User Group Found. . . "
	else
		# Create the Group if it doesn't exist
		echo "Creating User Group. . . "
		sudo groupadd $2 
	fi

else 
	echo "Please use either -a to create 
	
fi







