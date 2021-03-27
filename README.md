# PHP file magic extension

## Description

This extension is similar to the [fileinfo](https://www.php.net/manual/en/book.fileinfo.php) extension, but only one FileBin function is provided to make it a simple use.

The filemagic function returns the same result as the result of the /bin/file command as a string.

***The v2 branch*** supports PHP4.3 and PHP 5. For PHP 7 and above, use the mater branch.

Since autotools version is too low, older versions, including PHP 4.2.0, are difficult to implement the QA environment. Therefore, it is officially supported from 4.3.

## Installation

```bash
[root@host magic]$ phpize
[root@host magic]$ ./configure
[root@host magic]$ make test PHP_EXECUTABLE=/usr/bin/php
[root@host magic]$ make install
```

If the libmagic library is not in the OS standard path, use the ```--with-magic=@libMagic_prefix@``` option.

```bash
[root@host magic]$ phpize
[root@host magic]$ ./configure --with-magic=/opt/libmagic
```

## Loading

regist magic.so in php.ini or php sub configuration.

```ini
extension = magic.so
```

## Proto types:
```php
filemagic (string path, int flag = MAGIC_NONE, string magic_path = MIGIC_FILE): string|false
filemagic (string path, string magic_path = MIGIC_FILE, int flag = MAGIC_NONE): string|false
```

If the libmagic library is not in the OS standard path, use the ```--with-magic=@libMagic_prefix@``` option.

**Return value :**
if success, return mime strings.
if failure, return false.


## Constant
Name | Value
-- | --
**MAGIC_FILE** | /usr/share/file/magic

## Flags
See also https://github.com/file/file/blob/master/src/magic.h.in

Name | Description
-- | --
**MAGIC_NONE** | No special handling.
**MAGIC_DEBUG** | Print debugging messages to stderr.
**MAGIC_SYMLINK** | If the file queried is a symlink, follow it.
**MAGIC_COMPRESS** | If the file is compressed, unpack it and look at the contents.
**MAGIC_DEVICES** | If the file is a block or character special device, then open the device and try to look in its contents.
**MAGIC_MIME_TYPE** | Return a MIME type string, instead of a textual description.
**MAGIC_MIME_ENCODING** | Return a MIME encoding, instead of a textual description.
**MAGIC_CONTINUE** | Return all matches, not just the first.
**MAGIC_CHECK** | Check the magic database for consistency and print warnings to stderr.
**MAGIC_PRESERVE_ATIME** | On systems that support utime(2) or utimes(2), attempt to preserve the access time of files analyzed.
**MAGIC_RAW** | Don't translate unprintable characters to a \ooo octal representation.
**MAGIC_ERROR** | Treat operating system errors while trying to open files and follow symlinks as real errors, instead of printing them in the magic buffer.
**MAGIC_NO_CHECK_APPTYPE** | Check for EMX application type (only on EMX).
**MAGIC_NO_CHECK_ASCII** | Check for various types of ascii files.
**MAGIC_NO_CHECK_COMPRESS** | Don't look for, or inside compressed files.
**MAGIC_NO_CHECK_ELF** | Don't print elf details.
**MAGIC_NO_CHECK_FORTRAN** | Don't look for fortran sequences inside ascii files.
**MAGIC_NO_CHECK_SOFT** | Don't consult magic files.
**MAGIC_NO_CHECK_TAR** | Don't examine tar files.
**MAGIC_NO_CHECK_TOKENS** | Don't look for known tokens inside ascii files.
**MAGIC_NO_CHECK_TROFF** | Don't look for troff sequences inside ascii files.
**MAGIC_MIME** | Returns MIME type with encoding. (MAGIC_MIME_TYPE\|MAGIC_MIME_ENCODING)
**MAGIC_APPLE** | Return the Apple creator and type.
**MAGIC_NO_CHECK_TEXT** | Don't check for text files.
**MAGIC_NO_CHECK_CDF** | Don't check for cdf files.
**MAGIC_NO_CHECK_ENCODING** | Don't check text encodings.


## Usage

**Most common usage :**
```php
<?php
if ( ($buf = filemagic ('modules/magic.so')) != false )
    echo "$buf\n";
?>

Resutl:
ELF 64-bit LSB shared object, x86-64, version 1 (SYSV), dynamically linked, BuildID[sha1]=0b1c92efa1398676c226544835a64d0edd68f491, not stripped
```

**Execute with custom MAGIC file :**
```php
<?php
if ( ($buf = filemagic ('modules/magic.so', '/usr/share/misc/magic.mgc')) != false )
    echo "$buf\n";
?>

Result:
ELF 64-bit LSB shared object, x86-64, version 1 (SYSV), dynamically linked, BuildID[sha1]=0b1c92efa1398676c226544835a64d0edd68f491, not stripped
```

**Get only file mime type :**
```php
if ( ($buf = filemagic ('modules/magic.so', MAGIC_MIME_ENCODING)) != false )
    echo "$buf\n";
?>

Result:
binary
```

**Get only file mime type with custom MAGIC file :**
```php
if ( ($buf = filemagic ('modules/magic.so', MAGIC_MIME, '/usr/share/misc/magic.mgc')) != false )
    echo "$buf\n";
?>

Result:
application/x-sharedlib; charset=binary
```

**switch order of 2th and 3th argurments :**
```php
if ( ($buf = filemagic ('modules/magic.so', '/usr/share/misc/magic.mgc', MAGIC_MIME)) != false )
    echo "$buf\n";
?>

Result:
application/x-sharedlib; charset=binary
```

**filemagic buffer control :**
```php
$buf = file_get_contents ('./php_magic.h');
if ( ($buf = filemagic ('DATA:' . $fbuf, MAGIC_MIME) != false ) {
	echo $buf . "\n";
}

Result:
text/x-c; charset=us-ascii
```
- if want to get file magic information with buffer data instead of file, use 'DATA:' prefix.


**filemagic function error control :**
```php
<?php
if ( ($buf = filemagic ('wrong_path')) == false ) {
	if ( version_comapre(PHP_VERSION, '7.2.0', '<') ) {
	    ini_set ('track_errors', true)
		printf ("ERROR: %s\n", $php_errormsg);
	}
	if ( version_compare (PHP_VERSION, '5.2.0', '>=') ) {
		$err = error_get_last ();
		print_r ($err);
	}
}
?>

Result:
ERROR: wrong_path file not found.
Array
(
    [type] => 2
    [message] => wrong_path file not found.
    [file] => /some/path/test.php
    [line] => 2
)
```
- $php_errormsg must have the track_errors setting enabled.
- $php_errormsg was deprecated in 7.2.0.
- error_get_last () function is supported from 5.2.0 and after
- The filemagic function does not call any error handlers when an error occurs. However, the error message is assigned to $php_errormsg or error_get_last ().
