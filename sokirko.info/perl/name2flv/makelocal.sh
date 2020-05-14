# copy  new htmls to C:\Sokirko\SokirkoSlideFilm
gfind C:/Sokirko/SokirkoSlideFilm/sokirko.info/Part* -name 'index.html' >films.txt

for f in `cat films.txt`; do
   dir=`dirname $f`
   name=`basename $dir`   
   cat $f | perl makeloc.pl $name >$f.new
   mv $f.new $f
done;
