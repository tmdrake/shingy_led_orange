#include <Adafruit_NeoPixel.h>
#include "Timer.h"

// Parameter 1 = number of pixels in back
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)

//Adjust this for each back control, note the LED count is set to 50, Please adjust
#define LED_BACK      12

#define LED_LEFT_ARM  9//11
#define LED_RIGHT_ARM 8//10
#define LED_LEFT_LEG   11//9
#define LED_RIGHT_LEG  10//8
///End of LED pin definitions

Adafruit_NeoPixel back = Adafruit_NeoPixel(65, LED_BACK, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel left_arm = Adafruit_NeoPixel(50, LED_LEFT_ARM, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel right_arm = Adafruit_NeoPixel(50, LED_RIGHT_ARM, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel left_leg = Adafruit_NeoPixel(54, LED_LEFT_LEG, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel right_leg = Adafruit_NeoPixel(54, LED_RIGHT_LEG, NEO_RGB + NEO_KHZ800);

 

//Pin assigments.
#define MODE_UP   2     //Moves Mode Down
#define MODE_DOWN 5     //Moves Mode Up
#define FLASH     3     //Flash 

#define MIC_G     A3
#define MIC_A     A0    //Mic normal line in //Aka D0 without
#define MIC_LP    A1    //Mic filtered line in, prevents HF from interfereing as LF.
#define POT       A2    //Pot connections
#define POT_EN    4     //Enable Pot voltage.

#define LED_PIN   13    //HB LED pin
#define PWR_EN    6    //Power Enable, Active Low
#define PWR_V     A6    //For Measuring Power Voltage
///For keypad controls



//////////////////////////////////////////
byte mode = 1;  //default startup mode
#include <EEPROM.h>
#define MODE_ADDR 10 //eeprom address of mode
boolean in_mode = false;

Timer t;
int afterEvent = 0;

int potval = 0; //potval

boolean flash = false; //all lights on, stop mode.

void setup() {
  //Restore Mode Parameter
  byte tempMode = EEPROM.read(MODE_ADDR);
//  Serial.print(F("Last Mode:"));
//  Serial.print(tempMode);
//  
  if (tempMode <= 100)
    mode = tempMode;
  //
  Serial.begin(115200);
  Serial.print( F("Compiled: "));
  Serial.print( F(__DATE__));
  Serial.print( F(", "));
  Serial.print( F(__TIME__));
  Serial.print( F(", "));
  Serial.println( F(__VERSION__));
  Serial.println( F("Main Program"));
  //Serial.println();

  pinMode(LED_BACK, OUTPUT);
  pinMode(LED_LEFT_ARM, OUTPUT);
  pinMode(LED_RIGHT_ARM, OUTPUT);
  pinMode(LED_LEFT_LEG, OUTPUT);
  pinMode(LED_RIGHT_LEG, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  
  pinMode(POT_EN, OUTPUT); //
  digitalWrite(POT_EN, HIGH); //Pot power

  pinMode(MODE_UP, INPUT_PULLUP);
  pinMode(MODE_DOWN, INPUT_PULLUP);
  pinMode(FLASH, INPUT_PULLUP);
  pinMode(MIC_G, INPUT);
  pinMode(MIC_A, INPUT);
  pinMode(POT, INPUT);
  pinMode(PWR_V,INPUT);
  
  delay(100);
  pinMode(PWR_EN, OUTPUT);
  digitalWrite(PWR_EN, LOW);  //Turns On Main Power
  
  
  

  //reset LED
  back.begin();
  back.show();

  left_arm.begin();
  left_arm.show();

  right_arm.begin();
  right_arm.show();

  left_leg.begin();
  left_leg.show();

  right_leg.begin();
  right_leg.show();


  flash_lamp();

  //run heartbeat LED
  t.oscillate(LED_PIN, 1000, HIGH);
  t.every(10000, checkpower);
  
  
//  Serial.print(F("Last mode:"));
//  Serial.println(tempMode);
  
  //digitalWrite(MIC_G, HIGH);
  Serial.setTimeout(100);
}




void flash_lamp(){
  //Handles hardware flashing lamp
  turn_all_on();

  delay(200);

  turn_all_off();
  
}


void turn_all_on()
{
  for(uint16_t i=0; i<back.numPixels(); i++)
      back.setPixelColor(i, back.Color(127, 127, 127));
  for(uint16_t i=0; i<left_arm.numPixels(); i++)
      left_arm.setPixelColor(i, left_arm.Color(255, 255, 255));
  for(uint16_t i=0; i<right_arm.numPixels(); i++)
      right_arm.setPixelColor(i, right_arm.Color(255, 255, 255));
  for(uint16_t i=0; i<left_leg.numPixels(); i++)
      left_leg.setPixelColor(i, left_leg.Color(255, 255, 255));
  for(uint16_t i=0; i<right_leg.numPixels(); i++)
      right_leg.setPixelColor(i, right_leg.Color(255, 255, 255));
   
  back.show();
  left_arm.show();
  right_arm.show();
  left_leg.show();
  right_leg.show();
      
}

void turn_all_off()
{
  //todo: change to begin to reset
  for(uint16_t i=0; i<back.numPixels(); i++)
      back.setPixelColor(i, back.Color(0, 0, 0));
  for(uint16_t i=0; i<left_arm.numPixels(); i++)
      left_arm.setPixelColor(i, left_arm.Color(0, 0, 0));
  for(uint16_t i=0; i<right_arm.numPixels(); i++)
      right_arm.setPixelColor(i, right_arm.Color(0, 0, 0));
  for(uint16_t i=0; i<left_leg.numPixels(); i++)
      left_leg.setPixelColor(i, left_leg.Color(0, 0, 0));
  for(uint16_t i=0; i<right_leg.numPixels(); i++)
      right_leg.setPixelColor(i, right_leg.Color(0, 0, 0));

  back.show();
  left_arm.show();
  right_arm.show();
  left_leg.show();
  right_leg.show();  
}



void loop() {
//
//  menu();  //check if menu or flash button pressed
//
  if (flash)
    turn_all_on();
  else
      if (!in_mode)
          runmode(mode);
    
  
t.update(); //update main timer



while (Serial.available())
{

    byte inByte = Serial.read();

    switch (inByte)
    {
//      case 'L':
//        lamp_test();  
//      break;

      case 'F':
      {
        int val = Serial.parseInt();
        if (val == 0)
          flash = false;
        else if (val == 1)  
          flash = true;
      }
      break;
      
      case 'P':
      {
        potval = 1023-Serial.parseInt();
        //update potval
        
//        Serial.print("POT:");
//        Serial.println(potval);
      }
      break;      

      case 'B': //Mapped to button commands
      //case 'M':
      {
        int val = Serial.parseInt();
        if (val > 0 && val < 9)
        {
          mode = val; 
//        Serial.print(F("Mode is now:")); 
//        Serial.println(mode);
          if (EEPROM.read(MODE_ADDR) != mode) 
            EEPROM.write(MODE_ADDR, mode); //save mode settings until next tim, only if its different
        //showmode(mode);B2
        
        }
        
      }
      break;
      
//      case 'C':
//      {
//        turn_all_off();
//        color_test();
//      }
//      break;
//      
//      case 'Q':
//      {
//        turn_all_off();
//        led_test();
//      }
//      break;
//      
      
//      case '?':
//          Serial.println(F("Help"));
//          Serial.println(F("L<int> Test Output"));
//          Serial.println(F("Q, Enter LED test mode"));
//          Serial.println(F("M<int> Switch mode"));
//          Serial.println(F("C, Enter color test"));
//          Serial.print(F("Mode:"));
//          Serial.println(mode);
//      break;
      
    }
    
  
}






}
////END OF MAIN LOOP

void led_test()
{
  Serial.println(F("LED TESTER"));
  Serial.println(F("O - Switch Output"));
  Serial.println(F("-/+ - Light LED"));
  Serial.println(F("X - EXIT"));  

  byte inByte = 0;
  int led_to_light = 0;
  int string = 0;
  
  
  while (inByte != 'X')
  {
    if (Serial.available() > 0)
    {
        inByte = Serial.read();
        
        switch (inByte)
          {  
            case 'O':
            case 'o':
                string++;
                if (string > 5)
                  string = 0;
                delay(100);
                break;
            case '+':
                led_to_light++;
                delay(100);
                break;
            case '-':
                led_to_light--;
                delay(100);
                break;
             
         
           }
           
         Serial.print(F("Setting Output:")); Serial.println(string); 
         Serial.print(F("Setting LED:")); Serial.println(led_to_light);
         
         turn_all_off();  
          switch (string)
            {
                case 0:
                  back.setPixelColor(led_to_light, 255, 255, 255);
                  break;
                case 1:
                  left_arm.setPixelColor(led_to_light, 255, 255, 255);
                  break;
                case 2:
                  right_arm.setPixelColor(led_to_light, 255, 255, 255);
                  break;
                case 3:
                  left_leg.setPixelColor(led_to_light, 255, 255, 255);
                  break;
                case 4:
                  right_leg.setPixelColor(led_to_light, 255, 255, 255);
                  break;
                  
            }
           
        //Update LED   
        back.show();
        left_arm.show();
        right_arm.show();
        left_leg.show();
        right_leg.show();
           
    }
  }  
}


void color_test()
{
  //Turns LED on based on RGB varables
  byte red = 0;
  byte green = 0;
  byte blue = 0;
  
  Serial.println(F("COLOR TESTER"));
  Serial.println(F("R/E - RED"));
  Serial.println(F("G/F - GREEN"));
  Serial.println(F("B/V - BLUE"));
  Serial.println(F("X - EXIT"));
  
  
  byte inByte = 0;
  
  while (inByte != 'X')
  {
    if (Serial.available() > 0)
    {
        inByte = Serial.read();
        
        switch (inByte)
          {  
            case 'R':
            case 'r':
                red++;
                break;
            case 'G':
            case 'g':
                green++;
                break;
            case 'B':
            case 'b':
                blue++;
                break;
            case 'E':
            case 'e':
                red--;
                break;
            case 'F':
            case 'f':
                green--;
                break;
            case 'V':
            case 'v':
                blue--;
                break;            
         
            }
        turn_on_color( red,  green,  blue);
        Serial.print("RED:"); Serial.println(red);
        Serial.print("GREEN:"); Serial.println(green);
        Serial.print("BLUE:"); Serial.println(blue);
        

    }
  
  
  }

} //end of color tuning

void turn_on_color(byte red, byte green, byte blue)
{
  //all on and all off could use this functin
  for(uint16_t i=0; i<back.numPixels(); i++)
      back.setPixelColor(i, back.Color(red, green, blue));
  for(uint16_t i=0; i<left_arm.numPixels(); i++)
      left_arm.setPixelColor(i, left_arm.Color(red, green, blue));
  for(uint16_t i=0; i<right_arm.numPixels(); i++)
      right_arm.setPixelColor(i, right_arm.Color(red, green, blue));
  for(uint16_t i=0; i<left_leg.numPixels(); i++)
      left_leg.setPixelColor(i, left_leg.Color(red, green, blue));
  for(uint16_t i=0; i<right_leg.numPixels(); i++)
      right_leg.setPixelColor(i, right_leg.Color(red, green, blue));
   
  back.show();
  left_arm.show();
  right_arm.show();
  left_leg.show();
  right_leg.show();  
  
}



void lamp_test()
{

    //Setup mode
    Serial.setTimeout(1000); 
    turn_all_off();   
//    
//    Serial.println(F("MENU:"));
//    Serial.println(F("*******************************"));
//    Serial.println(F("1-BACK"));
//    Serial.println(F("2-LEFT ARM"));
//    Serial.println(F("3-RIGHT ARM"));
//    Serial.println(F("4-LEFT LEG"));
//    Serial.println(F("5-RIGH LEG"));

    int val = Serial.parseInt();

    Serial.print(F(":")); Serial.print(val);Serial.print(F(":"));

    switch (val)
    {
      case 1:
        Serial.println(F("Back"));
        for(uint16_t i=0; i<back.numPixels(); i++)
        back.setPixelColor(i, back.Color(0, 75, 0));
        back.show();
        delay(5000);
        turn_all_off();
        break;
        
      case 2: 
        Serial.println(F("Left Arm"));
        for(uint16_t i=0; i<left_arm.numPixels(); i++)
        left_arm.setPixelColor(i, left_arm.Color(0, 127, 0));
        left_arm.show();
        delay(5000);
        turn_all_off();
        break;
        
      case 3:  
        Serial.println(F("Right Arm"));
        for(uint16_t i=0; i<right_arm.numPixels(); i++)
        right_arm.setPixelColor(i, right_arm.Color(0, 127, 0));
        right_arm.show();
        delay(5000);
        turn_all_off();
        break;

      case 4:
        Serial.println(F("Left Leg"));
        for(uint16_t i=0; i<left_leg.numPixels(); i++)
        left_leg.setPixelColor(i, left_leg.Color(0, 127, 0));
        left_leg.show();
        delay(5000);
        turn_all_off();
        break;

      case 5:
        Serial.println(F("Right Leg"));
        for(uint16_t i=0; i<right_leg.numPixels(); i++)
        right_leg.setPixelColor(i, right_leg.Color(0, 127, 0));
        right_leg.show();
        delay(5000);
        turn_all_off(); 
        break;
        
    }
}



void runmode( byte mode){

static byte lastmode;
bool newmode = false; 

if (lastmode != mode) //detect a mode change
{
  newmode = true;
  lastmode = mode;
//  initmodes();
  
} 

  
  switch (mode) 
  {
    case 1:
      //Pot mode
        //digitalWrite(POT_EN, HIGH);
      potmode(newmode);
      break;
    case 2:
      mode2(newmode);
//      musicmode(newmode, false);
      break;
    case 3:
      //Rainbow
      mode3(newmode);   
      break; 
    case 4:
      mode4(newmode);
      break;   
    case 5:
      mode5(newmode);
      break;
    case 6:
      mode6(newmode);
      //mode = 1; //route back to 1
      break;
    case 7:
      mode7(newmode);
      break;
    case 8:
      mode8(newmode);
      break;
    case 9:
      mode9(newmode);
      break;
//    case 10:
//      mode10(newmode);
//      break;
//    case 11:
//      mode11(newmode);
//      break;
//    case 12:
//      mode12(newmode);
//      break;
//      
     
  }


  //digitalWrite(POT_EN, LOW);
}



//void initmodes()
//{
//  //use this function to STOP event timers and such
//  
//}




void menu()
{

bool mode_up = digitalRead(MODE_UP);
bool mode_down = digitalRead(MODE_DOWN);
bool flash = digitalRead(FLASH);

if (flash == false) // flash command
  flash_lamp();

if (mode_down == false){
  mode--;
  if (mode <= 0)
    mode = 1;
  delay(100);
  Serial.print(F("Mode:"));
  Serial.println(mode);
  showmode(mode);
  delay(500);
}

if (mode_up == false){
  mode++;
  if (mode > 5)
    mode = 5;
  delay(100);
  Serial.print(F("Mode:"));
  Serial.println(mode);
  showmode(mode);
  delay(500);
  
  
}
//////////////////////////////////////
}

#define MODE_LED_OFFSET 36 //WHERE THE FINGER INDICATORS

void showmode(byte mode){
  //Shows the mode we are on using the arms
//   for(uint16_t i=0; i < left_arm.numPixels(); i++)   //Turn off LEDS
//      left_arm.setPixelColor(i, left_arm.Color(0, 0, 0)); 
//    
//   left_arm.setPixelColor(mode + MODE_LED_OFFSET, left_arm.Color(127, 0, 0));   

   for(uint16_t i=0; i < right_arm.numPixels(); i++) //turn off LEDS
      right_arm.setPixelColor(i, right_arm.Color(0, 0, 0));
 
   right_arm.setPixelColor(mode + MODE_LED_OFFSET, right_arm.Color(127, 0, 0));
   
//   left_arm.show();
   right_arm.show();


   t.stop(afterEvent);
   afterEvent = t.after(1500, hidemode);
   
   
   in_mode = true; //skips lights until mode expires
   if (EEPROM.read(MODE_ADDR) != mode) 
       EEPROM.write(MODE_ADDR, mode); //save mode settings until next tim, only if its different
   
}

void hidemode()
{
    //return to normal programming
    for(uint16_t i=0; i < right_arm.numPixels(); i++) //turn off LEDS
      right_arm.setPixelColor(i, right_arm.Color(0, 0, 0)); 

    for(uint16_t i=0; i < left_arm.numPixels(); i++)   //Turn off LEDS
      left_arm.setPixelColor(i, left_arm.Color(0, 0, 0)); 

    left_arm.show();
    right_arm.show();
    
    in_mode = false;

}


#define low_pot_val   100
#define hight_pot_val 500//600
int getpot()
{
  //returns pot value and Bass or All sound modes.
  //int potval = analogRead(POT); //Reverse the direction
//  static int lastpot;
//  if (lastpot != potval)
//  {
//    Serial.print(F("Pot Value:"));
//    Serial.println(potval);
//    lastpot = potval;
//  }
static int return_pot;



    //Serial.println(potval);
   
  if (potval >= 512)
  {
    //all sound mode
    return_pot = map(potval-512, 0, 512, low_pot_val, hight_pot_val); //Reverse the direction
    return_pot = analogRead(MIC_A) - return_pot; //potval may need scaling
    //this part should really be a selection by Mode/flash switch.
    if (return_pot  < 0)
        return_pot = 0;
        
    //Serial.print(F("Normal:"));
    //Serial.println(return_pot);

    
    return return_pot;    
    
  }
  else
  {
    return_pot = map(potval, 0, 512, low_pot_val, hight_pot_val); //Reverse the direction
    return_pot = analogRead(MIC_LP) - return_pot; //potval may need scaling, using low pass filtering
    //this part should really be a selection by Mode/flash switch.

    if (return_pot  < 0)
        return_pot = 0;
        
    //Serial.print(F("BASS:"));
    //Serial.println(return_pot);  
    
    return return_pot;  
  }
  
  
}




