sub createorder(){
   for ($i=0; $i<scalar @order; $i++){
      if ($i>0) {
        $prev{$order[$i]} =  $order[$i-1];
      }
      if ($i<scalar @order-1) {
        $next{$order[$i]} =  $order[$i+1];
      }
   }
}

sub printorder($){
 my $c = shift;
 print OUT "<table width=600> <tr> <td>\n";
     if (defined $prev{$c}){
        print OUT "<a href=../pris/".$prev{$c}.".htm> предыдущая </a>\n";
     }
 print OUT "</td><td><a href=\"../$main.htm\">оглавление</a></td><td>\n";
 if (defined $next{$c}){
        print OUT "<a href=../pris/".$next{$c}.".htm> следующая </a>\n";
 }
 print OUT "</td></tr></table>\n";
}

sub endfile(){
 print OUT "</html>";
 close OUT;
 if ($nl < 3) {
   die  "chapter $chap $cn{$chap} is too small";
 }
}

$dir="pris";
$main="prisyaghnye";
$num = 0;
system ("rm -rf pris");
system ("mkdir pris");
$chap = "start";
open OUT,">$main.htm";
while (<STDIN>){
 if ($_ =~ /<!--start-->/){
   $cont = 1;
   
 }
 if ($cont == 1) {
    if ($_ =~ /<a href=#([^>]+)>(.*)<\/a>/) {
       $cn{$1} = "$2"; 
       push @order, $1;
       print OUT "<a href=$dir/$1.htm>$2</a><br>";
       next; 
    }
 }
 if ($_ =~ /<!--end-->/){
   createorder();
   $cont = 0;

 }
 if ($_ =~ /<a +name=([^>]+)>/){
     $temp = $1;
    
     if ($num > 0) {
        print OUT "<br>\n";
        printorder($chap);
     }
  
     endfile();

     $chap = $temp;
     if  (! defined $cn{$chap}) {
       die "cannot find chapter $chap in contents";
     }
     $num++;
     $nl = 0;
     open OUT, ">pris/$chap.htm";
     print OUT "<html>\n";
     print OUT "<head>\n";
     print OUT "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=windows-1251\">\n";
     print OUT "<title>Сокирко В.В. ".$cn{$chap}."</title>\n";
     printorder($chap);
 }
 print OUT $_;
 $nl++;
}

endfile();
 