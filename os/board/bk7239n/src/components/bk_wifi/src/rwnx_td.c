
/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <string.h>
#include "common/bk_typedef.h"
#include "fhost_msg.h"
#include "rwnx_rx.h"
#include "rwnx_td.h"
#include "rwnx_defs.h"

#if CONFIG_RWNX_TD
/*
 * VARIABLES
 ****************************************************************************************
 */

struct rwnx_td_env_tag rwnx_td_env_tab[NX_VIRT_DEV_MAX];

/*
 * PUBLIC FUNCTIONS
 ****************************************************************************************
 */

void rwnx_td_init(void)
{
    uint8_t counter;

    // Reset all rwnx td environments
    for (counter = 0; counter < NX_VIRT_DEV_MAX; counter++)
    {
        rwnx_td_reset(counter);
    }
}

void rwnx_td_reset(uint8_t vif_index)
{
    struct rwnx_td_env_tag *rwnx_td_env = &rwnx_td_env_tab[vif_index];

    // Initialize memory
    memset(rwnx_td_env, 0, sizeof(struct rwnx_td_env_tag));
}

void rwnx_td_pkt_ind(uint8_t vif_index,uint8_t access_category)
{
    struct rwnx_td_env_tag *rwnx_td_env = &rwnx_td_env_tab[vif_index];

    GLOBAL_INT_DECLARATION();

    GLOBAL_INT_DISABLE();
    rwnx_td_env->pkt_cnt++;
    rwnx_td_env->ac_pkt_cnt[access_category]++;
    GLOBAL_INT_RESTORE();

    #if 0
    RWNX_LOGI("ind vif %x ac %x cnt %d ac cnt %d\r\n",vif_index,access_category,
             rwnx_td_env->pkt_cnt,
             rwnx_td_env->ac_pkt_cnt[access_category]);
    #endif

}

void rwnx_td_pkt_dec(uint8_t vif_index,uint8_t access_category)
{
    struct rwnx_td_env_tag *rwnx_td_env = &rwnx_td_env_tab[vif_index];

    GLOBAL_INT_DECLARATION();
    if ((rwnx_td_env->pkt_cnt) == 0)
    {
        RWNX_LOGE("rwnx_td_pkt_dec: pkt_cnt %x\r\n",rwnx_td_env->pkt_cnt);
        BK_ASSERT(0); /* ASSERT VERIFIED */
    }

    GLOBAL_INT_DISABLE();
    rwnx_td_env->pkt_cnt--;
    rwnx_td_env->ac_pkt_cnt[access_category]--;
    GLOBAL_INT_RESTORE();

    #if 0
    RWNX_LOGI("dec vif %x ac %x cnt %d ac cnt %d\r\n",vif_index,access_category,
             rwnx_td_env->pkt_cnt,
             rwnx_td_env->ac_pkt_cnt[access_category]);
    #endif
}

void rwnx_get_td_info(uint8_t vif_index, uint16 *pkt_cnt, bool *vivo)
{
    struct rwnx_td_env_tag *rwnx_td_env = &rwnx_td_env_tab[vif_index];

    GLOBAL_INT_DECLARATION();

    GLOBAL_INT_DISABLE();
    *vivo = (rwnx_td_env->ac_pkt_cnt[AC_VI] + rwnx_td_env->ac_pkt_cnt[AC_VO]) ? true : false;
    *pkt_cnt = rwnx_td_env->pkt_cnt;
    GLOBAL_INT_RESTORE();
}
#else
void rwnx_td_pkt_dec(uint8_t vif_index,uint8_t access_category)
{
}

void rwnx_get_td_info(uint8_t vif_index, uint16 *pkt_cnt, bool *vivo)
{
}
#endif
