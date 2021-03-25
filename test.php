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

/*
 * flags
 * 
 * MAGIC_NONE
 * MAGIC_DEBUG
 * MAGIC_SYMLINK
 * MAGIC_COMPRESS
 * MAGIC_DEVICES
 * MAGIC_MIME_TYPE
 * MAGIC_CONTINUE
 * MAGIC_CHECK
 * MAGIC_PRESERVE_ATIME
 * MAGIC_RAW
 * MAGIC_ERROR
 * MAGIC_MIME_ENCODING
 * MAGIC_MIME
 * MAGIC_APPLE
 * MAGIC_NO_CHECK_COMPRESS
 * MAGIC_NO_CHECK_TAR
 * MAGIC_NO_CHECK_SOFT
 * MAGIC_NO_CHECK_APPTYPE
 * MAGIC_NO_CHECK_ELF
 * MAGIC_NO_CHECK_TEXT
 * MAGIC_NO_CHECK_CDF
 * MAGIC_NO_CHECK_TOKENS
 * MAGIC_NO_CHECK_ENCODING
 */

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
