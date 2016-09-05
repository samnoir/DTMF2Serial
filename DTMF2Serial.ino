/* ==================================================================================================================================================
         Project: DTMF2Serial
          Author: Sreeni N
         Created: 02 September 2016
     Arduino IDE: 1.6.10
         Website:
     Description: To read the parallel out put from the decoder from WunRom and print it as strings on the serial port
===================================================================================================================================================== */



//Global variables-----------------------------------------------------------------------------------------
byte DTMFread;            // The DTMFread variable will be used to interpret the output of the DTMF module.
const int STQ = 3;        // Attach DTMF Module STQ Pin to Arduino Digital Pin 3
 const int Q4 = 4;        // Attach DTMF Module Q4  Pin to Arduino Digital Pin 4
 const int Q3 = 5;        // Attach DTMF Module Q3  Pin to Arduino Digital Pin 5
 const int Q2 = 6;        // Attach DTMF Module Q2  Pin to Arduino Digital Pin 6
 const int Q1 = 7;        // Attach DTMF Module Q1  Pin to Arduino Digital Pin 7

 long lastDigitReadTime = 0;
 const long MAX_READ_DELAY = 100; //ms

char digitMap[]  = {(char)0x00, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '*', '#'};

/*=========================================================================================================
setup() : will setup the pins to receive input, clear rthe write buffer etc.
========================================================================================================== */
void setup() {

  //Setup the INPUT pins on the Arduino
  pinMode(STQ, INPUT);
  pinMode(Q4, INPUT);
  pinMode(Q3, INPUT);
  pinMode(Q2, INPUT);
  pinMode(Q1, INPUT);
  Serial.begin(9600);
}



char digitConvert(byte in) {
  if (in < sizeof(digitMap)) {
    return digitMap[in];
  }
  return (char)0x00;
  
}

void writeBuffer(char buf[], int size) {
  int i;
  
  Serial.write(0xff); // Header
  for (i = 0; i < size; i++) {
    Serial.write(buf[i]);
  }
  Serial.write((byte) 0x00); // Trailer
}
/*=========================================================================================================
loop() : Arduino will interpret the DTMF module output and progressvely construct the write buffer
========================================================================================================== */
void loop() {
  long now = millis();
  byte digit;

  if(digitalRead(STQ)==HIGH && (now - lastDigitReadTime > MAX_READ_DELAY)){       //When a DTMF tone is detected, STQ will read HIGH for the duration of the tone.
    DTMFread=0;
    if(digitalRead(Q1)==HIGH){      //If Q1 reads HIGH, then add 1 to the DTMFread variable
      DTMFread=DTMFread+1;
    }
    if(digitalRead(Q2)==HIGH){      //If Q2 reads HIGH, then add 2 to the DTMFread variable
      DTMFread=DTMFread+2;
    }
    if(digitalRead(Q3)==HIGH){      //If Q3 reads HIGH, then add 4 to the DTMFread variable
      DTMFread=DTMFread+4;
    }
    if(digitalRead(Q4)==HIGH){      //If Q4 reads HIGH, then add 8 to the DTMFread variable
      DTMFread=DTMFread+8;
    }

    if ((digit = digitConvert(DTMFread)) != 0x00)
      Serial.write(digit);
      
    lastDigitReadTime = now;
  }

}
