#include <Arduino.h>



void setup() {
  Serial.begin(115200);
  // esp_spiram_init();
  // size_t psram_size = esp_spiram_get_size();
  // psramInit();
  // Serial.printf("PSRAM size: %d bytes\n", psram_size);
  // Serial.printf("Deafult free size: %d\n" aps_get_free_size(MALLOC_CAP_SPIRAM));
  Serial.printf("Deafult free size: %d\n", heap_caps_get_free_size(MALLOC_CAP_DEFAULT));
  Serial.printf("PSRAM free size: %d\n", heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
}

void loop()
{
  Serial.println("ok");
  delay(3000);
}
