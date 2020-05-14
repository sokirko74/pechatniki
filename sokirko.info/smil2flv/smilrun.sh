#gfind C:/Sokirko/victor/victor.sokirko.com  -name '*.smil' >files
#tar xfz exe.tar.gz

for f in `cat files`; do
  sh -x smil2flv.sh  $f  2>>log
done;
