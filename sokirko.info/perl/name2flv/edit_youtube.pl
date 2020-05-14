$name = $ARGV[1];
$table_file = $ARGV[2];
open T, "<$table_file";
while (<T>) {
 chomp;
 ($n,$f) = split;
 $name2flv{$n} = $f;
}
close $T;

if (! defined $name2flv{$name}) {
 die "cannot find $name in  flv table";
}

$flv =  $name2flv{$name};

while (<STDIN>) 
{
    #if ($_ =~ /http:..video.yandex.ru.users.sokirko-v.view.[0-9]+..hq/ ) {
    #if ($_ =~ /body/) {
    if ($_ =~ /<body>/) {

        #$s =~ s/http:..video.yandex.ru.users.sokirko-v.view.[0-9]+.#hq/$flv/;
        print s
    }
    #else {
        print
    #}
}


