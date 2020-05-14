$filename = $ARGV[0];

while (<STDIN>) {

if ($_ =~ /^\s*<head>/) {
   print;
   if ($filename =~ /economy.Zes/) {
     $h = "zes.jpg";
   }
   else {
     $h = "header2.jpg"
   }
   print "<img src=\"http://www.sokirko.info/images/$h\">\n"; 
}
elsif ($_ =~ /^\s*<body> *<h2><i><b>В. *и *Л. *Сокирко<\/b><\/i><\/h2>\s*$/) {
 print "<body>\n";
}
else {
 print;
}

}