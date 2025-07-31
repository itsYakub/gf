CC		= cc
CFLAGS	= -Wall -Wextra -Werror -std=c99
AR		= ar
ARFLAGS	= rcs

SRCS_X11= \
	./src/x11/gf-window-create.c \
	./src/x11/gf-window-destroy.c \
	./src/x11/gf-context.c \
	./src/x11/gf-events.c \
	./src/x11/gf-utils.c \
	./src/x11/gf-config.c

SRCS	= $(SRCS_X11)
OBJS	= $(SRCS:.c=.o)



# SECTION:
#  OPTIONS
# ----------------
# General section:
# ----------------
# ON / OFF (DEFAULT)
SHARED	= OFF
# ON / OFF (DEFAULT)
VERBOSE	= ON
# ---------------------
# X11 specific section:
# ---------------------
# EGL (DEFAULT) / GLX
X11_USE_API = EGL



# SECTION:
#  OPTIONS Configuration

ifeq ($(SHARED),OFF)
	TARGET = libgf.a
else ifeq ($(SHARED),ON)
	TARGET = libgf.so
else
$(error SHARED flag not set to the correct value (EXPECTED: ON/OFF) (GOT: $(SHARED)))
endif

ifeq ($(VERBOSE),ON)
	CFLAGS += -DVERBOSE
else ifneq ($(VERBOSE),OFF)
$(error VERBOSE flag not set to the correct value (EXPECTED: ON/OFF) (GOT: $(VERBOSE)))
endif

ifeq ($(X11_USE_API),EGL)
	CFLAGS += -DUSE_EGL
else ifeq ($(X11_USE_API),GLX)
	CFLAGS += -DUSE_GLX
else
$(error X11_USE_API flag not set to the correct value (EXPECTED: EGL/GLX) (GOT: $(VERBOSE)))
endif



# SECTION:
#  TARGET Build

.PHONY : all

all : release

.PHONY : debug

debug : CFLAGS += -DDEBUG
debug : CFLAGS += -ggdb3 -O0
debug : $(TARGET)

.PHONY : relWithDebInfo

relWithDebInfo : CFLAGS += -DRELEASE
relWithDebInfo : CFLAGS += -O3 -ggdb3
relWithDebInfo : $(TARGET)

.PHONY : release

release : CFLAGS += -DRELEASE
release : CFLAGS += -s -O3
release : $(TARGET)

.PHONY : clean

clean :
	rm -f $(TARGET)
	rm -f $(OBJS)

.PHONY : install

install :
	cp $(TARGET) /usr/local/lib
	cp ./src/gf.h /usr/local/include

$(TARGET) : $(OBJS)

ifeq ($(SHARED),OFF)
	$(AR) $(ARFLAGS) $@ $^
else ifeq ($(SHARED),ON)
	$(CC) $(CFLAGS) -shared -o $@ $^ 
endif

$(OBJS) : %.o : %.c

ifeq ($(SHARED),OFF)
	$(CC) $(CFLAGS) -c $< -o $@
else ifeq ($(SHARED),ON)
	$(CC) $(CFLAGS) -fPIC -c $< -o $@
endif
