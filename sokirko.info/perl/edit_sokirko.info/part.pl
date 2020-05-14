$filename = $ARGV[0];

while (<STDIN>) {

if ($filename =~ /Part[0-9]+/) {
 if ($_ =~ /^\s<h2><i><b>¬. *Ë *À. *—Œ »– Œ<\/b><\/i><\/h2> *$/) {
    next;
 }
 else {
   if ($_ =~ /^<body>$/) { 
       print;
       print "<br><br>";  
   }
   else {
      print;
   }
 }
}
else {
  print;
}

}