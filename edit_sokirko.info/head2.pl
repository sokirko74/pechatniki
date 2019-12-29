while (<STDIN>) {

if (! ($_ =~ /^\s*<meta http-equiv=.Content-Type. content=.application.xhtml.xml; charset=windows-1251..>\s*$/)) {
   print;
}
else { 
   print "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=windows-1251\"/>\n";
}
}