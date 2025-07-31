CC		= cc
CFLAGS	= -Wall -Wextra -Werror -std=c99
AR		= ar
ARFLAGS	= rcs

SRCS_X11= \
	./src/x11/gf-window-create.c \
	./src/x11/gf-window-destroy.c \
	./src/x11/gf-window-events.c \
	./src/x11/gf-window-utils.c \
	./src/x11/gf-window-config.c

SRCS_WL= \
	./src/wl/gf-window-create.c \
	./src/wl/gf-window-destroy.c \
	./src/wl/gf-window-events.c \
	./src/wl/gf-window-utils.c \
	./src/wl/gf-window-config.c

SRCS_EGL= \
	./src/egl/gf-context-create.c \
	./src/egl/gf-context-destroy.c \
	./src/egl/gf-context-config.c

SRCS_GLX= \
	./src/glx/gf-context-create.c \
	./src/glx/gf-context-destroy.c \
	./src/glx/gf-context-config.c



# SECTION:
#  OPTIONS
# ----------------
# General section:
# ----------------
# ON / OFF (DEFAULT)
SHARED	= OFF
# ON / OFF (DEFAULT)
VERBOSE	= OFF
# ---------------------
# X11 specific section:
# ---------------------
# EGL (DEFAULT) / GLX
X11_USE_API = EGL



# SECTION:
#  PLATFORM Configuration

ifeq ($(OS),Windows_NT)
	UNAME_S := Windows_NT
else
	UNAME_S := $(shell uname -s)
endif

ifeq ($(UNAME_S),Linux)
	XDG_SESSION_TYPE = $(shell echo $${XDG_SESSION_TYPE})

	ifeq ($(XDG_SESSION_TYPE),x11)
		CFLAGS	+= -DUSE_X11
		SRCS	:= $(SRCS_X11)

		ifeq ($(X11_USE_API),EGL)
			CFLAGS	+= -DUSE_EGL
			SRCS	+= $(SRCS_EGL)
		else ifeq ($(X11_USE_API),GLX)
			CFLAGS	+= -DUSE_GLX
			SRCS	+= $(SRCS_GLX)
		else
			$(error Unrecognized OpenGL API: $(X11_USE_API))
		endif
	else ifeq ($(XDG_SESSION_TYPE), wayland)
		CFLAGS	+= -DUSE_WL
		CFLAGS	+= -DUSE_EGL # wayland uses EGL by default
		SRCS	:= $(SRCS_WL)
	else
		$(error Unrecognized session type: $(XDG_SESSION_TYPE))
	endif
else ifeq ($(UNAME_S), Windows_NT)
	CFLAGS	+= -DUSE_WIN32
	SRCS	:= $(SRCS_WIN32)
	SRCS	+= $(SRCS_WGL)
else
	$(error Unrecognized Platform: $(UNAME_S))
endif

OBJS	= $(SRCS:.c=.o)

# SECTION:
#  OPTIONS Configuration

ifeq ($(SHARED),OFF)
	TARGET := libgf.a
else ifeq ($(SHARED),ON)
	TARGET := libgf.so
else
$(error SHARED flag not set to the correct value (EXPECTED: ON/OFF) (GOT: $(SHARED)))
endif

ifeq ($(VERBOSE),ON)
	CFLAGS += -DVERBOSE
else ifneq ($(VERBOSE),OFF)
$(error VERBOSE flag not set to the correct value (EXPECTED: ON/OFF) (GOT: $(VERBOSE)))
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
