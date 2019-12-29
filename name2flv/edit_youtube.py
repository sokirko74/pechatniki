import sys
import re
import os.path

dirname = sys.argv[1]
table_file = sys.argv[2]

name2flv = {}
for x in open(table_file, "r"):
    (film_name, path, youtube, yavideo, yafotki) = x.strip().split("\t")
    name2flv[name] = youtube

for n in name2flv:
    filename = "%s/%s/index.html" % (dirname, n)
    youtube = name2flv[n]
    if not os.path.isfile(filename):
        print >>sys.stderr, "cannot open file %s"  % filename
        exit (1)
    new_filename = "%s.new" % filename
    new_file = open(new_filename, "w")
    replace_count = 0    
    for x in open (filename, "r"):
        r = re.sub('http:..video.yandex.ru.users.sokirko.*view.[0-9]+..hq', youtube, x)
        if r != x:
            replace_count += 1
        x = r
        new_file.write (x)

    if replace_count != 2:
        print >>sys.stderr, "cannot find 2 lines to replace in %s " % filename
        exit (1)
    new_file.close()
    os.rename(new_filename, filename)
    #break