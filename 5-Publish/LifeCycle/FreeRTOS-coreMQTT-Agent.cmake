# Add ${CMAKE_CURRENT_LIST_DIR}rary cpp files

if (NOT DEFINED CORE_MQTT_AGENT_DIR)
    set(CORE_MQTT_AGENT_DIR "${CMAKE_CURRENT_LIST_DIR}/lib/coreMQTT-Agent")
endif()

add_library(coreMQTTAgent STATIC)
include("${CORE_MQTT_AGENT_DIR}/mqttAgentFilePaths.cmake")

target_sources(coreMQTTAgent PUBLIC
	${MQTT_AGENT_SOURCES}
)
add_subdirectory(${COREMQTT_AGENT_PORT_PATH})

target_include_directories(coreMQTTAgent PUBLIC 
	${MQTT_AGENT_INCLUDE_PUBLIC_DIRS}
	${COREMQTT_AGENT_PORT_PATH}
)



# Add the standard ${CMAKE_CURRENT_LIST_DIR}rary to the build
target_link_libraries(coreMQTTAgent coreMQTT FreeRTOS-Kernel-Heap4 pico_stdlib)