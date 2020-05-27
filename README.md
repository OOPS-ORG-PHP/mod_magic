# PHP file extension

## Description

filebin PHP 확장은 /bin/file 명령의 결과와 동일하게 동작한다.

## Installation

```bash
[root@host filebin]$ phpize
[root@host filebin]$ # --with-libdir 옵션은 64bit 환경에서만 지정합니다.
[root@host filebin]$ ./configure --with-libdir=lib64
[root@host filebin]$ make test PHP_EXECUTABLE=/usr/bin/php
[root@host filebin]$ make install
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
