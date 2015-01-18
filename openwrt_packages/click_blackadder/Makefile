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

CONFIGURE_ARGS += \
	--enable-tools=host \
	--enable-userlevel \
	--enable-wifi \
	--disable-linuxmodule \
	--disable-dynamic-linking \

define Build/Compile
	# First, make? + make install Click
	$(call Build/Install/Default, install)
	
	# From Build/Prepare in hello_openwrt
	# Couldn't figure out how to do 'git download, then copy' in prepare
	$(CP) ./src/* $(BUILD_DIR)/
	
	# Then, configure, make (+ make install?) Blackadder
	# (Blackadder's configure needs to happen after building click to work)
	(cd $(BUILD_DIR)/blackadder/src; \
		./configure \
			--disable-linuxmodule \
			--with-click=$(PKG_INSTALL_DIR)/usr \
			--prefix=$(PKG_INSTALL_DIR)/usr \
	)
	
	$(MAKE) -C $(BUILD_DIR)/blackadder/src
	# make install?
endef

define Package/click/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/usr/share/click
	$(CP) $(PKG_INSTALL_DIR)/usr/bin/click $(1)/usr/bin/click
	$(CP) $(PKG_INSTALL_DIR)/usr/bin/click-align $(1)/usr/bin/click-align
	$(CP) $(PKG_INSTALL_DIR)/usr/share/click/elementmap.xml $(1)/usr/share/click/elementmap.xml
endef

$(eval $(call BuildPackage,click))

