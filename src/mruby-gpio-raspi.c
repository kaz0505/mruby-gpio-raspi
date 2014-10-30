#include <stdlib.h>
#include <string.h>
#include "mruby.h"
#include "mruby/value.h"

static mrb_value
f_gpio_pinmode(mrb_state *mrb, mrb_value self)
{
  char buf[100];
  mrb_int pin, dir;

  mrb_get_args(mrb, "ii", &pin, &dir);
  if( dir == 1 ){  // output
    sprintf(buf, "echo %d > /sys/class/gpio/export", pin);
    system(buf);
    sprintf(buf, "echo out > /sys/class/gpio/gpio%d/direction", pin);
    system(buf);
  } else {         // input
    sprintf(buf, "echo %d > /sys/class/gpio/export", pin);
    system(buf);
    sprintf(buf, "echo in > /sys/class/gpio/gpio%d/direction", pin);
    system(buf);
  }

  return mrb_nil_value();
}

static mrb_value
f_gpio_dwrite(mrb_state *mrb, mrb_value self)
{
  char buf[100];
  mrb_int pin, value;
  mrb_get_args(mrb, "ii", &pin, &value);

  sprintf(buf, "echo %d > /sys/class/gpio/gpio%d/value", value, pin);
  system(buf);

  return mrb_nil_value();
}

static mrb_value
f_gpio_dread(mrb_state *mrb, mrb_value self)
{
  mrb_int pin, value;
  char buf[100], chval[10];
  FILE *fval;
  mrb_get_args(mrb, "i", &pin);

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
}

void
mrb_mruby_gpio_raspi_gem_final(mrb_state* mrb)
{
}
