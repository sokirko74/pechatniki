while (<STDIN>) {
if ($_ =~ /^\s*<.DOCTYPE html PUBLIC "-..W3C..DTD XHTML 1.0 Transitional..EN"\s*$/) {
  $s=<STDIN>;
  if ($s =~ /^\s*"http:..www.w3.org.TR.xhtml1.DTD.xhtml1-transitional.dtd">\s*$/) {
  } 
  else {
    print $s;
  }
}
else {
 if ($_ =~ /^\s*<html +xmlns="http:..www.w3.org.1999.xhtml">$/) 
 {
   print "<html>\n";
 }
 else {
   print;
 }
}
}