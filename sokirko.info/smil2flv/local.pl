while (<STDIN>) {
if ($_ =~ /^<title>$/) {
  $s=<STDIN>;
  if ($s =~ /^ *�. +� +�. +�������/) {
     print "<title>$s";
  } 
  else {
    print $s;
  }
}
else {
  print;
}
}