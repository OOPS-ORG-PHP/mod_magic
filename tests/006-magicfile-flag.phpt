--TEST--
Check for with swaping order of MAGIC_MIME flag and user magic file
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

if ( filebin ('modules/filebin.so', '/usr/share/misc/magic.mgc', MAGIC_MIME) == false )
	echo 'skip';
else
	echo "success";

?>
--EXPECT--
success
