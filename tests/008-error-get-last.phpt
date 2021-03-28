--TEST--
Check for error_get_last function
--SKIPIF--
<?php
if ( ! extension_loaded ('magic') ) {
	if ( version_compare(PHP_VERSION, "5.1.0", "<") ) {
		dl ('magic.so');
		if ( ! extension_loaded ('magic') )
    		print 'skip';
	} else {
		print 'skip';
	}
}
?>
--POST--
--GET--
--INI--
--FILE--
<?php
if ( version_compare(PHP_VERSION, "5.1.0", "<") )
	dl ('magic.so');

if ( filemagic ('modules/magic.so1') == false ) {
	if ( version_compare (PHP_VERSION, '5.2.0', '<') ) {
		# error_get_last function was added from PHP 5.2.0
		echo "E_WARNING: modules/magic.so1 file not found.";
		exit;
	}
	$p = error_get_last ();
	if ( $p['type'] == E_WARNING )
		$s = 'E_WARNING';
	else
		$s = 'E_UNKNONW';
	printf ("%s: %s\n", $s, $p['message']);
}
?>
--EXPECT--
E_WARNING: modules/magic.so1 file not found.
