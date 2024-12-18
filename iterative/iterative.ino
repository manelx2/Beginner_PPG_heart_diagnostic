const int bpmInput = A4;  // Analog pin for the signal
int threshold = 60;       // Threshold value
unsigned long lastTime = 0;
unsigned long currentTime = 0;
float frequency = 0;
int n_hypo = 0;
int n_hyper = 0;
int lic = 50;
int lsc = 110;
#define LED_PIN 2
void setup() {
  Serial.begin(9600);
  pinMode(bpmInput, INPUT);
  pinMode(LED_PIN, OUTPUT);
  
}

void loop() {
  int inputState = analogRead(bpmInput);
  Serial.println(inputState);

  // Frequency calculation
  if (inputState > threshold) {
    if (currentTime == 0) {
      lastTime = micros();  // First crossing
    } else {
      currentTime = micros();
      unsigned long period = currentTime - lastTime; // Calculate the period
      frequency = 1000000.0 / period; // Frequency in Hz
      lastTime = currentTime;

      //Serial.print("Frequency: ");
      //Serial.print(frequency);
      //Serial.println(" Hz");
    }
  }

  // Hypo/Hyper count
  if (inputState < lic) {
    n_hypo++;
  }
  if (inputState > lsc) {
    n_hyper++;
  }

  delay(550); // Small delay for stability
   if(n_hypo>1||n_hyper>1){
  digitalWrite(LED_PIN, HIGH);
  delay(200);
  }
  digitalWrite(LED_PIN, LOW);
}
