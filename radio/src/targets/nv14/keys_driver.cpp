/*
 * Copyright (C) OpenTX
 *
 * Based on code named
 *   th9x - http://code.google.com/p/th9x
 *   er9x - http://code.google.com/p/er9x
 *   gruvin9x - http://code.google.com/p/gruvin9x
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "opentx.h"

uint32_t readKeys()
{
  return 0;
}

uint32_t readTrims()
{
  uint32_t result = 0;

  if (TRIMS_GPIO_REG_LHL & TRIMS_GPIO_PIN_LHL)
    result |= 0x01;
  if (TRIMS_GPIO_REG_LHR & TRIMS_GPIO_PIN_LHR)
    result |= 0x02;
  if (TRIMS_GPIO_REG_LVD & TRIMS_GPIO_PIN_LVD)
    result |= 0x04;
  if (TRIMS_GPIO_REG_LVU & TRIMS_GPIO_PIN_LVU)
    result |= 0x08;
  if (TRIMS_GPIO_REG_RVD & TRIMS_GPIO_PIN_RVD)
    result |= 0x10;
  if (TRIMS_GPIO_REG_RVU & TRIMS_GPIO_PIN_RVU)
    result |= 0x20;
  if (TRIMS_GPIO_REG_RHL & TRIMS_GPIO_PIN_RHL)
    result |= 0x40;
  if (TRIMS_GPIO_REG_RHR & TRIMS_GPIO_PIN_RHR)
    result |= 0x80;

  // TRACE("readTrims(): result=0x%02x", result);

  return result;
}

uint8_t trimDown(uint8_t idx)
{
  return readTrims() & (1 << idx);
}

uint8_t keyDown()
{
  return 0;
}

/* TODO common to ARM */
void readKeysAndTrims()
{
  int i;

  uint8_t index = 0;
  uint32_t in = readKeys();
  for (i = 0; i < TRM_BASE; i++) {
    keys[index++].input(in & (1 << i));
  }

  in = readTrims();
  for (i = 1; i <= 1 << (TRM_LAST-TRM_BASE); i <<= 1) {
    keys[index++].input(in & i);
  }
}

#if !defined(BOOT)
uint32_t switchState(uint8_t index)
{
  uint16_t value = getAnalogValue(SWITCH_FIRST + index / 3);
  uint8_t position;

  if (value < 1024)
    position = 0;
  else if (value > 3 * 1024)
    position = 2;
  else
    position = 1;

  return position == (index % 3);
}
#endif

void monitorInit()
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

  GPIO_InitStructure.GPIO_Pin = VBUS_MONITOR_PIN;
  GPIO_Init(GPIOJ, &GPIO_InitStructure);
}

void keysInit()
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

  GPIO_InitStructure.GPIO_Pin = KEYS_GPIOB_PINS;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = KEYS_GPIOC_PINS;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = KEYS_GPIOD_PINS;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = KEYS_GPIOG_PINS;
  GPIO_Init(GPIOG, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = KEYS_GPIOH_PINS;
  GPIO_Init(GPIOH, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = KEYS_GPIOJ_PINS;
  GPIO_Init(GPIOJ, &GPIO_InitStructure);
}