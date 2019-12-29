$id =0;
$fps=25;
$sum = 0;
#$width=640;
#$height=480;
#$width=1024;
#$height=768;
$width=800;
$height=600;

while (<STDIN>) {
 if($_ =~ /<img src="([^"]+)".*dur="([0-9]+)s"/)
 {
  $f = $1;
  $d = $2;
  $st = $d*$fps;
  #print " $f $d\n";
  print "[PICTURE_$id]\n";
  if ( $id == 0) {
    print "AudioFadingTime=0\nAudioFile=audio.wav\n";
  }
  $sum += $d;

print <<FOO;
ColorFade_ColorRef=0
Fade2NextPicture=0
FadeOption=-1
Fading2NextPictureName=Cross Slide Close
FileName=$f
FlightPoints=
FlightPointsColor=255
FlightPointsRadius=10
IsColorFade=0
LoopAudio=0
NormalizeAudio=0
PanoramaScrolling=0
PanoramaScrollingType=-1
PictureMove=0
PictureMoveSettings=
PictureText=
Rotation=0
ShowPicture=$st
SynchronizeFF2NP=0
Text-FadeIn=0
Text-FadeOut=0
TextBackWidth=2
TextFadeInEffectName=Cross Dissolve
TextFadeOutEffectName=Cross Dissolve
TextPosition=1

FOO

$id++;
$time_start += $d;
if ($id > 5) {
 #last;
} 
}

}

print <<FOOO;
[PROJECT_SETTINGS]
CropBottom=0
CropLeft=0
CropRight=0
CropTop=0
Frames-Fading2NextPicture=1
Frames-Showing-Picture=100
Frames-Text-Fade-In=25
Frames-Text-Fade-Out=15
FramesPerSeconds=$fps
GlobalBackgroundColorB=0
GlobalBackgroundColorG=0
GlobalBackgroundColorR=0
MaxFileSizeLimit=640
Movie-Size-X=$width
Movie-Size-Y=$height
Output-AVI=audio.avi
PictureMove_Quality=2
PictureMove_ScaleLimit=2
RescalePictures=1
TempDir=.
UseFadingBetweenPictures=0
UseTextFading=1
FOOO

print "";
$m=$sum/60;
$s=$sum%60;
print STDERR "Film duration: $m:$s ($sum seconds); Slides: $id\n"