
// Área de inclusão das bibliotecas
//-----------------------------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "HCF_SOFT.h"  
#include "HCF_DHT.h"
#include "driver/gpio.h"
#include "HCF_WNOLOGY.h"

#define TRIG_PIN 19  // Defina o pino TRIG
#define ECHO_PIN 21  // Defina o pino ECHO
#define DHT_PIN 23 //Defina o pino de dados o DHT /23


// Área das macros
//-----------------------------------------------------------------------------------------------------------------------
//#define MODO 1 // Automatico
#define MODO 0 // Manual

#define ESP 1 

#define WIFI_SSID "coqueiro"
#define WIFI_PASS "amigos12"

//#define WIFI_SSID "GUEST"
//#define WIFI_PASS "cade204820"


    #define DEVICE_ID "65774aa82623fd911ab650c1" //ESP1

#define W_ACCESS_KEY "76ac5ed2-ed18-4e96-9e02-d2dd572db083" //use a chave de acesso e a senha
#define W_PASSWORD "f52797619b7205bc2ac8d796d80fd0cb23f988e882cd0b82d575b26939f78c1c"

// Área de declaração de variáveis e protótipos de funções
//-----------------------------------------------------------------------------------------------------------------------

char *TAG = "HCF";
uint8_t entradas, saidas = 0; //variáveis de controle de entradas e saídas
char tecla = '-' ;
char escrever[40];
bool direcao = false;
int angulo = 0;
float temperatura = 0.0, umidade = 0.0;

// Funções e ramos auxiliares
//-----------------------------------------------------------------------------------------------------------------------


// Função para receber dados e extrair o JSON do tópico command
/*
*   "name": "Ativando"
*   "payload": {"LED":"true"}
*   "time": "2025-05-05T16:15:22.263Z"
*/
void handler_led(const char *value) {
    if (strcmp(value, "true") == 0) {
        gpio_set_level(GPIO_NUM_2, 1);
    } else {
        gpio_set_level(GPIO_NUM_2, 0);
    }
}



// Programa Principal
//-----------------------------------------------------------------------------------------------------------------------

void app_main(void)
{
    /////////////////////////////////////////////////////////////////////////////////////   Programa principal

    /////////////////////////////////////////////////////////////////////////////////////   Inicializações de periféricos (manter assim)

    piscar_LED(3,2,100,100);
    iniciar_DHT(DHT_PIN);

    vTaskDelay(1000 / portTICK_PERIOD_MS); 

    iniciar_wnology_wifi(WIFI_SSID, WIFI_PASS, DEVICE_ID, W_ACCESS_KEY, W_PASSWORD); //Inicializa o MQTT
    wegnology_register_key_handler("LED", handler_led); //Regitra o atributo de subscrição do tópico command

    /////////////////////////////////////////////////////////////////////////////////////   Periféricos inicializados

    while (1) {
        float temperatura=0, umidade=0;
        //char buffer[15];
        if(DHT_temp_umidade(&temperatura, &umidade))
        {
            // float temperatura = 25.0 + (rand() % 100) / 10.0f;  // valor simulado
            // mqtt_wegnology_send_float("Temperatura", temperatura); // envio direto
            // mqtt_wegnology_send_float("Umidade", umidade);

            char temp_str[16], umid_str[16];
            snprintf(temp_str, sizeof(temp_str), "%.2f", temperatura);
            snprintf(umid_str, sizeof(umid_str), "%.2f", umidade);

            const char *keys[] = { "Temperatura", "Umidade" }; //Chaves são os rótulos dos atributos do Wegnology
            const char *values[] = { temp_str, umid_str }; //Os valores são os valores a serem passados para os atributos

            mqtt_wegnology_publish_json(keys, values, 2, 1);


            printf("Temperatura: %.2f Umidade: %.2f\n", temperatura, umidade);
            ESP_LOGI(TAG, "Temperatura enviada: %.2f", temperatura);
        }
        else
        {
            printf("Erro sensor DHT\n");
        }
        vTaskDelay(pdMS_TO_TICKS(10000));  // publica a cada 10s
    }


    /////////////////////////////////////////////////////////////////////////////////////   Fim do ramo principal
    
}
