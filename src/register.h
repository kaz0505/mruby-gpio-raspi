//  register address
#define PERI_BASE     0x20000000
#define GPIO_BASE     (PERI_BASE + 0x200000)
#define BLOCK_SIZE    4096

static volatile unsigned int *Gpio = NULL;
void gpio_init ()
{
    int fd;
    void *gpio_map;
    fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd == -1) {
        printf("error: cannot open /dev/mem (gpio_setup)\n");
        exit(-1);
    }
    gpio_map = mmap(NULL, BLOCK_SIZE,
                    PROT_READ | PROT_WRITE, MAP_SHARED,
                    fd, GPIO_BASE );
    if ((int) gpio_map == -1) {
        printf("error: cannot map /dev/mem on the memory (gpio_setup)\n");
        exit(-1);
    }
    close(fd);
    Gpio = (unsigned int *) gpio_map;
}

// pinmode
#define INPUT    0x0
#define OUTPUT   0x1
#define ALT0     0x4
#define ALT1     0x5
#define ALT2     0x6
#define ALT3     0x7
#define ALT4     0x3
#define ALT5     0x2

void gpio_configure (int pin, int mode)
{
    int index = pin / 10;
    unsigned int mask = ~(0x7 << ((pin % 10) * 3));
    Gpio[index] = (Gpio[index] & mask) | ((mode & 0x7) << ((pin % 10) * 3));
}

void gpio_set (int pin)
{
    Gpio[7] = 0x1 << pin;
}
void gpio_clear (int pin)
{
    Gpio[10] = 0x1 << pin;
}