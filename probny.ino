int sensor_A0 = D2;    // podłączenie od A0 na czujniku do A0 na Arduino
int sensor_D0 = D4;     // podłączenie od D0 na czujniku do pinu 2 na Arduino
int sensor_A = D5;
int wartosc_A0;        // zmienna dla wartości A0
int wartosc_D0;        // zmienna dla wartości A0
int wartosc_A;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);   // uruchomienie monitora szeregowego
 pinMode(D4, INPUT);    // ustawienie pinu 2 jako wejście
 pinMode(D5, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
 wartosc_A0 = analogRead(sensor_A0);      // pobranie wartości z A0
 wartosc_D0 = digitalRead(sensor_D0);     // pobranie wartości z D0
 wartosc_A = analogRead(sensor_A);
 
 //Serial.print("D0: ");                    // wyświetlenie na monitorze szeregowym
 //Serial.print(wartosc_D0);
 //Serial.print("  --  A0: ");
 //Serial.println(wartosc_A0);
   Serial.print("Moisture Sensor Value:");
  Serial.println(analogRead(wartosc_A0));
  delay(100);
 delay(200);                              // opóźnienie pomiędzy kolejnymi odczytami
}
