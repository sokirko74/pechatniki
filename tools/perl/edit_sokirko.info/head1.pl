while (<STDIN>) {

if (! ($_ =~ /^\s*<.DOCTYPE HTML PUBLIC .-..W3C..DTD HTML 4.01 Transitional..EN.\s+.http:..www.w3c.org.TR.1999.REC-html401-19991224.loose.dtd.>\s*$/)) {
   print;
}
}