#!/bin/bash

NAME="nicenorme"
kernel=$(uname)

WHITE="\x1b[37;01m"
CYAN="\x1b[36;01m"
PINK="\x1b[35;01m"
BLUE="\x1b[34;01m"
YELLOW="\x1b[33;01m"
GREEN="\x1b[32;01m"
RED="\x1b[31;01m"
BLACK="\x1b[30;01m"
RESET="\x1b[0m"
if [ "$kernel" = "Linux" ]; then
	WHITE="\033[37;01m"
	CYAN="\033[36;01m"
	PINK="\033[35;01m"
	BLUE="\033[34;01m"
	YELLOW="\033[33;01m"
	GREEN="\033[32;01m"
	RED="\033[31;01m"
	BLACK="\033[30;01m"
	RESET="\033[0m"
fi

get_answer () {
	printf "$CYAN$NAME$RESET is already installed: do you want to reinstall? (Y/n)	"
	answered=false
	install=false
	while [ "$answered" = false ]; do
		read answer
		if [ "${answer::1}" = "Y" ] || [ "${answer::1}" = "y" ] || [ "${answer::1}" = "" ]; then
			install=true
			answered=true
		elif [ "${answer::1}" = "N" ] || [ "${answer::1}" = "n" ] ; then
			answered=true
		else
			echo "please provide valid input"
		fi
	done
}

install () {
	printf "Installing $CYAN$NAME$RESET at $WHITE$FILE$NAME$RESET...\n"
	make > /dev/null &
	sleep 1
	sudo cp $NAME $FILE

	make fclean > /dev/null
}

print_help () {
	printf "usage:		./install [--help] [--update] [<args>] <install_dir>\n\n"
	echo "example:	./install $HOME/here/"
}

update () {
	git pull
	FILE=$(which nicenorme | rev | cut -d ' ' -f1 | rev)
	FILE=${FILE%/*}
	FILE=${FILE}/
	install
}

FILE=/usr/local/bin/
if [ $# -eq 1 ]; then
	if [ "$1" = "--help" ]; then
		print_help
		exit ;
	elif [ "$1" = "--update" ]; then
		if ! [ -f "$FILE$NAME" ]; then
			install
			exit ;
		fi
		update
		exit ;
	else
		FILE="$1"
	fi
elif [ $# -ne 0 ]; then
	echo "too many arguments"
	exit ;
fi

if [ -f "$FILE$NAME" ]; then
	get_answer
fi

if [ "$install" = false ]; then
	exit ;
else
	install
fi
