$id =0;
$fps=$ARGV[0];
$dir=$ARGV[1];
                                                                  
while (<STDIN>) {
 if($_ =~ /<img src="([^"]+)".*dur="([0-9]+)s"/)
 {
  $f = $1;
  $d = $2;
  push @slides, "$f $d";
  $id++;
  #if ($id > 5) {
  # last;
  #} 
 }
}

$cnt = scalar @slides;
$name="nida";
#$width=640;
#$height=480;
$width=1024;
$height=768;
print STDERR "Slides:$cnt\n";

for $s (@slides) {
 ($f,$d) = split(/ /,$s);
 for ($i=0;  $i < $fps*$d; $i++) {
  print "$dir$f\n"; 
 }
}

