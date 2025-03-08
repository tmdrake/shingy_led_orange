//////////////////////
//This is mode one
//THis mode takes in input and direct drives the pot
void potmode(boolean newmode)
{
  if (newmode)
  {
    turn_all_off();
    //Serial.println(F("Direct Pot to Bargraph Mode"));
    //Mode one
  }  
  //digitalWrite(POT_EN, HIGH); //enables pot



  static unsigned long previousMillis;
  if(millis() - previousMillis >= 100) //time expired
  {
    int val = 1024-potval;
    val = map(val, 0, 1000, 0, back.numPixels());
    show_level_on_back(val);
//    delay(100);
    
    previousMillis = millis();

  }
  



  
  
}

void show_level_on_back(int val)
{
   for(uint16_t i=0; i<back.numPixels(); i++) //
      back.setPixelColor(i, back.Color(0, 0, 0));

  
  for(uint16_t i=0; i < val; i++) //
      back.setPixelColor(back.numPixels() - i, back.Color(178, 3, 238));

  for(uint16_t i=0; i<left_arm.numPixels(); i++)
      left_arm.setPixelColor(left_arm.numPixels() - i, left_arm.Color(178, 3, 238));
  for(uint16_t i=0; i<right_arm.numPixels(); i++)
      right_arm.setPixelColor(right_arm.numPixels() - i, right_arm.Color(178, 3, 238));
  for(uint16_t i=0; i<left_leg.numPixels(); i++)
      left_leg.setPixelColor(left_leg.numPixels() - i, left_leg.Color(178, 3, 238));
  for(uint16_t i=0; i<right_leg.numPixels(); i++)
      right_leg.setPixelColor(right_leg.numPixels() - i, right_leg.Color(178, 3, 238));
      
 
 //Set any LEDS that need to be ON
// back.setPixelColor(60, 255, 255, 0);
// back.setPixelColor(61, 255, 255, 0);
// back.setPixelColor(62, 255, 255, 0);
// back.setPixelColor(63, 255, 255, 0);
// back.setPixelColor(64, 255, 255, 0); 
 
// left_leg.setPixelColor(27, 255, 255, 0);
// right_leg.setPixelColor(27, 255, 255, 0); 
// 
// left_leg.setPixelColor(28, 255, 255, 0);
// right_leg.setPixelColor(28, 255, 255, 0); 
// left_leg.setPixelColor(29, 255, 255, 0);
// right_leg.setPixelColor(29, 255, 255, 0);  
// left_leg.setPixelColor(30, 255, 255, 0);
// right_leg.setPixelColor(30, 255, 255, 0);
//
// left_arm.setPixelColor(26, 255, 255, 0);
// right_arm.setPixelColor(26, 255, 255, 0);
// left_arm.setPixelColor(27, 255, 255, 0);
// right_arm.setPixelColor(27, 255, 255, 0);
// left_arm.setPixelColor(28, 255, 255, 0);
// right_arm.setPixelColor(28, 255, 255, 0);
// left_arm.setPixelColor(29, 255, 255, 0);
// right_arm.setPixelColor(29, 255, 255, 0);  
// left_arm.setPixelColor(30, 255, 255, 0);
// right_arm.setPixelColor(30, 255, 255, 0); 
 
  
  back.show();
  left_arm.show();
  right_arm.show();
  left_leg.show();
  right_leg.show();      
  
}


