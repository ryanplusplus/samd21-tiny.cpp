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
  static const uint8_t src[] = { 0xA5, 0x5A, 0x12, 0x34, 1, 2, 3, 4 };
  static uint8_t dst[sizeof(src)];
  dma_init();
  dma_claim_channel(0);
  DmacDescriptor* d = dma_channel_descriptor(0);
  d->BTCTRL.bit.STEPSIZE = 0;
  d->BTCTRL.bit.STEPSEL = 1;
  d->BTCTRL.bit.DSTINC = 1;
  d->BTCTRL.bit.SRCINC = 1;
  d->BTCTRL.bit.BEATSIZE = 1;
  d->BTCTRL.bit.BLOCKACT = 0;
  d->BTCTRL.bit.VALID = 1;
  d->BTCNT.bit.BTCNT = sizeof(src);
  d->SRCADDR.bit.SRCADDR = (uintptr_t)src;
  d->DSTADDR.bit.DSTADDR = (uintptr_t)dst;
  dma_enable_channel(0, DMAC_CHCTRLB_TRIGACT_BLOCK_Val, 0, 0);
  dma_trigger_channel(0);

  while(1) {
    if(!tiny_timer_group_run(&timer_group)) {
      interrupts_wait_for_interrupt();
    }
  }
}
