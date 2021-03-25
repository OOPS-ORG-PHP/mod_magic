--TEST--
Check for data control
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
$tmp = file_get_contents ('modules/magic.so');
if ( ($buf = filemagic ('DATA:'. $tmp, MAGIC_MIME)) != false ) {
	echo "$buf\n";
}
?>
--EXPECT--
application/x-sharedlib; charset=binary
