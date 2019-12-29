p=tmp

new=../../../sokirko.info/$p
old=../../../victor/victor.sokirko.com/$p

gfind $new -name '*.h*' | xargs cat | tr -d "\r\n" |  perl extract_ngrams.pl 3 | grep -v "[a-zA-Z]"| gsort -u >new
gfind $old -name '*.h*' | xargs cat | tr -d "\r\n" | perl extract_ngrams.pl 3 | grep -v "[a-zA-Z]"| gsort -u >old
diff old new  >diff.txt