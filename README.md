# OldWyk

# Wiring
5 wires of the OldWyk SPI interface have to be connected to the Hifive1: GND, SCLK, MISO,
MOSI, and CS. The picture below shows where these wires have to be connected to
the Hifive1. The pinout of the SPI slave of the OldWyk PCB can be found on the
backside of the PCB.
<p align="center">
<img src="https://raw.githubusercontent.com/hni-sct/pulpino-sdk/master/doc/wiring.svg">
</p>

# Testing if OldWyk is alive
Connect to the micropython shell on the HiFive 1:
```
$ screen /dev/ttyUSB1 115200
```

If you hit ENTER you should see a python shell:
```
>>>
```

Now import pyb and create a PULPINO object:
```
>>> import pyb
>>> pulpino = pyb.PULPINO()
```

## SPI Slave test
To check if it is working, reset pulpino:
```
>>> pulpino.reset()
```
This will test if the SPI slave of OldWyk is working. If something is wrong you
will see:
```
Error can't write reg1 of SPI-Slave>>>
```

## LED Blink test
One LED is connected to GPIO 10. To make it blink we first set the GPIO to
output (0 means input, 1 means output):
```
>>> pulpino.gpio_dir(10, 1)
```
Now we can turn on the LED:
```
>>> pulpino.gpio_set(10, 1)
```
You should now see LED `D2` turn on.

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
