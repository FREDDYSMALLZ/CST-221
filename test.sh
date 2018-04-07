# Author: Chris Hyde
# Class: CST-221
# Assignment: Unix Family of Operating Systems
# Date: April 7, 2018

#!/bin/sh

# Objective: test 5 commands
echo "Moving to Documents directory from Desktop directory."
echo "... "
cd
cd Documents 
echo "Searching Places for prescott key word."
echo "... "
echo "Found the following matches for the keyword prescott in Places:"
grep -i prescott Places
echo "Creating a new file to store matches called PrescottZips:"
echo "... "
grep -i prescott Places >PrescottZips
echo "File created Documents now contains the following files:"
ls 
echo "Compareing files PrescottZips to MyAddress and print matching lines"
echo "... "
comm -12 MyAddress PrescottZips
echo "Shell Script Execution complete!"
echo "Have a nice day"

