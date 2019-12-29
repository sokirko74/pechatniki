$name = $ARGV[0];
$table_file = $ARGV[1];
open T, "<$table_file";
while (<T>) {
 chomp;
 ($n,$f, $i) = split;
 $name2flv{$n} = $f;
 $name2img{$n} = $i;
}
close $T;

if (! defined $name2flv{$name}) {
 die "cannot find $name in  flv table";
}

$flv =  $name2flv{$name};
$img =  $name2img{$name};

$in_ref = 0;
$in_start_body = 0;

while (<STDIN>) {
if ($_ =~ /<body>/) {
 $in_start_body = 1;
 print;
}
elsif ($_ =~ /^(.*)<script type="text.javascript">/) {
   push @header, $1;
   print "\n<table> <tr> <td width=\"70%\">\n";
   for $c (@header) {
       print $c;
   }


print  <<HTML;
</td> <td>
<h2><a href=\"$flv\"><img src=\"mini.jpg\"> </a> </h2>
</td> 
<td>
<table height=180>
 <tr>
  <td>
   <a href="$flv"> Смотреть онлайн </a> 
  </td>
 </tr>
 <tr height="80%"> <td><td></tr>
 <tr>
  <td>
    <a href="$img"> Отдельные слайды </a>
  </td>
 </tr>
</table>
</td>
</tr> </table>


HTML

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