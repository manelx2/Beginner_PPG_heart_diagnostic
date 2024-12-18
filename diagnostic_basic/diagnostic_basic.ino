
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
//configuration pour utiliser un seul core de l'esp32
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu=0;
#else
static const BaseType_t app_cpu=1;
#endif
// declaration
const int bpmInput= A4; 
int cardiograph;
int n_hyper = 0;      // Hyper count
int n_hypo = 0;       // Hypo count
int threshold =10;
float frequency=0;
unsigned long lastTime = 0;  // Time of the last crossing
unsigned long currentTime = 0;
TaskHandle_t DangerTaskHandle = NULL;
#define LED_PIN 2 
void Symptoms(void *pvParameters);
void Danger( void *pvParameters ); 
void setup() {
    Serial.begin(9600);
    pinMode(LED_PIN, OUTPUT);
    // Create a FreeRTOS task
    xTaskCreatePinnedToCore(
        Symptoms,      // Function to implement the task
        "Symptoms",    // Name of the task (for debugging)
        2048,            // Stack size (in words, not bytes)
        NULL,            // Task input parameter
        1,               // Priority (higher value = higher priority)
        NULL,             // Task handle (optional)
        app_cpu
    );
    xTaskCreatePinnedToCore(
        Danger,      // Function to implement the task
        "Danger",    // Name of the task (for debugging)
        2048,            // Stack size (in words, not bytes)
        NULL,            // Task input parameter
        2,               // Priority (higher value = higher priority)
        NULL,             // Task handle (optional)
        app_cpu
    );

}

void loop() {
    // Main code running in loop()
    //Serial.println("Loop task is running...");
    cardiograph=analogRead(bpmInput);
     Serial.print("S:");
    Serial.println(cardiograph);
    delay(500);
}

// Task function definition
void Symptoms(void *pvParameters) {
    const int lic = 60;   // Lower threshold
    const int lsc = 175;  // Upper threshold
    int value = 0;

    // Detect if the signal crosses the threshold  
      while (1) {
        Serial.println("Task1 is running...");
        value = cardiograph;
         if (value > threshold && currentTime == 0) { // First crossing
        lastTime = micros();                    // Record the crossing time
         } else if (value > threshold && currentTime > 0) { // Subsequent crossings
        currentTime = micros();                 // Record the current time
        unsigned long period = currentTime - lastTime; // Calculate the period
        frequency = 1000000.0 / period;         // Frequency in Hz
        lastTime = currentTime;                 // Update last crossing time
         }
        Serial.println("Heart rate= "+String(frequency));
        if (cardiograph < lic) {
            n_hypo++;
        }
        if (cardiograph > lsc) {
            n_hyper++;
        }
         if (n_hyper >= 3 || n_hypo >= 3) {
            Serial.println("Threshold reached! Resuming Danger task...");
            vTaskResume(DangerTaskHandle);
        }

        // Debug counts
        //Serial.print("Hypo Count: ");
        //vTaskDelay(200 / portTICK_PERIOD_MS);
        //Serial.println(n_hypo);
        //vTaskDelay(200 / portTICK_PERIOD_MS);
        //Serial.print("Hyper Count: ");
        //Serial.println(n_hyper);

       vTaskDelay(pdMS_TO_TICKS(300)); 
    }
}
//danger definition
void Danger(void *pvParameters){
  while (1) {
        Serial.println("Danger task is running!");

        // Simulate danger handling
        digitalWrite(LED_PIN, HIGH);
        vTaskDelay(pdMS_TO_TICKS(2000)); // Simulated work (2 seconds)
        
        // Reset thresholds and suspend the task
        n_hyper = 0;
        n_hypo = 0;
        digitalWrite(LED_PIN, LOW);
        Serial.println("Danger handled. Suspending task...");
        vTaskSuspend(NULL); // Suspend itself
    }
}
