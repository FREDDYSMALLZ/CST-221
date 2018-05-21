#!/bin/bash

# Store the first argument as a variable
pass="$1"

#Make sure pass is not < 8
if [ ${#pass} -lt 8 ] ; then
	echo "The password did not meet minumum length requirement."
else
	#Make sure pass contains at least 1 numeric character
	if [[ "$pass" =~ [0-9] ]] ; then
		
		#Make sure pass contains at leas one of the specified special characters
		if [[ "$pass" =~ [@#$%^'&'+-=] ]] ; then
			echo "The password you entered is correct"
		else
			echo "The password must contain one of the following characters: @#$%^&+-=" 
		fi
	else
		echo "The password must contain at least one numeric character."
	fi
fi


