#!/bin/sh

while true
do
	echo "Select"
	echo -e "\t1 to enter SVN"
	echo -e "\t2 to commit"
	echo -e "\t3 to update"
	echo -e "\t4 to check the difference between working copy and Head revision"
	echo -e "\t5 to add a directory or a file"
	echo -e "\t6 to create a new repositry"
	echo -e "\t7 to clear the Terminal"
	echo -e "\t8 to EXIT"
	
	read text
	
	case $text in
		1) `ssh group02@192.168.1.152`;;
		
		2) echo "Enter the message to be shown when successfully committed"
			read msg ;
		   	svn commit -m "$msg" ;;
		
		   
		3) echo -e "\tPress H to update to Head revision \n\tPress S to update to Specific revision";
			read update_choice;
			case $update_choice in
				H) svn update;;
				S) echo "Enter the revision number";
				   read r;
				   svn update -r$r;
			esac;;
			
		4) svn diff;;
		
		5) echo "Enter the path of directory or file";
			read path;
			svn add $path;;
			
		
		6) echo "Enter repositry name";
		   read repo_name;
		   `svnadmin create "$repo_name"`;;
		
		7) clear;;
		
		8) exit;;
	
		*) echo -e "\n\t\t!!!!! Enter a valid input !!!!!\n";;
	
	esac	
done
