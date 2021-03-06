/*
 * Arm SCP/MCP Software
 * Copyright (c) 2015-2020, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <fwk_id.h>
#include <fwk_macros.h>
#include <fwk_module.h>
#include <fwk_module_idx.h>
#include <juno_irq.h>
#include <juno_power_domain.h>
#include <juno_ppu_idx.h>
#include <mod_juno_ppu.h>
#include <mod_system_power.h>

/*
 * These PPUs in this list are there as a temporary workaround, until
 * Linux supports handling these domains on its own.
 */
static const struct mod_system_power_ext_ppu_config ext_ppus[] = {
    {
        .ppu_id = FWK_ID_ELEMENT_INIT(FWK_MODULE_IDX_JUNO_PPU,
            JUNO_PPU_DEV_IDX_GPUTOP),
        .api_id = FWK_ID_API_INIT(FWK_MODULE_IDX_JUNO_PPU,
            MOD_JUNO_PPU_API_IDX_PD),
    },
};

static const uint8_t system_power_to_sys_ppu_state[] = {
    [MOD_PD_STATE_ON]                     = (uint8_t)MOD_PD_STATE_ON,
    [MOD_SYSTEM_POWER_POWER_STATE_SLEEP0] =
        (uint8_t)JUNO_POWER_DOMAIN_CSS_SLEEP0,
    [MOD_PD_STATE_OFF]                    = (uint8_t)MOD_PD_STATE_OFF,
};

static const struct fwk_element system_power_element_table[] = {
    [0] = {
        .name = "SYS-PPU-0",
        .data = &((struct mod_system_power_dev_config) {
            .sys_ppu_id = FWK_ID_ELEMENT_INIT(FWK_MODULE_IDX_JUNO_PPU,
                JUNO_PPU_DEV_IDX_SYSTOP),
            .api_id = FWK_ID_API_INIT(FWK_MODULE_IDX_JUNO_PPU,
                MOD_JUNO_PPU_API_IDX_PD),
            .sys_state_table = system_power_to_sys_ppu_state,
        }),
    },

    [1] = { 0 }, /* Termination description */
};

static const struct fwk_element *system_power_get_element_table(
    fwk_id_t module_id)
{
    return system_power_element_table;
}

const struct fwk_module_config config_system_power = {
    .data = &((struct mod_system_power_config) {
        .soc_wakeup_irq = EXT_WAKEUP_IRQ,
        .ext_ppus = ext_ppus,
        .ext_ppus_count = FWK_ARRAY_SIZE(ext_ppus),
        .driver_id = FWK_ID_MODULE_INIT(FWK_MODULE_IDX_JUNO_SYSTEM),
        .driver_api_id = FWK_ID_API_INIT(FWK_MODULE_IDX_JUNO_SYSTEM, 0),
        .initial_system_power_state = MOD_PD_STATE_ON,
    }),
    .get_element_table = system_power_get_element_table,
};
