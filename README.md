# PHP file extension

## Description

This extension is similar to the [fileinfo](https://www.php.net/manual/en/book.fileinfo.php) extension, but only one FileBin function is provided to make it a simple use.

The filebin function returns the same result as the result of the /bin/file command as a string.

## Installation

```bash
[root@host filebin]$ phpize
[root@host filebin]$ ./configure
[root@host filebin]$ make test PHP_EXECUTABLE=/usr/bin/php
[root@host filebin]$ make install
```

If the libmagic library is not in the OS standard path, use the ```--with-filebin=@libMagic_prefix@``` option.

```bash
[root@host filebin]$ phpize
[root@host filebin]$ ./configure --with-filebin=/opt/libmagic
```

## Loading

regist filebin.so in php.ini or php sub configuration.

```ini
; for PHP 7.1 and before
extension = filebin.so

; for PHP 7.2 and after
extension = filebin
; and also regist with absolute path
extension = /path/filebin.so
```

## Usage

See also included sample.php on source
