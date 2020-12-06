# The default target of this Makefile is this
all::

# Define V=1 to have a more verbose compile.
#
# Define NO_PANEL if you don't want to use -lpanel.
#
# Define NEEDS_NCURSES5 if you need linking with ncurses5.
#
# Define NEEDS_NCURSESW5 if you need linking with ncursesw5.
#
# Define NEEDS_NCURSES6 if you need linking with ncurses6.
#
# Define NEEDS_NCURSESW6 if you need linking with ncursesw6.

MAKEFLAGS += --jobs=$(shell cat /proc/cpuinfo | grep processor | wc -l || echo 4)

VERSION-FILE: FORCE
	@$(SHELL_PATH) ./GEN-VERSION-FILE
-include VERSION-FILE

CFLAGS = -g -O2 -Wall -W -Werror=format-security
LDFLAGS =
VIPTRAF_CFLAGS := -D_GNU_SOURCE
ALL_CFLAGS = $(CPPFLAGS) $(CFLAGS) $(VIPTRAF_CFLAGS)
ALL_LDFLAGS = $(LDFLAGS)
STRIP ?= strip

prefix = $(HOME)
sbindir_relative = sbin
sbindir = $(prefix)/$(sbindir_relative)
mandir = $(prefix)/share/man
sharedir = $(prefix)/share
localedir = $(sharedir)/locale
lib = lib
man8dir = $(mandir)/man8

# DESTDIR=
pathsep = :

export prefix bindir sharedir sysconfdir gitwebdir localedir

CC = c++
RM = rm -f
INSTALL = install
RPMBUILD = rpmbuild
TAR = tar

### --- END CONFIGURATION SECTION ---


# Those must not be GNU-specific; they are shared with perl/ which may
# be built by a different compiler. (Note that this is an artifact now
# but it still might be nice to keep that distinction.)
BASIC_CFLAGS = -I. -Isrc/
BASIC_LDFLAGS =

# Guard against environment variables
viptraf-h :=
viptraf-o :=

ALL_PROGRAMS := viptraf

ifndef SHELL_PATH
	SHELL_PATH = /bin/sh
endif

viptraf-h += src/tui/input.h
viptraf-h += src/tui/labels.h
viptraf-h += src/tui/listbox.h
viptraf-h += src/tui/menurt.h
viptraf-h += src/tui/msgboxes.h
viptraf-h += src/tui/winops.h
viptraf-h += src/iptraf-ng-compat.h
viptraf-h += src/parse-options.h
viptraf-h += src/packet.h
viptraf-h += src/tcptable.h
viptraf-h += src/othptab.h
viptraf-h += src/ifstats.h
viptraf-h += src/deskman.h
viptraf-h += src/hostmon.h
viptraf-h += src/fltedit.h
viptraf-h += src/cidr.h
viptraf-h += src/fltselect.h
viptraf-h += src/ipfilter.h
viptraf-h += src/fltmgr.h
viptraf-h += src/ipfrag.h
viptraf-h += src/serv.h
viptraf-h += src/servname.h
viptraf-h += src/timer.h
viptraf-h += src/ifaces.h
viptraf-h += src/error.h
viptraf-h += src/revname.h
viptraf-h += src/log.h
viptraf-h += src/pktsize.h
viptraf-h += src/landesc.h
viptraf-h += src/dirs.h
viptraf-h += src/getpath.h
viptraf-h += src/options.h
viptraf-h += src/promisc.h
viptraf-h += src/parseproto.h
viptraf-h += src/addproto.h
viptraf-h += src/arphdr.h
viptraf-h += src/attrs.h
viptraf-h += src/fltdefs.h
viptraf-h += src/logvars.h
viptraf-h += src/list.h
viptraf-h += src/counters.h
#viptraf-h += src/rate.h
viptraf-h += src/built-in.h
viptraf-h += src/sockaddr.h
viptraf-h += src/capt.h
viptraf-h += src/capt-recvmsg.h
viptraf-h += src/capt-recvmmsg.h
viptraf-h += src/capt-mmap-v2.h
viptraf-h += src/capt-mmap-v3.h
viptraf-h += " \
        src/traf_log.h \
        src/video.h \
        src/video_ncurses.h \
        src/text_field.h \
        src/text_field_text.h \
        src/text_box.h \
        src/dialog.h \
        src/dlg_about.h \
        src/addr.h \
        src/desktop.h \
        src/traf_rate.h \
        src/ethernet.h \
        src/tcp_con.h \
        src/app_host_monitor.h \
    "

