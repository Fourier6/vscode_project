#include <Arduino.h>




void setup() {
  delay(500);
  Serial.begin(115200);
  delay(500);
  // Serial.printf("Deafult free size: %d\n", heap_caps_get_free_size(MALLOC_CAP_DEFAULT));
  // Serial.printf("PSRAM free size: %d\n", heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
  uint8_t *psram_buffer = (uint8_t *)heap_caps_malloc(1024, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
  memset(psram_buffer, 0, 1024);
}

void loop() {
  Serial.println("ok");
  delay(1000);
}
