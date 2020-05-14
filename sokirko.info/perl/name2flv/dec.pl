require Encode;
use utf8;

open F, "<$ARGV[0]";
while (<F>) {
    $line = $_; 
	Encode::from_to($line, 'cp-1251', 'utf-8'); 

    if ($line =~ /<title>([^.]+)\. (.*)<\/title>/) {
#   if ($line =~ /<title>(.*)<\/title>/) {
		$html_title = $2;
		$html_title =~ s/"//g;
	}
}
close F;

#print "==========  $html_title\n";

while(<STDIN>) {
	$line = $_; 
	if ($line =~ /<meta name="title" content="([^"]*)"\/>/) {
		if (( ($1 eq "unk") || ($1 eq "") || ($1 =~ /^[\-A-z0-9]+$/) ) && ($html_title ne "")) {
			print "\t\t<meta name=\"title\" content=\"$html_title\"/>\n";
			next;
		}
	};
	if ($line =~ /<meta name="author" content="([^"]*)"\/>/) {
		if (( ($1 eq "unk") || ($1 eq ""))) {
		      print "\t\t<meta name=\"author\" content=\"В. и Л. Сокирко; Партизанское кино\"/>\n";
			  next;
		}
	}
    $line =~ s/(\\u2116)/р/eg; 
    $line =~ s/(\\u[0-9a-h]+)/chr(hex(substr($1,2,length($1)-2))-16)/eg; 
	print $line;
}