cat $2 | perl $1 $2 >$2.n
diff $2 $2.n
mv $2.n $2