viptraf-o += src/tui/input.o
viptraf-o += src/tui/labels.o
viptraf-o += src/tui/listbox.o
viptraf-o += src/tui/menurt.o
viptraf-o += src/tui/msgboxes.o
viptraf-o += src/tui/winops.o
viptraf-o += src/error.o
viptraf-o += src/log.o
viptraf-o += src/getpath.o
viptraf-o += src/parseproto.o
viptraf-o += src/fltselect.o
viptraf-o += src/ipfilter.o
viptraf-o += src/fltmgr.o
viptraf-o += src/ipfrag.o
viptraf-o += src/serv.o
viptraf-o += src/servname.o
viptraf-o += src/timer.o
viptraf-o += src/revname.o
viptraf-o += src/pktsize.o
viptraf-o += src/landesc.o
viptraf-o += src/options.o
viptraf-o += src/promisc.o
viptraf-o += src/ifaces.o
viptraf-o += src/usage.o
viptraf-o += src/iptraf.o
viptraf-o += src/itrafmon.o
viptraf-o += src/wrapper.o
viptraf-o += src/parse-options.o
viptraf-o += src/packet.o
viptraf-o += src/tcptable.o
viptraf-o += src/othptab.o
viptraf-o += src/ifstats.o
viptraf-o += src/detstats.o
viptraf-o += src/deskman.o
viptraf-o += src/hostmon.o
viptraf-o += src/fltedit.o
viptraf-o += src/cidr.o
viptraf-o += src/counters.o
#viptraf-o += src/rate.o
viptraf-o += src/capture-pkt.o
viptraf-o += src/sockaddr.o
viptraf-o += src/capt.o
viptraf-o += src/capt-recvmsg.o
viptraf-o += src/capt-recvmmsg.o
viptraf-o += src/capt-mmap-v2.o
viptraf-o += src/capt-mmap-v3.o
viptraf-o += src/rvnamed.o
viptraf-o += src/addr.o

viptraf-opp += \
    src/traf_log.opp \
    src/video.opp \
    src/video_ncurses.opp \
    src/text_field.opp \
    src/text_field_text.opp \
    src/text_box.opp \
    src/dialog.opp \
    src/dlg_about.opp \
    src/desktop.opp \
    src/traf_rate.opp \
    src/ethernet.opp \
    src/tcp_con.opp \
    src/app_host_monitor.opp

ifndef sysconfdir
ifeq ($(prefix),/usr)
sysconfdir = /etc
else
sysconfdir = etc
endif
endif

ifdef CHECK_HEADER_DEPENDENCIES
COMPUTE_HEADER_DEPENDENCIES = no
USE_COMPUTED_HEADER_DEPENDENCIES =
endif

ifndef COMPUTE_HEADER_DEPENDENCIES
COMPUTE_HEADER_DEPENDENCIES = auto
endif

ifeq ($(COMPUTE_HEADER_DEPENDENCIES),auto)
dep_check = $(shell $(CC) $(ALL_CFLAGS) \
	-c -MF /dev/null -MMD -MP -x c /dev/null -o /dev/null 2>&1; \
	echo $$?)
ifeq ($(dep_check),0)
override COMPUTE_HEADER_DEPENDENCIES = yes
else
override COMPUTE_HEADER_DEPENDENCIES = no
endif
endif

ifeq ($(COMPUTE_HEADER_DEPENDENCIES),yes)
USE_COMPUTED_HEADER_DEPENDENCIES = YesPlease
else
ifneq ($(COMPUTE_HEADER_DEPENDENCIES),no)
$(error please set COMPUTE_HEADER_DEPENDENCIES to yes, no, or auto \
(not "$(COMPUTE_HEADER_DEPENDENCIES)"))
endif
endif

ifndef NCURSES_LDFLAGS
ifdef NEEDS_NCURSES5
	NCURSES_CFLAGS := $(shell ncurses5-config --cflags 2>/dev/null)
	NCURSES_LDFLAGS := $(shell ncurses5-config --libs 2>/dev/null)
	ifndef NO_PANEL
		NCURSES_LDFLAGS += -lpanel
	endif
endif
endif

ifndef NCURSES_LDFLAGS
ifdef NEEDS_NCURSESW5
	NCURSES_CFLAGS := $(shell ncursesw5-config --cflags 2>/dev/null)
	NCURSES_LDFLAGS := $(shell ncursesw5-config --libs 2>/dev/null)
	ifndef NO_PANEL
		NCURSES_LDFLAGS += -lpanelw
	endif
endif
endif

ifndef NCURSES_LDFLAGS
ifdef NEEDS_NCURSES6
	NCURSES_CFLAGS := $(shell ncurses6-config --cflags 2>/dev/null)
	NCURSES_LDFLAGS := $(shell ncurses6-config --libs 2>/dev/null)
	ifndef NO_PANEL
		NCURSES_LDFLAGS += -lpanel
	endif
endif
endif

ifndef NCURSES_LDFLAGS
ifdef NEEDS_NCURSESW6
	NCURSES_CFLAGS := $(shell ncursesw6-config --cflags 2>/dev/null)
	NCURSES_LDFLAGS := $(shell ncursesw6-config --libs 2>/dev/null)
	ifndef NO_PANEL
		NCURSES_LDFLAGS += -lpanelw
	endif
