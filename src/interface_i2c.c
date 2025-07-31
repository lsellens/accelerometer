#include "interface_i2c.h"
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

static int i2c_fd = -1;
const uint8_t I2C_ADDR = 0x53;

int i2c_open(const char *device, uint8_t addr) {
  i2c_fd = open(device, O_RDWR);
  if (i2c_fd < 0)
    return -1;
  if (ioctl(i2c_fd, I2C_SLAVE, addr) < 0)
    return -1;
  return 0;
}

void i2c_close(void) {
  if (i2c_fd >= 0) {
    close(i2c_fd);
    i2c_fd = -1;
  }
}

int i2c_transfer(uint8_t *tx, int tx_len, uint8_t *rx, int rx_len) {
  if (tx_len > 0) {
    if (write(i2c_fd, tx, tx_len) != tx_len)
      return -1;
  }
  if (rx_len > 0) {
    if (read(i2c_fd, rx, rx_len) != rx_len)
      return -1;
  }
  return tx_len + rx_len;
}
