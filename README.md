# Toolchain

    wget https://launchpad.net/gcc-arm-embedded/4.9/4.9-2015-q3-update/gcc-arm-none-eabi-4_9-2015q3-20150921-linux.tar.bz2 
    tar -xvf gcc-arm-none-eabi-4_9-2015q3-20150921-linux.tar.bz2 

# U-Boot

## Loading Firmware SD Card
    fatload mmc 0:1 ${loadaddr} Control.elf
    bootaux ${loadaddr}

## Loading Firmware TFTP[TFTP] Network
    tftpboot ${loadaddr} Control.elf
    bootaux ${loadaddr}

## Usefull
ubi write ${loadaddr} m4firmware ${filesize}

### Clocks
    setenv defargs clk_ignore_unused

### Disable UART-B
    setenv fdt_fixup 'fdt addr ${fdt_addr_r} && fdt rm /soc/aips-bus@30800000/spba-bus@30800000/serial@30890000'

# Linux Userspace

    modprobe imx_rpmsg_tty

