#gfind C:/Sokirko/sokirko.info/Part*  -name 'index.html' >films.txt
# edit

for f in `cat films.txt`; do
   dir=`dirname $f`
   name=`basename $dir`   
   new=`echo $f | sed 's/\(.*\.\)html/\1htm/'`;                     
   rm $new
done;
