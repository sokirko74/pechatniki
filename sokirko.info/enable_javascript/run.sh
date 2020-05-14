cat files.txt | while read LINE; do 
    python get_slides.py $LINE
    if [ $?  != 0 ]; then
        echo "failed on  $LINE"
        exit 1
    fi;
done
