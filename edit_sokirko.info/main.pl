$in_style = 0;

while (<STDIN>) {

if ($_ =~ /^\s*(<img src="\/images\/header.*.jpg">)\s*$/) {
   print "<a href=\"http://www.sokirko.info\">$1</a>\n";
}
else {
  print;
}

}