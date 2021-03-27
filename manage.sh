#!/bin/bash

source /usr/share/annyung-release/functions.d/bash/functions

errmsg () {
	echo "$*" 1>&2
}

usage () {
	echo "Usage: $0 [clean|pack|test [php-version]]"
	exit 1
}

normal="$( tput sgr0 )"
bold="$( tput bold )"
black="$( tput setaf 0 )"
red="$( tput setaf 1 )"
green="$( tput setaf 2 )"
yellow="$( tput setaf 3 )"
blue="$( tput setaf 4 )"
megenta="$( tput setaf 5 )"
cyan="$( tput setaf 6 )"
white="$( tput setaf 7 )"
bblack="${bold}${black}"
bred="${bold}${red}"
bgreen="${bold}${green}"
byellow="${bold}${yellow}"
bblue="${bold}${blue}"
bmegenta="${bold}${megenta}"
bcyan="${bold}${cyan}"
bwhite="${bold}${white}"
tcolor="${bblack}$( tput setab 7 )"

opts=$(getopt -u -o h -l help -- "$@")
[ $? != 0 ] && usage

set -- ${opts}
for i
do
	case "$i" in
		-h|--help)
			usage
			shift
			;;
		--)
			shift
			break
			;;
	esac
done

mode="${1}"

case "${mode}" in
	clean)
		cat <<-EOL
			[ -f Makefile ] && make distclean
			rm -rf autom4te.cache build include modules
			rm -f .deps Makefile* ac*.m4 compile
			rm -f config.h* config.nice configure* config.sub config.guess
			rm -f install-sh ltmain.sh missing mkinstalldirs run-tests.php

			rm -f package.xml
			find ./tests ! -name '*.phpt' -a ! -name '*.txt' -a -type f
			----->
		EOL

		[ -f Makefile ] && make distclean
		rm -rf autom4te.cache build include modules
		rm -f .deps Makefile* ac*.m4 dyn*.m4 compile tags
		rm -f config.h* config.nice configure* config.sub config.guess
		rm -f install-sh ltmain.sh missing mkinstalldirs run-tests.php
		rm -f tests/*.{diff,exp,log,out,php,sh,mem}
		;;
	pack)
		cp -af package.xml.tmpl package.xml
		list=$(grep "md5sum" ./package.xml | sed -r 's/.*"@|@".*//g')

		for i in ${list}
		do
			md5s=$(md5sum "$i" | awk '{print $1}')
			perl -pi -e "s!\@${i}\@!${md5s}!g" ./package.xml
		done

		curdate=$(date +'%Y-%m-%d')
		curtime=$(date +'%H:%M:%S')

		perl -pi -e "s!\@curdate\@!${curdate}!g" ./package.xml
		perl -pi -e "s!\@curtime\@!${curtime}!g" ./package.xml

		pecl package
		rm -f package.xml
		;;
	test)
		# don't support scandir 4.0 ~ 4.2

		PHPBIN=/opt/php-qa/php${2}/bin/php
		PHPIZE=/opt/php-qa/php${2}/bin/phpize
		PHPCONFIG=/opt/php-qa/php${2}/bin/php-config
		PHP_OPT=""

		#PHP_VERSION_ID="$(
		#	${PHPCONFIG} --version | awk '{ printf ("%d%02d%02d", $1, $2, $3); }' FS="."
		#)"

		if [[ $# == 2 ]]; then
			./manage.sh clean
			echo "${PHPIZE} && ./configure"
			${PHPIZE} && ./configure && make -j8 || exit 0

			if [[ ! -f ./run-tests.php ]]; then
				set $( echo "${1} ${2} sample.php" )
				PHP_OPT="-n -d 'enable_dl=1' -d 'safe_mode=0'"
			fi
		fi

		PHP_OPT+=" -d 'track_errors=1' -d 'extension_dir=./modules/' -d 'extension=magic.so'"

		if [[ -f tests/${3}.php ]]; then
			cat <<-EOL
				${bgreen}------------------------------------------------------------------------
				Sample code execution:

				${bcyan}${PHPBIN} ${PHP_OPT} test/${3}.php
				${bgreen}------------------------------------------------------------------------${normal}

			EOL

			${PHPBIN} ${PHP_OPT} test/${3}.php
			exit $?
		elif [[ -f ${3} ]]; then
			cat <<-EOL
				${bgreen}------------------------------------------------------------------------
				Sample code execution:

				${bcyan}${PHPBIN} ${PHP_OPT} ${3}
				${bgreen}------------------------------------------------------------------------${normal}

			EOL

			eval "${PHPBIN} ${PHP_OPT} ${3}"
			exit $?
		fi

		cat <<-EOL
			${bwhite}~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			${tcolor}** MAKE test::                                                       ${normal}
			${bwhite}~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

			${bcyan}make test PHP_EXECUTABLE=${PHPBIN}
			${bwhite}~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~${normal}

		EOL
		make test PHP_EXECUTABLE=${PHPBIN} <<< n

		exit $?;
		;;
	*)
		errmsg "Unsupport mode '${1}'"
		exit 1
esac

exit 0
