

//declaration 
const int bpmInput= A4;        // Analog pin for the signal
int threshold = 60;         // Set a threshold value (halfway in 10-bit range)
unsigned long lastTime = 0;  // Time of the last crossing
unsigned long currentTime = 0;
float frequency = 0;
int inputState;

//vtask(500/portTICK_PERIOD_MS); delay in running freertos
void freq() {
    int value = inputState;

    // Detect if the signal crosses the threshold
    if (value > threshold && currentTime == 0) { // First crossing
        lastTime = micros();                    // Record the crossing time
    } else if (value > threshold && currentTime > 0) { // Subsequent crossings
        currentTime = micros();                 // Record the current time
        unsigned long period = currentTime - lastTime; // Calculate the period
        frequency = 1000000.0 / period;         // Frequency in Hz
        lastTime = currentTime;                 // Update last crossing time

        Serial.print("Frequency: ");
        Serial.print(frequency);
        Serial.println(" Hz");
        delay(500); // Wait to avoid rapid repeats
    }
}
void symptoms(int cardiograph){
  //condition parameters 
  int lic=50;
  int lsc=110;
  int n_hyper=0;
  int n_hypo=0;
  if(cardiograph < lic){
     n_hypo++;
  }
  if(cardiograph > lsc){
    n_hyper++;
  }
  delay(200);//to endsure you count every overlap once
}

void print(){
  //hedhi to print hypo and hyper so let it be for now
}

void setup() {
  Serial.begin(9600); 
  pinMode(bpmInput,INPUT);

}

void loop() {
 
  inputState =analogRead(bpmInput);
  Serial.println(inputState);
  //Serial.println(frequency);
  symptoms(inputState);
  freq();
  delay(550);
  //xtaskCreatePinnedToCore() //only in freertos in esp 
}