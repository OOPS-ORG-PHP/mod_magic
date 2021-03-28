--TEST--
Check for magic presence
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
echo "magic extension is available";
/*
        you can add regression tests for your extension here

  the output of your test code has to be equal to the
  text in the --EXPECT-- section below for the tests
  to pass, differences between the output and the
  expected text are interpreted as failure

        see php4/README.TESTING for further information on
  writing regression tests
*/
?>
--EXPECT--
magic extension is available
