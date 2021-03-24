--TEST--
Check for with MAGIC_MIME_ENCODING flag
--SKIPIF--
<?php
if ( ! extension_loaded ('filebin') ) {
    print 'skip';
}
?>
--POST--
--GET--
--INI--
--FILE--
<?php

if ( filebin ('modules/filebin.so', MAGIC_MIME_ENCODING) == false )
	echo 'skip';
else
	echo "using with MAGIC_MIME_ENCODING flag success";

?>
--EXPECT--
using with MAGIC_MIME_ENCODING flag success