endif
endif

# try find ncuses by autodetect
ifndef NCURSES_LDFLAGS
	ifneq ($(shell ncursesw6-config --libs 2>/dev/null),)
		NCURSES_CFLAGS := $(shell ncursesw6-config --cflags 2>/dev/null)
		NCURSES_LDFLAGS := $(shell ncursesw6-config --libs 2>/dev/null)
		ifndef NO_PANEL
			NCURSES_LDFLAGS += -lpanelw
		endif
	else ifneq ($(shell ncurses6-config --libs 2>/dev/null),)
		NCURSES_CFLAGS := $(shell ncurses6-config --cflags 2>/dev/null)
		NCURSES_LDFLAGS := $(shell ncurses6-config --libs 2>/dev/null)
		ifndef NO_PANEL
			NCURSES_LDFLAGS += -lpanel
		endif
	else ifneq ($(shell ncursesw5-config --libs 2>/dev/null),)
		NCURSES_CFLAGS := $(shell ncursesw5-config --cflags 2>/dev/null)
		NCURSES_LDFLAGS := $(shell ncursesw5-config --libs 2>/dev/null)
		ifndef NO_PANEL
			NCURSES_LDFLAGS += -lpanelw
		endif
	else ifneq ($(shell ncurses5-config --libs 2>/dev/null),)
		NCURSES_CFLAGS := $(shell ncurses5-config --cflags 2>/dev/null)
		NCURSES_LDFLAGS := $(shell ncurses5-config --libs 2>/dev/null)
		ifndef NO_PANEL
			NCURSES_LDFLAGS += -lpanel
		endif
	endif
endif

QUIET_SUBDIR0  = +$(MAKE) -C # space to separate -C and subdir
QUIET_SUBDIR1  =

ifneq ($(findstring $(MAKEFLAGS),w),w)
PRINT_DIR = --no-print-directory
else # "make -w"
NO_SUBDIR = :
endif

ifneq ($(findstring $(MAKEFLAGS),s),s)
ifndef V
	QUIET_CC       = @echo '   ' CC $@;
	QUIET_LINK     = @echo '   ' LINK $@;
	QUIET_GEN      = @echo '   ' GEN $@;
	QUIET_SUBDIR0  = +@subdir=
	QUIET_SUBDIR1  = ;$(NO_SUBDIR) echo '   ' SUBDIR $$subdir; \
			 $(MAKE) $(PRINT_DIR) -C $$subdir
	export V
	export QUIET_GEN
	export QUIET_BUILT_IN
endif
endif


DESTDIR_SQ = $(subst ','\'',$(DESTDIR))
sbindir_SQ = $(subst ','\'',$(sbindir))

ALL_CFLAGS += $(BASIC_CFLAGS)
ALL_LDFLAGS += $(BASIC_LDFLAGS)

export TAR INSTALL DESTDIR SHELL_PATH

### Build rules

SHELL = $(SHELL_PATH)

#all:: shell_compatibility_test
#please_set_SHELL_PATH_to_a_more_modern_shell:
#	@$$(:)
#shell_compatibility_test: please_set_SHELL_PATH_to_a_more_modern_shell


all:: $(ALL_PROGRAMS)

viptraf: $(viptraf-o) $(viptraf-opp) VERSION-FILE
	$(QUIET_LINK)$(CC) $(ALL_CFLAGS) -o $@ \
		$(viptraf-o) $(viptraf-opp) $(ALL_LDFLAGS) $(NCURSES_LDFLAGS)

EXTRA_CPPFLAGS = -DVIPTRAF_VERSION='"$(VIPTRAF_VERSION)"' -DVIPTRAF_NAME='"viptraf"'

OBJECTS-C := $(sort $(viptraf-o))
OBJECTS-CPP := $(sort $(viptraf-opp))

dep_files := $(foreach f,$(OBJECTS-C) $(OBJECTS-CPP),$(dir $f).depend/$(notdir $f).d)
dep_dirs := $(addsuffix .depend,$(sort $(dir $(OBJECTS-C) $(OBJECTS-CPP))))

ifeq ($(COMPUTE_HEADER_DEPENDENCIES),yes)
$(dep_dirs):
	@mkdir -p $@

missing_dep_dirs := $(filter-out $(wildcard $(dep_dirs)),$(dep_dirs))
dep_file = $(dir $@).depend/$(notdir $@).d
dep_args = -MF $(dep_file) -MMD -MP
ifdef CHECK_HEADER_DEPENDENCIES
$(error cannot compute header dependencies outside a normal build. \
Please unset CHECK_HEADER_DEPENDENCIES and try again)
endif
endif

.SUFFIXES:

