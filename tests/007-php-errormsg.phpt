--TEST--
Check for $php_errormsg variables
--SKIPIF--
<?php
if ( ! extension_loaded ('magic') ) {
    print 'skip';
}
# Since PHP 8.0, track_errors is removed, so force skip test
if ( version_compare (PHP_VERSION, '8.0.0', '>=') ) {
	print 'skip';
}
?>
--POST--
--GET--
--INI--
track_errors=1
; for track_errors deprecate on PHP 7.2 and after
error_reporting = E_ALL & ~E_DEPRECATED
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
