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

/*
 * Proto Types:
 *
 * filebin (string path, int flag = MAGIC_NONE, string magic_file = MAGIC_FILE): string|NULL
 * filebin (string path, string magic_file = MAGIC_FILE, int flag = MAGIC_NONE): string|NULL
 * filebin (string path, string magic_file = MAGIC_FILE): string|NULL
 *
 * Returns:
 *
 *  If true, return magic strings, and fail, return NULL;
 *
 * Flags:
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
 *
 * Constants:
 *
 * MAGIC_FILE => /usr/share/file/magic
 */

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
