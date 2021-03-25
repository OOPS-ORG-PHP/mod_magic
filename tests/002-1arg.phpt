--TEST--
Check for execute with filemagic ('modules/magic.so')
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

if ( filemagic ('modules/magic.so') == null )
	echo 'skip';
else
	echo "filemagic ('modules/magic.so') success";

?>
--EXPECT--
filemagic ('modules/magic.so') success
