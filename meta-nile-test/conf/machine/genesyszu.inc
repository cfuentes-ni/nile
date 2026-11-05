#@TYPE: Machine
#@NAME: genesyszu
#@DESCRIPTION: Machine support for Digilent Genesys ZU family

#### Preamble
MACHINEOVERRIDES =. "${@['', 'genesyszu:']['genesyszu' !='${MACHINE}']}"
#### Regular settings follow

XILINX_RELEASE_VERSION = "2026.1"

require conf/machine/zynqmp${ZYNQMP_SUBMODEL}-generic.conf

PREFERRED_PROVIDER_virtual/kernel ?= "linux-xlnx"
EXTRA_IMAGEDEPENDS += "\
	u-boot-xlnx-uenv \
	"

IMAGE_BOOT_FILES += "\
	boot.bin \
	uEnv.txt \
	"

### No additional settings should be after the Postamble
#### Postamble
PACKAGE_EXTRA_ARCHS:append = "${@['', ' genesyszu']['genesyszu' != '${MACHINE}']}"
