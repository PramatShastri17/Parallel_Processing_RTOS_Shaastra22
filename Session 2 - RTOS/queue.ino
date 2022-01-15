/*
 * Sending data to the serial terminal using a queue.
 */
 
// We will be using only one core of ESP32
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else 
static const BaseType_t app_cpu = 1;
#endif

// Settings
static const uint8_t msg_queue_len = 5;

// Globals
static QueueHandle_t msg_queue;

// Task: Read message from Serial buffer
void printMessages(void *parameters) {

  int item;

  // Loop forever
  while(1){

    // See if there's a message in the queue (do not block)
    if(xQueueReceive(msg_queue, (void*)&item, 0) == pdTRUE){
    // Serial.println(item);
    }
    Serial.println(item);

    // Wait before trying again
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void setup() {
  // Initializing serial terminal 
  Serial.begin(115200);
  
  // We wait for a moment so that we don t miss Serial output
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Queue Demo---");
  
  // Create our queue
  msg_queue = xQueueCreate(msg_queue_len, sizeof(int));

  // Start the only task here
  xTaskCreatePinnedToCore(
            printMessages,      // Function to to be called
            "Print Messages",   // Name of Task
            1024,           // Stack Size (bytes in ESP32)
            NULL,           // Parameter to pass to function
            1,              // TAsk priority (0 to configMAX_PRIORITIES - 1)
            NULL,           // Task handle
            app_cpu         // Run on one core for this project (ESP32 Only)
  );
}

void loop() {
  static int num = 0;

  // Try to add item to queue for 10 ticks, fail if queue is full
  if(xQueueSend(msg_queue, (void*)&num, 10) != pdTRUE){
    Serial.println("Queue Full");			// Error Mesage
  }
  num++;		// Increment number

  // Wait before trying again
  vTaskDelay(1000/ portTICK_PERIOD_MS);
}
