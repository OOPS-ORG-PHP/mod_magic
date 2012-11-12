/*
 * Copyright (c) Ian F. Darwin 1986-1995.
 * Software written by Ian F. Darwin and others;
 * maintained 1995-present by Christos Zoulas and others.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice immediately at the beginning of the file, without modification,
 *    this list of conditions, and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by Ian F. Darwin and others.
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *  
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
/*
 * file - find type of a file or files - main program.
 */

#include "file.h"
#include "magic.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/param.h>	/* for MAXPATHLEN */
#include <sys/stat.h>
#include <fcntl.h>	/* for open() */
#ifdef RESTORE_TIME
# if (__COHERENT__ >= 0x420)
#  include <sys/utime.h>
# else
#  ifdef USE_UTIMES
#   include <sys/time.h>
#  else
#   include <utime.h>
#  endif
# endif
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>	/* for read() */
#endif
#ifdef HAVE_LOCALE_H
#include <locale.h>
#endif

#ifdef HAVE_GETOPT_H
#include <getopt.h>	/* for long options (is this portable?)*/
#endif

#include <netinet/in.h>		/* for byte swapping */

#ifndef	lint
FILE_RCSID("@(#)$Id: file.c,v 1.1 2004-08-20 13:53:42 oops Exp $")
#endif	/* lint */

/* for PHP */
#include "php.h"
#include "SAPI.h"

#define OB_DEFAULT_HANDLER_NAME "default output handler"

#ifdef stderr
#undef stderr
#endif

#define printf php_printf
#define fprintf php_error
#define stderr E_WARNING
/* for PHP */


#ifdef S_IFLNK
#define SYMLINKFLAG "L"
#else
#define SYMLINKFLAG ""
#endif

#ifndef MAXPATHLEN
#define	MAXPATHLEN	512
#endif

private int 		/* Global command-line options */
	bflag = 0,		/* brief output format */
	nopad = 0,		/* Don't pad output */
	nobuffer = 0;	/* Do not buffer stdout */

private const char *magicfile = 0;	/* where the magic is */
private const char *default_magicfile = MAGIC;
private char *separator = ":";	/* Default field separator */

private struct magic_set *magic;

private int unwrap(char *);
private void usage(char c, char *s);
#if 0
private int byteconv4(int, int, int);
private short byteconv2(int, int, int);
#endif

int main(int, char *[]);
private void process(const char *, int);
private int load(const char *, int);


/*
 * main - parse arguments and handle options
 */
int
file_main(int argc, char *argv[])
{
	int c;
	int action = 0, didsomefiles = 0, errflg = 0;
	int flags = 0;
	char *home, *usermagic;
	struct stat sb;
#define OPTSTRING	"bcCdf:F:ikLm:nNprsz"
#ifdef HAVE_GETOPT_LONG
	int longindex;
	private struct option long_options[] =
	{
		{"brief", 0, 0, 'b'},
		{"checking-printout", 0, 0, 'c'},
		{"debug", 0, 0, 'd'},
		{"files-from", 1, 0, 'f'},
		{"separator", 1, 0, 'F'},
		{"mime", 0, 0, 'i'},
		{"keep-going", 0, 0, 'k'},
#ifdef S_IFLNK
		{"dereference", 0, 0, 'L'},
#endif
		{"magic-file", 1, 0, 'm'},
#if defined(HAVE_UTIME) || defined(HAVE_UTIMES)
		{"preserve-date", 0, 0, 'p'},
#endif
		{"uncompress", 0, 0, 'z'},
		{"raw", 0, 0, 'r'},
		{"no-buffer", 0, 0, 'n'},
		{"no-pad", 0, 0, 'N'},
		{"special-files", 0, 0, 's'},
		{"compile", 0, 0, 'C'},
		{0, 0, 0, 0},
	};
#endif

#ifdef LC_CTYPE
	setlocale(LC_CTYPE, ""); /* makes islower etc work for other langs */
#endif

#ifdef __EMX__
	/* sh-like wildcard expansion! Shouldn't hurt at least ... */
	_wildcard(&argc, &argv);
#endif

	magicfile = default_magicfile;
	if ((usermagic = getenv("MAGIC")) != NULL)
		magicfile = usermagic;
	else
		if ((home = getenv("HOME")) != NULL) {
			if ((usermagic = malloc(strlen(home) + 8)) != NULL) {
				(void)strcpy(usermagic, home);
				(void)strcat(usermagic, "/.magic");
				if (stat(usermagic, &sb)<0) 
					free(usermagic);
				else
					magicfile = usermagic;
			}
		}

#ifndef HAVE_GETOPT_LONG
	while ((c = getopt(argc, argv, OPTSTRING)) != -1)
#else
	while ((c = getopt_long(argc, argv, OPTSTRING, long_options, &longindex)) != -1)
#endif
		switch (c) {
#ifdef HAVE_GETOPT_LONG
		case 0 :
			if (longindex == 1) {
				usage(c, "");
				return 1;
			}
			break;
#endif
		case 'b':
			++bflag;
			break;
		case 'c':
			action = FILE_CHECK;
			break;
		case 'C':
			action = FILE_COMPILE;
			break;
		case 'd':
			flags |= MAGIC_DEBUG|MAGIC_CHECK;
			break;
		case 'f':
			if(action) {
				usage(c, "");
				return 1;
			}
			if ( load(magicfile, flags) != 0 ) {
				return 1;
			}
			if ( unwrap(optarg) != 0 )
				return 1;

			++didsomefiles;
			break;
		case 'F':
			separator = optarg;
			break;
		case 'i':
			flags |= MAGIC_MIME;
			break;
		case 'k':
			flags |= MAGIC_CONTINUE;
			break;
		case 'm':
			magicfile = optarg;
			break;
		case 'n':
			++nobuffer;
			break;
		case 'N':
			++nopad;
			break;
#if defined(HAVE_UTIME) || defined(HAVE_UTIMES)
		case 'p':
			flags |= MAGIC_PRESERVE_ATIME;
			break;
#endif
		case 'r':
			flags |= MAGIC_RAW;
			break;
		case 's':
			flags |= MAGIC_DEVICES;
			break;
		case 'z':
			flags |= MAGIC_COMPRESS;
			break;
#ifdef S_IFLNK
		case 'L':
			flags |= MAGIC_SYMLINK;
			break;
#endif
		case '?':
		default:
			usage (c, "");
			return 1;
			break;
		}

	switch(action) {
		case FILE_CHECK:
		case FILE_COMPILE:
			magic = magic_open(flags|MAGIC_CHECK);
			if (magic == NULL) {
				(void)fprintf(stderr, "%s\n", strerror(errno));
				return 1;
			}
			c = action == FILE_CHECK ? magic_check(magic, magicfile) : magic_compile(magic, magicfile);
			if (c == -1) {
				(void)fprintf(stderr, "%s\n", magic_error(magic));
				return -1;
			}
			return 0;
		default:
			if ( load(magicfile, flags) != 0 )
				return 1;
	
			break;
	}

	if (optind == argc) {
		if (!didsomefiles) {
			usage(0, "Must existed file for checking");
			return 1;
		}
	}
	else {
		int i, wid, nw;
		for (wid = 0, i = optind; i < argc; i++) {
			nw = strlen(argv[i]);
			if (nw > wid)
				wid = nw;
		}
		for (; optind < argc; optind++) {
			process(argv[optind], wid);
		}
	}

	return 0;
}


