TARGET		= eboot
SRC			= $(wildcard *.cpp)
OBJS			= $(SRC:.cpp=.o)

#PSP_FW_VERSION	= 200
#BUILD_PRX	= 1

INCDIR		= 
CFLAGS		= -G0 -Wall -O2
CXXFLAGS		= $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS		= $(CFLAGS)
 
LIBDIR		=
LDFLAGS		=
STDLIBS		= -losl -lpng -lz -lpspsdk -lpspctrl -lpspumd -lpsprtc -lpsppower -lpspgu -lpspaudiolib -lpspaudio -lm -lstdc++
LIBS			= $(STDLIBS)

EXTRA_TARGETS		= EBOOT.PBP
PSP_EBOOT_TITLE	= EndlessAlice
PSP_EBOOT_ICON	= ICON0.png
 
PSPSDK		= $(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
