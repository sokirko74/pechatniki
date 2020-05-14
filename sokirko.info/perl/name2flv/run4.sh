#gfind C:/Sokirko/sokirko.info/Part*  -name 'index.html' >films.txt
# edit

for f in `cat films.txt`; do
   dir=`dirname $f`
   old=${dir/C:\/Sokirko\/disk\/sokirko.info/C:\/Sokirko\/victor\/victor.sokirko.com}
   echo $dir, $old
   ls $old/*.jpg
   cp $old/*.jpg   $dir
   cp $old/smil.smil $dir
   cp $old/audio.mp3 $dir
#   break

done;
           