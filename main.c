```cpp
// Área de inclusão das bibliotecas
//-----------------------------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#include "HCF_SOFT.h"
#include "HCF_WNOLOGY.h"

#include "driver/gpio.h"


//-----------------------------------------------------------------------------------------------------------------------
// Área das macros
//-----------------------------------------------------------------------------------------------------------------------

// Wi-Fi
#define WIFI_SSID "coqueiro"
#define WIFI_PASS "amigos12"

// ESP utilizado
#define ESP 1


// Dados do dispositivo no Wegnology
#define DEVICE_ID "65774aa82623fd911ab650c1"

#define W_ACCESS_KEY "76ac5ed2-ed18-4e96-9e02-d2dd572db083"

#define W_PASSWORD "f52797619b7205bc2ac8d796d80fd0cb23f988e882cd0b82d575b26939f78c1c"


//-----------------------------------------------------------------------------------------------------------------------
// Área de declaração de variáveis
//-----------------------------------------------------------------------------------------------------------------------

char *TAG = "HCF";


// Variáveis de entradas e saídas
uint8_t entradas, saidas = 0;


// Variável para armazenar a tecla pressionada
char tecla = '-';


// Vetor para textos
char escrever[40];


//-----------------------------------------------------------------------------------------------------------------------
// Variáveis dos sensores
//-----------------------------------------------------------------------------------------------------------------------

// Luminosidade
float luminosidade = 0.0;


// Resistência
float resistencia = 0.0;


// Corrente
float corrente = 0.0;


//-----------------------------------------------------------------------------------------------------------------------
// Função para receber comandos do Wegnology
//-----------------------------------------------------------------------------------------------------------------------

void handler_led(const char *value)
{
    if (strcmp(value, "true") == 0)
    {
        gpio_set_level(GPIO_NUM_2, 1);
    }
    else
    {
        gpio_set_level(GPIO_NUM_2, 0);
    }
}


//-----------------------------------------------------------------------------------------------------------------------
// Programa Principal
//-----------------------------------------------------------------------------------------------------------------------

void app_main(void)
{
    /////////////////////////////////////////////////////////////////////////////////////
    // Inicialização
    /////////////////////////////////////////////////////////////////////////////////////

    // Pisca os LEDs para indicar que o ESP32 iniciou
    piscar_LED(3,2,100,100);


    /////////////////////////////////////////////////////////////////////////////////////
    // Inicialização do Wi-Fi e Wegnology
    /////////////////////////////////////////////////////////////////////////////////////

    iniciar_wnology_wifi(
        WIFI_SSID,
        WIFI_PASS,
        DEVICE_ID,
        W_ACCESS_KEY,
        W_PASSWORD
    );


    // Registra o comando LED
    wegnology_register_key_handler("LED", handler_led);


    /////////////////////////////////////////////////////////////////////////////////////
    // Loop principal
    /////////////////////////////////////////////////////////////////////////////////////

    while (1)
    {

        //////////////////////////////////////////////////////////////////////////////////
        // LEITURA DOS SENSORES
        //////////////////////////////////////////////////////////////////////////////////

        /*
         * Aqui serão colocadas as leituras reais:
         *
         * luminosidade → LDR
         *
         * resistencia → circuito de resistência
         *
         * corrente → shunt
         *
         * Por enquanto deixei valores de exemplo
         * para testar o envio para o Wegnology.
         */


        // Valor de exemplo da luminosidade
        luminosidade = 500.0;


        // Valor de exemplo da resistência
        resistencia = 1000.0;


        // Valor de exemplo da corrente
        corrente = 0.100;


        //////////////////////////////////////////////////////////////////////////////////
        // Conversão dos valores para texto
        //////////////////////////////////////////////////////////////////////////////////

        char luminosidade_str[16];
        char resistencia_str[16];
        char corrente_str[16];


        // Converte luminosidade para texto
        snprintf(
            luminosidade_str,
            sizeof(luminosidade_str),
            "%.2f",
            luminosidade
        );


        // Converte resistência para texto
        snprintf(
            resistencia_str,
            sizeof(resistencia_str),
            "%.2f",
            resistencia
        );


        // Converte corrente para texto
        snprintf(
            corrente_str,
            sizeof(corrente_str),
            "%.3f",
            corrente
        );


        //////////////////////////////////////////////////////////////////////////////////
        // Montagem dos atributos do Wegnology
        //////////////////////////////////////////////////////////////////////////////////

        /*
         * Esses serão os nomes que aparecerão no Wegnology:
         *
         * Luminosidade
         * Resistencia
         * Corrente
         */

        const char *keys[] =
        {
            "Luminosidade",
            "Resistencia",
            "Corrente"
        };


        // Valores que serão enviados
        const char *values[] =
        {
            luminosidade_str,
            resistencia_str,
            corrente_str
        };


        //////////////////////////////////////////////////////////////////////////////////
        // Envio dos dados
        //////////////////////////////////////////////////////////////////////////////////

        mqtt_wegnology_publish_json(
            keys,
            values,
            3,
            1
        );


        //////////////////////////////////////////////////////////////////////////////////
        // Mostra os valores no monitor serial
        //////////////////////////////////////////////////////////////////////////////////

        printf(
            "Luminosidade: %.2f | Resistencia: %.2f ohms | Corrente: %.3f A\n",
            luminosidade,
            resistencia,
            corrente
        );


        // Mensagem no monitor serial
        ESP_LOGI(
            TAG,
            "Dados enviados: L=%.2f | R=%.2f ohms | I=%.3f A",
            luminosidade,
            resistencia,
            corrente
        );


        //////////////////////////////////////////////////////////////////////////////////
        // Aguarda 10 segundos
        //////////////////////////////////////////////////////////////////////////////////

        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}
```
