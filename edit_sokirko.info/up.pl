$filename = $ARGV[0];

while (<STDIN>) {
$s = $_;
$s =~ s/<HTML>/<html>/;
$s =~ s/<\/HTML>/<\/html>/;
$s =~ s/<HEAD>/<head>/;
$s =~ s/<\/HEAD>/<\/head>/;
$s =~ s/<TITLE>/<title>/;
$s =~ s/<\/TITLE>/<\/title>/;
$s =~ s/<BODY>/<body>/;
$s =~ s/<\/BODY>/<\/body>/;
$s =~ s/<META/<meta/;
$s =~ s/ALIGN="RIGHT"/align="right"/;
$s =~ s/ALIGN="CENTER"/align="center"/;
$s =~ s/HTTP-EQUIV/http-equiv/;
$s =~ s/CONTENT=/content=/;
print $s;
}