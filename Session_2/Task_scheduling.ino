/*
 * FreeRTOS Task Scheduling Demo
 */
 
// We will be using only one core of ESP32
#if CONFIG_FREERTOS_UNICORE
	static const BaseType_t app_cpu = 0;
#else 
	static const BaseType_t app_cpu = 1;
#endif

// Message
const char msg[] = "Lorem ipsum dolor sit integer.";

// Task handles
static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL;

// Tasks

// Our task: Print to serial terminal with lower priority
void startTask1(void *parameter){
  int msg_len = strlen(msg);

  //Print to serial monitor
  while(1){
    Serial.println();

    for(int i = 0; i < msg_len; i++){
      Serial.print(msg[i]);
    }
    Serial.println();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// Our task: Print to serial terminal with higher priority
void startTask2(void *parameter){
  while(1){
    Serial.print("*");
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

//Main: Runs as its own task with priority 1 on core 1
void setup() {
  // Initializing serial terminal 
  Serial.begin(300);
  
  // We wait for a moment so that we don t miss Serial output
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Task Demo---");

  //Printing the self priority
  Serial.print("Setup and loop task  running on core ");
  Serial.print(xPortGetCoreID());
  Serial.print(" with priority ");
  Serial.println(uxTaskPriorityGet(NULL));
  
  // Task to run forever
  xTaskCreatePinnedToCore(
            startTask1,      // Function to to be called
            "Task 1",        // Name of Task
            1024,            // Stack Size (bytes in ESP32)
            NULL,            // Parameter to pass to function
            1,               // TAsk priority (0 to configMAX_PRIORITIES - 1)
            &task_1,         // Task handle
            app_cpu          // Run on one core for this project (ESP32 Only)
  );

  xTaskCreatePinnedToCore(
            startTask2,      // Function to to be called
            "Task 2",        // Name of Task
            1024,            // Stack Size (bytes in ESP32)
            NULL,            // Parameter to pass to function
            2,               // Task priority (0 to configMAX_PRIORITIES - 1)
            &task_2,         // Task handle
            app_cpu          // Run on one core for this project (ESP32 Only)
  );

}

void loop() {
  // Suspend the higher priority task for some intervals
  for(int i = 0; i < 3; i++){
    vTaskSuspend(task_2);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    vTaskResume(task_2);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }

  // Delete lower priority task
  if(task_1 != NULL){
    vTaskDelete(task_1);
    task_1 = NULL;
  }

}
