/*
 * Memory Allocation - Generate a stack overflow on purpose.
 */

// We will be using only one core of ESP32
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else 
static const BaseType_t app_cpu = 1;
#endif

// Task: Perform some mundane task
void testTask(void *parameter){
  while(1){
    int a = 1;
    int b[100];
    
    // Doing something with the array
    for(int i = 0; i < 100; i++){
      b[i] = a+1;
    }
    Serial.println(b[0]);
  }
}

void setup() {
  // Initializing serial terminal 
  Serial.begin(115200);
  
  // We wait for a moment so that we don t miss Serial output
  vTaskDelay(500 / portTICK_PERIOD_MS);
  Serial.println("---FreeRTOS Memory Demo---");
  
  // Strat the only task here
  xTaskCreatePinnedToCore(
            testTask,      // Function to to be called
            "Test Task",   // Name of Task
            1500,           // Stack Size (bytes in ESP32)
            NULL,           // Parameter to pass to function
            1,              // TAsk priority (0 to configMAX_PRIORITIES - 1)
            NULL,           // Task handle
            app_cpu         // Run on one core for this project (ESP32 Only)
  );

  // Delete the setup and loop tasks
  vTaskDelete(NULL);
}

void loop() {
  // Execution should no come here
}
