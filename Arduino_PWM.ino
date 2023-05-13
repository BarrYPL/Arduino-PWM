static int order[4] = {0, 0, 0 };
int pwmPins[] = { 10, 9, 5 };
char inChar;
char inData[40];
char argFt[5];
char argSc[5];
char argTr[5];
int flag = 0;
byte incommingOrderFlag = 0;

void setup() {
  pinMode(9, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  Serial.begin(115200);
  setTimers();
  sei();
}

void loop() {
  int index1 = 0;
  int index2 = 0;
  int index3 = 0;
  while (Serial.available() > 0) {
    inChar = Serial.read();
    if (inChar == ',') {
      flag++;
    }
    if (flag == 0) {
      argFt[index1] = inChar;
      index1++;
      argFt[index1] = '\0';
    }
    if ((flag == 1) && (index1 == 0)) {
      if (inChar != ',') {
        argSc[index2] = inChar;
        index2++;
        argSc[index2] = '\0';
      }
    }
    if ((flag == 2) && (index2 == 0)) {
      if (inChar != ',') {
        argTr[index3] = inChar;
        index3++;
        argTr[index3] = '\0';
      }
    }
    if (flag == 1) {
      index1 = 0;
    }
    if (flag == 2) {
      index2 = 0;
    }
    if (flag == 3) {
      index3 = 0;
    }
    incommingOrderFlag = 1;
  }
  if (flag == 2) {
    order[0] = atoi(argFt);
    order[1] = atoi(argSc);
    order[2] = atoi(argTr);
    flag = 0;
  }
  if (incommingOrderFlag == 1) {
    applyChanges();
    incommingOrderFlag = 0;
    order[0] = -1;
    order[1] = -1;
    order[2] = -1;
  }

  for (int a = 0; a < 255;a++){
    analogWrite(10,a);
    delay(20);
    //Serial.println(a);
  }
  for (int a = 255; a >0; a--){
    analogWrite(10,a);
    delay(20);
    //Serial.println(a);
  }
}

int pwmPin(int k) {
  int pin = 0;
  if (k > 0 && k < 4) {
    k = k - 1;
    pin = pwmPins[k];
  } else {
    pin = 0;
  }
  return pin;
}

void applyChanges() {
//  Serial.print("order[0] = ");
//  Serial.print(order[0]);
//  Serial.println("");
//  Serial.print("order[1] = ");
//  Serial.print(order[1]);
//  Serial.println("");
//  Serial.print("order[2] = ");
//  Serial.print(order[2]);
//  Serial.println("");
  for (byte i = 0; i < sizeof(order) - 1; i++) {
    if (order[i] >= 0 && order[i] <= 255){
      analogWrite(pwmPins[i],order[i]);
    }
  }  
}

void setTimers(){
  TCCR1A = 0;
  TCCR1B = 0; 
  TCCR3A = 0;

  TCCR3A |= (1 << COM3A1);
  TCCR1A |= (1 << COM1A1) | (1 << COM1B1);
  OCR1B = 0;
  OCR1A = 0;
  OCR3A = 0;
  TCCR3A |= (1 << WGM10);
  TCCR3A |= (1 << WGM12) | (1 << CS12);
  TCCR1A |= (1 << WGM10);
  TCCR1B |= (1 << WGM12) | (1 << CS12);
}
