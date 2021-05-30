bindir=`pwd`
set -e
cd $1
ls 
if [ ! -e index.html ]; then
    exit 1;
fi

if [ ! -e smil.smil ]; then
    exit 1;
fi

rm -rf mini
mkdir mini

for f in *.jpg; do
  iconvert $f -size 80x60  -resize 80x60 xc:black +swap -gravity center -composite mini/$f
done;

mv  index.html index.html.old
cp $bindir/AddImage.py .
python AddImage.py smil.smil index.html.old >index.html.new
rm AddImage.py
mv index.html.new index.html