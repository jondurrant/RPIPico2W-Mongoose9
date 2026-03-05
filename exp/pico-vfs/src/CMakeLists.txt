add_executable(${NAME}
        main.c
        )



# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(${NAME} PUBLIC
    pico_stdlib 
    )



# create map/bin/hex file etc.
pico_add_extra_outputs(${NAME})

# enable usb output, disable uart output
pico_enable_stdio_usb(${NAME} 0)
pico_enable_stdio_uart(${NAME} 1)
target_compile_definitions(${NAME} PRIVATE
    PICO_DEFAULT_UART_RX_PIN=17
    PICO_DEFAULT_UART_TX_PIN=16
)

target_compile_definitions(${NAME} PRIVATE
    WIFI_SSID=\"$ENV{WIFI_SSID}\"
    WIFI_PASS=\"$ENV{WIFI_PASSWORD}\"
    CYW43_HOST_NAME="Pico2W-Web"
)

pico_enable_filesystem(${NAME})
