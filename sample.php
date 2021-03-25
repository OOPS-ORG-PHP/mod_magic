<?php

if ( ! extension_loaded ('magic') ) {
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

?>
