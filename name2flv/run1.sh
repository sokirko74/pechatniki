#gfind C:/Sokirko/sokirko.info/Part*  -name 'index.html' >films.txt
# edit

for f in `cat films.txt`; do
   new=`echo $f | sed 's/\(.*\.\)orig/\1html/'`;                     
   cp $new $f 
done;
