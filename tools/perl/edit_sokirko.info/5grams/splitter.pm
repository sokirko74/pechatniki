package splitter;

use strict;
use warnings;

sub to_lower {
    my $str = shift;
    $str =~ tr/A-Z�-ߨ\x8E/a-z�-��\x9E/;
    return $str;
}

sub get_tokens {
    my ($str, $grams, $case_sensitive) = @_;
    if ($case_sensitive == 0) {
		$str = to_lower($str);
    }
    my @words = split(/[^0-9A-Za-z���-��-�'-]+/, $str);
    if (scalar @words > 0) 
    {
        if ($words[0] eq "") {
           shift @words;
        }
        if ($case_sensitive) {
           shift @words;
        }

       if (scalar @words > 0) {
          if ($grams > 0) {
              push  (@words, "#");
              unshift (@words, "#");
          }
       }
    }
    return \@words
}
1;
