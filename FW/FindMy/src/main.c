#include "CONFIG.h"
#include "HAL.h"
#include "SLEEP.h"
#include "broadcaster.h"

uint8_t public_key[] = {
    0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xef,
    0xfe,0xdd,0xcc,0xbb,0xaa,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11
};

__attribute__((aligned(4))) uint32_t MEM_BUF[BLE_MEMHEAP_SIZE / 4];

#if(defined(BLE_MAC)) && (BLE_MAC == TRUE)
uint8_t MacAddr[6] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
#endif

__HIGH_CODE
__attribute__((noinline))
void Main_Circulation()
{
    while(1)
    {
        TMOS_SystemProcess();
    }
}

int main(void)
{
#if(defined(DCDC_ENABLE)) && (DCDC_ENABLE == TRUE)
    PWR_DCDCCfg(ENABLE);
#endif
    SetSysClock(CLK_SOURCE_PLL_60MHz);
#if(defined(HAL_SLEEP)) && (HAL_SLEEP == TRUE)
    GPIOA_ModeCfg(GPIO_Pin_All, GPIO_ModeIN_PU);
    GPIOB_ModeCfg(GPIO_Pin_All, GPIO_ModeIN_PU);
#endif
#ifdef DEBUG
    GPIOA_SetBits(bTXD1);
    GPIOA_ModeCfg(bTXD1, GPIO_ModeOut_PP_5mA);
    UART1_DefInit();
#endif

    // GAP - Advertisement data (max size = 31 bytes, though this is
    // best kept short to conserve power while advertisting)
    uint8_t advertData[] = {
        0x1e, /* Length (30) */
        0xff, /* Manufacturer Specific Data (type 0xff) */
        0x4c, 0x00, /* Company ID (Apple) */
        0x12, 0x19, /* Offline Finding type and length */
        0x00, /* State */
        0x11, 0x22, 0x33, 0x22, 0x11, 0x22, 0x33, 0x22,
        0x11, 0x22, 0x33, 0x22, 0x11, 0x22, 0x33, 0x22,
        0x11, 0x22, 0x33, 0x22, 0x11, 0x22,
        0x00, /* First two bits */
        0x00, /* Hint (0x00) */
    };
    memcpy(&advertData[7], &public_key[6], 22);
    advertData[29] = public_key[0] >> 6;

    MacAddr[0] = public_key[0] | 0xc0,
    MacAddr[1] = public_key[1],
    MacAddr[2] = public_key[2],
    MacAddr[3] = public_key[3],
    MacAddr[4] = public_key[4],
    MacAddr[5] = public_key[5],

    CH59x_BLEInit();
    HAL_Init();
    GAPRole_BroadcasterInit();
    Broadcaster_Init(advertData, sizeof(advertData));
    Main_Circulation();
}

/******************************** endfile @ main ******************************/
