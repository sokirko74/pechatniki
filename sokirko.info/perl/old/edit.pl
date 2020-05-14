$name = $ARGV[0];
$table_file = $ARGV[1];
open T, "<$table_file";
while (<T>) {
 chomp;
 ($n,$f) = split;
 $name2flv{$n} = $f;
}
close $T;

if (! defined $name2flv{$name}) {
 die "cannot find $name in  flv table";
}
$flv =  $name2flv{$name};

$in_ref = 0;
$in_start_body = 0;

while (<STDIN>) {
if ($_ =~ /<body>/) {
 $in_start_body = 1;
 print;
}
elsif ($_ =~ /^(.*)<script type="text.javascript">/) {
   push @header, $1;
   print "\n<table> <tr> <td width=\"80%\">\n";
   for $c (@header) {
       print $c;
   }
   print "</td> <td>\n";
   print "<h2><a href=\"$flv\"><img src=\"mini.jpg\"> Смотреть онлайн</a> </h2>";
   print  "</td> </tr> </table>\n\n";
   #print "<!-- video_ref -->\n";
   $in_ref = 1;
   $in_start_body = 0;
}
elsif ( $in_start_body == 1 ) {
  push @header, $_
}
elsif ($_ =~ /<\/ol>(.*)$/) {
   $in_ref = 0;
   print $1;
}
elsif ($in_ref == 0) {
  print;
}

}