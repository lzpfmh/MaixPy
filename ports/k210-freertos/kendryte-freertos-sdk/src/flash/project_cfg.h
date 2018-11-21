/* Copyright 2018 Canaan Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef _PROJECT_CFG_H_
#define _PROJECT_CFG_H_
#include <pin_cfg.h>

const fpioa_cfg_t g_fpioa_cfg = 
{
    .version = PIN_CFG_VERSION,
    .functions_count = 6,
    .functions[0] = {30, FUNC_SPI0_SS0},
    .functions[1] = {32, FUNC_SPI0_SCLK},
    .functions[2] = {34, FUNC_SPI0_D0},
    .functions[3] = {43, FUNC_SPI0_D1},
    .functions[4] = {42, FUNC_SPI0_D2},
    .functions[5] = {41, FUNC_SPI0_D3}
};

#endif
