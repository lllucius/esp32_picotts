#
# Declare extra partition subtypes that we'll use for the resource files
#
idf_build_set_property( EXTRA_PARTITION_SUBTYPES "data, de_de_ta, 0x40" APPEND )
idf_build_set_property( EXTRA_PARTITION_SUBTYPES "data, de_de_sg, 0x41" APPEND )
idf_build_set_property( EXTRA_PARTITION_SUBTYPES "data, en_gb_ta, 0x42" APPEND )
idf_build_set_property( EXTRA_PARTITION_SUBTYPES "data, en_gb_sg, 0x43" APPEND )
idf_build_set_property( EXTRA_PARTITION_SUBTYPES "data, en_us_ta, 0x44" APPEND )
idf_build_set_property( EXTRA_PARTITION_SUBTYPES "data, en_us_sg, 0x45" APPEND )
idf_build_set_property( EXTRA_PARTITION_SUBTYPES "data, es_es_ta, 0x46" APPEND )
idf_build_set_property( EXTRA_PARTITION_SUBTYPES "data, es_es_sg, 0x47" APPEND )
idf_build_set_property( EXTRA_PARTITION_SUBTYPES "data, fr_fr_ta, 0x48" APPEND )
idf_build_set_property( EXTRA_PARTITION_SUBTYPES "data, fr_fr_sg, 0x49" APPEND )
idf_build_set_property( EXTRA_PARTITION_SUBTYPES "data, it_it_ta, 0x4a" APPEND )
idf_build_set_property( EXTRA_PARTITION_SUBTYPES "data, it_it_sg, 0x4b" APPEND )

#
# Declare the resource file paths
#
set( PICOTTS_DE_DE_SG_FILE "lang/de-DE_gl0_sg.bin" )
set( PICOTTS_DE_DE_TA_FILE "lang/de-DE_ta.bin" )
set( PICOTTS_EN_GB_SG_FILE "lang/en-GB_kh0_sg.bin" )
set( PICOTTS_EN_GB_TA_FILE "lang/en-GB_ta.bin" )
set( PICOTTS_EN_US_SG_FILE "lang/en-US_lh0_sg.bin" )
set( PICOTTS_EN_US_TA_FILE "lang/en-US_ta.bin" )
set( PICOTTS_ES_ES_SG_FILE "lang/es-ES_zl0_sg.bin" )
set( PICOTTS_ES_ES_TA_FILE "lang/es-ES_ta.bin" )
set( PICOTTS_FR_FR_SG_FILE "lang/fr-FR_nk0_sg.bin" )
set( PICOTTS_FR_FR_TA_FILE "lang/fr-FR_ta.bin" )
set( PICOTTS_IT_IT_SG_FILE "lang/it-IT_cm0_sg.bin" )
set( PICOTTS_IT_IT_TA_FILE "lang/it-IT_ta.bin" )

#
# Copy the original partitions CSV in case we need to add partitions. Override
# the original path to force usage of this one.
#
idf_build_get_property( build_dir BUILD_DIR )
set( extra_parts "${build_dir}/extra_parts.csv" )

file( COPY_FILE "${PARTITION_CSV_PATH}" "${extra_parts}" )

set( PARTITION_CSV_PATH "${extra_parts}" CACHE INTERNAL "extra partitions" FORCE )

