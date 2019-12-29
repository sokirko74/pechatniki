dir=`dirname $1`
cd $dir
name=`basename $dir`                        
export fps=1;
export width=240;
export height=180;
export size=$width\x$height;

m=`cat smil.smil | perl C:/Sokirko/scripts/smil2flv/smil2frame.pl 1 | head -n 1` 
iconvert $m -resize $size -size $size xc:black +swap -gravity center -composite mini.jpg
