set(leddemo_SOURCES demo.c gpio_spi.c bmi160.c led-matrix.c)

add_application(led-bmi160-demo "${leddemo_SOURCES}")
