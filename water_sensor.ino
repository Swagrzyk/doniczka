const int waterSensorPin = 2; // Przypisz pin czujnika wody

void setup() {
  Serial.begin(9600); // Inicjalizacja komunikacji szeregowej
  pinMode(waterSensorPin, INPUT); // Ustaw pin czujnika wody jako wejście
}

void loop() {
  int waterLevel = digitalRead(waterSensorPin); // Odczytaj stan czujnika wody

  if (waterLevel == HIGH) {
    Serial.println("Woda wykryta"); // Jeśli czujnik wykryje wodę, wyświetl komunikat
  } else {
    Serial.println("Brak wody"); // Jeśli czujnik nie wykryje wody, wyświetl komunikat
  }

  delay(1000); // Poczekaj sekundę przed kolejnym odczytem
}