const int out_pin = 5;

void carrier ();
void setup() 
{
  // put your setup code here, to run once:
  pinMode(out_pin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  carrier();
}

void carrier()
{
  for(int i = 0 ; i <=52 ; i++)//57
  {
  digitalWrite(out_pin, HIGH);
  delayMicroseconds(10);//10
  digitalWrite(out_pin, LOW);
  delayMicroseconds(10);//10
  }
  digitalWrite(out_pin, LOW);
  delay(4);//180 hz delay(4)
}