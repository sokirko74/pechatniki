#gfind C:/Sokirko/victor/victor.sokirko.com  -name '*.smil' >files

for f in `cat files`; do
  sh -x smil2local.sh  $f  2>>log
#  exit;
done;
