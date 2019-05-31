 // include the library code:
#include <LiquidCrystal.h>
#include <Wire.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8,9,10,11,12,13);

byte temp;
int buttonState2 = LOW; //Initiate button state to 0 for all 3 buttons
int buttonState3 = LOW;
int buttonState4 = LOW;
long lastDebounceTime_2 = 0;
long lastDebounceTime_3 = 0;
long debounceDelay = 100;
long timer1 = 0;
long timer2 = 0;
long timer3 = 0;
long timer4 = 0;
int incrementUp = 1;
int incrementDown = 1;
int sendInfo = 0;

String settingsMenu[4] = {"Min Length:" , "Max Length:", "Min Speed: ", "Max Speed: "}; //Writing displayed when changing settings
String settingsMenuShort[6] = {"Len:", "-", "rev", "Vel:", "-", "rps"}; //Shortened writing displayed on main menu
int settings[4] = {50, 300, 200, 500}; //Initial settings (in steps) for min length, max length, min speed and max speed, respectively

void setup() {
delay(5000);
// set up the LCD’s number of columns and rows:
lcd.begin(16,2);

// set the pins to read input from buttons 
pinMode(2, INPUT);
pinMode(3, INPUT);
pinMode(4, INPUT);

Wire.begin(); // Start I2C Bus as Master  
Serial.begin(9600);
}

void loop() {
  int stopLoop = 0; //Initiate variable to 0 at the beginning of ever loop

  lcd.setCursor(0,0);                 // Displays main menu
  lcd.print(settingsMenuShort[0]);
  lcd.print((double)settings[0]/200); //Divide by 200 to obtain rotational speed in rps instead of steps
  lcd.print(settingsMenuShort[1]);
  lcd.print((double)settings[1]/200);
  lcd.print(settingsMenuShort[2]);
  lcd.setCursor(0, 1);
  lcd.print(settingsMenuShort[3]);
  lcd.print((double)settings[2]/200); //Divide by 200 to obtain rotation instead of steps
  lcd.print(settingsMenuShort[4]);
  lcd.print((double)settings[3]/200);
  lcd.print(settingsMenuShort[5]);

buttonState4 = digitalRead(4);
  if(buttonState4 == HIGH){ //if menu button is pressed
    lcd.clear();
    delay(1000);
    lcd.clear();
    buttonState4 = LOW;
    
    //goes through all settings to change
    while(buttonState4 == LOW){
      lcd.setCursor(0,0);
      lcd.print(settingsMenu[0]); //Displays setting to be changed 
      lcd.print((double)settings[0]/200, 3); //Should limit output to 3 decimal places
      settings[0] = increaseValue(incrementUp, 0);
      settings[0] = decreaseValue(incrementDown, 0);
      delay(100);
      buttonState4 = digitalRead(4);
    }
    
    lcd.clear();
    delay(200);
    buttonState4 = LOW;
    
    while(buttonState4 == LOW){
      lcd.setCursor(0,0);
      lcd.print(settingsMenu[1]); //Displays setting to be changed 
      lcd.print((double)settings[1]/200, 3); //Should limit output to 3 decimal places
      settings[1] = increaseValue(incrementUp, 1);
      settings[1] = decreaseValue(incrementDown, 1);
      delay(100);
      buttonState4 = digitalRead(4);
    }
    
    lcd.clear();
    delay(200);
    buttonState4 = LOW;
    
    while(buttonState4 == LOW){
      lcd.setCursor(0,0);
      lcd.print(settingsMenu[2]); //Displays setting to be changed 
      lcd.print((double)settings[2]/200, 3); //Should limit output to 3 decimal places
      settings[2] = increaseValue(incrementUp, 2);
      settings[2] = decreaseValue(incrementDown, 2);
      delay(100);
      buttonState4 = digitalRead(4);
    }
    
    lcd.clear();
    delay(200);
    buttonState4 = LOW;
    
    while(buttonState4 == LOW){
      lcd.setCursor(0,0);
      lcd.print(settingsMenu[3]); //Displays setting to be changed 
      lcd.print((double)settings[3]/200, 3); //Should limit output to 3 decimal places
      settings[3] = increaseValue(incrementUp, 3);
      settings[3] = decreaseValue(incrementDown, 3);
      delay(100);
      buttonState4 = digitalRead(4);
    }

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Saving settings");
    lcd.setCursor(0,1);
    lcd.print("      ...       ");
    delay(1000);
    buttonState4 = LOW; 
  }
//Include safety conditions not to damage motors or electronics from improper input
if (settings[0] >= settings[1]){
  settings[1] = settings[0] + 1;
  lcd.clear();
}
if (settings[2] >= settings[3]){
  settings[3] = settings[2] + 1;
  lcd.clear();
}
if (settings[0] < 5){
  settings[0] = 5;
  lcd.clear();
}
if (settings[1] > 10000){
  settings[0] = 10000;
  lcd.clear();
}
if (settings[2] < 5){
  settings[2] = 5;
  lcd.clear();
}
if (settings[3] > 800){
  settings[3] = 800;
  lcd.clear();
}

if (sendInfo == 1)
  {
    int a;
    int k = 1;
    while (k<=10) //send transmission j times
    {
      Wire.beginTransmission(8);
      for (a = 0; a < 4; a++) 
      {
        temp = settings[a];
        Wire.write(temp);
        temp = settings[a] >> 8;
        Wire.write(temp);
      }
      Wire.endTransmission();
      k++;
      Serial.println(settings[0]);
      Serial.println(settings[1]);
      Serial.println(settings[2]);
      Serial.println(settings[3]);
      
      sendInfo = 0;
    }
  }
}

//function to increase value displayed
int increaseValue(int increase, int i){
  buttonState3 = digitalRead(3);
  if((millis() - lastDebounceTime_3) > debounceDelay){
    timer1 = millis();
    while(buttonState3 == HIGH){
      timer2 = millis();
      if (timer2 - timer1 > 2000){
        incrementUp = 10;
      }
      else{
        incrementUp = 1;
      }
      
      settings[i] = settings[i] + incrementUp;
      sendInfo = 1;
      lcd.setCursor(0,0);
      lcd.print(settingsMenu[i]); //Displays setting to be changed 
      lcd.print((double)settings[i]/200, 3); //Should limit output to 3 decimal places
      delay(150);
      buttonState3 = digitalRead(3);
    }
  }
  return settings[i];
}
//function to decrease value displayed
int decreaseValue(int decrease, int i){
  buttonState2 = digitalRead(2);
  if((millis() - lastDebounceTime_2) > debounceDelay){
    timer3 = millis();
    while(buttonState2 == HIGH){
      timer4 = millis();
      if (timer4 - timer3 > 2000){
        incrementDown = 10;
      }
      else{
        incrementDown = 1;
      }
      
      settings[i] = settings[i] - incrementDown;
      sendInfo = 1;
      lcd.setCursor(0,0);
      lcd.print(settingsMenu[i]); //Displays setting to be changed 
      lcd.print((double)settings[i]/200, 3); //Should limit output to 3 decimal places
      delay(150);
      buttonState2 = digitalRead(2);
    }
  }
  return settings[i];
}
