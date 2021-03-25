--TEST--
Check for with MAGIC_MIME flag and user magic file
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

if ( filemagic ('modules/magic.so', MAGIC_MIME, '/usr/share/misc/magic.mgc') == null )
	echo 'skip';
else
	echo "success";

?>
--EXPECT--
success
