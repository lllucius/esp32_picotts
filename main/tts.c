
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "esp_check.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "picoapi.h"
#include "picoextapi.h"

static const char *TAG = "TTS";

static pico_System picoSystem = NULL;
static pico_Engine picoEngine = NULL;

static pico_Char samples[512];

static const pico_Uint32 picoMemSize = 2500000;

void app_main(void)
{
    char *text = "This is a test of the emergency broadcasting system.";
    pico_Int16 text_remaining = strlen(text) + 1; // we want the null terminator also
    pico_Retstring msg;
    pico_Status ret;
    pico_Int32 used;
    pico_Int32 incrUsed;
    pico_Int32 maxUsed;
    int total_bytes = 0;

    void *mem = malloc(picoMemSize);
    ret = pico_initialize(mem, picoMemSize, &picoSystem);
    if (ret)
    {
        pico_getSystemStatusMessage(picoSystem, ret, msg);
        ESP_LOGE(TAG, "Cannot initialize pico (%i): %s\n", ret, msg);
        abort();
    }
    printf("System initialized: %p\n", picoSystem);

    picoext_getSystemMemUsage(picoSystem, 0, &used, &incrUsed, &maxUsed);
    printf("System memory used %d, incrUsed %d, max used %d\n", used, incrUsed, maxUsed);

    ret = pico_loadVoices(picoSystem);
    if (ret)
    {
        pico_getSystemStatusMessage(picoSystem, ret, msg);
        ESP_LOGE(TAG, "Loading voices failed (%i): %s\n", ret, msg);
        abort();
    }
    printf("Voices loaded\n");

    ret = pico_newEngine(picoSystem, PICO_VOICE_EN_GB, &picoEngine);
    if (ret)
    {
        pico_getSystemStatusMessage(picoSystem, ret, msg);
        ESP_LOGE(TAG, "Cannot create a new pico engine (%i): %s\n", ret, msg);
        abort();
    }
    printf("Engine created: %p\n", picoEngine);

    picoext_getEngineMemUsage(picoEngine, 0, &used, &incrUsed, &maxUsed);
    printf("Engine memory used %d, incrUsed %d, max used %d\n", used, incrUsed, maxUsed);

    picoext_getSystemMemUsage(picoSystem, 0, &used, &incrUsed, &maxUsed);
    printf("System memory used %d, incrUsed %d, max used %d\n", used, incrUsed, maxUsed);

    ret = PICO_STEP_IDLE;
    for (;;)
    {
        pico_Int16 bytes_sent;
        pico_Int16 bytes_recv;
        pico_Int16 out_data_type;

        if (ret == PICO_STEP_IDLE)
        {
            if (text_remaining == 0)
            {
                break;
            }

            ret = pico_putTextUtf8(picoEngine, (pico_Char *) text, text_remaining, &bytes_sent);
            if (ret)
            {
                pico_getSystemStatusMessage(picoSystem, ret, msg);
                printf("Cannot put Text (%i): %s\n", ret, msg);
                abort();
            }

            text_remaining -= bytes_sent;
            text += bytes_sent;

            printf("Sent %d bytes of text to the TTS engine, %d remaining\n", bytes_sent, text_remaining);
        }

        ret = pico_getData(picoEngine,
                           (void *) samples,
                           sizeof(samples),
                           &bytes_recv,
                           &out_data_type);
        if ((ret != PICO_STEP_BUSY) && (ret != PICO_STEP_IDLE))
        {
            pico_getSystemStatusMessage(picoSystem, ret, msg);
            printf("Cannot get Data (%i): %s\n", ret, msg);
            abort();
        }

        if (bytes_recv)
        {
            // Process the returned samples here...

            total_bytes += bytes_recv;
        }
    }

    printf("# of samples received = %d\n", total_bytes / sizeof(pico_Int16));

    return;
}


