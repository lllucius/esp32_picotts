# esp32_picotts
PicoTTS for the ESP32

This is a port of the SVOX Pico TTS engine to the ESP Platform.

All of the original Pico API functionality is still available and may be used
directly, but new functions have been added to make loading memory resource files
much easier.

The main addition is a function (see below) that allows you to "load" a resource
file from a given memory location. Normally, you'd load them from a filesystem,
but this can be slow and may not be acceptable if you defined your voices at
startup.

You use this new function in combination with the esp-idf embedding feature. Or
you can store the resource files in separate partitions on the system flash and
then use the partition API to map them into memory before calling the new
function.

Additionally, functions have been added that simplify voice creation by handling
the voice creation for you. These 3 functions will "load" the resources from a
memory location, partition, or file and then create the voice as well.

Finally, there's a function that will automatically load all voices configured
using "idf.py menuconfig".

See "main/tts.c" for an example of the usage of the automatic method.

You will need a chip with at least 4MB of PSRAM and 4MB of flash.

This was created using esp-idf v5.2, but it may work with older
versions as well.

The public API function for loading embedded resources is below.  It is
called just like the pico_loadResource() function, but instead of a file
name, you pass the address of an in-memory resource.

```
/**
   Adds a preloaded resource into the Pico system. The number of resource
   files loaded in parallel is limited by PICO_MAX_NUM_RESOURCES.
   Loading of a resource file may be done at any time (even in
   parallel to a running engine doing TTS synthesis), but with the
   general restriction that functions taking a system handle as their
   first argument must be called in a mutually exclusive fashion. The
   loaded resource will be available only to engines started after the
   resource is fully loaded, i.e., not to engines currently
   running.
*/
PICO_FUNC pico_loadMemoryResource(
        pico_System system,
        const pico_Char *resourceAddress,
        pico_Resource *outResource
        );
```

These are the 3 new functions that will load resources and create the
voice.  Examples of their use can be found in:

    component/picotts/lib/picoapi.c

```
PICO_FUNC pico_defineVoice(pico_System system,
                           const pico_Char *name,
                           const uint8_t sg[],
                           const uint8_t ta[]);

PICO_FUNC pico_loadVoiceFromPart(pico_System system,
                                 const pico_Char *name,
                                 esp_partition_subtype_t sg,
                                 esp_partition_subtype_t ta);

PICO_FUNC pico_loadVoiceFromFile(pico_System system,
                                 const pico_Char *name,
                                 const char *sg,
                                 const char *ta);
```

And this the function that will load and define all of the voices you
have configured in "menuconfig":

```
PICO_FUNC pico_loadVoices(pico_System system);
```


