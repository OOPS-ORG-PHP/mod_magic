--TEST--
Check for data control
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

$tmp = file_get_contents ('modules/magic.so');
if ( ($buf = filemagic ('DATA:'. $tmp, MAGIC_MIME)) != false ) {
	echo "$buf\n";
}
?>
--EXPECT--
application/x-sharedlib; charset=binary
