# esp32_picotts
PicoTTS for the ESP32

This is a port of the SVOX Pico TTS engine.

Additional functions have been added to allow "loading" of voice
resource files using esp-idf's file embedding feature.  This speeds
initialization up a fair amount as the files do not have to be read
from a filesystem.

See "main/tts.c" for an example of the usage.

You will need a chip with at least 4MB of PSRAM and 4MB of flash.

This was created using esp-idf v5.2, but it should work with older
versions as well.

The public API function for loading embedded resources is below.  It is
called just like the pico_loadResource() function, but instead of a file
name, you pass the address of an in-memory resource.

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


