include $(TOPDIR)/rules.mk

PKG_NAME:=tuya-sdk
PKG_RELEASE:=1
PKG_VERSION:=1.0.0
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/tuya/tuya-iot-core-sdk.git
PKG_SOURCE_VERSION:=0208badefb2003d30e70b5b875395e3187c850d9
PKG_HASH:=04cd43a0f785ef6c23e6ae712369fa05fb25a4de9f2d3904c7b79fa99b287753

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

CMAKE_BINARY_SUBDIR:=build

define Package/libtuya-sdk
	CATEGORY:=Libraries
	TITLE:=libtuya-sdk
endef

define Build/Install
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/lib $(1)/usr/include/tuya-sdk
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/build/lib/*.so $(1)/usr/lib/

	$(INSTALL_DATA) $(PKG_BUILD_DIR)/utils/*.h $(1)/usr/include/tuya-sdk
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/interface/*.h $(1)/usr/include/tuya-sdk
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/include/*.h $(1)/usr/include/tuya-sdk
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/examples/data_model_basic_demo/tuya_cacert.h $(1)/usr/include/tuya-sdk
endef

define Package/libtuya-sdk/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/build/lib/*.so $(1)/usr/lib
endef

$(eval $(call BuildPackage,libtuya-sdk))
