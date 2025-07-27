CC		= cc
CFLAGS	= -Wall -Wextra -Werror -std=c99
AR		= ar
ARFLAGS	= rcs

# NOTE(yakub):
#  Temporary solution for the backend compilation
#  When there'll be more backends, the SRCS assignment will be improved

SRCS	= ./src/x11/gf.c
OBJS	= $(SRCS:.c=.o)

# SECTION:
#  OPTIONS

# ON / OFF (DEFAULT)
SHARED	= OFF

# ON / OFF (DEFAULT)
VERBOSE	= ON

# X11 specific section:
# ---------------------
# ON (DEFAULT) / OFF
X11_USE_GLX = ON
# ON / OFF (DEFAULT)
X11_USE_EGL = OFF

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

ifeq ($(X11_USE_GLX),ON)
	CFLAGS += -DUSE_GLX
else ifeq ($(X11_USE_EGL),ON)
	CFLAGS += -DUSE_EGL
else
$(error X11_USE_GLX or X11_USE_EGL flags not set to the correct values))
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
