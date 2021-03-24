--TEST--
Check for execute with filebin ('modules/filebin.so')
--SKIPIF--
<?php
if ( ! extension_loaded ('filebin') ) {
    print 'skip';
}
?>
--POST--
--GET--
--INI--
--FILE--
<?php

if ( filebin ('modules/filebin.so') == false )
	echo 'skip';
else
	echo "filebin ('modules/filebin.so') success";

?>
--EXPECT--
filebin ('modules/filebin.so') success
