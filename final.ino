// C++ code
//
#define NOTE_C3 131
#define NOTE_D3 147
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_G3 196
#define NOTE_A3 220
#define NOTE_B3 247
#define NOTE_C4 262

int latchPin = 8;
int clockPin = 12;
int dataPin = 11;
int ledR = A0;
int ledB = A1;
int ledG = A2;
int buttonPin[] = {2, 3, 4, 5, 6, 7, 9, 10};

int b1 = 1, b2 = 1, b3 = 1, b4 = 1, 
    b5 = 1, b6 = 1, b7 = 1, b8 = 1;
int p1 = 1, p2 = 1, p3 = 1, p4 = 1,
    p5 = 1, p6 = 1, p7 = 1, p8 = 1;
int order = 0;
int noteNum[8] = { 0 };

int vR = 0, vG = 0, vB = 0;
int rVal[8] = {0, 255, 255, 0, 80, 0, 160, 255};
int gVal[8] = {0, 0, 224, 192, 208, 32, 32, 255};
int bVal[8] = {0, 0, 32, 0, 255, 255, 255, 255};

int note[] = {
  NOTE_C3, NOTE_D3, NOTE_E3, NOTE_F3, 
  NOTE_G3, NOTE_A3, NOTE_B3, NOTE_C4
};

byte colDataMatrix[8] = {
  B11111110,
  B11111101,
  B11111011,
  B11110111,
  B11101111,
  B11011111,
  B10111111,
  B01111111
};

byte rowDataMatrix[8] = { 0 };

void setup() {
  pinMode(latchPin,OUTPUT);
  pinMode(clockPin,OUTPUT);
  pinMode(dataPin,OUTPUT); 
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);
  for(int i = 0; i < 8; i++){
    pinMode(buttonPin[i], INPUT_PULLUP);
  }
}

void update(int n, int &order){
  int row = 1;
  for(int i = order; i < 7; i++){
    row *= 2;
  }
  noteNum[n] += row;
  rowDataMatrix[7 - n] = noteNum[n];
}

void edge(int &bs, int &ps, int n){
  if(bs == LOW && ps == HIGH){
    vR = rVal[n];
    vG = gVal[n];
    vB = bVal[n];
    if(order > 7){
      order = 0;
      for(int i = 0; i < 8; i++){
        noteNum[i] = 0;
        rowDataMatrix[i] = 0;
      }
    }
    update(n, order);
    order++;
  }
  delay(2); ps = bs;
}
void piano(){
  b1 = digitalRead(buttonPin[0]);
  b2 = digitalRead(buttonPin[1]);
  b3 = digitalRead(buttonPin[2]);
  b4 = digitalRead(buttonPin[3]);
  b5 = digitalRead(buttonPin[4]);
  b6 = digitalRead(buttonPin[5]);
  b7 = digitalRead(buttonPin[6]);
  b8 = digitalRead(buttonPin[7]);
  
  edge(b1, p1, 0);
  edge(b2, p2, 1);
  edge(b3, p3, 2);
  edge(b4, p4, 3);
  edge(b5, p5, 4);
  edge(b6, p6, 5);
  edge(b7, p7, 6);
  edge(b8, p8, 7);

  if(b1 == LOW)tone(13, note[0]);
  else if(b2 == LOW)tone(13, note[1]); 
  else if(b3 == LOW)tone(13, note[2]);
  else if(b4 == LOW)tone(13, note[3]);
  else if(b5 == LOW)tone(13, note[4]);
  else if(b6 == LOW)tone(13, note[5]);
  else if(b7 == LOW)tone(13, note[6]);
  else if(b8 == LOW)tone(13, note[7]);
  else noTone(13);
}
void loop() {
  piano();
  analogWrite(ledR,vR);
  analogWrite(ledG,vG);
  analogWrite(ledB,vB);
  for(int i = 0; i < 8 ; i++){
    digitalWrite(latchPin,LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, colDataMatrix[i]);
    shiftOut(dataPin, clockPin, LSBFIRST, rowDataMatrix[i]);
    digitalWrite(latchPin,HIGH);
    delay(1);
  }
}

