FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI:append:class-nativesdk = " file://0001-qemu-xilinx-sched-attr-glibc-version.patch"