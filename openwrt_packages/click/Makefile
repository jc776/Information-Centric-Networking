###################################################
# OpenWRT Makefile for Click Router from Local Repo
#
###################################################

include $(TOPDIR)/rules.mk

# Name and release number of this package
PKG_NAME:=click
PKG_RELEASE:=1

# This specifies the directory where we're going to build the program.
# The root build directory, $(BUILD_DIR), is by default the build_mipsel
# directory in your OpenWrt SDK directory
PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

# Minimal from click-only Makefile
define Package/click
	SECTION:=net
	CATEGORY:=Network
	TITLE:=Click Modular Router
	# DESCRIPTION no longer in this section.
endef

# From click-only Makefile
define Package/click/description
	Click Modular Router userspace package
endef

# From click-only Makefile - static linking/other GCC options
TARGET_CFLAGS += "-static -O2 -MD"
TARGET_CXXFLAGS += "-static -O2 -MD"

# From 'build locally' tutorial, replacing default 'download from source URL'
define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

# From click-only Makefile
# cd changed to BUILD_DIR/click from BUILD_DIR
define Build/Configure
	(cd $(PKG_BUILD_DIR)/click; \
		rm -rf config.{cache,status} ; \
		./configure \
			--prefix=/usr \
			--target=$(GNU_TARGET_NAME) \
			--host=$(GNU_HOST_NAME) \
			--build=$(GNU_BUILD_NAME) \
			--enable-tools=mixed \
			--enable-userlevel \
			--enable-wifi \
			--enable-fixincludes \
			--disable-linuxmodule \
	)
endef

# From click-only Makefile
# Directories changed to /click
define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)/click \
		tools elementmap.xml
	(cd $(PKG_BUILD_DIR)/click/userlevel; \
		../tools/click-mkmindriver/click-mkmindriver -p $(PKG_NAME) -C .. \
		-f $(PKG_BUILD_DIR)/click/conf/wifi/dump.click \
		-A --all -E Discard -E Print; \
	)
	$(MAKE) -C $(PKG_BUILD_DIR)/click MINDRIVER=$(PKG_NAME)
endef

# From click-only Makefile
# $(1) "/, on the router"
# $(INSTALL_DIR) "Command to prepare the install dir if it doesn't exist"
# $(CP) "Copy command"

# BUILD_DIR changed to BUILD_DIR/click
define Package/click/install
	$(INSTALL_DIR) $(1)/usr
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/usr/share/click
	$(CP) $(PKG_BUILD_DIR)/click/userlevel/$(PKG_NAME)click $(1)/usr/bin/click
	$(CP) $(PKG_BUILD_DIR)/click/tools/click-align/click-align $(1)/usr/bin/click-align
	$(CP) $(PKG_BUILD_DIR)/click/elementmap.xml $(1)/usr/share/click/elementmap.xml
endef

# From click-only Makefile
$(eval $(call BuildPackage,click))
