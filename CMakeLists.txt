cmake_minimum_required(VERSION 3.23)
project(Payment_Application C)

set(CMAKE_C_STANDARD 99)

add_executable(Payment_Application main.c Application/app.c Application/app.h Card/card.c Card/card.h Terminal/terminal.c Terminal/terminal.h Server/server.c Server/server.h)
