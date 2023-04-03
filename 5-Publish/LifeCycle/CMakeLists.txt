cmake_minimum_required(VERSION 3.12)

# Change your executable name to something creative!
set(NAME MQTTLC) # <-- Name your project/executable here!

set(PICO_BOARD pico_w)

include(pico_sdk_import.cmake)

# Gooey boilerplate
project(${NAME} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

set(TWIN_THING_PICO_CONFIG_PATH ${CMAKE_CURRENT_LIST_DIR}/port/twinThing)
set(COREMQTT_PORT_PATH      ${CMAKE_CURRENT_LIST_DIR}/port/CoreMQTT)
set(COREMQTT_AGENT_PORT_PATH      ${CMAKE_CURRENT_LIST_DIR}/port/CoreMQTT-Agent)


include(lwip_import.cmake)

SET(FREERTOS_KERNEL_PATH "${CMAKE_CURRENT_LIST_DIR}/../../lib/FreeRTOS-Kernel" CACHE STRING "Course Common Lib")
SET(FREERTOS_CONFIG_FILE_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/port/FreeRTOS-Kernel" CACHE STRING "Local Config")
include_directories("${FREERTOS_CONFIG_FILE_DIRECTORY}") 
include(FreeRTOS_Kernel_import.cmake)

set(CORE_MQTT_DIR "${CMAKE_CURRENT_LIST_DIR}/../../lib/coreMQTT" CACHE STRING "Course Common Lib")
include("FreeRTOS-coreMQTT.cmake")

set(CORE_MQTT_AGENT_DIR "${CMAKE_CURRENT_LIST_DIR}/../../lib/coreMQTT-Agent" CACHE STRING "Course Common Lib")
include("FreeRTOS-coreMQTT-Agent.cmake")

include_directories(
    "${CMAKE_CURRENT_LIST_DIR}/port/twinThing"
)

add_subdirectory(src)

#Set up files for the release packages
install(CODE "execute_process(COMMAND $ENV{HOME}/bin/picoDeploy.sh ${CMAKE_CURRENT_BINARY_DIR}/src/${NAME}.elf)")

# Set up files for the release packages
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/src/${NAME}.uf2
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)

set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_GENERATOR "ZIP" "TGZ")
include(CPack)
