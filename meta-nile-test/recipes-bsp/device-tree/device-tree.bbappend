FILESEXTRAPATHS:prepend:genesyszu-3eg := "${THISDIR}/files/genesyszu_3eg:"

# Linux board settings must not be added to the standalone FSBL/PMU trees.
EXTRA_DT_INCLUDE_FILES:append:genesyszu-3eg = "${@' system-user.dtsi' if os.path.basename(d.getVar('CONFIG_DTFILE') or '') == 'cortexa53-linux.dts' else ''}"
DT_INCLUDE:append:genesyszu-3eg = " ${SYSTEM_DTFILE_DIR}"
