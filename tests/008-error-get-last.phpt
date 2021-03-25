--TEST--
Check for error_get_last function
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
