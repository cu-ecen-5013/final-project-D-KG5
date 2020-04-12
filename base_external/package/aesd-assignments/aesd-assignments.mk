
##############################################################
#
# AESD-ASSIGNMENTS
#
##############################################################

#TODO: Fill up the contents below in order to reference your assignment 3 git contents
AESD_ASSIGNMENTS_VERSION = 'bc2b8f1c072c52c6ce1f9537eeda3168ee6e9326'
AESD_ASSIGNMENTS_SITE = 'git@github.com:chwe3468/cu-ecen-5013-final-project-shared.git'
AESD_ASSIGNMENTS_SITE_METHOD = git
AESD_ASSIGNMENTS_LICENSE = GPL-2.0
AESD_ASSIGNMENTS_LICENSE_FILES = COPYING

# AESD_ASSIGNMENTS_MODULE_SUBDIRS = /Dhruva/platform_testing
# AESD_ASSIGNMENTS_MODULE_MAKE_OPTS = KVERSION=$(LINUX_VERSION_PROBED)

define AESD_ASSIGNMENTS_BUILD_CMDS
	$(MAKE) $(TARGET_CONFIGURE_OPTS) -C $(@D) all
endef

define AESD_ASSIGNMENTS_INSTALL_TARGET_CMDS
	$(INSTALL) -m 0755 $(@D)/Dhruva/platform_testing/sensor $(TARGET_DIR)/usr/bin
	$(INSTALL) -m 0755 $(@D)/Dhruva/platform_testing/gettemp.sh $(TARGET_DIR)/usr/bin
	
	$(INSTALL) -m 0755 $(@D)/Dhruva/platform_testing/sensor-start-stop.sh $(TARGET_DIR)/etc/init.d/S98sensor
endef


$(eval $(generic-package))
