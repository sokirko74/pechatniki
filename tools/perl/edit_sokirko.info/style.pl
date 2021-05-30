$in_style = 0;

while (<STDIN>) {

if ($_ =~ /^\s*<style type=.text.css.>\s*$/) {
   $in_style = 1;

   print "<link type=\"text/css\" rel=\"stylesheet\" media=\"all\" href=\"http://www.sokirko.info/modules/m.css\" />\n";
}
else { 
   if ($in_style == 0) {
     print;
   }
   if ($_ =~ /^\s*<\/style>\s*$/) {
	 $in_style = 0;
   }  
}
}