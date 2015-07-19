int blinkPin = 0;

// NovaLabsCadClassTest 2
// LED1 PB3
// lED2 PB4

#define LED1 3
#definde LED2 4
void setup()
{
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
}

void loop()
{
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  delay(100);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  
  delay(100);
}