ifdef PRINT_HEADER_DEPENDENCIES
$(OBJECTS-C): %.o: %.c FORCE
$(OBJECTS-C++): %.opp: %.cpp FORCE
	echo $^

ifndef CHECK_HEADER_DEPENDENCIES
$(error cannot print header dependencies during a normal build. \
Please set CHECK_HEADER_DEPENDENCIES and try again)
endif
endif

ifndef PRINT_HEADER_DEPENDENCIES
ifdef CHECK_HEADER_DEPENDENCIES
$(OBJECTS-C): %.o: %.c $(dep_files) FORCE
	@set -e; echo CHECK $@; \
	missing_deps="$(missing_deps)"; \
	if test "$$missing_deps"; \
	then \
		echo missing dependencies: $$missing_deps; \
		false; \
	fi
$(OBJECTS-CPP): %.opp: %.cpp $(dep_files) FORCE
	@set -e; echo CHECK $@; \
	missing_deps="$(missing_deps)"; \
	if test "$$missing_deps"; \
	then \
		echo missing dependencies: $$missing_deps; \
		false; \
	fi
endif
endif

ifndef CHECK_HEADER_DEPENDENCIES
$(OBJECTS-C): %.o: %.c $(missing_dep_dirs)
	$(QUIET_CC)$(CC) -o $*.o -c $(dep_args) $(NCURSES_CFLAGS) $(ALL_CFLAGS) $(EXTRA_CPPFLAGS) $<
$(OBJECTS-CPP): %.opp: %.cpp $(missing_dep_dirs)
	$(QUIET_CC)$(CC) -o $*.opp -c $(dep_args) $(NCURSES_CFLAGS) $(ALL_CFLAGS) $(EXTRA_CPPFLAGS) $<
endif

ifdef USE_COMPUTED_HEADER_DEPENDENCIES
# Take advantage of gcc's on-the-fly dependency generation
# See <http://gcc.gnu.org/gcc-3.0/features.html>.
dep_files_present := $(wildcard $(dep_files))
ifneq ($(dep_files_present),)
include $(dep_files_present)
endif
else
# Dependencies on header files, for platforms that do not support
# the gcc -MMD option.
#
# Dependencies on automatically generated headers such as common-cmds.h
# should _not_ be included here, since they are necessary even when
# building an object for the first time.
#
# XXX. Please check occasionally that these include all dependencies
# gcc detects!

$(OBJECTS-C): $(viptraf-h)
$(OBJECTS-CPP): $(viptraf-h)
endif


### Maintainer's dist rules

VIPTRAF_TARNAME = viptraf-$(VIPTRAF_VERSION)
dist:
	@mkdir -p $(VIPTRAF_TARNAME)
	@cp --parents `git ls-files` $(VIPTRAF_TARNAME)
	$(TAR) cf $(VIPRRAF_TARNAME).tar $(VIPTRAF_TARNAME)
	@$(RM) -rf $(VIPTRAF_TARNAME)
	gzip -f -9 $(VIPTRAF_TARNAME).tar

### Documentation rules
html: Documentation/book1.html
pdf: Documentation/manual.pdf

Documentation/book1.html: Documentation/manual.sgml
	cd Documentation && docbook2html manual.sgml

Documentation/manual.pdf: Documentation/manual.sgml
	cd Documentation && docbook2pdf manual.sgml

Documentation/manual.sgml: Documentation/manual.sgml.in VERSION-FILE
	cat $< | sed \
		-e s/@@version@@/`echo $(VIPTRAF_VERSION) | cut -d. -f1-3`/ \
		-e s/@@major@@/`echo $(VIPTRAF_VERSION) | cut -d. -f1-2`/ \
	> $@

## TODO: use asciidoc to generate mans

### Installation rules
install: all
	@echo $(DESTDIR_SQ)$(man8dir)
	$(INSTALL) -d -m 755 '$(DESTDIR_SQ)$(sbindir_SQ)'
	$(INSTALL) $(ALL_PROGRAMS) '$(DESTDIR_SQ)$(sbindir_SQ)'
	$(INSTALL) -d -m 755 $(DESTDIR)$(man8dir)
	$(INSTALL) -m 644 src/viptraf.8  $(DESTDIR)$(man8dir)

### Cleaning rules

distclean: clean

clean:
	$(RM) Documentation/manual.sgml
	$(RM) Documentation/manual.pdf
	$(RM) Documentation/*.html
	$(RM) $(viptraf-opp)
	$(RM) $(viptraf-o)
	$(RM) $(ALL_PROGRAMS)
	$(RM) -r $(dep_dirs)
	$(RM) $(VIPTRAF_TARNAME).tar.gz
	$(RM) VERSION-FILE

gtags:
	$(QUIET_GEN) gtags

.PHONY: clean distclean all install html pdf gtags FORCE
