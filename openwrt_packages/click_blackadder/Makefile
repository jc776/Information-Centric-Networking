#
# Copyright (C) 2006-2013 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=click

PKG_VERSION:=20140803
PKG_RELEASE:=1
PKG_REV:=d0a93f20beb786a55089657a08b663d721c2e9bc

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=https://github.com/kohler/click.git
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_PROTO:=git

# Custom var.
PKG_BLACKADDER_URL:=https://github.com/fp7-pursuit/blackadder.git
# could be made version specific. As of 26/01/2015, the latest commit I'm using is from 30/04/2013.
# https://github.com/fp7-pursuit/blackadder/commit/866c3d72135e44317b59eb4ba1e461a1b403c7e5

include $(INCLUDE_DIR)/package.mk

define Package/click
  SECTION:=net
  CATEGORY:=Network
  SUBMENU:=Routing and Redirection
  TITLE:=Click Modular Router w/ Blackadder
  MAINTAINER:=John Coady
  DEPENDS:=+kmod-tun +libpcap +libstdcpp
  URL:=http://www.read.cs.ucla.edu/
endef

define Package/click/Description
  Click Modular Router w/ Blackadder
endef

EXTRA_CFLAGS=-MD

PKG_FIXUP:=autoreconf

# From a different Click-package makefile.
#TARGET_CFLAGS += "-static -O2 -MD"
#TARGET_CXXFLAGS += "-static -O2 -MD"

# From 'click by itself':
#CONFIGURE_ARGS += \
#	--enable-tools=host \
#	--enable-userlevel \
#	--enable-wifi \
#	--disable-linuxmodule \
#	--disable-dynamic-linking \

# From 'blackadder w/ cross-compiler'
#--disable-linuxmodule --target=mips-openwrt-linux  --enable-blackadder  --enable-tools=mixed --disable-dynamic-linking ARCH=mips CFLAGS=-static CXXFLAGS=-static
# tools=mixed: "cc1plus: error: unrecognized command line option"
# ARCH - should be done by SDK?

CONFIGURE_ARGS += \
	--enable-tools=host \
	--enable-userlevel \
	--disable-linuxmodule \
	--enable-blackadder
#	--disable-dynamic-linking \
#	--enable-ipsec \

# The last one is a test.

define Build/Prepare
	#echo $(PKG_INSTALL_DIR);
	#false;
	
	# Get Click.
	$(Build/Prepare/Default)
	
	# Get Blackadder.
	(	cd $(BUILD_DIR); \
		git clone $(PKG_BLACKADDER_URL) \
	)
	
	# ~ "NS3" instructions
	# OK: This causes "/elements/blackadder" to compile.
	ln -s $(BUILD_DIR)/blackadder/src $(PKG_BUILD_DIR)/elements/blackadder
	
	# I should really be patching 'configure.in' then running autoconf.
	$(CP) ./src/configure_mod $(PKG_BUILD_DIR)/configure
endef

define Build/Compile
	# First, rebuild 'element list'
	$(MAKE_VARS);
	$(MAKE) -C $(PKG_BUILD_DIR)/$(MAKE_PATH) \
		$(MAKE_FLAGS) \
		elemlist;
	
	# Click: make install (which also does make build)
	$(call Build/Install/Default, install)
	
	# Blackadder: ./configure
	# (Blackadder's configure needs to happen after building click to work)
	# This adds blackadder as a click "package" (otherwise: 'requirement blackadder not available')
	(	cd $(BUILD_DIR)/blackadder/src; \
		./configure \
			--disable-linuxmodule \
			--with-click=$(PKG_INSTALL_DIR)/usr \
			--prefix=$(PKG_INSTALL_DIR)/usr; \
		echo "Configure worked." \
	)
	
	# Blackadder: make
	$(MAKE) -C $(BUILD_DIR)/blackadder/src
	
	# Blackadder: make install
	# $(MAKE) ... install breaks at 'mkinstalldirs'
	# Could patch the file
	# Instead, just make our own directories
	#$(INSTALL_BIN) ./src/blankinstalldirs $(PKG_INSTALL_DIR)/usr/share/click/mkinstalldirs
	
	# for blackadder.uo
	# can't figure out how to run this without "/home/jack/.../sdk/.../ipkg-install/home/jack/..." or "mkinstalldirs permission denied" for "/usr/share/click"
	# modified from Build/Install/Default (SDK's include/package-defaults.mk)
	#$(MAKE_VARS) \
	#$(MAKE) -C $(BUILD_DIR)/blackadder/src \
	#	$(MAKE_FLAGS) \
	#	DESTDIR="" \
	#	install
	
	$(MAKE_VARS) \
	$(MAKE) -C $(BUILD_DIR)/blackadder/src \
		$(MAKE_FLAGS) \
		DESTDIR="" \
		elementmap-blackadder.xml
	
	# $(PKG_INSTALL_DIR)
endef

define Package/click/install
	# From click by itself
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/usr/share/click
	$(CP) $(PKG_INSTALL_DIR)/usr/bin/click $(1)/usr/bin/click
	$(CP) $(PKG_INSTALL_DIR)/usr/bin/click-align $(1)/usr/bin/click-align
	$(CP) $(PKG_INSTALL_DIR)/usr/share/click/elementmap.xml $(1)/usr/share/click/elementmap.xml
	
	# Blackadder: Copy blackadder.uo
	# just get the file myself instead of blackadder's make install
	# put the file in ipkg-install
	$(CP) $(BUILD_DIR)/blackadder/src/blackadder.uo $(PKG_INSTALL_DIR)/usr/lib/blackadder.uo
	$(CP) $(BUILD_DIR)/blackadder/src/elementmap-blackadder.xml $(PKG_INSTALL_DIR)/usr/share/click/elementmap-blackadder.xml
	# make a lib dir in the package
	$(INSTALL_DIR) $(1)/usr/lib
	# copy the things over manually instead of make install
	# put the file in the actual ipkg
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/lib/blackadder.uo $(1)/usr/lib/blackadder.uo
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/share/click/elementmap-blackadder.xml $(1)/usr/share/click/elementmap-blackadder.xml
	
	# blackadder elementmap
	
	# Unused directories generated
	#$(INSTALL_DIR) $(1)/usr/include - shouldn't be compiling things on the router
	#$(INSTALL_DIR) $(1)/usr/man - don't fill the router with the manual
endef

$(eval $(call BuildPackage,click))

