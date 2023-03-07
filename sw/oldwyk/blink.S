.text
    li x31, 0x1A101000 # gpio_dir
    li x30, 0x1A101008 # gpio_out
    li x29, (1 << 10) # bitmask gpio 10
    sw x29, 0(x31) # set gpio 10 as output

toggle_loop:
    # enable LED via gpio 10
    sw x29, 0(x30) 

    # wait ~10000000 cycles
    li x1, 10000000
wait_loop:
    addi x1, x1, -1
    bne x1, x0, wait_loop

    # disable LED via gpio 10
    sw x0, 0(x30) 

    # wait ~10000000 cycles
    li x1, 10000000
wait_loop2:
    addi x1, x1, -1
    bne x1, x0, wait_loop2

    # loop to top
    j toggle_loop
