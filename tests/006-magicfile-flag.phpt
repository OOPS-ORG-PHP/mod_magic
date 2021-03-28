--TEST--
Check for with swaping order of MAGIC_MIME flag and user magic file
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

if ( filemagic ('modules/magic.so', '/usr/share/misc/magic.mgc', MAGIC_MIME) == false )
	echo 'skip';
else
	echo "success";

?>
--EXPECT--
success
