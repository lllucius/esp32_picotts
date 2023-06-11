# esp32_picotts
PicoTTS for the ESP32

This is a port of the SVOX Pico TTS engine.

Additional functions have been added to allow "loading" of voice
resource files using esp-idf's file embedding feature.  This speeds
initialization up a fair amount as the files do not have to be read
from a filesystem.

See "main/tts.c" for an example of the usage.

You will need a chip with at least 4MB of PSRAM and 4MB of flash.

