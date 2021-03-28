--TEST--
Check for with MAGIC_MIME flag and user magic file
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

if ( filemagic ('modules/magic.so', MAGIC_MIME, '/usr/share/misc/magic.mgc') == false )
	echo 'skip';
else
	echo "success";

?>
--EXPECT--
success
