#gfind C:/Sokirko/sokirko.info/Part*  -name 'index.html' >films.txt
# edit

for f in `cat films.txt`; do
   dir=`dirname $f`
   name=`basename $dir`   
   new=`echo $f | sed 's/\(.*\.\)orig/\1html/'`;                     
   perl edit.pl $name name2flv.txt <$f >$new
   echo $f;
   diff $f $new | wc  
done;
