#gfind C:/Sokirko/sokirko.info/Part*  -name 'index.html' >films.txt
# edit

for f in `cat films.txt`; do
   dir=`dirname $f`
   name=`basename $dir`   
   new=`echo $f | sed 's/\(.*\.\)orig/\1html/'`;                     
   cat $f | perl makeloc.pl $name >$new
done;
