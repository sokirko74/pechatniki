path=/cygdrive/c/Sokirko/disk/sokirko.info
export script=$1
rm nohup.out
tar cf save.tar $path --exclude="*.JPG" --exclude="*.jpg" 
nohup gfind $path \( -name '*.html' -or -name '*.htm' \) -exec sh -x runfile.sh $script {} \;
