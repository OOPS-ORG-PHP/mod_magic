--TEST--
Check for $php_errormsg variables
--SKIPIF--
<?php
if ( ! extension_loaded ('magic') ) {
    print 'skip';
}
?>
--POST--
--GET--
--INI--
track_errors=1
--FILE--
<?php

if ( filemagic ('modules/magic.so1') == false ) {
	ini_set ('track_errors', true);
	if ( ! ini_get ('track_errors') ) {
		echo "track_errors is disable. To fix, enable track_errors ini option\n";
		exit;
	}
	echo $php_errormsg . "\n";
}
?>
--EXPECT--
modules/magic.so1 file not found.
