#include "ibus.h"
#include <PS2X_lib.h>

// How often to send data?
#define UPDATE_INTERVAL 2  // milliseconds

#define NUM_ANALOG_INPUTS 4
#define NUM_DIGITAL_INPUTS 16
bool digital[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

#define ANALOG_REFERENCE EXTERNAL
#define BAUD_RATE 115200
#define NUM_CHANNELS ((NUM_ANALOG_INPUTS) + (NUM_DIGITAL_INPUTS))

PS2X ps2x;

IBus ibus(NUM_CHANNELS);
int error = 0;

void setup() {
  // setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
  error = ps2x.config_gamepad(13, 11, 10, 12, false, false);

  if(error == 0)
  {
    Serial.println("Found Controller, configured successful");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Go to www.billporter.info for updates and to report bugs.");
  }

  analogReference(ANALOG_REFERENCE);  // use the defined ADC reference voltage source
  Serial.begin(BAUD_RATE);            // setup serial
}

void loop() {
  unsigned long time = millis();
  ps2x.read_gamepad();
  ibus.begin();

  ibus.write(map(ps2x.Analog(PSS_LY), 0, 255, 0, 1023));
  ibus.write(map(ps2x.Analog(PSS_LX), 0, 255, 0, 1023));
  ibus.write(map(ps2x.Analog(PSS_RY), 0, 255, 0, 1023));
  ibus.write(map(ps2x.Analog(PSS_RX), 0, 255, 0, 1023));

  if (ps2x.Button(PSB_GREEN))
    digital[0]=1;
  else
    digital[0]=0;
  if (ps2x.Button(PSB_RED))
    digital[1]=1;
  else
    digital[1]=0;
  if (ps2x.Button(PSB_BLUE))
    digital[2]=1;
  else
    digital[2]=0;
  if (ps2x.Button(PSB_PINK))
    digital[3]=1;
  else
    digital[3]=0;

  if (ps2x.Button(PSB_PAD_LEFT))
    digital[4]=1;
  else
    digital[4]=0;
  if (ps2x.Button(PSB_PAD_RIGHT))
    digital[5]=1;
  else
    digital[5]=0;
  if (ps2x.Button(PSB_PAD_UP))
    digital[6]=1;
  else
    digital[6]=0;
  if (ps2x.Button(PSB_PAD_DOWN))
    digital[7]=1;
  else
    digital[7]=0;

  if (ps2x.Button(PSB_L1))
    digital[8]=1;
  else
    digital[8]=0;
  if (ps2x.Button(PSB_L2))
    digital[9]=1;
  else
    digital[9]=0;
  if (ps2x.Button(PSB_L3))
    digital[10]=1;
  else
    digital[10]=0;
  if (ps2x.Button(PSB_R1))
    digital[11]=1;
  else
    digital[11]=0;
  if (ps2x.Button(PSB_R2))
    digital[12]=1;
  else
    digital[12]=0;
  if (ps2x.Button(PSB_R3))
    digital[13]=1;
  else
    digital[13]=0;

  if (ps2x.Button(PSB_START))
    digital[14]=1;
  else
    digital[14]=0;
  if (ps2x.Button(PSB_SELECT))
    digital[15]=1;
  else
    digital[15]=0;

  for (int i = 0; i < NUM_DIGITAL_INPUTS; i++)
    ibus.write(digital[i] == 0 ? 0 : 1023);

  ibus.end();

  time = millis() - time;  // time elapsed in reading the inputs
  if (time < UPDATE_INTERVAL)
    // sleep till it is time for the next update
    delay(UPDATE_INTERVAL - time);
}