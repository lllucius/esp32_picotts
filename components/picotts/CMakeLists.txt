if( CONFIG_PICOTTS_RESOURCE_ENGLISH_UK )
    set( PICOTTS_LANG_NAME "en-GB" )
    set( PICOTTS_SG_SFX "kh0" )
elseif( CONFIG_PICOTTS_RESOURCE_ENGLISH_US )
    set( PICOTTS_LANG_NAME "en-US" )
    set( PICOTTS_SG_SFX "lh0" )
elseif( CONFIG_PICOTTS_RESOURCE_FRENCH )
    set( PICOTTS_LANG_NAME "fr-FR" )
    set( PICOTTS_SG_SFX "nk0" )
elseif( CONFIG_PICOTTS_RESOURCE_GERMAN )
    set( PICOTTS_LANG_NAME "de-DE" )
    set( PICOTTS_SG_SFX "gl0" )
elseif( CONFIG_PICOTTS_RESOURCE_ITALIAN )
    set( PICOTTS_LANG_NAME "it-IT" )
    set( PICOTTS_SG_SFX "cm0" )
elseif( CONFIG_PICOTTS_RESOURCE_SPANISH )
    set( PICOTTS_LANG_NAME "es-ES" )
    set( PICOTTS_SG_SFX "zl0" )
endif()

if( PICOTTS_LANG_NAME )
    set( SG_NAME "${PICOTTS_LANG_NAME}_${PICOTTS_SG_SFX}_sg.bin" )
    set( TA_NAME "${PICOTTS_LANG_NAME}_ta.bin" )
    set( PICOTTS_EMBED_FILES "lang/${SG_NAME}" "lang/${TA_NAME}" )

    string( REGEX REPLACE "[-.]" "_" PICOTTS_SG_NAME "${SG_NAME}" )
    string( REGEX REPLACE "[-.]" "_" PICOTTS_TA_NAME "${TA_NAME}" )

    set( PICOTTS_DEFINES
        "-DPICOTTS_SG_NAME=\"${PICOTTS_SG_NAME}\""
        "-DPICOTTS_TA_NAME=\"${PICOTTS_TA_NAME}\""
    )
endif()

idf_component_register(
    SRCS
        "lib/picoacph.c"
        "lib/picoapi.c"
        "lib/picobase.c"
        "lib/picocep.c"
        "lib/picoctrl.c"
        "lib/picodata.c"
        "lib/picodbg.c"
        "lib/picoextapi.c"
        "lib/picofftsg.c"
        "lib/picokdbg.c"
        "lib/picokdt.c"
        "lib/picokfst.c"
        "lib/picoklex.c"
        "lib/picoknow.c"
        "lib/picokpdf.c"
        "lib/picokpr.c"
        "lib/picoktab.c"
        "lib/picoos.c"
        "lib/picopal.c"
        "lib/picopam.c"
        "lib/picopr.c"
        "lib/picorsrc.c"
        "lib/picosa.c"
        "lib/picosig.c"
        "lib/picosig2.c"
        "lib/picospho.c"
        "lib/picotok.c"
        "lib/picotrns.c"
        "lib/picowa.c"
    INCLUDE_DIRS
        "lib"
    EMBED_FILES
        ${PICOTTS_EMBED_FILES}
)

target_compile_options(${COMPONENT_LIB}
    PUBLIC
        ${PICOTTS_DEFINES}
    PRIVATE
        -Wno-implicit-fallthrough
        -Wno-unused-but-set-variable
        -Wno-unused-function
        -Wno-stringop-overflow
)

