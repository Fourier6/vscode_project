#include "task_mpu6500.h"
TaskHandle_t Task_mpu6500;
float pitch;
const int csPin = 12;  // Chip Select Pin
bool useSPI = true;    // SPI use flag
MPU6500_WE myMPU6500 = MPU6500_WE(&SPI, csPin, useSPI);

void mpu6500_init(void);
void task_mpu6500 (void *pvParameters)
{
  const TickType_t xFrequency = pdMS_TO_TICKS(10); 
  mpu6500_init();
    while(1)
  {
    TickType_t xLastWakeTime = xTaskGetTickCount();
      pitch=myMPU6500.getPitch();
      // Serial.println(pitch);
      // Serial.print("Pitch=");
      // Serial.println(Pitch);
      vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}
void mpu6500_init(void)
{
    SPI.begin(9,11,10,12);
  // Wire.begin();
    if(!myMPU6500.init()){
    Serial.println("MPU6500 does not respond");
    }
    else{
    Serial.println("MPU6500 is connected");
    }
    /* Choose the SPI clock speed, default is 8 MHz 
        This function must be used only after init(), not before */
    //myMPU9250.setSPIClockSpeed(4000000);

    Serial.println("Position you MPU6500 flat and don't move it - calibrating...");
    vTaskDelay(500);
    myMPU6500.autoOffsets();
    Serial.println("Done!");
    
  //myMPU6500.setAccOffsets(-14240.0, 18220.0, -17280.0, 15590.0, -20930.0, 12080.0);
  //myMPU6500.setGyrOffsets(45.0, 145.0, -105.0);
    myMPU6500.enableGyrDLPF();
  //myMPU6500.disableGyrDLPF(MPU6500_BW_WO_DLPF_8800); // bandwdith without DLPF
    myMPU6500.setGyrDLPF(MPU6500_DLPF_6);
    myMPU6500.setSampleRateDivider(5);
    myMPU6500.setGyrRange(MPU6500_GYRO_RANGE_250);
    myMPU6500.setAccRange(MPU6500_ACC_RANGE_2G);
    myMPU6500.enableAccDLPF(true);
    myMPU6500.setAccDLPF(MPU6500_DLPF_6);
  //myMPU6500.enableAccAxes(MPU6500_ENABLE_XYZ);
  //myMPU6500.enableGyrAxes(MPU6500_ENABLE_XYZ);
}