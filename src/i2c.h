int i2cSetup(int devId)
{
    int rev, fd ;
    char *device ;

    device = "/dev/i2c-1";

    if ((fd = open (device, O_RDWR)) < 0)
        return -1 ;

    if (ioctl (fd, I2C_SLAVE, devId) < 0)
        return -1 ;

    return fd ;
}

int i2cWrite(int fd, int data)
{
    return i2c_smbus_access (fd, I2C_SMBUS_WRITE, data, I2C_SMBUS_BYTE, NULL) ;
}

int i2cWriteReg8 (int fd, int reg, int data)
{
    return i2c_smbus_write_byte_data (fd, reg, data) ;
}

int i2cWriteReg16 (int fd, int reg, int data)
{
    return i2c_smbus_write_word_data (fd, reg, data) ;
}

int i2cRead(int fd)
{
    return i2c_smbus_read_byte (fd) ;
}

int i2cReadReg8 (int fd, int reg)
{
    return i2c_smbus_read_byte_data (fd, reg) ;
}

int i2cReadReg16(int fd, int reg)
{
    return i2c_smbus_read_word_data (fd, reg) ;
}
