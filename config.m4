dnl config.m4 for filebin extension

dnl build support C99
AC_PROG_CC_C99

AC_DEFUN([FBIN_HEADER_CHECKED], [
	for HDNAME in $1
	do
		for i in $FILEBIN_PREFIX_PATH
		do
				if test -f $i/include/$HDNAME; then
				FILEBIN_HEADER_DIR=$i/include
				break
			fi
		done

		for i in `sed 's/-I//g' <<< "$CPPFLAGS" `
		do
			if test -f $i/$HDNAME; then
				FILEBIN_HEADER_DIR=$i
				break
			fi
		done

		OLD_CPPFLAGS="$CPPFLAGS"
		if test "x$FILEBIN_HEADER_DIR" != "x"; then
			CPPFLAGS="-I$FILEBIN_HEADER_DIR"
		fi
		AC_CHECK_HEADERS($HDNAME, [], [
			AC_MSG_ERROR([$HDNAME not found.])
		])
		CPPFLAGS="$OLD_CPPFLAGS"
	done
])

AC_DEFUN([FBIN_LIBRARY_CHECKED], [
	if test "x$with_static_magic" = "xyes"; then
		LIBNAME="lib$1.a"
	else
		LIBNAME="lib$1.$SHLIB_SUFFIX_NAME"
	fi

	for i in $FILEBIN_PREFIX_PATH
	do
		if test -f $i/$PHP_LIBDIR/$LIBNAME ; then
			FILEBIN_LIB_DIR=$i/$PHP_LIBDIR
			break
		fi
	done

	for i in `sed 's/-L//g' <<< "$LDFLAGS"`
	do
		test "x$FILEBIN_LIB_DIR" != "x" && break
		if test -f $i/$LIBNAME; then
			FILEBIN_LIB_DIR=$i
			break
		fi
	done

	if test "x$FILEBIN_LIB_DIR" == "x"; then
		AC_MSG_CHECKING([for $2 in -l$1])
		AC_MSG_ERROR([$LIBNAME not found.])
	fi

	if test "x$with_static_magic" = "xyes"; then
		AC_MSG_CHECKING([for $2 in -l$1])
		strings "$FILEBIN_LIB_DIR/$LIBNAME" | grep -q "^$2\$"
		if test $? == 0; then
			AC_MSG_RESULT([yes, $FILEBIN_LIB_DIR/$LIBNAME])
			#FILEBIN_SHARED_LIBADD="-Wl,-Bstatic -lmagic"
			LDFLAGS="$LDFLAGS $FILEBIN_LIB_DIR/$LIBNAME"
			FILEBIN_LIB_DIR=
		else
			AC_MSG_ERROR([$LIBNAME does not include $2.])
		fi
	else
		OLD_LDFLAGS="$LDFLAGS"
		LDFLAGS="-L$FILEBIN_LIB_DIR"
		AC_CHECK_LIB(magic, magic_open, [ ], [ AC_MSG_ERROR([$LIBNAME not found.]) ])
		LDFLAGS="$OLD_LDFLAGS"
	fi
])

PHP_ARG_WITH(filebin, for filebin support,
[  --with-filebin[=DIR]      Include filebin support.  DIR is the file
                          install directory.])

#echo "_______________________________________________________"

if test "x$PHP_EXECUTABLE" = "xNONE"; then
	PHP_EXECUTABLE="/usr/bin/php"
fi

if test "$PHP_FILEBIN" != "no"; then
	AC_DEFINE(HAVE_FILEBIN,1,[ ])

	FILEBIN_PREFIX_PATH="$PHP_FILEBIN /usr /usr/local /opt/file /usr/local/file"

	if test "$with_libdir" != "$PHP_LIBDIR"; then
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
	test -z "$PHP_LIBDIR" && PHP_LIBDIR="lib"

	FILEBIN_PARAMETER=$CFLAGS
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
	FBIN_LIBRARY_CHECKED(magic, magic_open)

	FBIN_HEADER_CHECKED(locale.h fcntl.h magic.h)

	extra_src=""

	PHP_EXPAND_PATH($FILEBIN_HEADER_DIR, FILEBIN_HEADER_DIR)
	PHP_ADD_INCLUDE($FILEBIN_HEADER_DIR)
	if test "x$FILEBIN_LIB_DIR" != "x"; then
		PHP_ADD_LIBRARY_WITH_PATH(magic, $FILEBIN_LIB_DIR, FILEBIN_SHARED_LIBADD)
	fi
	PHP_SUBST(FILEBIN_PARAMTER)
	PHP_SUBST(FILEBIN_SHARED_LIBADD)


	PHP_NEW_EXTENSION(filebin, filebin.c $extra_src, $ext_shared,, \\$(FILEBIN_PARAMETER))
fi
