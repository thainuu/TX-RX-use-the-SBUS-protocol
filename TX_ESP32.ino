#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const uint64_t pipeOut = 161202270703LL;
RF24 radio(33, 34); // CE, CSN

struct Signal {
  byte roll;
  byte pitch;
  byte yaw;
  byte throttle;
  byte analog1;
  byte analog2;
  byte aux1;
  byte aux2;
  byte aux3;
  byte aux4;
  byte aux5;
  byte aux6;
  byte aux7;
  byte aux8;
};

Signal signalData;

int Throttle = 0;

void setup() {
  Serial.begin(115200);

  pinMode(9, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP); 
  pinMode(21, INPUT_PULLUP); 
  pinMode(39, INPUT_PULLUP);
  pinMode(40, INPUT_PULLUP);
  pinMode(18, INPUT_PULLUP); 

  pinMode(15, OUTPUT);
  digitalWrite(15, HIGH);  
  
  // Cấu hình radio
  radio.begin();
  radio.openWritingPipe(pipeOut);
  radio.setChannel(113);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();
}

void loop() {
  if (analogRead(13) > 5300 && analogRead(13) <= 8191) { Throttle --;               
  } else if (analogRead(13) < 4900 && analogRead(13) >= 0) { Throttle ++;                 
  }
  signalData.throttle = constrain(Throttle, 0, 255);

//-------------------------------------------------------------------
  if (analogRead(10) >= 2000 && analogRead(10) <= 7500) { signalData.roll = 128;  //yaw
  } if (analogRead(10) < 2000) { signalData.roll = map(analogRead(10), 2000, 0, 128, 0);
  } if (analogRead(10) > 7500) { signalData.roll = map(analogRead(10), 7500, 8191, 128, 255);
  }

//-------------------------------------------------------------------
  if (analogRead(2) >= 5200 && analogRead(2) <= 5300) { signalData.yaw = 128;    //roll
  } if (analogRead(2) < 5200) { signalData.yaw = map(analogRead(2), 5200, 0, 128, 0);
  } if (analogRead(2) > 5300) { signalData.yaw = map(analogRead(2), 5300, 8191, 128, 255);
  }

//-------------------------------------------------------------------
  if (analogRead(4) >= 5000 && analogRead(4) <= 5200) { signalData.pitch = 128;  
  } if (analogRead(4) < 5000) { signalData.pitch = map(analogRead(4), 5000, 0, 128, 255);
  } if (analogRead(4) > 5200) { signalData.pitch = map(analogRead(4), 5200, 8191, 128, 0);
  }

  signalData.analog1 = map(analogRead(6), 0, 8191, 0, 255);
  signalData.analog2 = map(analogRead(8), 0, 8191, 0, 255);

  if (digitalRead(39)) {
    bitWrite(signalData.aux1, 0, 0);
  } else {
    bitWrite(signalData.aux1, 0, 1);
  }

  if (digitalRead(40)) {
    bitWrite(signalData.aux2, 0, 0);
  } else {
    bitWrite(signalData.aux2, 0, 1);
  }

  if (digitalRead(9)) {
    bitWrite(signalData.aux3, 0, 0);
  } else {
    bitWrite(signalData.aux3, 0, 1);
  }

  if (digitalRead(5)) {
    bitWrite(signalData.aux4, 0, 0);
  } else {
    bitWrite(signalData.aux4, 0, 1);
  }

  if (digitalRead(21)) {
    bitWrite(signalData.aux5, 0, 0);
  } else {
    bitWrite(signalData.aux5, 0, 1);
  }

  if (digitalRead(18)) {
    bitWrite(signalData.aux6, 0, 0);
  } else {
    bitWrite(signalData.aux6, 0, 1);
  }

  if (digitalRead(7)) {
    bitWrite(signalData.aux7, 0, 0);
  } else {
    bitWrite(signalData.aux7, 0, 1);
  }

  if (digitalRead(11)) {
    bitWrite(signalData.aux8, 0, 0);
  } else {
    bitWrite(signalData.aux8, 0, 1);
  }
/*
  //Serial.print("Throttle: ");
  //Serial.println(signalData.throttle);
  //Serial.print("Yaw: ");
  //Serial.println(signalData.yaw); 
  //Serial.print("Roll: ");
  //Serial.println(signalData.roll); 
  //Serial.print("Pitch: ");
  //Serial.println(signalData.pitch);

  //Serial.print("analog1: ");
  //Serial.println(signalData.analog1);
  //Serial.print("analog2: ");
  //Serial.println(signalData.analog2); 

  //Serial.print("sw1: ");
  //Serial.println(signalData.aux1); 
  //Serial.print("sw2: ");
  //Serial.println(signalData.aux2);
  //Serial.print("sw3: ");
  //Serial.println(signalData.aux3); 
  //Serial.print("sw4: ");
  //Serial.println(signalData.aux4);
  //Serial.print("sw5: ");
  //Serial.println(signalData.aux5);
  //Serial.print("sw6: ");
  //Serial.println(signalData.aux6);
  //Serial.print("sw7: ");
  //Serial.println(signalData.aux7);
  //Serial.print("sw8: ");
  //Serial.println(signalData.aux8);
*/
  radio.write(&signalData, sizeof(Signal));

  delay(10); // Tùy chỉnh độ trễ theo nhu cầu
}
