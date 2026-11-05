# meta-nile-test

Experimental board support for testing. Downstream clients should not
consume this layer.

## Genesys ZU 3EG SDT support

The [genesyszu-3eg machine](conf/machine/genesyszu-3eg.conf) uses Xilinx
2026.1 SDT with Linux, FSBL, and PMU domains. Hardware inputs are pinned
from `Digilent/sdt`; domain configurations come from `meta-digilent-bsp`
revision `e1457902c645ca954f28e6ff9fc60a5348555159` under the
[MIT license](COPYING.MIT).

Add this layer to a Linux Yocto build with `meta-xilinx-standalone-sdt`
and its dependencies, then select `MACHINE = "genesyszu-3eg"`. Disable
other BSP layers defining the same machine. Kas targets are unchanged.

Validate from the initialized build environment:

```sh
bitbake-getvar -r device-tree CONFIG_DTFILE_DIR
bitbake -p
bitbake device-tree
bitbake mc:genesyszu-3eg-cortexa53-fsbl:fsbl-firmware mc:genesyszu-3eg-microblaze-pmu:pmu-firmware
bitbake u-boot-xlnx xilinx-bootbin
```

Confirm `CONFIG_DTFILE_DIR` points into this layer. Build the desired image
and test DDR initialization, SD/UHS access, Ethernet MAC loading, and USB
enumeration on hardware.

The [user DTSI](recipes-bsp/device-tree/files/genesyszu_3eg/system-user.dtsi)
applies only to Linux. Legacy UIO PL peripherals are absent from this
SDT design; restoring them requires matching hardware and regenerated
trees. 5EV support is not included.
