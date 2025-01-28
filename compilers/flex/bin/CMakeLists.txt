cmake_minimum_required(VERSION 3.16)
project(lex_project)  # Укажите ваше имя проекта

set(CMAKE_C_STANDARD 99)  # Установите стандарт C

# Добавьте исходный файл lex.yy.c
add_executable(lexer lex.yy.c)
