#include <stdlib.h>
#include <string.h>
#include "mruby.h"
#include "mruby/value.h"
#include "./set_rpin.h"
//register
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "./register.h"
//i2c
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include"./i2c.h"

static mrb_value
f_gpio_pinmode(mrb_state *mrb, mrb_value self)
{
  gpio_init();
  mrb_int pin, dir;
  mrb_get_args(mrb, "ii", &pin, &dir);
  pin=set_rpin(pin);
  
  if( dir == 1 ){  // output
    gpio_configure (pin, OUTPUT);
  } else {         // input
    gpio_configure (pin, INPUT);
  }

  return mrb_nil_value();
}

static mrb_value
f_gpio_dwrite(mrb_state *mrb, mrb_value self)
{
  mrb_int pin, value;
  mrb_get_args(mrb, "ii", &pin, &value);
  pin=set_rpin(pin);
  
  if(value==1){ // high
    gpio_set(pin);
  } else { // low
    gpio_clear(pin);
  }
	
  return mrb_nil_value();
}

static mrb_value
f_gpio_dread(mrb_state *mrb, mrb_value self)
{
  mrb_int pin, value;
  char buf[100], chval[10];
  FILE *fval;
  mrb_get_args(mrb, "i", &pin);
  pin=set_rpin(pin);

  sprintf(buf,  "cat /sys/class/gpio/gpio%d/value", pin);
  fval=popen(buf, "r");
  fgets(chval,sizeof(chval),fval);
  pclose(fval);

  if(strcmp(chval,"0\n")==0){
    value=0;
  }else if(strcmp(chval,"1\n")==0){
    value=1;
  }else{ //error
  }

  return mrb_fixnum_value(value);
}

static mrb_value
f_gpio_awrite(mrb_state *mrb, mrb_value self)
{
  // not implemented...
  return mrb_nil_value();
}

static mrb_value
f_gpio_isetup(mrb_state *mrb, mrb_value self)
{
  mrb_int devId, ret;
  mrb_get_args(mrb, "i", &devId);
  ret = i2cSetup(devId);

  return mrb_fixnum_value(ret);
}

static mrb_value
f_gpio_iwrite(mrb_state *mrb, mrb_value self)
{
  mrb_int fd, data, ret;
  mrb_get_args(mrb, "ii", &fd, &data);
  ret = i2cWrite(fd, data);
	
  return mrb_fixnum_value(ret);
}

static mrb_value
f_gpio_iwrite8(mrb_state *mrb, mrb_value self)
{
  mrb_int fd, reg, data, ret;
  mrb_get_args(mrb, "iii", &fd, &reg, &data);
  ret = i2cWriteReg8(fd, reg, data);

  return mrb_fixnum_value(ret);
}

static mrb_value
f_gpio_iwrite16(mrb_state *mrb, mrb_value self)
{
  mrb_int fd, reg, data, ret;
  mrb_get_args(mrb, "iii", &fd, &reg, &data);
  ret = i2cWriteReg16(fd, reg, data);

  return mrb_fixnum_value(ret);
}

static mrb_value
f_gpio_iread(mrb_state *mrb, mrb_value self)
{
  mrb_int fd, ret;
  mrb_get_args(mrb, "i", &fd);
  ret = i2cRead(fd);
	
  return mrb_fixnum_value(ret);
}

static mrb_value
f_gpio_iread8(mrb_state *mrb, mrb_value self)
{
  mrb_int fd, reg, ret;
  mrb_get_args(mrb, "ii", &fd, &reg);
  ret = i2cReadReg8(fd, reg);

  return mrb_fixnum_value(ret);
}

static mrb_value
f_gpio_iread16(mrb_state *mrb, mrb_value self)
{
  mrb_int fd, reg, ret;
  mrb_get_args(mrb, "ii", &fd, &reg);
  ret = i2cReadReg16(fd, reg);

  return mrb_fixnum_value(ret);
}

static mrb_value
f_gpio_ireadbyte(mrb_state *mrb, mrb_value self)
{
  mrb_int fd, byte, ret;
  mrb_get_args(mrb, "ii", &fd, &byte);
  ret = i2cReadByte(fd, byte);

  return mrb_fixnum_value(ret);
}

void
mrb_mruby_gpio_raspi_gem_init(mrb_state* mrb)
{
  struct RClass *c;
  c = mrb_define_module(mrb, "Gpio");

  mrb_define_const(mrb, c, "OUTPUT", mrb_fixnum_value(1));
  mrb_define_const(mrb, c, "INPUT", mrb_fixnum_value(0));
  mrb_define_const(mrb, c, "HIGH", mrb_fixnum_value(1));
  mrb_define_const(mrb, c, "LOW", mrb_fixnum_value(0));

  mrb_define_module_function(mrb, c, "pinMode", f_gpio_pinmode, MRB_ARGS_REQ(2));
  mrb_define_module_function(mrb, c, "digitalWrite", f_gpio_dwrite, MRB_ARGS_REQ(2));
  mrb_define_module_function(mrb, c, "digitalRead", f_gpio_dread, MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, c, "analogWrite", f_gpio_awrite, MRB_ARGS_REQ(2));

  //i2c
  mrb_define_module_function(mrb, c, "i2cSetup", f_gpio_isetup, MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, c, "i2cWrite", f_gpio_iwrite, MRB_ARGS_REQ(2));
  mrb_define_module_function(mrb, c, "i2cWriteReg8", f_gpio_iwrite8, MRB_ARGS_REQ(3));
  mrb_define_module_function(mrb, c, "i2cWriteReg16", f_gpio_iwrite16, MRB_ARGS_REQ(3));
  mrb_define_module_function(mrb, c, "i2cRead", f_gpio_iread, MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, c, "i2cReadReg8", f_gpio_iread8, MRB_ARGS_REQ(2));
  mrb_define_module_function(mrb, c, "i2cReadReg16", f_gpio_iread16, MRB_ARGS_REQ(2));
  mrb_define_module_function(mrb, c, "i2cReadByte", f_gpio_ireadbyte, MRB_ARGS_REQ(2));
}

void
mrb_mruby_gpio_raspi_gem_final(mrb_state* mrb)
{
}

