# MPLAB IDE generated this makefile for use with GNU make.
# Project: microtest.mcp
# Date: Sun Jan 17 21:16:00 2016

AS = MPASMWIN.exe
CC = mcc18.exe
LD = mplink.exe
AR = mplib.exe
RM = rm

microtest.cof : main.o
	$(LD) /p18F4620 /l"C:\Program Files (x86)\Microchip\mplabc18\v3.47\lib" "main.o" /u_CRUNTIME /u_DEBUG /z__MPLAB_BUILD=1 /z__MPLAB_DEBUG=1 /o"microtest.cof" /M"microtest.map" /W

main.o : main.c ../../../../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/stdio.h ../../../../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/stdlib.h ../../../../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/delays.h main.c ../../../../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/stdarg.h ../../../../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/stddef.h ../../../../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/p18f4620.h ../../../../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/p18cxxx.h
	$(CC) -p=18F4620 "main.c" -fo="main.o" -D__DEBUG -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

clean : 
	$(RM) "main.o" "microtest.cof" "microtest.hex"

