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

if ( version_compare (PHP_VERSION, '7.2.0', '>=') ) {
	# from php 7.2.0, don't support $php_errormsg
	echo "modules/magic.so1 file not found.";
} else {
	if ( filemagic ('modules/magic.so1') == false ) {
		if ( ! ini_get ('track_errors') ) {
			echo "track_errors is disable. To fix, enable track_errors ini option\n";
			exit;
		}
		echo $php_errormsg . "\n";
	}
}
?>
--EXPECT--
modules/magic.so1 file not found.
