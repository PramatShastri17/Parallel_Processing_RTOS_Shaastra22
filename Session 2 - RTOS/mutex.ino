/*
 * Mutex - Increment a shared global variable with mutex protection.
 */

// We will be using only one core of ESP32
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else 
static const BaseType_t app_cpu = 1;
#endif

// Globals
static int shared_var = 0;
static SemaphoreHandle_t mutex;

//*****************************************************************************
// Tasks

// Increment shared variable
void incTask(void *parameters) {

  int local_var;

  // Loop forever
  while (1) {

    // Take the mutex prior to crtitcal section of code
    if(xSemaphoreTake(mutex,0) == pdTRUE){
      // Critical section of code
      // Controls the outcome of the tasks
      shared_var++;
      vTaskDelay(random(10, 100) / portTICK_PERIOD_MS);

      // Give the mutex after critical section
      xSemaphoreGive(mutex);

      // Print out new shared variable
      Serial.println(shared_var);
    } else{
      // Additional tasks to perform while waiting
    }
  }
}

//*****************************************************************************
// Main (runs as its own task with priority 1 on core 1)

void setup() {

  // Hack to kinda get randomness
  randomSeed(analogRead(0));

  // Configure Serial
  Serial.begin(115200);

  // Wait a moment to start (so we don't miss Serial output)
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Race Condition Demo---");

  // Create mutex before starting tasks
  mutex = xSemaphoreCreateMutex();

  // Start task 1
  xTaskCreatePinnedToCore(incTask,
                          "Increment Task 1",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);

  // Start task 2
  xTaskCreatePinnedToCore(incTask,
                          "Increment Task 2",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);           

  // Delete "setup and loop" task
  vTaskDelete(NULL);
}

void loop() {
  // Execution should never get here
}