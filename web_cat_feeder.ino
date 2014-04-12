#include <SPI.h>
#include <Ethernet.h>
#include <Servo.h>

Servo microservo;  // create servo object to control a servo
int pos = 0;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //physical mac address
byte ip[] = { 192, 168, 1, 100 }; // ip in lan
byte gateway[] = { 192, 168, 1, 1 }; // internet access via router
byte subnet[] = { 255, 255, 255, 0 }; //subnet mask
EthernetServer server(80); //server port
 
String readString;
 
//////////////////////
 
void setup(){
 
  microservo.attach(7);
  //start Ethernet
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
  //the pin for the servo co
  //enable serial data print
  Serial.begin(9600);
  Serial.println("server Servo test 1.0"); // so I can keep track of what is loaded
}
 
void loop(){
  // Create a client connection
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
 
        //read char by char HTTP request
        if (readString.length() < 100) {
 
          //store characters to string
          readString += c;
          //Serial.print(c);
        }
 
        //if HTTP request has ended
        if (c == '\n') {
 
          ///////////////
          Serial.println(readString); //print to serial monitor for debuging
 
          client.println("HTTP/1.1 200 OK"); //send new page
          client.println("Content-Type: text/html");
          client.println();
          client.println("<HTML>");
          client.println("<HEAD>");
          client.println("<meta name='apple-mobile-web-app-capable' content='yes' />");
          client.println("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />");
          client.println("<link rel='stylesheet' type='text/css' href='http://homeautocss.net84.net/a.css' />");
          client.println("<TITLE>Arduino Home Automation</TITLE>");
          client.println("</HEAD>");
          client.println("<BODY>");
          client.println("<H1>Arduino Home Automation</H1>");
          client.println("<hr />");
          client.println("<br />");
          client.println("<a href=\"/?button2on\"\">Open Feeder</a><br />"); 
          client.println("<br />");
          client.println("<a href=\"/?button3on\"\">Close Feeder</a><br />");  
          client.println("<br />");
          client.println("<a href=\"/?button1on\"\">Feed Cats</a>");
          client.println("<br />");
          client.println("<br />"); 
          client.println("  <iframe width='420' height='345' src='http://www.justin.tv/widgets/live_embed_player.swf?channel=antigenius0910'>  />"); 
          client.println("   </iframe>  "); 
          client.println("</BODY>");
          client.println("</HTML>");

          delay(1);
          //stopping client
          client.stop();
 
          ///////////////////// control arduino pin
          
          if(readString.indexOf("?button1on") >0)//checks for on "Feed Cats"
          {
            for(pos = 0; pos < 60; pos += 3)  // goes from 0 degrees to 69 degrees 
                {                                  // in steps of 1 degree 
                  microservo.write(pos);              // tell servo to go to position in variable 'pos' 
                  delay(15);                       // waits 15ms for the servo to reach the position 
                } 
            delay(250);
            
            for(pos = 60; pos>=1; pos-=3)     // goes from 180 degrees to 0 degrees 
                {                                
                  microservo.write(pos);              // tell servo to go to position in variable 'pos' 
                  delay(15);                       // waits 15ms for the servo to reach the position 
                }
            Serial.println("Feed finished");
          }


          if(readString.indexOf("?button2on") >0)//checks for on "Feed Open"
          {
            for(pos = 0; pos < 180; pos += 3)  // goes from 0 degrees to 180 degrees 
                {                                  // in steps of 1 degree 
                  microservo.write(pos);              // tell servo to go to position in variable 'pos' 
                  delay(15);                       // waits 15ms for the servo to reach the position 
                }         
            Serial.println("Feed Open");
          }


          if(readString.indexOf("?button3on") >0)//checks for on "Feed Close"
          {
            for(pos = 180; pos>=1; pos-=3)     // goes from 180 degrees to 0 degrees 
                {                                
                  microservo.write(pos);              // tell servo to go to position in variable 'pos' 
                  delay(15);                       // waits 15ms for the servo to reach the position 
                }                 
            Serial.println("Feed Open");
          }

          //clearing string for next read
          readString="";
 
        }
      }
    }
  }
}
