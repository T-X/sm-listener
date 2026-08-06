#
# Copyright (C) 2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=sm-listener
PKG_VERSION:=2016.0

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/T-X/sm-listener.git
PKG_SOURCE_VERSION:=238880d0f078f1063870264a62cfe794c42dc062
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz

include $(INCLUDE_DIR)/package.mk

define Package/sm-listener
  URL:=https://github.com/T-X/sm-listener
  SECTION:=net
  CATEGORY:=Network
  DEPENDS:=+libc
  TITLE:=Simple Multicast Listener
  MAINTAINER:=Simon Wunderlich <sw@simonwunderlich.de>
endef

define Package/sm-listener/description
  A tool to create a simple IPv6 dummy listener, which isn't actually receiving anything.
endef

define Build/Compile
	$(TARGET_CC) $(TARGET_CFLAGS) -D__UCLIBC__ $(PKG_BUILD_DIR)/listener.c -o $(PKG_BUILD_DIR)/sm-listener
endef

define Package/sm-listener/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/sm-listener $(1)/usr/bin/
endef

$(eval $(call BuildPackage,sm-listener))
