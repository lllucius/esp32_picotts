#
# Build list of embedded resource files
#
set( PICOTTS_EMBED_FILES )

if( CONFIG_PICOTTS_DE_DE_EMBED )
    list( APPEND PICOTTS_EMBED_FILES ${PICOTTS_DE_DE_SG_FILE} ${PICOTTS_DE_DE_TA_FILE} )
endif()

if( CONFIG_PICOTTS_EN_GB_EMBED )
    list( APPEND PICOTTS_EMBED_FILES ${PICOTTS_EN_GB_SG_FILE} ${PICOTTS_EN_GB_TA_FILE} )
endif()

if( CONFIG_PICOTTS_EN_US_EMBED )
    list( APPEND PICOTTS_EMBED_FILES ${PICOTTS_EN_US_SG_FILE} ${PICOTTS_EN_US_TA_FILE} )
endif()

if( CONFIG_PICOTTS_ES_ES_EMBED )
    list( APPEND PICOTTS_EMBED_FILES ${PICOTTS_ES_ES_SG_FILE} ${PICOTTS_ES_ES_TA_FILE} )
endif()

if( CONFIG_PICOTTS_FR_FR_EMBED )
    list( APPEND PICOTTS_EMBED_FILES ${PICOTTS_FR_FR_SG_FILE} ${PICOTTS_FR_FR_TA_FILE} )
endif()

if( CONFIG_PICOTTS_IT_IT_EMBED )
    list( APPEND PICOTTS_EMBED_FILES ${PICOTTS_IT_IT_SG_FILE} ${PICOTTS_IT_IT_TA_FILE} )
endif()

#
# Define this components build 
#
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
    REQUIRES
        esp_partition
)

#
# The Pico source throws several unwanted (and ignorable) warnings. Ignore them since
# they will fail the build.
#
target_compile_options(
    ${COMPONENT_LIB}
    PUBLIC
        ${PICOTTS_DEFINES}
    PRIVATE
        -Wno-implicit-fallthrough
        -Wno-unused-but-set-variable
        -Wno-unused-function
        -Wno-stringop-overflow
)

#
# Ensures any existing partitions are large enough and not missing. While doing
# this, we build a list of targets that will be defined to flash the resource
# files to the partition.
#
function( check_part lang type file )
    string( TOLOWER "${lang}_${type}" type )

    # Get full path to resource file
    set( file "${CMAKE_CURRENT_SOURCE_DIR}/${file}" )

    # Get resource file size and calculate size of flash needed (rounded up to block size)
    file( SIZE "${file}" req )
    math( EXPR req "(((${req} + 4095) / 4096) * 4096)" )

    # Determine if we're supposed to automatically add the partitions to the partition table
    if( CONFIG_PICOTTS_AUTOCREATE_PARTITIONS )
        # Check to see if already exists
        partition_table_get_partition_info( name
            "--partition-type data --partition-subtype ${type}"
            "name"
        )
        if( name )
            list( APPEND messages "    partition with subtype of ${type} already exists" )
        else()
            # Remember this new partition. It will be added to the partitions table below.
            set( name "${type}" )
            set( partitions "${partitions};${name},data,${type},,${req}\n" PARENT_SCOPE )
        endif()
    # Not added, so ensure predefined partitions exist in partition table
    else()
        # Retrieve predefined partition info
        partition_table_get_partition_info( name
            "--partition-type data --partition-subtype ${type}"
            "name"
        )
        partition_table_get_partition_info( offset
            "--partition-type data --partition-subtype ${type}"
            "offset"
        )
        partition_table_get_partition_info( size
            "--partition-type data --partition-subtype ${type}"
            "size"
        )

        # Ensure we have an offset AND size
        if( NOT offset OR NOT size )
            list( APPEND messages "    partition with subtype of ${type} is required" )
        endif()

        # Ensure the partitions is big enough to hold the resource file
        if( size LESS req )
            list( APPEND messages "    ${name} partition must be at least ${req} bytes" )
        endif()
    endif()

    # Remember new target
    list( APPEND flash_targets "${name}" "${type}" "${file}" )

    # Propogate new values to the caller
    set( flash_targets "${flash_targets}" PARENT_SCOPE )
    set( messages "${messages}" PARENT_SCOPE )
