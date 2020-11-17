DESCRIPTION = "Ripe Atlas Probe"
HOMEPAGE = "https://github.com/RIPE-NCC/ripe-atlas-software-probe"
LICENSE = "GPLv3"
LIC_FILES_CHKSUM = "file://git/LICENSE;md5=4ddd17b0c9241d7b24a4960caefe8e40"
PREFERRED_PROVIDER_libevent="libevent"


S = "${WORKDIR}/"
TMP_DATA_DIR = "/usr/local/atlas"

DEPENDS += "virtual/libc utopia"
RDEPENDS_${PN} += "tar libtool libssl libevent glibc utopia"

PV = "5020"
SRC_URI = "gitsm://github.com/RIPE-NCC/ripe-atlas-software-probe.git;tag=v${PV}"
SRC_URI[md5sum] = "e75d80f5397565576518fae3fc2b085e"


inherit gettext autotools pkgconfig systemd 

SYSTEMD_SERVICE_${PN} = "RipeAtlas.service"


FILESEXTRAPATHS_prepend := "${THISDIR}/patches:${THISDIR}:"
SRC_URI += " \
    file://01-RAP-parameter-pass-through.patch \
    file://02-RAP-script-fixes.patch \
    file://03-RAP-create-local-dropbear-ssh-keys.patch \
    file://04-RAP-busybox-disable-wdg.patch \
    file://05-RAP-ssh-servers-and-args.patch \
    file://06-RAP-use-system-ssh-know_host-file.patch \
    file://07-RAP-FIX-get-erouter0-mac.patch \
    file://08-RAP-use-local-busybox-tee.patch \
    file://other/ \
    file://agent/ \
"

FILES_${PN} += " \
    /usr/local/atlas/* \
    /var/atlasdata \
    /var/atlas-probe/*  \
    /usr/ccsp/ripe-atlas \
    /nvram/atlas/* \
    /home/root/.ssh/known_hosts \
"

EXTRA_OEMAKE += "V=1 ARCH=${TARGET_ARCH} CROSS_COMPILE=${TARGET_PREFIX} SKIP_STRIP=y"
AUTOTOOLS_SCRIPT_PATH = "${S}/agent"
B = "${S}/agent/build"

do_compile_append() {
    cd ${S}

    #Check if the symlink already exists
    if [ ! -L ripe-atlas-software-probe ]; then
        ln -s git ripe-atlas-software-probe
    fi

    export EXTRA_OEMAKE2="${EXTRA_OEMAKE}"

    ./ripe-atlas-software-probe/build-config/debian/bin/make-deb --host=${TARGET_PREFIX}
}

do_install_append() {
    # ------------ RIPE Atlas SW probes ------------
    ATLAS_SRC=${S}atlasswprobe-${PV}-1/
    
    install -d ${D}/etc/systemd/system/
    cp -r     ${ATLAS_SRC}/etc/systemd/system/                ${D}/etc/systemd/

    install -d ${D}/var/atlasdata/

    install -d ${D}/usr/local/atlas/

    cp -r      ${ATLAS_SRC}/usr/local/atlas/bin/                 ${D}/usr/local/atlas/
    cp -r      ${ATLAS_SRC}/usr/local/atlas/etc/                 ${D}/usr/local/atlas/


    install -d ${D}/usr/local/atlas/state/
    install    ${ATLAS_SRC}/usr/local/atlas/state/FIRMWARE_APPS_VERSION  ${D}/usr/local/atlas/state/
    
    install -d ${D}/usr/local/atlas/bb-13.3/
    cp -r  ${ATLAS_SRC}/usr/local/atlas/bb-13.3 ${D}/usr/local/atlas/

    install -d ${D}/usr/local/atlas/lib/
    install    ${ATLAS_SRC}/usr/local/atlas/lib/libevent-*so* ${D}/usr/local/atlas/lib/
    install    ${ATLAS_SRC}/usr/local/atlas/lib/libevent_openssl-*so* ${D}/usr/local/atlas/lib/

    install -d ${D}/${TMP_DATA_DIR}/var/atlas-probe/bin/
    install    ${ATLAS_SRC}/var/atlas-probe/bin/config.sh                ${D}/${TMP_DATA_DIR}/var/atlas-probe/bin/
    
    install -d ${D}/${TMP_DATA_DIR}/var/atlas-probe/state/
    install    ${ATLAS_SRC}/var/atlas-probe/state/mode                   ${D}/${TMP_DATA_DIR}/var/atlas-probe/state/

    echo "RXTXRPT=yes" > ${ATLAS_SRC}/var/atlas-probe/state/config.txt
    install    ${ATLAS_SRC}/var/atlas-probe/state/config.txt             ${D}/${TMP_DATA_DIR}/var/atlas-probe/state/

    install -d ${D}/${TMP_DATA_DIR}/var/atlasdata

    # ------------ Agent related files ------------

    install -d ${D}/usr/ccsp/ripe-atlas
    install -m 0644 ${S}/other/RipeAtlas.xml \
                    ${D}/usr/ccsp/ripe-atlas/RipeAtlas.xml

    install -d ${D}/etc/systemd/system/
    install -m 0644 ${S}/other/RipeAtlas.service \
                    ${D}/etc/systemd/system/RipeAtlas.service

    install -d ${D}/${TMP_DATA_DIR}/
    install  -m 777 ${S}/other/BootScript.sh      ${D}/${TMP_DATA_DIR}/BootScript.sh

    # ------------ Others ------------

    install -d ${D}/home/root/.ssh/
    
    ln -s /nvram/.ssh/known_hosts ${D}/home/root/.ssh/known_hosts

    echo ${PV} > ${D}${TMP_DATA_DIR}/var/comp_version.txt
}