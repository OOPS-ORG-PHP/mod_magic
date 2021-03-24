<?php

if ( ! extension_loaded ('filebin') ) {
	if ( version_compare (PHP_VERSION, '5.4.0', '<') ) {
		fprintf (STDERR, "filebin extension is not loaded\n");
		exit (1);
	} else if ( version_compare (PHP_VERSION, '5.3.0', '<') )
		dl ('filebin.so');
	else
		dl ('./modules/filebin.so');
}

echo <<<EOF
  *
  * execute with filebin ('modules/filebin.so');
  *

EOF;

echo filebin ('modules/filebin.so') . "\n";

echo <<<EOF

  *
  * execute with filebin ('modules/filebin.so', '/usr/share/misc/magic.mgc');
  *

EOF;

echo filebin ('modules/filebin.so', '/usr/share/misc/magic.mgc') . "\n";


echo <<<EOF

  *
  * execute with filebin ('modules/filebin.so', MAGIC_MIME_ENCODING);
  *

EOF;

echo filebin ('modules/filebin.so', MAGIC_MIME_ENCODING) . "\n";

echo <<<EOF

  *
  * execute with filebin ('modules/filebin.so', MAGIC_MIME, '/usr/share/misc/magic.mgc');
  *

EOF;

echo filebin ('modules/filebin.so', MAGIC_MIME, '/usr/share/misc/magic.mgc') . "\n";

echo <<<EOF

  *
  * execute with filebin ('modules/filebin.so', '/usr/share/misc/magic.mgc', MAGIC_MIME);
  *

EOF;

echo filebin ('modules/filebin.so', '/usr/share/misc/magic.mgc', MAGIC_MIME) . "\n";

?>
