# -*- coding: UTF-8 -*-
import sys, re;

Slid = {};

def find_image (s):
    if s in Slid:
        return s;
    if ('0' + s) in  Slid:
        return '0'+s;
    if ('00' + s) in  Slid:
        return '00'+s;
    return '';

for a in open(sys.argv[1],"r"):
    #<img src="110-1.jpg" region="images" regPoint="center" regAlign="center" dur="10s"/>
    matchobj = re.search('.*<img src="([^"]+).(?:(?:jpg)|(?:JPG))".*dur="([0-9]+)s.*', a)
    if matchobj:
        fil =  matchobj.group(1);
        dur = int(matchobj.group(2));
        #Slides.append( (fil, dur) );
        Slid[fil] = dur;

#print Slid;
print >> sys.stderr, "read %i slides" % len(Slid);

for line in open(sys.argv[2],"r"):
    line = line.decode('cp1251').rstrip();
    #<p>265.<u>  Взамен эпилога.</u></p>
    #      <p>263-264.</p>
    #      <p>1.  <u>Восточный Кавказ - 1979</u></p>      <p>2. <u> Часть 4. "Чеченцы"</u></p>

    
    matchobj = re.search('^ *<p>([0-9]+)\.(.*)$', line)
    if matchobj:
        s = find_image( matchobj.group(1))
        if s != '':
            line =  u'   <p> <a href=%s.jpg style="text-decoration: none"> <img src="mini/%s.jpg">  %s.%s </a>' % (s, s,matchobj.group(1), matchobj.group(2));
    print line.encode('cp1251');    
        
        
    
          