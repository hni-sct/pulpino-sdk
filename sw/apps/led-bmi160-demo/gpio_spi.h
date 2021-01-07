uint8_t spi_transfer_byte(uint8_t clk, uint8_t miso, uint8_t mosi, uint8_t data);
int8_t spi_write(uint8_t clk, uint8_t miso, uint8_t mosi, uint8_t cs, uint8_t reg_addr, uint8_t *data, uint16_t len);
int8_t spi_read(uint8_t clk, uint8_t miso, uint8_t mosi, uint8_t cs, uint8_t reg_addr, uint8_t *data, uint16_t len);
void spi_begin(uint8_t clk, uint8_t miso, uint8_t mosi, uint8_t cs, uint32_t max_slave_freq);