private int
load(const char *m, int flags)
{
	if (magic)
		return 0;
	magic = magic_open(flags);
	if (magic == NULL) {
		(void)fprintf(stderr, "%s\n", strerror(errno));
		return 1;
	}
	if (magic_load(magic, magicfile) == -1) {
		(void)fprintf(stderr, "%s\n", magic_error(magic));
		return 1;
	}

	return 0;
}

/*
 * unwrap -- read a file of filenames, do each one.
 */
private int
unwrap(char *fn)
{
	char buf[MAXPATHLEN];
	FILE *f;
	int wid = 0, cwid;

	if (strcmp("-", fn) == 0) {
		f = stdin;
		wid = 1;
	} else {
		if ((f = fopen(fn, "r")) == NULL) {
			(void)fprintf(stderr, "Cannot open `%s' (%s).\n", fn, strerror(errno));
			return 1;
		}

		while (fgets(buf, MAXPATHLEN, f) != NULL) {
			cwid = strlen(buf) - 1;
			if (cwid > wid)
				wid = cwid;
		}

		rewind(f);
	}

	while (fgets(buf, MAXPATHLEN, f) != NULL) {
		buf[strlen(buf)-1] = '\0';
		process(buf, wid);
		if(nobuffer)
			(void) fflush(stdout);
	}

	(void) fclose(f);

	return 0;
}

private void
process(const char *inname, int wid)
{
	const char *type;
	int std_in = strcmp(inname, "-") == 0;

	if (wid > 0 && !bflag)
		(void) printf("%s%s%*s ", std_in ? "/dev/stdin" : inname,
					  separator, (int) (nopad ? 0 : (wid - strlen(inname))), "");

	type = magic_file(magic, std_in ? NULL : inname);
	if (type == NULL)
		printf("ERROR: %s\n", magic_error(magic));
	else
		printf("%s\n", type);
}


#if 0
/*
 * byteconv4
 * Input:
 *	from		4 byte quantity to convert
 *	same		whether to perform byte swapping
 *	big_endian	whether we are a big endian host
 */
private int
byteconv4(int from, int same, int big_endian)
{
	if (same)
		return from;
	else if (big_endian) {		/* lsb -> msb conversion on msb */
		union {
			int i;
			char c[4];
		} retval, tmpval;

		tmpval.i = from;
		retval.c[0] = tmpval.c[3];
		retval.c[1] = tmpval.c[2];
		retval.c[2] = tmpval.c[1];
		retval.c[3] = tmpval.c[0];

		return retval.i;
	}
	else
		return ntohl(from);	/* msb -> lsb conversion on lsb */
}

/*
 * byteconv2
 * Same as byteconv4, but for shorts
 */
private short
byteconv2(int from, int same, int big_endian)
{
	if (same)
		return from;
	else if (big_endian) {		/* lsb -> msb conversion on msb */
		union {
			short s;
			char c[2];
		} retval, tmpval;

		tmpval.s = (short) from;
		retval.c[0] = tmpval.c[1];
		retval.c[1] = tmpval.c[0];

		return retval.s;
	}
	else
		return ntohs(from);	/* msb -> lsb conversion on lsb */
}
#endif

private void
usage(char c, char * s)
{
	if ( c != 0 )
		fprintf(stderr, "-%c is unsupported option\n", c);
	else
		fprintf(stderr, "%s\n", s);
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
