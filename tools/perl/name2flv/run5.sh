#gfind C:/Sokirko/sokirko.info/Part*  -name 'index.html' >films.txt
# edit

for f in `cat films.txt`; do
   
   dir=`dirname $f`
   echo $dir
   perl dec.pl $dir/index.html < $dir/smil.smil >$dir/smil.smi
   mv $dir/smil.smi $dir/smil.smil
   #break

done;
           