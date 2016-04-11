#ifndef __APP_PREINCLUDE_H__
#define __APP_PREINCLUDE_H__

/* 
 * KSDK configuration 
 */
#define CPU_MKW40Z160VHT4               1
#define FSL_RTOS_FREE_RTOS              1
#define gTotalHeapSize_c                8000


/* 
 * 802.15.4 PHY configuration 
 */
#define gPhyDefaultTxPowerLevel_d       5


/* 
 * 802.15.4 MAC configuration 
 */
#define gMacFeatureSet_d gMacFeatureSet_06M0_d


/* 
 * Connectivity Framework configuration 
 */
#define PRODUCTION_DRV                  1
#define gSerialMgrRxBufSize_c           256
#define gKBD_KeysCount_c                2
#define gLEDsOnTargetBoardCnt_c         4
#define gEepromType_d gEepromDevice_AT45DB021E_c


/* 
 * Application configuration 
 */
#define mDeviceInfo "Kinetis_MKW40 End Device"

#endif /* __APP_PREINCLUDE_H__ */
