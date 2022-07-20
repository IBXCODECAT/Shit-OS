# ====== Check Script is Running as Root ======

if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi

# ====== Install Required Packages ======

apt install build-essential
apt install bison
apt install flex
apt install libgmp3-dev
apt install libmpc-dev
apt isstall libmpfr-dev
apt install texinfo

# ====== Setup binutils and binutils build ======

#Setup system variables
export PREFIX="/usr/local/x86_64elfgcc"
export TARGET=x86_64-elf
export PATH="$PREFIX/bin:$PATH"


# Move into a binutil directory
mkdir /tmp/src
cd /tmp/src

#Download our binutil files
curl -O http://ftp.gnu.org/gnu/binutils/binutils-2.35.1.tar.gz
tar xf binutils-2.35.1.tar.gz

#Create and setup our binutils directory
mkdir binutils-build
cd binutils-build
../binutils-2.35.1/configure --target=$TARGET --enable-interwork --enable-multilib --disable-nls --disable-werror --prefix=$PREFIX 2>&1 | tee configure.log
make all install 2>&1 | tee make.log

# ====== Start building our gcc ======

# Move back to our src directory
cd /tmp/src

#Download the GCC files
curl -O https://ftp.gnu.org/gnu/gcc/gcc-10.2.0/gcc-10.2.0.tar.gz
tar xf gcc-10.2.0.tar.gz

#Create and setup our GCC Directory
mkdir gcc-build
cd gcc-build
../gcc-10.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --disable-libssp --enable-language=c++ --without-headers
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc

# ====== Check if our binaries have been created ======
ls /usr/local/x86_64elfgcc/bin
