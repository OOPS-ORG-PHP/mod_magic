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

if ( filemagic ('modules/magic.so1') == false ) {
	ini_set ('track_errors', true);
	echo $php_errormsg . "\n";
}
?>
--EXPECT--
modules/magic.so1 file not found.
