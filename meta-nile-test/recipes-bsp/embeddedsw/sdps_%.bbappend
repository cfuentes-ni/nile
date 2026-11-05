# SDT builds the SD driver separately from the FSBL.
CFLAGS:append:genesyszu-3eg = " -DUHS_MODE_ENABLE"
