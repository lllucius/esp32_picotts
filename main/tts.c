
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

static const pico_Char *picoVoiceName = (pico_Char *) "PicoVoice";
static const pico_Uint32 picoMemSize = 2500000;

extern const uint8_t sg_bin_start[] asm("_binary_" PICOTTS_SG_NAME "_start");
extern const uint8_t ta_bin_start[] asm("_binary_" PICOTTS_TA_NAME "_start");

void app_main(void)
{
    char *text = "This is a test of the emergency broadcasting system.";
    pico_Int16 text_remaining = strlen(text) + 1; // we want the null terminator also
    pico_Retstring msg;
    pico_Status ret;
    int total_bytes = 0;

    const uint8_t *resources[] =
    {
        ta_bin_start,
        sg_bin_start,
    };

    void *mem = malloc(picoMemSize);
    ret = pico_initialize(mem, picoMemSize, &picoSystem);
    if (ret)
    {
        pico_getSystemStatusMessage(picoSystem, ret, msg);
        ESP_LOGE(TAG, "Cannot initialize pico (%i): %s\n", ret, msg);
        abort();
    }
    printf("System initialized: %p\n", picoSystem);

    ret = pico_createVoiceDefinition(picoSystem, picoVoiceName);
    if (ret)
    {
        pico_getSystemStatusMessage(picoSystem, ret, msg);
        ESP_LOGE(TAG, "Cannot create voice definition (%i): %s\n", ret, msg);
        abort();
    }
    printf("Voice definition '%s' created\n", (char *) picoVoiceName);

    for (int i = 0; i < 2; ++i)
    {
        pico_Resource resource;
        pico_Retstring resourceName;

        ret = pico_loadMemoryResource(picoSystem, resources[i], &resource);
        if (ret)
        {
            pico_getSystemStatusMessage(picoSystem, ret, msg);
            ESP_LOGE(TAG, "Cannot load tts resource file (%i): %s\n", ret, msg);
            abort();
        }
        printf("Memory resource %p loaded\n", resource);

        ret = pico_getResourceName(picoSystem, resource, resourceName);
        if (ret)
        {
            pico_getSystemStatusMessage(picoSystem, ret, msg);
            ESP_LOGE(TAG, "Cannot get the tts resource name (%i): %s\n", ret, msg);
            abort();
        }
        printf("Resource is named '%s'\n", resourceName);

        ret = pico_addResourceToVoiceDefinition(picoSystem, picoVoiceName, (pico_Char *) resourceName);
        if (ret)
        {
            pico_getSystemStatusMessage(picoSystem, ret, msg);
            ESP_LOGE(TAG, "Cannot add the tts resource to the voice (%i): %s\n", ret, msg);
            abort();
        }
        printf("Added resource to voice\n");
    }

    ret = pico_newEngine(picoSystem, picoVoiceName, &picoEngine);
    if (ret)
    {
        pico_getSystemStatusMessage(picoSystem, ret, msg);
        ESP_LOGE(TAG, "Cannot create a new pico engine (%i): %s\n", ret, msg);
        abort();
    }
    printf("Engine created: %p\n", picoEngine);

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


