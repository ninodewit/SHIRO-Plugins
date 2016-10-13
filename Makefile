#!/usr/bin/make -f
# Makefile for SHIRO Plugins #
# ---------------------------- #
# Created by falkTX
#

include Makefile.mk

all: plugins gen

# --------------------------------------------------------------

plugins:
	$(MAKE) all -C plugins/shiroverb
	$(MAKE) all -C plugins/pitchotto
	$(MAKE) all -C plugins/modulay
	$(MAKE) all -C plugins/larynx
	$(MAKE) all -C plugins/harmless

gen: plugins dpf/utils/lv2_ttl_generator
	@$(CURDIR)/dpf/utils/generate-ttl.sh
ifeq ($(MACOS),true)
	@$(CURDIR)/dpf/utils/generate-vst-bundles.sh
endif

dpf/utils/lv2_ttl_generator:
	$(MAKE) -C dpf/utils/lv2-ttl-generator

# --------------------------------------------------------------

clean:
	$(MAKE) clean -C dpf/utils/lv2-ttl-generator
	$(MAKE) clean -C plugins/shiroverb
	$(MAKE) clean -C plugins/pitchotto
	$(MAKE) clean -C plugins/modulay
	$(MAKE) clean -C plugins/larynx
	$(MAKE) clean -C plugins/harmless

# --------------------------------------------------------------

install:
	install -d $(DESTDIR)$(PREFIX)/lib/ladspa/
	install -d $(DESTDIR)$(PREFIX)/lib/dssi/
	install -d $(DESTDIR)$(PREFIX)/lib/lv2/
	install -d $(DESTDIR)$(PREFIX)/lib/vst/

	cp bin/*-ladspa.* $(DESTDIR)$(PREFIX)/lib/ladspa/
	cp bin/*-vst.*    $(DESTDIR)$(PREFIX)/lib/vst/
	cp -r bin/*.lv2   $(DESTDIR)$(PREFIX)/lib/lv2/

# --------------------------------------------------------------

.PHONY: plugins
