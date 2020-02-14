//Version 0.9
#include "course_control.h"

void setup() 
{
  // set up PWM pins as outputs
  pinMode(RS0, OUTPUT);
  pinMode(GS0, OUTPUT);
  pinMode(BS0, OUTPUT);
  pinMode(RS1, OUTPUT);
  pinMode(GS1, OUTPUT);
  pinMode(BS1, OUTPUT);
  //setup button and button LED pins
  pinMode(B0BUTT, INPUT_PULLUP);
  pinMode(B1BUTT, INPUT_PULLUP);
  pinMode(STRBUTT, INPUT_PULLUP);
  pinMode(R0BUTT, INPUT_PULLUP);
  pinMode(R1BUTT, INPUT_PULLUP);
  pinMode(B0LED, OUTPUT);
  pinMode(B1LED, OUTPUT);
  pinMode(STRLED, OUTPUT);
  pinMode(R0LED, OUTPUT);
  pinMode(R1LED, OUTPUT);
  
  Serial.begin(9600);
}

void loop() 
{
  resetvalues();
  R = 1; //1 to start rainbow
  G = 0;
  B = 0;//reset values to off
  colorChangeBoth(0,0,0); //turn LEDs off to begin
  //random fun patterns between rounds, ie before start button pressed
  long timeStamp = millis(); //timestamp for prematch light stuff
  int color = 0; //for color switcher
  while(digitalRead(STRBUTT)) //while start button not pressed
  {
    //rainbow(&reddir,&greendir,&bluedir);
    if((millis() - timeStamp)>2000)
    {
      switch(color)
      {
        case 0: //turn on red
          colorChangeBoth(255,0,0);
          color = 1;
          break;
        case 1: //turn on green
          colorChangeBoth(0,255,0);
          color = 2;
          break;
        case 2: //turn on blue
          colorChangeBoth(0,0,255);
          color = 0;
          break;
      }
    }
  } //comp start from button press
  
  colorChangeBoth(0,0,0); //lights off
  delay(3000);             //for three seconds
  onesecfade();           //3
  onesecfade();           //2
  onesecfade();           //1
  colorChangeBoth(0,255,0); //GO
  startTime = millis(); //store round start time
  delay(2000);    //wait two seconds
  side0color = redorblue(0); //assign bomb color to side 0
  side1color = redorblue(1); //assign bomb color to side 1
  while((millis()-startTime) < ROUNDTIME)
  {
    if(triggeredZero == false)
    {
      checkButtons(0,&R0Pressed,&B0Pressed); //check if buttons pressed and debounced
      resolveButtons(0,side0color, R0Pressed, B0Pressed); //resolve button presses
    }
    else
    {
      if(!zerostayoff)
      {
        if(successZero)
        {
          if((millis() - Zeroposttimer) < 3000)
          {
            colorChange(0,0,255,0);
          }
          else
          {
            colorChange(0,0,0,0);
            zerostayoff = true;
          }
        }
        else
        {
          colorChange(0,0,0,0);
          zerostayoff = true;
        }
      }
    }
    if(triggeredOne == false)
    {
      checkButtons(1,&R1Pressed,&B1Pressed); //check if buttons pressed and debounced
      resolveButtons(1,side1color, R1Pressed, B1Pressed); //resolve button presses
    }
    else
    {
      if(!onestayoff)
      {
        if(successOne && (onestayoff == false))
        {
          if((millis() - Oneposttimer) < 3000)
          {
            colorChange(1,0,255,0);
          }
          else
          {
            colorChange(1,0,0,0);
            onestayoff = true;
          }
        }
        else
        {
          colorChange(1,0,0,0);
          onestayoff = true;
        }
      }
    }
  }//after 3 minutes the round ends
  colorChangeBoth(255,255,255);
  delay(10000);
}

int redorblue(int side)
{
  randomSeed(analogRead(0)); //set randomization seed based on noise from A0 (unconnected)
  randNumber = random(9002); //generate a random number 0-over 9000

  if(randNumber % 2) //if even, change to blue (well, cyan)
  {
    R = 0;
    G = 255;
    B = 255;
    colorChange(side, R,G,B);
    return BLUE;
  }
  else //if odd, change to red
  {
    R = 255;
    B = 0;
    G = 0;
    colorChange(side, R,G,B);
    return RED;
  }
}

