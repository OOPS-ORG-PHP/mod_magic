<?

dl("./modules/filebin.so");

if ( $argc < 2 ) {
  echo "Usage: $argv[0] filepath\n";
  exit;
}

echo "Case 1: given -m /usr/share/file/magic option value\n\n";


echo "        \$ary = array ('-m', '/usr/share/file/magic');\n" .
     "        \$c = filebin (\$argv[1], \$ary, count(\$ary));\n\n";

echo "Result:\n";

$ary = array ('-m', '/usr/share/file/magic');
$c = filebin ($argv[1], $ary, count($ary));
echo $c;


echo "\n\n";
echo "Case 2: given no option\n\n";

echo "        \$c = filebin (\$argv[1])\n\n";

echo "Result:\n";
$c = filebin ($argvs[1]);
echo $c;

echo "\n";

exit;
?>
