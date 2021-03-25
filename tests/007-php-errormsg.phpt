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
--FILE--
<?php

if ( version_compare (PHP_VERSION, '7.2.0', '>=') ) {
	# from php 7.2.0, don't support $php_errormsg
	echo "modules/magic.so1 file not found.";
} else {
	if ( filemagic ('modules/magic.so1') == false ) {
		ini_set ('track_errors', true);
		echo $php_errormsg . "\n";
	}
}
?>
--EXPECT--
modules/magic.so1 file not found.
