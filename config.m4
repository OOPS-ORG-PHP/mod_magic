dnl config.m4 for magic extension

dnl build support C99
AC_PROG_CC_C99

AC_DEFUN([MAGIC_HEADER_CHECKED], [
	for HDNAME in $1
	do
		for i in $MAGIC_PREFIX_PATH
		do
				if test -f $i/include/$HDNAME; then
				MAGIC_HEADER_DIR=$i/include
				break
			fi
		done

		for i in `sed 's/-I//g' <<< "$CPPFLAGS" `
		do
			if test -f $i/$HDNAME; then
				MAGIC_HEADER_DIR=$i
				break
			fi
		done

		OLD_CPPFLAGS="$CPPFLAGS"
		if test "x$MAGIC_HEADER_DIR" != "x"; then
			CPPFLAGS="-I$MAGIC_HEADER_DIR"
		fi
		AC_CHECK_HEADERS($HDNAME, [], [
			AC_MSG_ERROR([$HDNAME not found.])
		])
		CPPFLAGS="$OLD_CPPFLAGS"
	done
])

AC_DEFUN([MAGIC_LIBRARY_CHECKED], [
	if test "x$with_static_magic" = "xyes"; then
		LIBNAME="lib$1.a"
	else
		LIBNAME="lib$1.$SHLIB_SUFFIX_NAME"
	fi

	for i in $MAGIC_PREFIX_PATH
	do
		if test -f $i/$PHP_LIBDIR/$LIBNAME ; then
			MAGIC_LIB_DIR=$i/$PHP_LIBDIR
			break
		fi
	done

	for i in `sed 's/-L//g' <<< "$LDFLAGS"`
	do
		test "x$MAGIC_LIB_DIR" != "x" && break
		if test -f $i/$LIBNAME; then
			MAGIC_LIB_DIR=$i
			break
		fi
	done

	if test "x$MAGIC_LIB_DIR" == "x"; then
		AC_MSG_CHECKING([for $2 in -l$1])
		AC_MSG_ERROR([$LIBNAME not found.])
	fi

	if test "x$with_static_magic" = "xyes"; then
		AC_MSG_CHECKING([for $2 in -l$1])
		strings "$MAGIC_LIB_DIR/$LIBNAME" | grep -q "^$2\$"
		if test $? == 0; then
			AC_MSG_RESULT([yes, $MAGIC_LIB_DIR/$LIBNAME])
			#MAGIC_SHARED_LIBADD="-Wl,-Bstatic -lmagic"
			LDFLAGS="$LDFLAGS $MAGIC_LIB_DIR/$LIBNAME"
			MAGIC_LIB_DIR=
		else
			AC_MSG_ERROR([$LIBNAME does not include $2.])
		fi
	else
		OLD_LDFLAGS="$LDFLAGS"
		LDFLAGS="-L$MAGIC_LIB_DIR"
		AC_CHECK_LIB(magic, magic_open, [ ], [ AC_MSG_ERROR([$LIBNAME not found.]) ])
		LDFLAGS="$OLD_LDFLAGS"
	fi
])

PHP_ARG_WITH(magic, for magic support,
[  --with-magic[=DIR]      Include magic support.  DIR is the file
                          install directory.])

#echo "_______________________________________________________"

#if test "x$PHP_EXECUTABLE" = "xNONE"; then
#	PHP_EXECUTABLE="/usr/bin/php"
#fi

if test "$PHP_MAGIC" != "no"; then
	AC_DEFINE(HAVE_MAGIC,1,[ ])

	MAGIC_PREFIX_PATH="$PHP_MAGIC /usr /usr/local /opt/file /usr/local/file"

	test -z "$PHP_LIBDIR" && PHP_LIBDIR="lib"
	if test "$with_libdir" != "$PHP_LIBDIR:-lib"; then
		parch=`uname -m`
		if test "$parch" == "x86_64"; then
			if test -d /usr/lib64 || test -d /usr/local/lib64; then
				PHP_LIBDIR="lib64"
			elif test -d /usr/lib/64; then
				# for solaris style
				PHP_LIBDIR="lib/64"
			fi
		fi
	fi

	MAGIC_PARAMETER=$CFLAGS
	PHP_SUBST(CPPFLAGS)
	PHP_SUBST(LDFLAGS)

	AC_MSG_CHECKING(for magic file path)
	AC_ARG_WITH(magic-file, [  --with-magic-file=FILE  magic file path of file package [[default /usr/share/file/magic]]], [
		if test "x$with_magic_file" == "x"; then
			with_magic_file="/usr/share/file/magic"
		fi

		if test -f "$with_magic_file" ; then
			AC_MSG_RESULT($with_magic_file)
			AC_DEFINE_UNQUOTED(MAGIC, "$with_magic_file", [magic file path])
		else
			AC_MSG_ERROR([$with_magic_file file not found])
		fi
	], [
		with_magic_file=/usr/share/file/magic
		if test -f "$with_magic_file" ; then
			AC_MSG_RESULT($with_magic_file)
			AC_DEFINE_UNQUOTED(MAGIC, "$with_magic_file", [magic file path])
		else
			AC_MSG_ERROR([$with_magic_file file not found])
		fi
	])

	dnl
	dnl 64bit does not build because of the -fdic option.
	dnl -Wl, -bstatic options are changed by Libtool.
	dnl
	dnl AC_MSG_CHECKING(for static link with libmagic)
	dnl AC_ARG_WITH(static-magic, [  --with-static-magic     linked with static magic library], [
	dnl 	if test "x$with_static_magic" = "xyes"; then
	dnl 		AC_MSG_RESULT(yes)
	dnl 	else
	dnl 		AC_MSG_RESULT(no)
	dnl 	fi
	dnl ], [
	dnl 	AC_MSG_RESULT(no)
	dnl ])


	AC_CHECK_LIB(z, gzopen)
	MAGIC_LIBRARY_CHECKED(magic, magic_open)

	MAGIC_HEADER_CHECKED(locale.h fcntl.h magic.h)

	extra_src=""

	PHP_EXPAND_PATH($MAGIC_HEADER_DIR, MAGIC_HEADER_DIR)
	PHP_ADD_INCLUDE($MAGIC_HEADER_DIR)
	if test "x$MAGIC_LIB_DIR" != "x"; then
		PHP_ADD_LIBRARY_WITH_PATH(magic, $MAGIC_LIB_DIR, MAGIC_SHARED_LIBADD)
	fi
	PHP_SUBST(MAGIC_PARAMTER)
	PHP_SUBST(MAGIC_SHARED_LIBADD)


	PHP_NEW_EXTENSION(magic, magic.c $extra_src, $ext_shared,, \\$(MAGIC_PARAMETER))
fi