void colorChangeBoth(int R, int G, int B) //change both sides color to the same
{
  colorChange(0,R,G,B);
  colorChange(1,R,G,B);
}

void colorChange(int side, int R, int G, int B) //change a given side's color
{
  if(!side)
  {
    analogWrite(RS0, R);
    analogWrite(GS0, G);
    analogWrite(BS0, B);
  }
  else
  {
    analogWrite(RS1, R);
    analogWrite(GS1, G);
    analogWrite(BS1, B);
  }
}

void checkButtons(int side, bool *RedPressed, bool *BluePressed)
{
  if(side)
  {
    *RedPressed = debounce(digitalRead(R1BUTT), &r1state, &r1last, &r1lastDebounceTime);
    *BluePressed = debounce(digitalRead(B1BUTT), &b1state, &b1last, &b1lastDebounceTime);
  }
  else
  {
    *RedPressed = debounce(digitalRead(R0BUTT), &r0state, &r0last, &r0lastDebounceTime);
    *BluePressed = debounce(digitalRead(B0BUTT), &b0state, &b0last, &b0lastDebounceTime);
  }
}

bool debounce(int current, int *state, int *last, unsigned long *lasttime)
{
  if(current != *last) //if state has changed, reset debounce timer
  {
    *lasttime = millis();
  }
  if((millis() - *lasttime) > debounceDelay) //current state has existed longer than debounce timer, so valid
  {
    if(current != *state) //if state has changed, store that
    {
      *state = current;
      if(*state == LOW) //if state is now LOW, button has been pressed, return true
      {
        return true;
      }
    }
  }
  return false; //button not pressed, or not currently pressed
}

int resolveButtons(int side, int sidecolor, bool RedPressed, bool BluePressed)
{
  if(!(RedPressed||BluePressed)) //if nothing pressed, do nothing
  {
    return;
  }
  if(side == 0)
  {
    triggeredZero = true;
    Zeroposttimer = millis();
    if(RedPressed)
    {
      if(sidecolor == RED)
      {
        successZero = true;
      }
      else
      {
        successZero = false;
      }
    }
    if(BluePressed)
    {
      if(sidecolor == BLUE)
      {
        successZero = true;
      }
      else
      {
        successZero = false;
      }
    }
  }
  else
  {
    triggeredOne = true;
    Oneposttimer = millis();
    if(RedPressed)
    {
      if(sidecolor == RED)
      {
        successOne = true;
      }
      else
      {
        successOne = false;
      }
    }
    if(BluePressed)
    {
      if(sidecolor == BLUE)
      {
        successOne = true;
      }
      else
      {
        successOne = false;
      }
    }
  }
}


void onesecfade() //works out to roughly a 1000ms fade up and down
{
  float i;
  for(i=0;i<255;i+=2.61)
  {
    colorChange(0,int(i),int(i),int(i));
    colorChange(1,int(i),int(i),int(i));
    delay(5);
  }
  for(i=255;i>-1;i-=2.61)
  {
    colorChange(0,int(i),int(i),int(i));
    colorChange(1,int(i),int(i),int(i));
    delay(5);
  }
}

void rainbow(int *rdir, int *gdir, int *bdir)
{
  switch(*rdir)
  {
    case 0: //stay the same level
      break;
    case 1: //increase brightness
      R++;
      if(R == 255)
      {
        *rdir = 0;
      }
      break;
    case 2: //decrease brightness
      break;
  }
  switch(*gdir)
  {
    case 0: //stay the same level
      break;
    case 1: //increase brightness
      break;
    case 2: //decrease brightness
      break;    
  }
  switch(*bdir)
  {
    case 0: //stay the same level
      break;
    case 1: //increase brightness
      break;
    case 2: //decrease brightness
      break;    
  }
  colorChangeBoth(R,G,B);
}

void resetvalues()
{
  triggeredZero = false;
  triggeredOne = false;
  successZero = false;
  successOne = false;
  zerostayoff = false;
  onestayoff = false;
  int reddir = 0;
  int greendir = 0;
  int bluedir = 0;
  int r0last = HIGH;
  int b0last = HIGH;
  int r1last = HIGH;
  int b1last = HIGH;
  r0lastDebounceTime = 0;
  b0lastDebounceTime = 0;
  r1lastDebounceTime = 0;
  b1lastDebounceTime = 0;
  R0Pressed = false;
  R1Pressed = false;
  B0Pressed = false;
  B1Pressed = false;
}
