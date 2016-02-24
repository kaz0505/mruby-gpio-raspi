#include <unistd.h>

int powInt(int, int);

int i2cSetup(int devId)
{
	int fd;
	char *device;

	device = "/dev/i2c-1";

	if((fd = open(device, O_RDWR)) < 0){
		printf("error open device\n");
		return -1;
	}

	if(ioctl(fd, I2C_SLAVE, devId) < 0){
		printf("error ioCtl\n");
		return -1;
	}

	return fd;
}

int i2cWrite(int fd, unsigned char reg, unsigned char data)
{
	unsigned char buf[2];

	if(ioctl(fd, I2C_SLAVE, 0x40) < 0)
		return -1;

	buf[0] = reg;
	buf[1] = data;

	if((write(fd, buf, 2)) < 0)
		return -1;

	return 0;
}

unsigned char i2cRead(int fd, unsigned char reg)
{
	unsigned char buf[2];

	ioctl(fd, I2C_SLAVE, 0x40);

	buf[0] = reg;

	if(write(fd, buf, 1) < 0)
		return -1;

	read(fd, buf, 1);

	return buf[0];
}

int stdRead(int fd, int bsize)
{
	unsigned char buf[bsize];
	int ret,data,i;
	data = 0;

	ioctl(fd, I2C_SLAVE, 0x40);

	ret = read(fd, buf, bsize);
	if(ret != bsize){
		printf("Read only %d byte\n", ret);
		return -1;
	}

	for(i=0 ; i < bsize ; i++)
		data += buf[bsize-1-i] * powInt(16,i);

	return data;
}

int powInt(int x, int y)
{
	int i;
	for(i=0 ; i<y ; i++)
		x *= x;
	return x;
}