endfunction()

# Load the list of predefined partitions
if( CONFIG_PICOTTS_AUTOCREATE_PARTITIONS )
    file( STRINGS "${PARTITION_CSV_PATH}" partitions )
endif()

# Check partitions for German
if( CONFIG_PICOTTS_DE_DE_PART )
    check_part( "de_DE" "sg" "${PICOTTS_DE_DE_SG_FILE}" )
    check_part( "de_DE" "ta" "${PICOTTS_DE_DE_TA_FILE}" )
endif()

# Check partitions for British English
if( CONFIG_PICOTTS_EN_GB_PART )
    check_part( "en_GB" "sg" "${PICOTTS_EN_GB_SG_FILE}" )
    check_part( "en_GB" "ta" "${PICOTTS_EN_GB_TA_FILE}" )
endif()

# Check partitions for US English
if( CONFIG_PICOTTS_EN_US_PART )
    check_part( "en_US" "sg" "${PICOTTS_EN_US_SG_FILE}" )
    check_part( "en_US" "ta" "${PICOTTS_EN_US_TA_FILE}" )
endif()

# Check partitions for Spanish
if( CONFIG_PICOTTS_ES_ES_PART )
    check_part( "es_ES" "sg" "${PICOTTS_ES_ES_SG_FILE}" )
    check_part( "es_ES" "ta" "${PICOTTS_ES_ES_TA_FILE}" )
endif()

# Check partitions for French
if( CONFIG_PICOTTS_FR_FR_PART )
    check_part( "fr_FR" "sg" "${PICOTTS_FR_FR_SG_FILE}" )
    check_part( "fr_FR" "ta" "${PICOTTS_FR_FR_TA_FILE}" )
endif()

# Check partitions for Italian
if( CONFIG_PICOTTS_IT_IT_PART )
    check_part( "it_IT" "sg" "${PICOTTS_IT_IT_SG_FILE}" )
    check_part( "it_IT" "ta" "${PICOTTS_IT_IT_TA_FILE}" )
endif()

# If any problems were found, create a target to fail the build. This is done
# prevent locking the user out of "menuconfig".
if( messages )
    list( PREPEND messages "Problem(s) with Pico voice files:" )
    list( PREPEND messages "=====================================================" )
    list( APPEND messages "=====================================================" )
    fail_at_build_time( missing_voice_partitions ${messages})
else()
    # Add new partitions to the table and write them out
    if( partitions )
        string( REPLACE ";" "\n" partitions "${partitions}" )
        file( WRITE "${extra_parts}" "${partitions}" )
    endif()

    # Process flash target definitions
    if( flash_targets )
        while( flash_targets )
            # Get target info and remove from list
            list( POP_FRONT flash_targets name type file )

            # Create the target name
            string( REPLACE "_" "-" target "${type}-flash" )

            # Add it to the information message on what to flash
            list( APPEND cmds COMMAND ${CMAKE_COMMAND} -E echo "    idf.py ${target}" )

            # Create the CMake targets for flashing this partition
            esptool_py_flash_target( "${target}" "${main_args}" "${sub_args}" ALWAYS_PLAINTEXT )
            esptool_py_flash_to_partition( "${target}" "${name}" "${file}" )

            # And add it to the general flash target if the user wants it to be automatic
            if( CONFIG_PICOTTS_FLASH_IN_PROJECT )
                esptool_py_flash_to_partition( flash "${name}" "${file}" )
            endif()
        endwhile()

        # Define target that shows commands to use when flashing the voice resource
        # files. This will (should) run at the end of the build so the user won't 
        # miss it.
        add_custom_command(
            OUTPUT
                "Voice Flash Commands"
            COMMAND
                ${CMAKE_COMMAND} -E echo ""
            COMMAND
                ${CMAKE_COMMAND} -E echo "====================================================="
            COMMAND
                ${CMAKE_COMMAND} -E echo "To flash voice definitions files, use:"
            COMMAND
                ${cmds}
            COMMAND
                ${CMAKE_COMMAND} -E echo "====================================================="
            DEPENDS
                bootloader app_check_size
            VERBATIM
        )

        add_custom_target(rebuild-partition-table ALL
            DEPENDS "Voice Flash Commands"
        )
    endif()
endif()
