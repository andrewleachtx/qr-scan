cmake_minimum_required(VERSION 3.20)
project(qr-scan LANGUAGES CXX)

add_executable(qr-scan
    src/main.cpp
    vendor/qrcodegen.cpp
)

target_include_directories(qr-scan PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor
)
