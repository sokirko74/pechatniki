use splitter;

sub print_text ($) {
    my $p = shift;
	$p =~ s/^ +//;
	$p =~ s/ +$//;
    if($case_sensitive) {
           print $hostid." ".$p."\n";
    } else {
          	print $hostid." ".splitter::to_lower($p)."\n";
    }
}

if (scalar @ARGV != 1) {
  die "Usage: perl extract_ngrams.pl <GRAM> \nwhere <GRAM> can be q,1,2,3,4,5; <CaseSensitive> can be 0 or 1\nFor example:\n perl extract_ngrams.pl 1 doc2host.txt 0\n";
}

$case_sensitive = 1;


$grams=$ARGV[0];
$grams_decr =  $grams - 1;

while (<STDIN>) {
    if ($_ =~ /~~~~~~ [dD]ocument ([0-9]+) ~~~~~~/) {
            my $docno=$1;
            if(! defined $doc2host{$docno} ) {
                die "cannot find $docno in doc2host";
            }
            $hostid = $doc2host{$docno};
            next;
    }
	$line = $_;

	if($grams eq "p") { #parenthesis
   		if(    $line =~ /\(([^\)]{2,100})\)/ 
            || $line =~ /\[([^\]]{2,100})\]/
          ) 
        {
			$brk =  $1;
			$before = substr ($line, 0, $-[0]);
            $words = splitter::get_tokens($before, 0, $case_sensitive);
			$l = scalar @$words;
			if ($l > 0) {
				if ($l > 3) {
					$start = $l - 3;
				} 
				else {
					$start = 0;
				}
				$brk = join (" ", @$words[$start..$l])."( ".$brk." )";
				print_text($brk);
			}
		}

	}
    elsif($grams eq "q") { #quotes
   		# cp1251
   		if(    $line =~ /"([^"«»\r\n]{2,100})"/
            || $line =~ /«([^»\r\n]{2,100})»/
          )
        {
			print_text($1);
     	}
	}
	else {
	        $words = splitter::get_tokens($line, $grams_decr, $case_sensitive);
		
        	if (scalar @$words > 0) {
	        	$len = scalar @$words-$grams_decr;
				for($i = 0; $i < $len; $i++) {
        	    	print $hostid." ".join (" ", @$words[$i..$i+$grams_decr])."\n";
           		}
        	}
	}
} 
