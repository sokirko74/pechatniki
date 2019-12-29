bindir=`pwd`
which mencoder
which ffmpeg

dir=`dirname $1`
cd $dir
name=`basename $dir`                        
export fps=1;
export width=800;
export height=600;
export size=$width\x$height;

#rename .JPG .jpg *.JPG
sed "s/\.JPG/.jpg/g" <smil.smil >smil.smil1
cp  smil.smil1 smil.smil

rm -rf n;
mkdir n;
for f in *.jpg; do
  iconvert $f -resize $size -size $size xc:black +swap -gravity center -composite n/$f
done;

cat smil.smil  | perl $bindir/smil2frame.pl $fps 'n/' > a.lst
$bindir/mencoder mf://\@a.lst -mf w=$width:h=$height:fps=$fps:type=jpg -ovc copy -oac copy -really-quiet -o - | $bindir/ffmpeg -i - -i audio.mp3 -acodec copy -ar 22050 -ab 32k -f flv -b 700k -s $size -y $name.flv
#rm -rf n
#rm -rf a.lst 
