# download toybox
wget https://landley.net/toybox/downloads/binaries/latest/toybox-x86_64
chmod +x toybox-x86_64
mv toybox-x86_64 toybox

# download and extract linux kernel source
wget https://github.com/torvalds/linux/archive/refs/tags/v6.17.tar.gz
tar -xf v6.17.tar.gz

# build kernel and gdb scripts
mkdir linux-6.17_build
make -C linux-6.17 O=$PWD/linux-6.17_build \
    ARCH=x86_64 \
    KCONFIG_ALLCONFIG=$PWD/mini.config \
    allnoconfig
make -C linux-6.17 O=$PWD/linux-6.17_build -j$(nproc)
make -C linux-6.17 O=$PWD/linux-6.17_build scripts_gdb

# build nolibc sysroot
mkdir nolibc
make -C linux-6.17/tools/include/nolibc \
    ARCH=x86_64 OUTPUT=$PWD/nolibc/ headers_standalone
