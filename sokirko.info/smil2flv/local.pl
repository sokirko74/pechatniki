while (<STDIN>) {
if ($_ =~ /^<title>$/) {
  $s=<STDIN>;
  if ($s =~ /^ *¬. +Ë +À. +—Œ »– Œ/) {
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