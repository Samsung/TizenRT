/****************************************************************************
 *
 * Copyright 2023 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
#include <tinyara/config.h>
#include <tinyara/gpio.h>
#include <debug.h>
#include "stm32_gpio.h"
#include "nucleo-f446re.h"

struct stm32_gpio_s {
    struct gpio_lowerhalf_s lower;
    uint32_t pinset;
};

static int stm32_gpio_get(FAR struct gpio_lowerhalf_s *lower) {
    struct stm32_gpio_s *priv = (struct stm32_gpio_s *)lower;
    return stm32_gpioread(priv->pinset);
}

static void stm32_gpio_set(FAR struct gpio_lowerhalf_s *lower, unsigned int value) {
    struct stm32_gpio_s *priv = (struct stm32_gpio_s *)lower;
    stm32_gpiowrite(priv->pinset, value);
}

static int stm32_gpio_pull(FAR struct gpio_lowerhalf_s *lower, unsigned long arg) { return 0; }
static int stm32_gpio_setdir(FAR struct gpio_lowerhalf_s *lower, unsigned long arg) { return 0; }
static int stm32_gpio_enable(FAR struct gpio_lowerhalf_s *lower, int falling, int rising, gpio_handler_t handler) { return 0; }
static int stm32_gpio_ioctl(FAR struct gpio_lowerhalf_s *lower, int cmd, unsigned long args) { return 0; }

static const struct gpio_ops_s g_stm32_gpio_ops = {
    .get = stm32_gpio_get,
    .set = stm32_gpio_set,
    .pull = stm32_gpio_pull,
    .setdir = stm32_gpio_setdir,
    .enable = stm32_gpio_enable,
    .ioctl = stm32_gpio_ioctl,
};

static struct stm32_gpio_s g_led1 = {
    .lower = { .ops = &g_stm32_gpio_ops },
    .pinset = GPIO_LED1
};

void nucleo_gpio_initialize(void) {
    stm32_configgpio(GPIO_LED1);
    gpio_register(5, &g_led1.lower);
}
