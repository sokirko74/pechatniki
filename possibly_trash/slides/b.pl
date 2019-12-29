$id =0;                                                                  
while (<STDIN>) {
 if($_ =~ /<img src="([^"]+)".*dur="([0-9]+)s"/)
 {
  $f = $1;
  $d = $2;
  push @slides, "$f $d";
  $id++;
  #if ($id > 5) {
  # last;
  #} 
 }
}

$cnt = scalar @slides;
$fps=15;
$name="nida";
$dir="c:\\tmp\\nida";
#$width=640;
#$height=480;
$width=1024;
$height=768;

print  <<FOO1;
<?xml version="1.0" encoding="UTF-8" ?>
<Flash_Slide_Show_Project version="1.00" Photo_count="$cnt" Flash_Width="$width" Flash_Height="$height" Flash_Title="nida" Background_Color="0x000000" Background_Music_Count="1" Frame_Rate="1" duration_transition="1" duration_show="2" Theme_id="21" Decoration_id="0" _blank="0" theme_style="0">
    <Temp_Folder>C:\\tmp\\</Temp_Folder>
    <Output_Folder Output_SWF="$name.swf" Output_HTML="$name.html" Output_XML="slides.xml">$dir</Output_Folder>
    <Theme_Data xml_file="Theme.xml" xml_path="template\\custom\\Simple" preset_xml_file="Theme.xml" preset_xml_path="template\\premium\\template\\preset\\Simple">
        <flash_template_parameters_profile display_name="Item Name" item_category="0" web_album="0" folder="" swf_file="" preview="" xml_config_file="slides.xml" swf_path_base="" group_all_count="0" donot_create_big_image="0" donot_create_small_image="0">
        </flash_template_parameters_profile>
    </Theme_Data>
    <Background_Music id="0">audio.mp3</Background_Music>
FOO1


$id=0;
for $s (@slides) {
 ($f,$d) = split(/ /,$s);
 $d--;
 print <<FOO;
 <Photo_Data id="$id" rotate_angle="0" FlipX="0" FlipY="0" duration_show="$d" transition_duration="1" transition_effect="52" Title="$f" Description="" Url="">$dir\\$f</Photo_Data>
FOO
 $id++;
}

print <<FOOO;
</Flash_Slide_Show_Project>
FOOO
print "";