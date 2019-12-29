$filename = $ARGV[0];

while (<STDIN>) {

if ($_ =~ /^<img src="http:\/\/www.sokirko.info\/images\/header2.jpg">/) {
   if ($filename =~ /ecomomy.Zes/) {
     print "<img src=\"http://www.sokirko.info/images/zes.jpg\">;\n";
   }
   else {
     print;
   }
}
else {
   print;
}

}