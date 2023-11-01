#include <Arduino_FreeRTOS.h> 
#include <semphr.h> 

static void vHandlerTask( void *pvParameters );  
static void vPeriodicTask( void *pvParameters ); 
static void ultraSonic( void *pvParameters);    
static void vExampleInterruptHandler( void );  

int interruptPin = 3;
int counter = 0; 
int encoder_hole = 20;
int distanceFront = 0;
int distanceRight = 0;
int distanceLeft = 0;
float kecepatan;
unsigned long startTime = 0;
unsigned long elapsedTime = 0;
unsigned long prevTime, currentTime;
String direction = "Unknown Direction";

SemaphoreHandle_t xBinarySemaphore;  
SemaphoreHandle_t xSerialSemaphore; 

void setup() 
{
   Serial.begin(9600);         
   xBinarySemaphore = xSemaphoreCreateBinary();             
   pinMode(interruptPin, OUTPUT);      
   attachInterrupt(digitalPinToInterrupt(interruptPin), vExampleInterruptHandler, RISING);      // rising menunjukan dari low ke high\
   

   if( xBinarySemaphore != NULL )     
    {           
       xTaskCreate( vHandlerTask, "Handler", 200, NULL, 3, NULL );
       xTaskCreate( ultraSonic, "sensor Ultrasonic", 200, NULL, 2, NULL );        
       xTaskCreate( vPeriodicTask, "Periodic", 200, NULL, 1, NULL );             
       vTaskStartScheduler();     
    }         
    for( ;; );   
}   

void ultraSonic( void *pvParameters)  // This is a Task.
{   
  for(;;)
  {
    distanceFront = random(1, 400);
    distanceRight = random(1, 400);
    distanceLeft = random(1, 400);
      
    vTaskDelay(pdMS_TO_TICKS(1000));  
  }
}    
    
static void vHandlerTask( void *pvParameters )  
{         
    xSemaphoreTake( xBinarySemaphore, 0);          
    for( ;; )     
    {           
      xSemaphoreTake( xBinarySemaphore, portMAX_DELAY );   
      //Serial.println( "Handler task - Processing event.\r\n" ); // lapor semaphore sudah terjadi baru isi dengan logic
      currentTime=millis();
      float deltaTime=0;
      if(prevTime != currentTime){
          deltaTime=currentTime-prevTime;
          prevTime=currentTime;
        }
      if(counter==encoder_hole){
        kecepatan = encoder_hole / (deltaTime*0.001);
        Serial.print("Time max: ");
        Serial.print(deltaTime/1000); // hitung waktu
        Serial.print("s");
        // Serial.print(" Speed: ");
        // Serial.print(kecepatan); // hitung waktu
        // Serial.println(" m/s");
        Serial.println("");
        counter=0;
        deltaTime=0;
        Serial.println(currentTime);
      }
      else{
        Serial.print(counter); // hitung yang setiap saat
        Serial.print(" ");
        Serial.print(deltaTime/1000);
        Serial.println("s");
      }
     }
}


static void vPeriodicTask( void *pvParameters ) 
{         
  for( ;; )     
  {              
    // vTaskDelay( 500 / portTICK_PERIOD_MS );
    vTaskDelay(pdMS_TO_TICKS(1000));
              
    //Serial.println( "Perodic task - About to generate an interrupt.\r\n" );       
    digitalWrite(interruptPin, LOW);    
    digitalWrite(interruptPin, HIGH);  
    //Serial.println( "Periodic task - Interrupt generated.\r\n\r\n\r\n" );     
  }  
}


static void  vExampleInterruptHandler( void )   // ketika interupt masuk dari low ke high maka routine ini dieksekusi
{   
  static signed portBASE_TYPE xHigherPriorityTaskWoken;      
  xHigherPriorityTaskWoken = pdFALSE;    
  xSemaphoreGiveFromISR( xBinarySemaphore, (signed portBASE_TYPE*)&xHigherPriorityTaskWoken );       
  if( xHigherPriorityTaskWoken == pdTRUE )     
  {       
    counter++;      
    vPortYield();     
   }   
} 


void loop() 
{
  // put your main code here, to run repeatedly:

}
