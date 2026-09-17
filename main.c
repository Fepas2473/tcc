#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

int t = 5;

void app_main() {
   gpio_set_direction(2,GPIO_MODE_OUTPUT);

  for(t=5; t>=0; t--)
  {
    printf("%d\n",t);
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }

   gpio_set_level(2,1);

   while (true) {
    vTaskDelay(250 / portTICK_PERIOD_MS);
   }
}

  

