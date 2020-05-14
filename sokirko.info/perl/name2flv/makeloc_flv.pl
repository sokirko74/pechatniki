use File::Basename;

$name = $ARGV[0];
print STDERR "set link to $name.flv\n";

$inTable = 1;
while (<STDIN>) {
	push @a, $_;
}

for ($i=0; $i < scalar @a; $i++) {
	$_ = $a[$i];

if (! ($_ =~ /^<table> <tr> <td width="70%">$/)) {
   	print;
}
else { 
	$rest = 0;
	$heading = "";
	for ($i++; $i < scalar @a; $i++)  {
		$_ = $a[$i];
		if ($_ =~ /^<\/tr> <\/table>$/) {
			last;
		}
		if ($_ =~ /video.yandex.ru/) {
			$rest = 1;
		}
		if ($rest == 0) {
			$heading .= $_;
		}
	}
	$heading =~ s/<.?td>//g;
print <<HTML;
    <!-- start_heading -->    
	<table>
    <tr>
    <td width="70%">
       $heading
    <td>
    <td>
       <a href="$name.flv/#hq">
              <img src="mini.jpg"> <br>
              <h2> Смотреть  </h2> 
        </a> 
    </td>
    </tr>
	</table>
    <!-- end_heading -->    
HTML
}
}