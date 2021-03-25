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
  * USAGE:
  *        filemagic (path[, file_flags = MAGIC_NONE[, magic_file = NULL]]);
  *        filemagic (path[, magic_file = NULL]);
  *
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

?>
