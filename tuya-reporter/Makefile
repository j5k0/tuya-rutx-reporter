include $(TOPDIR)/rules.mk

PKG_NAME:=tuya-reporter
PKG_RELEASE:=1
PKG_VERSION:=1.0.0

include $(INCLUDE_DIR)/package.mk

define Package/tuya-reporter
	CATEGORY:=Base system
	TITLE:=tuya-reporter
	DEPENDS:=+libtuya-sdk +libubus +libubox +libblobmsg-json
endef

define Package/tuya-reporter/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tuya-reporterd $(1)/usr/bin
	$(INSTALL_BIN) ./files/tuya-reporter.init $(1)/etc/init.d/tuya-reporterd
	$(INSTALL_BIN) ./files/tuya-reporter.config $(1)/etc/config/tuya-reporterd
endef

$(eval $(call BuildPackage,tuya-reporter))
