
import java.util.*;
//#####################Server_stuff:####################################
import processing.net.*;
Server s; 
Client c;
String input;
//######################################################################

//Bilder:
ArrayList<PImage> bilder_a;
ArrayList<PImage> bilder_b;

int phase;      //Aktuelle Bild-Nr
int direc;      //An oder Aus?
int mode;       //Focus
int pic_anzahl; //Anzahl an Bildern

void setup() {
  //Video:
  //fullScreen();             
  size(600, 300);
  frameRate(25);  
  orientation(LANDSCAPE);   //Bug?!

  //Load Pictures:
  bilder_a=new ArrayList(); 
  bilder_b=new ArrayList();
  pic_anzahl=9;

  String pic_name;
  for (int i=0; i<=pic_anzahl; i++)
  {
    pic_name="pic_a"+i+".JPG";
    bilder_a.add(loadImage(pic_name));
    pic_name="pic_b"+i+".JPG";
    bilder_b.add(loadImage(pic_name));
  }

  //Setting_UP:
  phase=0;    //Wasser Bild 0
  mode=0;     //Focus auf Hebel
  direc=-1;   //Wasser auschalten

  //#####################Server_stuff:####################################
  s = new Server(this, 12345);  // Start a simple server on a port
  //######################################################################
}

void draw() {
  //#####################Server_stuff:####################################
  c = s.available();
  //println(c);
  if (c != null) 
  {
    input = c.readStringUntil('\r'); 
    //input = input.substring(0, input.indexOf("\n"));  // Only up to the newline
    println(input);

    if (input.contains("s"))
    {
      if (phase==0)
      {
        phase=1;
        //#####################Server_stuff:####################################
        s.write("1\r");
      } else {
        s.write("0\r");
        //######################################################################
      }
      direc=direc*-1;
    }
  } 

  //######################################################################

  if (phase>0)     //Wasser läuft
  {
    if (phase>8)  //Repeat:
    {
      phase=4;
    }
    phase+=direc; //Up or Down
  } 

  //Focus:
  if (mode==0)
  {
    image(bilder_a.get(phase), 0, 0, width, height);  //Focus: Hebel
  } else
  {
    image(bilder_b.get(phase), 0, 0, width, height);  //Focus: Hahn
  }
}

void mousePressed()
{
  if (mouseX>width/2)                              //switch foucs: Hebel/Hahn
  {
    mode++;
    mode=mode%2;
  } else                                           //switch Wasser: an/aus
  {

    if (phase==0)
    {
      phase=1;
      //#####################Server_stuff:####################################
      s.write("1\r");
    } else {
      s.write("0\r");
      //######################################################################
    }
    direc=direc*-1;
  }
}