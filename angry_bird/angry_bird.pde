/*
*Code for: Controlling mouse based on COM port value for Angry bird game
*Code by : Aswinth Raj B
*Dated : 3-8-2017
*Website: www.circuitdigest.com
*/
//Credit : https://gist.github.com/yoggy/8280330

import java.awt.*;
import java.awt.event.*;
import processing.serial.*;

Serial port;
int data;
int Pull, Turn, crntX, crntY;
boolean click;

Robot robot;
PFont pfont;
Point save_p;
 
void setup() {
  size(320, 240);
  port = new Serial(this,Serial.list()[0],9600); //Read the serial values from 0th COM (llok for device manager on your computer to modify this)
  println(Serial.list()); //COM ports are listed for reference
  
  try { 
    robot = new Robot();
    robot.setAutoDelay(0);
  } 
  catch (Exception e) {
    e.printStackTrace();
  }

  pfont = createFont("Impact", 32);
}
 
void draw() {
  background(#ffffff);
  fill(#000000);
  
   if (port.available()>0)
  {
    data=port.read();
    println(data); //Read the data from COM port and save it in data
  }
  
  if (data>=250 && data<=350) //If the value if from 250 to 350 then it must be from Potentiometer
  {
    Turn = int (map (data,250,350,0,100));   
    
  }
  
  if (data>=90 && data <=240)  //If the value if from 40 to 210 then it must be from Flex Sensor
  { Pull = int (map(data,90,240,0,100));} //USe that value to pull the catapullt 
  
  
  if (data == 0)
  {
    click = false; //USe that value to release the mouse button
  }
  if(data == 1)
  {
    click = true;
       //Move the mouse based on the Flex and POT value

  }
  Point p = getGlobalMouseLocation();
  
  
  textFont(pfont);
  text("now x=" + (int)p.getX() + ", y=" + (int)p.getY(), 10, 32); //display the position of X and Y
  
  text ("Pull =" + Pull, 40,175); //Display the value fo pull (Flex Sesnor)
  text ("Turn =" + Turn, 10,150); //Display the value fo Turn (Potentiometer)
  text ("Click =" + click, 40,200); //Display the status of mouse click
  
   if (click == false) //when Flex Sesnor is not pulled
  {
    crntX = (int)p.getX() ; crntY = (int)p.getY() ;
    
 // if (Pull>50)
  robot.mouseRelease(InputEvent.BUTTON1_DOWN_MASK); //Release the mouse button
  }
    
  if (Pull<90)
  {
    robot.mouseRelease(InputEvent.BUTTON1_DOWN_MASK); //Release the mouse button
  delay(500);
  }
  
  if (click == true) //when Flex Sesnor is pulled
  {
   
   robot.mousePress(InputEvent.BUTTON1_DOWN_MASK); //Press the mouse Button
   robot.mouseMove(crntX-Pull,crntY+Turn); //Move the mouse based on the Flex and POT value
  }
  
}


  Point getGlobalMouseLocation() {
  PointerInfo pointerInfo = MouseInfo.getPointerInfo();
  Point p = pointerInfo.getLocation();
  return p;  
}
