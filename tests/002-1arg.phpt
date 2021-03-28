--TEST--
Check for execute with filemagic ('modules/magic.so')
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

if ( filemagic ('modules/magic.so') == false )
	echo 'skip';
else
	echo "filemagic ('modules/magic.so') success";

?>
--EXPECT--
filemagic ('modules/magic.so') success
