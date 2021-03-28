--TEST--
Check for with MAGIC_MIME_ENCODING flag
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

if ( filemagic ('modules/magic.so', MAGIC_MIME_ENCODING) == false )
	echo 'skip';
else
	echo "using with MAGIC_MIME_ENCODING flag success";

?>
--EXPECT--
using with MAGIC_MIME_ENCODING flag success
