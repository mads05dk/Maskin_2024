cmake_minimum_required(VERSION 3.21)
project(Machine_Oriented_Programming_Project_2 C)

set(CMAKE_C_STANDARD 11)

add_executable(Yukon
        main.c
        yukon.c yukon.h
        commands.c commands.h
        cli.c cli.h
#        gui.c gui.h
        cards.c cards.h io.c io.h)
