cat $1 | tr -d "\r" >$1.n
mv $1.n $1