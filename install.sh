NAME="nicenorme"
kernel=$(uname)

get_answer () {
	echo "${NAME} is already installed: do you want to reinstall? (Y/n)"
	answered=false
	install=false
	while [ "$answered" = false ]; do
		read answer
		if [ "${answer::1}" == "Y" ] || [ "${answer::1}" == "y" ] || [ "${answer::1}" == "" ]; then
			echo "Installing ${NAME}..."
			install=true
			answered=true
		elif [ "${answer::1}" == "N" ] || [ "${answer::1}" == "n" ] ; then
			answered=true
		else
			echo "please provide valid input"
		fi
	done
}

if [ "$kernel" == "Darwin" ]; then
	FILE=/usr/local/bin/nicenorme
	if ! [ -f "$FILE" ]; then
		get_answer
	fi
else
	echo "LIENUUX"
fi