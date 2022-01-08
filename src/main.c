/*!
 * @file
 * @brief
 */

#include "board/metro_m0.h"
#include "tiny_timer.h"

// fixme
#include "dma.h"

static tiny_timer_group_t timer_group;

int main(void)
{
  interrupts_disable();
  {
    clock_init();
    tiny_timer_group_init(&timer_group, systick_init());
    watchdog_init(&timer_group);
    heartbeat_init(&timer_group);
  }
  interrupts_enable();

  // fixme
  const uint8_t src[] = { 0xA5, 0x5A, 0x12, 0x34, 1, 2, 3, 4 };
  uint8_t dst[sizeof(src)];
  dma_init();
  uint8_t channel = dma_claim_channel();
  DmacDescriptor* d = dma_channel_descriptor(channel);
  d->BTCTRL.bit.STEPSIZE = DMAC_BTCTRL_STEPSIZE_X1_Val;
  d->BTCTRL.bit.STEPSEL = DMAC_BTCTRL_STEPSEL_SRC_Val;
  d->BTCTRL.bit.DSTINC = 1;
  d->BTCTRL.bit.SRCINC = 1;
  d->BTCTRL.bit.BEATSIZE = DMAC_BTCTRL_BEATSIZE_BYTE_Val;
  d->BTCTRL.bit.BLOCKACT = DMAC_BTCTRL_BLOCKACT_NOACT_Val;
  d->BTCTRL.bit.VALID = 1;
  d->BTCNT.bit.BTCNT = sizeof(src);
  d->SRCADDR.bit.SRCADDR = (uintptr_t)(src + sizeof(src));
  d->DSTADDR.bit.DSTADDR = (uintptr_t)(dst + sizeof(dst));
  dma_enable_channel(
    channel,
    DMAC_CHCTRLB_TRIGACT_BLOCK_Val,
    DMAC_CHCTRLB_TRIGSRC_DISABLE_Val,
    DMAC_CHCTRLB_LVL_LVL0_Val);
  dma_trigger_channel(channel);

  while(1) {
    if(!tiny_timer_group_run(&timer_group)) {
      interrupts_wait_for_interrupt();
    }
  }
}
