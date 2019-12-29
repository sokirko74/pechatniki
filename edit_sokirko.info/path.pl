$filename = $ARGV[0];

while (<STDIN>) {
$s = $_;
$s =~ s/http:\/\/www.sokirko.info\//\//;
$s =~ s/http:\/\/sokirko.info\//\//;
print $s;
}