<?php

if ( ! extension_loaded ('magic') ) {
	if ( ! ini_get (enable_dl) ) {
		fprintf (STDERR, "Can't load magic extension. enable_dl is disabled\n");
		exit (1);
	}

	if ( version_compare (PHP_VERSION, '5.4.0', '<') ) {
		fprintf (STDERR, "magic extension is not loaded\n");
		exit (1);
	} else if ( version_compare (PHP_VERSION, '5.3.0', '<') )
		dl ('magic.so');
	else
		dl ('./modules/magic.so');
}

echo <<<EOF
  *
  * execute with filemagic ('modules/magic.so');
  *
  
EOF;

echo filemagic ('modules/magic.so') . "\n";

echo <<<EOF

  *
  * execute with filemagic ('modules/magic.so', '/usr/share/misc/magic.mgc');
  *
  
EOF;

echo filemagic ('modules/magic.so', '/usr/share/misc/magic.mgc') . "\n";


echo <<<EOF

  *
  * execute with filemagic ('modules/magic.so', MAGIC_MIME_ENCODING);
  *
  
EOF;

echo filemagic ('modules/magic.so', MAGIC_MIME_ENCODING) . "\n";

echo <<<EOF

  *
  * execute with filemagic ('modules/magic.so', MAGIC_MIME, '/usr/share/misc/magic.mgc');
  *
  
EOF;

echo filemagic ('modules/magic.so', MAGIC_MIME, '/usr/share/misc/magic.mgc') . "\n";

echo <<<EOF

  *
  * execute with filemagic ('modules/magic.so', '/usr/share/misc/magic.mgc', MAGIC_MIME);
  *
  
EOF;

echo filemagic ('modules/magic.so', '/usr/share/misc/magic.mgc', MAGIC_MIME) . "\n";

echo <<<EOF

  *
  * \$buf = file_get_contents ('modules/magic.so');
  * execute with filemagic ('DATA:' . \$buf, MAGIC_MIME);
  *
  
EOF;

$buf = file_get_contents ('modules/magic.so');
echo filemagic ('DATA:' . $buf, '/usr/share/misc/magic.mgc', MAGIC_MIME) . "\n";

echo <<<EOF

  *
  * error controls:
  *
  * if ( filemagic ('wrong_path', MAGIC_MIME) == false ) {
  *     ini_set ('track_errors', true);
  *     echo "## php_errormsg : " . \$php_errormsg . "\\n";
  *     print_r (error_get_last ());
  * }
  *
  
EOF;

if ( filemagic ('wrong_path', MAGIC_MIME) == false ) {
	ini_set ('track_errors', true);
	echo "## php_errormsg : " . $php_errormsg . "\n";
	print_r (error_get_last ());
}
?>
