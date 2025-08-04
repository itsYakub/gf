MK_ROOT	:=$(dir $(realpath $(firstword $(MAKEFILE_LIST))))

CC		= cc
CFLAGS	= -Wall -Wextra -Werror -std=c99
AR		= ar
ARFLAGS	= rcs



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



SRCS_X11= \
	$(MK_ROOT)src/x11/gf-window-create.c \
	$(MK_ROOT)src/x11/gf-window-destroy.c \
	$(MK_ROOT)src/x11/gf-window-events.c \
	$(MK_ROOT)src/x11/gf-window-utils.c \
	$(MK_ROOT)src/x11/gf-window-config.c

SRCS_WL	= \
	$(MK_ROOT)src/wl/gf-window-create.c \
	$(MK_ROOT)src/wl/gf-window-destroy.c \
	$(MK_ROOT)src/wl/gf-listener.c \
	$(MK_ROOT)src/wl/gf-window-events.c \
	$(MK_ROOT)src/wl/gf-window-utils.c \
	$(MK_ROOT)src/wl/gf-window-config.c

SRCS_XDG= \
	$(MK_ROOT)src/wl/xdg-shell.c \

SRCS_EGL= \
	$(MK_ROOT)src/egl/gf-context-create.c \
	$(MK_ROOT)src/egl/gf-context-destroy.c \
	$(MK_ROOT)src/egl/gf-context-config.c

SRCS_GLX= \
	$(MK_ROOT)src/glx/gf-context-create.c \
	$(MK_ROOT)src/glx/gf-context-destroy.c \
	$(MK_ROOT)src/glx/gf-context-config.c



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
		# wayland client is dependent on xdg, which is some sort of stdlib for windowing, to work
		# we need to generate it in order to use it properly
		XDG_HEADER := $(shell wayland-scanner client-header /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml $(MK_ROOT)src/wl/xdg-shell.h)
		XDG_SOURCE := $(shell wayland-scanner private-code /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml $(MK_ROOT)src/wl/xdg-shell.c)
		
		CFLAGS	+= -DUSE_WL
		CFLAGS	+= -DUSE_EGL # wayland uses EGL by default
		SRCS	:= $(SRCS_WL)
		SRCS	+= $(SRCS_XDG)
		SRCS	+= $(SRCS_EGL)
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
	cp $(MK_ROOT)src/gf.h /usr/local/include

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
