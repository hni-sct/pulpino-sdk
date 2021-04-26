# Building toolchain
## Prerequisites

### Ubuntu
```
$ sudo apt-get install autoconf automake autotools-dev curl python3 libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev libexpat-dev cmake
```

### Redhat
```
$ sudo yum install autoconf automake python3 libmpc-devel mpfr-devel gmp-devel gawk  bison flex texinfo patchutils gcc gcc-c++ zlib-devel expat-devel cmake
```

### Archlinux
```
$ pacman -Syyu autoconf automake curl python3 mpc mpfr gmp gawk base-devel bison flex texinfo gperf libtool patchutils bc zlib expat cmake
```

## Installation
Running
```
$ make tools
```
will install the toolchain to `prefix/bin`

# Building pulpino software
All software projects are in the `sw/apps` folder. If you want to build the software in `sw/apps/helloworld` then run 
```
$ PROGRAM=helloworld make software
```

# Uploading the sofware to the Arty 35 FPGA running [pulpino](https://github.com/hni-sct/arty_pulpino)

If you want to upload the project `helloworld` then run
```
$ PROGRAM=helloworld make upload
```
