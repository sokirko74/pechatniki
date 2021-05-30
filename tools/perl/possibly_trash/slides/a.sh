rm -rf n;
mkdir n;
for f in *.jpg; do
  iconvert $f -resize 800x600 -size 800x600 xc:black +swap -gravity center -composite n/$f
done;

mencoder mf://@a.lst -mf w=800:h=600:fps=1:type=jpg -ovc lavc -lavcopts vcodec=mpeg4:mbd=2:trell -oac copy -o output.avi