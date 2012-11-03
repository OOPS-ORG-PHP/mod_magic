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

if ( $argc < 2 ) {
  echo "Usage: $argv[0] filepath\n";
  exit;
}

echo <<<EOF
Case 1: given -m /usr/share/file/magic option value


        \$ary = array ('-m', '/usr/share/file/magic');
        \$c = filebin (\$argv[1], \$ary, count(\$ary));

Result:

EOF;

$ary = array ('-m', '/usr/share/file/magic');
$c = filebin ($argv[1], $ary, count($ary));
echo $c;

$c = filebin ($argv[1]);

echo <<<EOF


Case 2: given no option

        \$c = filebin (\$argv[1])

Result:

EOF;

$c = filebin ($argv[1]);
echo $c;

echo "\n";

?>
