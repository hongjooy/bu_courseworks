/*!
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* All rights reserved.
*
* \file App.c
* MyStarNetwork Demo End Device application demonstrates the data transfer between 
* end device and PAN coordinator.
*
* In 802.15.4 most communications are driven by the devices in a network. They
* are typically battery powered and need to be able to control the data flow,
* in order to optimize battery life. This is done by polling for data from
* the coordinator, and transmitting data directly to the coordinator. The
* coordinator only sends data to a device when it knows it is listening, i.e. 
* when the device has requested data.
*
* This example will demonstrate indirect data transfer from the coordinator to
* the device. Data sent to the device is not transmitted immediately but put
* into a queue in the coordinators MAC. Here it resides until the device sends
* a poll request. The poll request is initiated by the network or application
* layer on the device.
*
* To test the data transfer from the coordinator to the device, both should be
* connected to a PC with an RS232 terminal at 115200bps, 8N1. When sending an
* ASCII file (send as text) from the coordinators terminal, the file will be 
* printed to the terminal connected to the device. It resembles a bidirectional
* wireless RS232 cable replacement (though, without error checking and flow 
* control in this simple example).
*
* The steps required for receiving a data packet from the coordinator are:
* 1) We should have an association to the coordinator we want to receive from.
* 2) Allocate an MLME-Poll Request message and fill in the coordinator
*    information gained during Active Scan. Send the message to the MLME.
* 3) Shut down the tranceiver and the MCU for a certain period of time.
* 4) Wake-up and wait for MCPS-Data Indication message. It may contain the data
* from the coordinator.
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*
* o Redistributions of source code must retain the above copyright notice, this list
*   of conditions and the following disclaimer.
*
* o Redistributions in binary form must reproduce the above copyright notice, this
*   list of conditions and the following disclaimer in the documentation and/or
*   other materials provided with the distribution.
*
* o Neither the name of Freescale Semiconductor, Inc. nor the names of its
*   contributors may be used to endorse or promote products derived from this
*   software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include "App.h"

/* Drv */
#include "LED.h"
#include "Keyboard.h"

/* Fwk */
#include "SecLib.h"
#include "RNG_Interface.h"
#include "SerialManager.h"
#include "TimersManager.h"
#include "FunctionLib.h"
#include "fsl_osa_ext.h"

/* 802.15.4 */
#include "PhyInterface.h"
#include "MacInterface.h"

/* KSDK */
#include "board_combined.h"
#include "fsl_os_abstraction.h"

uint8_t flag = 0;


#include <stdio.h>
// SDK Included Files

//#include "fsl_lptmr_driver.h"
#include "fsl_debug_console.h"
#include "fsl_pmc_hal.h"
#include "fsl_adc16_driver.h"
#include "accel.h"



/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/

/* Default size of data payload in MCPS-Data.request.
   The length has no real meaning since MCPS-Data.requests
   are always sent in one-size buffers big enough to hold
   a maximum length data frame. */
#define mDefaultValueOfDataLen_c               20

/* Define the message that should be sent to the coordinator
   when some key is pressed*/
#define mPredefinedMsg_c	                    "Predefined message!\n\r"
#define mPredefinedMsgLen_c	                    22

/* Define the maximum hex value that can be displayed on the LEDs */
#define mDefaultValueOfMaxDisplayVal_c          0x0F

/* Maximum number of outstanding packets */
#define mMaxPendingDataPackets_c                2

/* Define wait interval in miliseconds before another associate retry */
#define mWaitInterval_c                         ( mDefaultValueOfWaitInterval_c * 1000 )

/* Define the interval, in milliseconds, between sensor info data messages sent
 * by the End Device to the Coordinator.
 */
#define mSensorTimeInterval_c                  (15.625)

/* Character that triggers sending of a Device Info Message on the serial terminal interface */
#define gMessageMarkSendDevInfo    0x01

#ifndef mDeviceInfo
#define mDeviceInfo "Kinetis_MKW2x End Device"
#endif

/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/
static uint8_t App_StartScan( macScanType_t scanType );
static uint8_t App_HandleScanActiveConfirm( nwkMessage_t *pMsg );
static uint8_t App_WaitMsg( nwkMessage_t *pMsg, uint8_t msgType );
static uint8_t App_SendAssociateRequest( void );
static uint8_t App_HandleAssociateConfirm( nwkMessage_t *pMsg );
static uint8_t App_HandleMlmeInput( nwkMessage_t *pMsg );
static void    App_HandleMcpsInput( mcpsToNwkMessage_t *pMsgIn );
static void    App_HandleMessage  ( mcpsToNwkMessage_t *pMsgIn );
static void    App_SendPredefinedMessage( void );
static void    App_SendSensorStatus( void );
static void    App_ReceiveData     ( void );
static bool_t  App_Idle            ( void );
static void    App_UpdateLEDs( uint8_t val );
static void    App_CommRxCallBack              ( void* );
static void    App_IntervalTimeoutHandler      ( void* );
static void    App_SensorIntervalTimeoutHandler( void * );
static void    App_PollIntervalTimeoutHandler  ( void * );
static void    App_HandleKeys( key_event_t events );

void App_init( void );
void AppThread (uint32_t argument);
resultType_t MLME_NWK_SapHandler (nwkMessage_t* pMsg, instanceId_t instanceId);
resultType_t MCPS_NWK_SapHandler (mcpsToNwkMessage_t* pMsg, instanceId_t instanceId);
extern void Mac_SetExtendedAddress(uint8_t *pAddr, instanceId_t instanceId);

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/



///////////////////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////////////////

#define LPTMR_INSTANCE                (0)       //LPTMR instance

#define ADC16_INSTANCE                (0)   // ADC instance
#define ADC16_INPUT_CHN               (kAdc16Chn1)  // ADC0_SE1 channel
#define ADC16_BANDGAP_CHN             (kAdc16Chn27) // ADC channel of BANDGAP
#define ADC16_CHN_GROUP               (0)   // ADC group configuration selection

///////////////////////////////////////////////////////////////////////////////
// Prototypes
///////////////////////////////////////////////////////////////////////////////
void ADC16_Measure(void);
void calibrateParams(void);

///////////////////////////////////////////////////////////////////////////////
// Variables
///////////////////////////////////////////////////////////////////////////////

//LPTMR
//volatile uint32_t               lptmrCounter=0;
//volatile uint32_t               lptmrPeriod=1000000/64; //64 samples/sec
//volatile bool                   sampleOn = true;

//ADC / Microphone
adc16_converter_config_t        adcUserConfig_1;   // structure for user config
uint16_t                        adcValue = 0;     // ADC value
uint16_t               micCounter=0;
uint16_t               numMicSamplesTakenCounter=0;
uint16_t               micPeriod=64*5; //one sample set every 5 sec
uint16_t               numMicSamplesPerPeriod=30; //10 samples per set

//Accelerometer
uint16_t               accelerometerCounter=0;
uint16_t               accelerometerPeriod=8;
accel_dev_t accDev;
accel_dev_interface_t accDevice;
accel_sensor_data_t accelData;
accel_i2c_interface_t i2cInterface;
int16_t xData, yData, zData;
uint8_t orientation;

//Pulse
uint16_t               pulseCounter=0;
uint16_t               numPulseSamplesTakenCounter=0;
uint16_t               pulsePeriod=64*60; //one sample set every 60 sec
uint16_t               numPulseSamplesPerPeriod=64*30; //64 samples per set

uint32_t inter_counter = 0;
 /* ADA, FIX THIS!! */
uint32_t gap = 123; 




/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
/* Information about the PAN we are part of */
static panDescriptor_t mCoordInfo;

/* This is the short address assigned by the PAN coordinator
   during association. */
static uint16_t mAddress;

/* Data request packet for sending serial terminal interface input to the coordinator */
static nwkToMcpsMessage_t *mpPacket1 = NULL;
static nwkToMcpsMessage_t *mpPacket2 = NULL;
static nwkToMcpsMessage_t *mpPacket = NULL;
/* The MSDU handle is a unique data packet identifier */
static uint8_t mMsduHandle = 0;

/* Number of pending data packets */
static uint8_t mcPendingPackets = 0;

/* Signals that an MLME-Poll request is pending, and that we should wait
   for the MLME-Poll confirm message before sending the next poll request. 
   At init we allow sending a poll request.
*/
static bool_t mWaitPollConfirm = FALSE;

/* Buffer for a message to be sent to coodinator when user presses a key */
static uint8_t maGeneralMsg[mPredefinedMsgLen_c] = mPredefinedMsg_c;  

/* Flag to stop pooling coordintator for data. This way we ensure that idle
   task gets executed and device can enter low power mode.
*/
static bool_t mStopPolling = FALSE;

/* Application input queues */
static anchor_t mMlmeNwkInputQueue;
static anchor_t mMcpsNwkInputQueue;

static tmrTimerID_t mSoftTimerId_c = gTmrInvalidTimerID_c;
static tmrTimerID_t mSensorTimerId_c = gTmrInvalidTimerID_c;
static tmrTimerID_t mPollIntervalTimerId_c = gTmrInvalidTimerID_c;

static const uint64_t mExtendedAddress  = mMacExtendedAddress_c;
static instanceId_t   mMacInstance;
static uint8_t        mInterfaceId;
static event_t   mAppEvent;

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

/* The current state of the applications state machine */
uint8_t gState;

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \brief  This is the first task created by the OS. This task will initialize 
*         the system
*
* \param[in]  argument
*
* \return  None.
*
* \pre
*
* \post
*
* \remarks
*
********************************************************************************** */

void ADC16_Measure(void)
{
    adc16_chn_config_t chnConfig;

    // Configure the conversion channel
    // differential and interrupt mode disable.
    chnConfig.chnIdx     = (adc16_chn_t)ADC16_INPUT_CHN;
#if FSL_FEATURE_ADC16_HAS_DIFF_MODE
    chnConfig.diffConvEnable = false;
#endif
    chnConfig.convCompletedIntEnable  = false;

    // Software trigger the conversion.
    ADC16_DRV_ConfigConvChn(ADC16_INSTANCE, ADC16_CHN_GROUP, &chnConfig);

    // Wait for the conversion to be done.
    ADC16_DRV_WaitConvDone(ADC16_INSTANCE, ADC16_CHN_GROUP);

    // Fetch the conversion value.
    adcValue = ADC16_DRV_GetConvValueRAW(ADC16_INSTANCE, ADC16_CHN_GROUP);

    // Show the current temperature value.
  //  PRINTF("\tADC converted value at time %d: %d\r\n", lptmrCounter/64, adcValue );  //%ld\t
    
    // Pause the conversion.
    ADC16_DRV_PauseConv(ADC16_INSTANCE, ADC16_CHN_GROUP);
}

void calibrateParams(void)
{
    adc16_chn_config_t adcChnConfig;
#if FSL_FEATURE_ADC16_HAS_HW_AVERAGE
    adc16_hw_average_config_t userHwAverageConfig;
#endif
    pmc_bandgap_buffer_config_t pmcBandgapConfig = {
        .enable = true,
#if FSL_FEATURE_PMC_HAS_BGEN
        .enableInLowPower = false,
#endif
#if FSL_FEATURE_PMC_HAS_BGBDS
        .drive = kPmcBandgapBufferDriveLow,
#endif
    };

    uint32_t bandgapValue = 0;  // ADC value of BANDGAP
    uint32_t vdd = 0;           // VDD in mV

#if FSL_FEATURE_ADC16_HAS_CALIBRATION
    // Auto calibration
    adc16_calibration_param_t adcCalibraitionParam;
    ADC16_DRV_GetAutoCalibrationParam(ADC16_INSTANCE, &adcCalibraitionParam);
    ADC16_DRV_SetCalibrationParam(ADC16_INSTANCE, &adcCalibraitionParam);
#endif // FSL_FEATURE_ADC16_HAS_CALIBRATION.

    // Enable BANDGAP reference voltage
    PMC_HAL_BandgapBufferConfig(PMC_BASE_PTR, &pmcBandgapConfig);

#if FSL_FEATURE_ADC16_HAS_HW_AVERAGE
    // Use hardware average to increase stability of the measurement.
    userHwAverageConfig.hwAverageEnable = true;
    userHwAverageConfig.hwAverageCountMode = kAdc16HwAverageCountOf32;
    ADC16_DRV_ConfigHwAverage(ADC16_INSTANCE, &userHwAverageConfig);
#endif // FSL_FEATURE_ADC16_HAS_HW_AVERAGE

    // Configure the conversion channel
    // differential and interrupt mode disable.
    adcChnConfig.chnIdx                  = (adc16_chn_t)ADC16_BANDGAP_CHN;
#if FSL_FEATURE_ADC16_HAS_DIFF_MODE
    adcChnConfig.diffConvEnable          = false;
#endif
    adcChnConfig.convCompletedIntEnable  = false;
    ADC16_DRV_ConfigConvChn(ADC16_INSTANCE, ADC16_CHN_GROUP, &adcChnConfig);

    // Wait for the conversion to be done
    ADC16_DRV_WaitConvDone(ADC16_INSTANCE, ADC16_CHN_GROUP);

    // Get current ADC BANDGAP value and format it.
    bandgapValue = ADC16_DRV_GetConvValueSigned(ADC16_INSTANCE, ADC16_CHN_GROUP);
    // Calculates bandgapValue in 16bit resolution
    // from 12bit resolution to calibrate.
#if (FSL_FEATURE_ADC16_MAX_RESOLUTION < 16)
    bandgapValue = bandgapValue << 4;
#endif
    // ADC stop conversion
    ADC16_DRV_PauseConv(ADC16_INSTANCE, ADC16_CHN_GROUP);

    // Get VDD value measured in mV
    // VDD = (ADCR_VDD x V_BG) / ADCR_BG
    //vdd = ADCR_VDD * V_BG / bandgapValue;

    // Disable BANDGAP reference voltage
    pmcBandgapConfig.enable = false;
    PMC_HAL_BandgapBufferConfig(PMC_BASE_PTR, &pmcBandgapConfig);
}







void main_task(uint32_t param)
{
    static uint8_t initialized = FALSE;

    if( !initialized )
    {
        initialized = TRUE;
        //hardware_init();
        hardware_init_combined();

        MEM_Init();
        TMR_Init();
        LED_Init();
        SecLib_Init();
        SerialManager_Init();
        Phy_Init();
        RNG_Init(); /* RNG must be initialized after the PHY is Initialized */
        MAC_Init();

        /* Bind to MAC layer */
        mMacInstance = BindToMAC( (instanceId_t)0 );
        Mac_RegisterSapHandlers( MCPS_NWK_SapHandler, MLME_NWK_SapHandler, mMacInstance );
        
        /* ADDED */
          // Register callback func for I2C
        i2cInterface.i2c_init       =  I2C_DRV_MasterInit;
        i2cInterface.i2c_read       =  I2C_HAL_MasterReceiveDataPolling;
        i2cInterface.i2c_write      =  I2C_HAL_MasterSendDataPolling;

        accDev.i2c = &i2cInterface;
        accDev.accel = &accDevice;

        accDev.slave.baudRate_kbps  = BOARD_ACCEL_BAUDRATE;
        accDev.slave.address        = BOARD_ACCEL_ADDR;
        accDev.slave.base           = I2C1_BASE_PTR;
        accDev.bus                  = BOARD_ACCEL_I2C_INSTANCE;
        
        // Init hardware.
        hardware_init_combined();
        
        // Initialize the Accelerometer
        accel_init(&accDev);
        
        uint32_t      currentCounter = 0;
        
        LED1_EN;
        
        // Initialization ADC for
        // 16bit resolution.
        // interrupt mode and hw trigger disabled,
        // normal convert speed, VREFH/L as reference,
        // disable continuous convert mode.
        ADC16_DRV_StructInitUserConfigDefault(&adcUserConfig_1);
        adcUserConfig_1.resolution = kAdc16ResolutionBitOf16;
        
    #if BOARD_ADC_USE_ALT_VREF
        adcUserConfig_1.refVoltSrc = kAdc16RefVoltSrcOfValt;
    #endif
        ADC16_DRV_Init(ADC16_INSTANCE, &adcUserConfig_1);
        // Calibrate VDD and ADCR_TEMP25
        calibrateParams();
  
 
        App_init();
    }

    /* Call application task */
    AppThread( param );
}


/*****************************************************************************
* Initialization function for the App Task. This is called during
* initialization and should contain any application specific initialization
* (ie. hardware initialization/setup, table initialization, power up
* notificaiton.
*
* Interface assumptions: None
*
* Return value: None
*
*****************************************************************************/
void App_init(void)
{   
    OSA_EventCreate(&mAppEvent, kEventAutoClear);
    /* The initial application state */
    gState = stateInit;    
    
    /* Initialize the MAC 802.15.4 extended address */
    Mac_SetExtendedAddress( (uint8_t*)&mExtendedAddress, mMacInstance );
    
    mSoftTimerId_c         = TMR_AllocateTimer();
    mPollIntervalTimerId_c = TMR_AllocateTimer();
    /* Allocate sensor data sending timer */
    mSensorTimerId_c       = TMR_AllocateTimer();
    
    /* Register keyboard callback function */
    KBD_Init( App_HandleKeys );
    
    /* Initialize the serial terminal interface so that we can print out status messages */
    Serial_InitInterface( &mInterfaceId, APP_SERIAL_INTERFACE_TYPE, APP_SERIAL_INTERFACE_INSTANCE );
    Serial_SetBaudRate  ( mInterfaceId, gUARTBaudRate57600_c );
    Serial_SetRxCallBack( mInterfaceId, App_CommRxCallBack, NULL );
    
    /* Prepare input queues.*/
    MSG_InitQueue( &mMlmeNwkInputQueue );
    MSG_InitQueue( &mMcpsNwkInputQueue );
    
    /*signal app ready*/  
    LED_StartSerialFlash(LED1);
    
    Serial_Print(mInterfaceId,"\n\rPress any switch on board to start running the application.\n\r", gAllowToBlock_d);  
}

/*****************************************************************************
* The AppTask(event_t events) function is the applicantion main loop and 
* will process any incoming event. Events include timers, messages and any 
* other user defined events.
*
* Interface assumptions:
*     None
*
* Return value: 
*     None
*****************************************************************************/
void AppThread(uint32_t argument)
{ 
    event_flags_t ev;
    /* Pointer for storing the messages from MLME, MCPS, and ASP. */
    void *pMsgIn = NULL;
    /* Stores the status code returned by some functions. */
    uint8_t rc;  
    bool_t (*pApp_Idle)(void) = NULL;  // this is only for removing compiler warning (about not calling App_Idle() ) 
    
    (void)pApp_Idle;
    
    
    
    while(1)
    {
        OSA_EventWait(&mAppEvent, 0x00FFFFFF, FALSE, OSA_WAIT_FOREVER, &ev);

        if( !gUseRtos_c && !ev)
        {
            break;
        }
        
        pMsgIn = NULL;

        /* Dequeue the MLME message */
        if( ev & gAppEvtMessageFromMLME_c )
        {
            /* Get the message from MLME */
            pMsgIn = MSG_DeQueue( &mMlmeNwkInputQueue );
            
            /* Any time a beacon might arrive. Always handle the beacon frame first */
            if (pMsgIn)
            {               
                rc = App_WaitMsg( pMsgIn, gMlmeBeaconNotifyInd_c );
                if( rc == errorNoError )
                {
                    /* ALWAYS free the beacon frame contained in the beacon notify indication.*/
                    /* ALSO the application can use the beacon payload.*/
                    MSG_Free( ((nwkMessage_t *)pMsgIn)->msgData.beaconNotifyInd.pBufferRoot );
                    Serial_Print( mInterfaceId,"Received an MLME-Beacon Notify Indication\n\r", gAllowToBlock_d );
                }
            }
        }

        switch( gState )
        {    
        case stateInit:       
            /* Goto Energy Detection state. */
            gState = stateScanActiveStart;
            OSA_EventSet(&mAppEvent, gAppEvtDummyEvent_c );  

            /* Print a welcome message to the serial terminal interface */
            Serial_Print( mInterfaceId,"\n\rMyStarNetworkDemo-EndDevice application is initialized and ready.\n\r", gAllowToBlock_d );
            break;    
            
        case stateScanActiveStart:
            /* Start the Active scan, and goto wait for confirm state. */
            Serial_Print( mInterfaceId,"Start scanning for a PAN coordinator.\n\r", gAllowToBlock_d );
            
            rc = App_StartScan( gScanModeActive_c );
            if( rc == errorNoError )
            {
                gState = stateScanActiveWaitConfirm;
            }
            break;
            
        case stateScanActiveWaitConfirm:
            /* Stay in this state until the Scan confirm message arrives, and then goto
            the associate state or do a rescan in case of invalid short address. 
            */
            
            /* ALWAYS free the beacon frame contained in the beacon notify indication.*/
            if( ev & gAppEvtMessageFromMLME_c )
            {
                if( pMsgIn )
                {
                    /* Handle the Scan Confirm message. */
                    rc = App_WaitMsg( pMsgIn, gMlmeScanCnf_c );
                    if( rc == errorNoError )
                    {        
                        rc = App_HandleScanActiveConfirm( pMsgIn );
                        if( rc == errorNoError )
                        { 
                            Serial_Print( mInterfaceId,"Found a coordinator with the following properties:.\n\r", gAllowToBlock_d );
                            Serial_Print( mInterfaceId,"----------------------------------------------------.", gAllowToBlock_d);
                            Serial_Print( mInterfaceId,"\n\rAddress...........0x", gAllowToBlock_d ); Serial_PrintHex( mInterfaceId,(uint8_t*)&mCoordInfo.coordAddress, mCoordInfo.coordAddrMode == gAddrModeShortAddress_c ? 2 : 8, 0 );Serial_Print( mInterfaceId,".\n\r", gAllowToBlock_d );
                            Serial_Print( mInterfaceId,"\n\rPAN ID............0x", gAllowToBlock_d ); Serial_PrintHex( mInterfaceId,(uint8_t*)&mCoordInfo.coordPanId, 2, 0);Serial_Print(mInterfaceId,".\n\r", gAllowToBlock_d );
                            Serial_Print( mInterfaceId,"\n\rLogical Channel...0x", gAllowToBlock_d ); Serial_PrintHex( mInterfaceId,&mCoordInfo.logicalChannel, 1, 0);Serial_Print(mInterfaceId,".\n\r", gAllowToBlock_d );
                            Serial_Print( mInterfaceId,"\n\rBeacon Spec.......0x", gAllowToBlock_d ); Serial_PrintHex( mInterfaceId,(uint8_t*)&mCoordInfo.superframeSpec, 2, 0);Serial_Print(mInterfaceId,".\n\r", gAllowToBlock_d );
                            Serial_Print( mInterfaceId,"\n\rLink Quality......0x", gAllowToBlock_d ); Serial_PrintHex( mInterfaceId,&mCoordInfo.linkQuality, 1, 0);Serial_Print(mInterfaceId,".\n\r", gAllowToBlock_d );
                            Serial_Print( mInterfaceId,"\n\r\n\r", gAllowToBlock_d );
                            
                            gState = stateAssociate;
                            OSA_EventSet(&mAppEvent, gAppEvtDummyEvent_c );
                        }
                        else
                        {                   
                            /* Restart scanning */
                            Serial_Print( mInterfaceId,"Scan did not find a suitable coordinator.\n\r", gAllowToBlock_d );
                            
                            TMR_StartSingleShotTimer( mSoftTimerId_c, mWaitInterval_c, App_IntervalTimeoutHandler, NULL );
                            gState = stateWaitInterval;
                        }
                    }
                }
            }
            break;
            
        case stateWaitInterval:
            if( ev & gAppEvtStartWait_c )
            {        
                gState = stateScanActiveStart;
                OSA_EventSet(&mAppEvent, gAppEvtDummyEvent_c );
            }
            break;
            
        case stateAssociate:
            /* Associate to the PAN coordinator */
            Serial_Print( mInterfaceId,"Associating to PAN coordinator on channel 0x", gAllowToBlock_d );
            Serial_PrintHex( mInterfaceId,&(mCoordInfo.logicalChannel), 1, 0 );
            Serial_Print( mInterfaceId,".\n\r", gAllowToBlock_d );
            
            rc = App_SendAssociateRequest();
            if( rc == errorNoError )
            {        
                gState = stateAssociateWaitConfirm;
            }
            break; 
            
        case stateAssociateWaitConfirm:
            /* Stay in this state until the Associate confirm message
            arrives, and then goto the Listen state. */
            if( ev & gAppEvtMessageFromMLME_c )
            {
                if( pMsgIn )
                {                    
                    rc = App_WaitMsg( pMsgIn, gMlmeAssociateCnf_c );
                    if( rc == errorNoError )
                    {        
                        /* Check for coordinator at full capacity error */
                        if( App_HandleAssociateConfirm(pMsgIn) == gSuccess_c )
                        {        
                            gState = stateListen;
                            Serial_Print(mInterfaceId,"Device successfully associated.\n\r", gAllowToBlock_d);
                            Serial_Print(mInterfaceId,"We were assigned the short address 0x", gAllowToBlock_d);
                            Serial_PrintHex(mInterfaceId, (uint8_t*)&mAddress, 2, 0);
                            Serial_Print(mInterfaceId,".\n\r\n\rEnd Device ready to send and receive data over the serial terminal interface.\n\r", gAllowToBlock_d);

                            OSA_EventSet(&mAppEvent, gAppEvtDummyEvent_c);

                            /* Start sensor data sending interval timer. */
                            TMR_StartIntervalTimer( mSensorTimerId_c, mSensorTimeInterval_c, App_SensorIntervalTimeoutHandler, NULL );
                            /* Start poll interval timer. */  
                            TMR_StartIntervalTimer( mPollIntervalTimerId_c, mDefaultValueOfPollInterval_c, App_PollIntervalTimeoutHandler, NULL );
                        }
                        else
                        {        
                            /* Restart scanning */
                            Serial_Print(mInterfaceId,"No valid short address. Retry scan.\n\r", gAllowToBlock_d);
                            TMR_StartSingleShotTimer( mSoftTimerId_c, mWaitInterval_c, App_IntervalTimeoutHandler, NULL );
                            gState = stateWaitInterval;
                        }
                    }
                }        
            }
            break; 
            
        case stateListen:
            /* Stay in this state forever. Handles poll confirm etc. */
            if( ev & gAppEvtMessageFromMLME_c )
            {  
                if( pMsgIn )
                {  
                    /* Process it */
                    rc = App_HandleMlmeInput( pMsgIn );
                }
            }
            
            break;
        } /* end switch */
        
        /* Free Mlme Messages */
        if( pMsgIn )
        {
            /* Messages must always be freed. */ 
            MSG_Free( pMsgIn );
            pMsgIn = NULL;
        }
        
        /* Handle MCPS confirms and transmit data from serial terminal interface */
        if( ev & gAppEvtMessageFromMCPS_c )
        {
            /* Get the message from MCPS */
            pMsgIn = MSG_DeQueue(&mMcpsNwkInputQueue);
            if( pMsgIn )
            {              
                /* Process it */
                App_HandleMcpsInput( pMsgIn );
                /* Messages from the MCPS must always be freed. */
                MSG_Free( pMsgIn );
                pMsgIn = NULL;
            }
        }
        
        if( gState == stateListen ) 
        {
            if( ev & gAppEvtSendPollReq_c )
            {	
                /* Poll coordinator for data. */	    
                if( !mStopPolling )
                {
                    App_ReceiveData();
#if (gLpmIncluded_d == 1)	                     	        
                    /* Stop polling coordintor in order to ensure that idle task 
                    gets executed. */
                    mStopPolling = TRUE;	      
#endif
                }
            }

#if (gLpmIncluded_d == 1)	                     	        
            /* Check and if possible allow device to enter sleep mode */
            if( App_Idle() == TRUE )         
            {
                /* Allow device to enter sleep mode. The device will enter
                in low power first time the idle task gets called.
                */
                flag = 0;
                PWR_AllowDeviceToSleep(); 
                mStopPolling = FALSE;
            }
            else
            {
                OSA_EventSet(&mAppEvent, gAppEvtDummyEvent_c );
            }
#else
            pApp_Idle = App_Idle;     // this is only for removing compiler warning (about not calling App_Idle() ) 
#endif      

            if( ev & gAppEvtSendMessage_c )
            {
                App_SendPredefinedMessage();                         
            }

            if( ev & gAppEvtSendSensorData_c )
            {
                App_SendSensorStatus();
            }
        }

        /* Check for pending messages in the Queue */ 
        if( MSG_Pending(&mMcpsNwkInputQueue) )
            OSA_EventSet(&mAppEvent, gAppEvtMessageFromMCPS_c );

        if( MSG_Pending(&mMlmeNwkInputQueue) )
            OSA_EventSet(&mAppEvent, gAppEvtMessageFromMLME_c );  

        if( !gUseRtos_c )
        {
            break;
        } 
    }
}

/*****************************************************************************
* App_CommRxCallBack
*
* This callback is triggered when a new byte is received over the serial terminal interface
*
*****************************************************************************/
static void App_CommRxCallBack( void *pData ) 
{
    uint8_t pressedKey;
    uint16_t count;
    
    (void)pData;
    (void)Serial_GetByteFromRxBuffer( mInterfaceId, &pressedKey, &count );
    
    if( count && gMessageMarkSendDevInfo == pressedKey )
    {
        /* Print device info */
        Serial_Print( mInterfaceId,mDeviceInfo, gAllowToBlock_d );
    }
}

/******************************************************************************
* The App_StartScan(scanType) function will start the scan process of the
* specified type in the MAC. This is accomplished by allocating a MAC message,
* which is then assigned the desired scan parameters and sent to the MLME
* service access point.
*
* The function may return either of the following values:
*   errorNoError:          The Scan message was sent successfully.
*   errorInvalidParameter: The MLME service access point rejected the
*                          message due to an invalid parameter.
*   errorAllocFailed:      A message buffer could not be allocated.
*
******************************************************************************/
static uint8_t App_StartScan( macScanType_t scanType )
{
    mlmeMessage_t *pMsg;
    mlmeScanReq_t *pScanReq;

    Serial_Print(mInterfaceId,"Sending the MLME-Scan Request message to the MAC... ", gAllowToBlock_d);

    /* Allocate a message for the MLME (We should check for NULL). */
    pMsg = MSG_AllocType( mlmeMessage_t );
    if( pMsg != NULL )
    {
        /* This is a MLME-SCAN.req command */
        pMsg->msgType = gMlmeScanReq_c;
        /* Create the Scan request message data. */
        pScanReq = &pMsg->msgData.scanReq;
        /* gScanModeED_c, gScanModeActive_c, gScanModePassive_c, or gScanModeOrphan_c */
        pScanReq->scanType = scanType;
        /* ChannelsToScan */
#ifdef gPHY_802_15_4g_d
        pScanReq->channelPage = gChannelPageId9_c;
        pScanReq->scanChannels[0] = mDefaultValueOfChannel_c;
        FLib_MemSet(pScanReq->scanChannels+1, 0, sizeof(channelMask_t)-sizeof(uint32_t));
#else
        pScanReq->scanChannels = mDefaultValueOfChannel_c;
#endif
        
        /* Duration per channel 0-14 (dc). T[sec] = (16*960*((2^dc)+1))/1000000.
        A scan duration of 5 on 16 channels approximately takes 8 secs. */
        pScanReq->scanDuration = 5;
        
        pScanReq->securityLevel = gMacSecurityNone_c; //No security is used
        
        /* Send the Scan request to the MLME. */
        if( NWK_MLME_SapHandler( pMsg, mMacInstance ) == gSuccess_c )
        {
            Serial_Print( mInterfaceId,"Done.\n\r", gAllowToBlock_d );  
            return errorNoError;
        }																	 
        else
        {
            Serial_Print( mInterfaceId,"Invalid parameter.\n\r", gAllowToBlock_d );
            return errorInvalidParameter;
        }
    }
    else
    {
        /* Allocation of a message buffer failed. */
        Serial_Print( mInterfaceId,"Message allocation failed.\n\r", gAllowToBlock_d );
        return errorAllocFailed;
    }
}

/******************************************************************************
* The App_HandleScanActiveConfirm(nwkMessage_t *pMsg) function will handle the
* Active Scan confirm message received from the MLME when the Active scan has
* completed. The message contains a list of PAN descriptors. Based on link
* quality inforamtion in the pan descriptors the nearest coordinator is chosen.
* The corresponding pan descriptor is stored in the global variable mCoordInfo. 
*
* The function may return either of the following values:
*   errorNoError:       A suitable pan descriptor was found.
*   errorNoScanResults: No scan results were present in the confirm message.
*
******************************************************************************/
static uint8_t App_HandleScanActiveConfirm( nwkMessage_t *pMsg )
{
    void    *pBlock;
    uint8_t panDescListSize = pMsg->msgData.scanCnf.resultListSize;
    uint8_t rc = errorNoScanResults;
    uint8_t j;
    uint8_t bestLinkQuality = 0;  
    panDescriptorBlock_t *pDescBlock = pMsg->msgData.scanCnf.resList.pPanDescriptorBlockList;  
    panDescriptor_t      *pPanDesc;     

    /* Check if the scan resulted in any coordinator responses. */  
    if( panDescListSize > 0 )
    {    
        /* Check all PAN descriptors. */
        while( NULL != pDescBlock )
        {
            for( j = 0; j < pDescBlock->panDescriptorCount; j++ )
            {            
                pPanDesc = &pDescBlock->panDescriptorList[j];

                /* Only attempt to associate if the coordinator
                accepts associations and is non-beacon. */
                if( ( pPanDesc->superframeSpec.associationPermit ) && 
                   ( pPanDesc->superframeSpec.beaconOrder == 0x0F) )
                {        
                    /* Find the nearest coordinator using the link quality measure. */
                    if( pPanDesc->linkQuality > bestLinkQuality )
                    {
                        /* Save the information of the coordinator candidate. If we
                        find a better candiate, the information will be replaced. */
                        FLib_MemCpy( &mCoordInfo, pPanDesc, sizeof(panDescriptor_t) );
                        bestLinkQuality = pPanDesc->linkQuality;
                        rc = errorNoError;
                    }
                }      
            }

            /* Free current block */
            pBlock = pDescBlock;
            pDescBlock = pDescBlock->pNext;              
            MSG_Free( pBlock );
        }
    }

    if( pDescBlock )
        MSG_Free( pDescBlock );

    return rc;
}

/******************************************************************************
* The App_SendAssociateRequest(void) will create an Associate Request message
* and send it to the coordinator it wishes to associate to. The function uses
* information gained about the coordinator during the scan procedure.
*
* The function may return either of the following values:
*   errorNoError:          The Associate Request message was sent successfully.
*   errorInvalidParameter: The MLME service access point rejected the
*                          message due to an invalid parameter.
*   errorAllocFailed:      A message buffer could not be allocated.
*
******************************************************************************/
static uint8_t App_SendAssociateRequest( void )
{
    mlmeMessage_t *pMsg;
    mlmeAssociateReq_t *pAssocReq;

    Serial_Print( mInterfaceId,"Sending the MLME-Associate Request message to the MAC ", gAllowToBlock_d );

    /* Allocate a message for the MLME message. */
    pMsg = MSG_AllocType( mlmeMessage_t );
    if( pMsg != NULL )
    {
        /* This is a MLME-ASSOCIATE.req command. */
        pMsg->msgType = gMlmeAssociateReq_c;
        /* Create the Associate request message data. */
        pAssocReq = &pMsg->msgData.associateReq;

        /* Use the coordinator info we got from the Active Scan. */
        FLib_MemCpy(&pAssocReq->coordAddress, &mCoordInfo.coordAddress, 8);
        FLib_MemCpy(&pAssocReq->coordPanId,   &mCoordInfo.coordPanId, 2);
        pAssocReq->coordAddrMode  = mCoordInfo.coordAddrMode;
        pAssocReq->logicalChannel = mCoordInfo.logicalChannel;
        pAssocReq->securityLevel  = gMacSecurityNone_c;
#ifdef gPHY_802_15_4g_d
        pAssocReq->channelPage = gChannelPageId9_c;
#else
        pAssocReq->channelPage = gDefaultChannelPageId_c;
#endif
        
        /* We want the coordinator to assign a short address to us. */
        pAssocReq->capabilityInfo     = gCapInfoAllocAddr_c;
        
        /* Send the Associate Request to the MLME. */
        if( NWK_MLME_SapHandler( pMsg, mMacInstance ) == gSuccess_c )
        {
            Serial_Print( mInterfaceId,"Done.\n\r", gAllowToBlock_d );
            return errorNoError;
        }
        else
        {
            /* One or more parameters in the message were invalid. */
            Serial_Print( mInterfaceId,"Invalid parameter.\n\r", gAllowToBlock_d );
            return errorInvalidParameter;
        }
    }
    else
    {
        /* Allocation of a message buffer failed. */
        Serial_Print( mInterfaceId,"Message allocation failed.\n\r", gAllowToBlock_d );
        return errorAllocFailed;
    }
}


/******************************************************************************
* The App_HandleAssociateConfirm(nwkMessage_t *pMsg) function will handle the
* Associate confirm message received from the MLME when the Association
* procedure has completed. The message contains the short address that the
* coordinator has assigned to us. The address and address mode are saved in
* global variables. They will be used in the next demo application when sending
* data .
*
* The function may return either of the following values:
*   gSuccess_c:          Correct short address 
*   gPanAccessDenied_c:  Invalid short address
*
******************************************************************************/
static uint8_t App_HandleAssociateConfirm( nwkMessage_t *pMsg )
{
    if( pMsg->msgData.associateCnf.status == gSuccess_c )
    {
        FLib_MemCpy( &mAddress, &pMsg->msgData.associateCnf.assocShortAddress, 2 );
        return gSuccess_c;        
    }
    else
    {
        /* No valid short address. */
        return gPanAccessDenied_c;  
    }  
}


/******************************************************************************
* The App_HandleMlmeInput(nwkMessage_t *pMsg) function will handle various
* messages from the MLME, e.g. poll confirm.
*
* The function may return either of the following values:
*   errorNoError:   The message was processed.
*   errorNoMessage: The message pointer is NULL.
*
******************************************************************************/
static uint8_t App_HandleMlmeInput( nwkMessage_t *pMsg )
{
    if( pMsg == NULL )
    {    
        return errorNoMessage;
    }
    
    /* Handle the incoming message. The type determines the sort of processing.*/
    switch( pMsg->msgType )
    {
    case gMlmePollCnf_c:
        if( pMsg->msgData.pollCnf.status != gSuccess_c )
        {      
            /* If we get to this point, then no data was available, and we
            allow a new poll request. Otherwise, we wait for the data
            indication before allowing the next poll request. */
            mWaitPollConfirm = FALSE;
        }
        break;
    }
    return errorNoError;
}


/******************************************************************************
* The App_HandleMcpsInput(mcpsToNwkMessage_t *pMsgIn) function will handle 
* messages from the MCPS, e.g. Data Confirm, and Data Indication.
*
******************************************************************************/
static void App_HandleMcpsInput( mcpsToNwkMessage_t *pMsgIn )
{
    switch( pMsgIn->msgType )
    {
        /* The MCPS-Data confirm is sent by the MAC to the network 
        or application layer when data has been sent. */
    case gMcpsDataCnf_c:
        if( mcPendingPackets )
        {        
            mcPendingPackets--;
        }
        break;
        
    case gMcpsDataInd_c:
        /* Handle message */
        App_HandleMessage( pMsgIn );
        
        /* Allow another MLME-Poll request. */
        mWaitPollConfirm = FALSE;
        break;
    }
}


/******************************************************************************
* The App_HandleMessage(mcpsToNwkMessage_t *pMsgIn) function will handle a 
* data indication message from the MCPS.If it is a network activity message,
* the value received from the coordinator is displayed on the LEDs . If it is
* a data packet , we send it to the serial terminal interface.
*
******************************************************************************/
static void App_HandleMessage( mcpsToNwkMessage_t *pMsgIn )
{
    uint8_t val = *(pMsgIn->msgData.dataInd.pMsdu);
    if( ( pMsgIn->msgData.dataInd.msduLength == 1 ) &&
        ( val <= mDefaultValueOfMaxDisplayVal_c ))
    {
        App_UpdateLEDs(val);
    }
    else
    {
        uint8_t hex = pMsgIn->msgData.dataInd.mpduLinkQuality;
        Serial_Print(mInterfaceId,"\n\rPAN Coordinator(0x", gAllowToBlock_d);
        Serial_PrintHex(mInterfaceId,&hex,1,0);
        Serial_Print(mInterfaceId,"):", gAllowToBlock_d);
        Serial_SyncWrite(mInterfaceId,pMsgIn->msgData.dataInd.pMsdu, pMsgIn->msgData.dataInd.msduLength);
        Serial_Print(mInterfaceId,"\n\r", gAllowToBlock_d);
    }
}


/******************************************************************************
* The App_WaitMsg(nwkMessage_t *pMsg, uint8_t msgType) function does not, as
* the name implies, wait for a message, thus blocking the execution of the
* state machine. Instead the function analyzes the supplied message to 
* determine whether or not the message is of the expected type.
*
* The function may return either of the following values:
*   errorNoError: The message was of the expected type.
*   errorNoMessage: The message pointer is NULL.
*   errorWrongConfirm: The message is not of the expected type.
*
******************************************************************************/
static uint8_t App_WaitMsg( nwkMessage_t *pMsg, uint8_t msgType )
{
  /* Do we have a message? If not, the exit with error code */
  if( pMsg == NULL )
  {    
    return errorNoMessage;
  }

  /* Is it the expected message type? If not then exit with error code */
  if( pMsg->msgType != msgType )
  {    
    return errorWrongConfirm;
  }

  /* Found the expected message. Return with success code */
  return errorNoError;
}


/******************************************************************************
* The App_SendPredefinedMessage() function will perform  data transmissions of
* a predefined message to show some kind of a network activity. Data could also 
* come from other sources such as sensors etc. This is completely determined by
* the application. The constant mMaxPendingDataPackets_c determine the maximum
* number of packets pending for transmission in the MAC. A global variable
* is incremented each time a data packet is sent to the MCPS, and decremented
* when the corresponding MCPS-Data Confirm message is received. If the counter
* reaches the defined maximum no more data buffers are allocated until the
* counter is decreased below the maximum number of pending packets.
*
* The function uses the coordinator information gained during the Active Scan,
* and the short address assigned to us by coordinator, for building an MCPS-
* Data Request message. The message is sent to the MCPS service access point
* in the MAC.
*
******************************************************************************/
static void App_SendPredefinedMessage( void )
{   
    mpPacket = NULL;
    
    /* Data request packet for sending serial terminal interface input to the coordinator */
    
    if( (mcPendingPackets < mMaxPendingDataPackets_c) && (mpPacket == NULL) )
    {
        /* If the maximum number of pending data buffes is below maximum limit 
        and we do not have a data buffer already then allocate one. */    
        mpPacket = MSG_Alloc(sizeof(nwkToMcpsMessage_t) + gMaxPHYPacketSize_c);
    }
    
    if( mpPacket != NULL )
    {  
        mpPacket->msgData.dataReq.pMsdu = (uint8_t*)(&(mpPacket->msgData.dataReq.pMsdu)) + sizeof(uint8_t*);
        FLib_MemCpy(mpPacket->msgData.dataReq.pMsdu, (uint8_t*) maGeneralMsg, mPredefinedMsgLen_c);
        /* Data was available in the serial terminal interface receive buffer. Now create an
        MCPS-Data Request message containing the serial terminal interface data. */
        mpPacket->msgType = gMcpsDataReq_c;
        /* Create the header using coordinator information gained during 
        the scan procedure. Also use the short address we were assigned
        by the coordinator during association. */
        FLib_MemCpy(&mpPacket->msgData.dataReq.dstAddr, &mCoordInfo.coordAddress, 8);               
        FLib_MemCpy(&mpPacket->msgData.dataReq.srcAddr, &mAddress, 2);
        FLib_MemCpy(&mpPacket->msgData.dataReq.dstPanId, &mCoordInfo.coordPanId, 2);           
        FLib_MemCpy(&mpPacket->msgData.dataReq.srcPanId, &mCoordInfo.coordPanId, 2);
        mpPacket->msgData.dataReq.dstAddrMode = mCoordInfo.coordAddrMode;
        mpPacket->msgData.dataReq.srcAddrMode = gAddrModeShortAddress_c;
        mpPacket->msgData.dataReq.msduLength = mPredefinedMsgLen_c;
        /* Request MAC level acknowledgement of the data packet */
        mpPacket->msgData.dataReq.txOptions = gMacTxOptionsAck_c;
        /* Give the data packet a handle. The handle is
        returned in the MCPS-Data Confirm message. */
        mpPacket->msgData.dataReq.msduHandle = mMsduHandle++;
        mpPacket->msgData.dataReq.securityLevel = gMacSecurityNone_c;
        /* Send the Data Request to the MCPS */
        (void)NWK_MCPS_SapHandler(mpPacket, mMacInstance);
        /* Prepare for another data buffer */
        mcPendingPackets++;
    }
}
uint32_t test = 0xABCDEF12;
/******************************************************************************
* The App_SendSensorStatus() function will perform  data transmissions of
* sensor data to the coordinator. It works exactly like the
* App_SendPredefinedMessage() function above. See it's description for details.
******************************************************************************/
static void App_SendSensorStatus( void )
{   
    //Dummy ON/OFF button status
    static uint8_t dummyToggleStatus = 0x00;
    // Dummy control info: suggests to the Coordinator that the End Device
    // will send info about: ON/OFF button, temperature sensor, Accelerometer and
    // pressure sensor
    //static uint8_t dummyControlInfo = 0x0F;
    // Dummy accelerometer data
   // static int16_t dummyAccelData[3] = {76, 84, 67};
    // Dummy pressure data
    //static int16_t dummyPulse = 14;
    // Dummy temperature data
   // static int8_t  dummyTemperature = 23;
    // Dummy battery level
   // static int8_t  dummyBatteryLevel = 31;
    
    mpPacket1 = NULL;
    mpPacket2 = NULL;
    
    /* added sensor stuff **********************************************************/
   // lptmrCounter++;
    micCounter++;
    accelerometerCounter++;
    pulseCounter++;
    inter_counter++;
    
    /* ======================================================== ACCEL ===================================*/
    if (accelerometerCounter>=accelerometerPeriod) {
      //Turn this else-if statement into an if statement for multiple
      //samples in one interrupt.
       mpPacket1 = NULL;
      
      //REPLACE THIS WITH MEASUREMENT FUNCTION
      //PRINTF("\tAccelerometer measurement\r\n");
      accelerometerCounter=0;
      // Get new accelerometer data.
      accDev.accel->accel_read_sensor_data(&accDev,&accelData);
      
      // Get the X, Y, and Z data from the sensor data structure.
      xData = (int16_t)((accelData.mma8451_data.accelXMSB << 8) | accelData.mma8451_data.accelXLSB) >> 2;
      yData = (int16_t)((accelData.mma8451_data.accelYMSB << 8) | accelData.mma8451_data.accelYLSB) >> 2;
      zData = (int16_t)((accelData.mma8451_data.accelZMSB << 8) | accelData.mma8451_data.accelZLSB) >> 2;
      orientation = accelData.mma8451_data.accelDir;
      
      //PRINTF("x = %d y = %d z = %d o = %d\r\n", xData, yData, zData, orientation);
        if( (mcPendingPackets < mMaxPendingDataPackets_c) && (mpPacket1 == NULL) )
    {
        /* If the maximum number of pending data buffes is below maximum limit 
        and we do not have a data buffer already then allocate one. */    
        mpPacket1 = MSG_Alloc(sizeof(nwkToMcpsMessage_t) + gMaxPHYPacketSize_c);
    }
    
    if( mpPacket1 != NULL )
    {       
        /* Data was available in the serial terminal interface receive buffer. Now create an
        MCPS-Data Request message containing the serial terminal interface data. */
        mpPacket1->msgType = gMcpsDataReq_c;
        /* Create the header using coordinator information gained during 
        the scan procedure. Also use the short address we were assigned
        by the coordinator during association. */
        mpPacket1->msgData.dataReq.pMsdu = (uint8_t*)(&(mpPacket1->msgData.dataReq.pMsdu)) + sizeof(uint8_t*);

        FLib_MemCpy(&mpPacket1->msgData.dataReq.dstAddr, &mCoordInfo.coordAddress, 8);               
        FLib_MemCpy(&mpPacket1->msgData.dataReq.srcAddr, &mAddress, 2);
        FLib_MemCpy(&mpPacket1->msgData.dataReq.dstPanId, &mCoordInfo.coordPanId, 2);           
        FLib_MemCpy(&mpPacket1->msgData.dataReq.srcPanId, &mCoordInfo.coordPanId, 2);
        mpPacket1->msgData.dataReq.dstAddrMode = mCoordInfo.coordAddrMode;
        mpPacket1->msgData.dataReq.srcAddrMode = gAddrModeShortAddress_c;
        mpPacket1->msgData.dataReq.msduLength = 16;
        // Signal to Coordinator that a sensor message follows
        mpPacket1->msgData.dataReq.pMsdu[0] = 0x01;
        // Put dummy sensor data into the data packet
      
        mpPacket1->msgData.dataReq.pMsdu[1] = (uint8_t)(xData);
        mpPacket1->msgData.dataReq.pMsdu[2] = (uint8_t)(xData>>8);
        mpPacket1->msgData.dataReq.pMsdu[3] = (uint8_t)(yData);
        mpPacket1->msgData.dataReq.pMsdu[4] = (uint8_t)(yData>>8);
        mpPacket1->msgData.dataReq.pMsdu[5] = (uint8_t)(zData);
        mpPacket1->msgData.dataReq.pMsdu[6] = (uint8_t)(zData>>8);
        mpPacket1->msgData.dataReq.pMsdu[7] = (uint8_t)orientation;
         mpPacket1->msgData.dataReq.pMsdu[8] = (uint8_t)(inter_counter);
         mpPacket1->msgData.dataReq.pMsdu[9] = (uint8_t)(inter_counter>>8);
         mpPacket1->msgData.dataReq.pMsdu[10] = (uint8_t)(inter_counter>>16);
         mpPacket1->msgData.dataReq.pMsdu[11] = (uint8_t)(inter_counter>>24);
        /* ADA, FIX THIS!! */
        mpPacket1->msgData.dataReq.pMsdu[12] = (uint8_t)(gap);
         mpPacket1->msgData.dataReq.pMsdu[13] = (uint8_t)(gap>>8);
         mpPacket1->msgData.dataReq.pMsdu[14] = (uint8_t)(gap>>16);
         mpPacket1->msgData.dataReq.pMsdu[15] = (uint8_t)(gap>>24);
        
       
        /* Request MAC level acknowledgment of the data packet */
        mpPacket1->msgData.dataReq.txOptions = gMacTxOptionsAck_c;
        /* Give the data packet a handle. The handle is
        returned in the MCPS-Data Confirm message. */
        mpPacket1->msgData.dataReq.msduHandle = mMsduHandle++;
        mpPacket1->msgData.dataReq.securityLevel = gMacSecurityNone_c;
        /* Send the Data Request to the MCPS */
        (void)NWK_MCPS_SapHandler(mpPacket1, mMacInstance);
        /* Prepare for another data buffer */
        mcPendingPackets++;
    }
}

      

    
    /*================================================= mic ================================================*/
    
    
    else if (micCounter>=micPeriod) {
      mpPacket2 = NULL;
      ADC16_Measure();
      numMicSamplesTakenCounter++;
      if(numMicSamplesTakenCounter>=numMicSamplesPerPeriod){
        micCounter=micCounter-micPeriod;
        numMicSamplesTakenCounter=0;
      }
      
      
        if( (mcPendingPackets < mMaxPendingDataPackets_c) && (mpPacket2 == NULL) )
    {
        /* If the maximum number of pending data buffes is below maximum limit 
        and we do not have a data buffer already then allocate one. */    
        mpPacket2 = MSG_Alloc(sizeof(nwkToMcpsMessage_t) + gMaxPHYPacketSize_c);
    }

    if( mpPacket2 != NULL )
    {       
        /* Data was available in the serial terminal interface receive buffer. Now create an
        MCPS-Data Request message containing the serial terminal interface data. */
        mpPacket2->msgType = gMcpsDataReq_c;
        /* Create the header using coordinator information gained during 
        the scan procedure. Also use the short address we were assigned
        by the coordinator during association. */
        mpPacket2->msgData.dataReq.pMsdu = (uint8_t*)(&(mpPacket2->msgData.dataReq.pMsdu)) + sizeof(uint8_t*);

        FLib_MemCpy(&mpPacket2->msgData.dataReq.dstAddr, &mCoordInfo.coordAddress, 8);               
        FLib_MemCpy(&mpPacket2->msgData.dataReq.srcAddr, &mAddress, 2);
        FLib_MemCpy(&mpPacket2->msgData.dataReq.dstPanId, &mCoordInfo.coordPanId, 2);           
        FLib_MemCpy(&mpPacket2->msgData.dataReq.srcPanId, &mCoordInfo.coordPanId, 2);
        mpPacket2->msgData.dataReq.dstAddrMode = mCoordInfo.coordAddrMode;
        mpPacket2->msgData.dataReq.srcAddrMode = gAddrModeShortAddress_c;
        mpPacket2->msgData.dataReq.msduLength = 11;
        // Signal to Coordinator that a sensor message follows
        mpPacket2->msgData.dataReq.pMsdu[0] = 0x02;
        // Put dummy sensor data into the data packet
        mpPacket2->msgData.dataReq.pMsdu[1] = (uint8_t)(adcValue);
        mpPacket2->msgData.dataReq.pMsdu[2] = (uint8_t)(adcValue>>8);
       mpPacket2->msgData.dataReq.pMsdu[3] = (uint8_t)(inter_counter);
         mpPacket2->msgData.dataReq.pMsdu[4] = (uint8_t)(inter_counter>>8);
        mpPacket2->msgData.dataReq.pMsdu[5] = (uint8_t)(inter_counter>>16);
         mpPacket2->msgData.dataReq.pMsdu[6] = (uint8_t)(inter_counter>>24);
         /* ADA, FIX THIS!! */
          mpPacket2->msgData.dataReq.pMsdu[7] = (uint8_t)(gap);
         mpPacket2->msgData.dataReq.pMsdu[8] = (uint8_t)(gap>>8);
         mpPacket2->msgData.dataReq.pMsdu[9] = (uint8_t)(gap>>16);
         mpPacket2->msgData.dataReq.pMsdu[10] = (uint8_t)(gap>>24);
            /*   mpPacket2->msgData.dataReq.pMsdu[7] = (uint8_t)(test);
         mpPacket2->msgData.dataReq.pMsdu[8] = (uint8_t)(test>>8);
        mpPacket2->msgData.dataReq.pMsdu[9] = (uint8_t)(test>>16);
         mpPacket2->msgData.dataReq.pMsdu[10] = (uint8_t)(test>>24);*/
         

        /* Request MAC level acknowledgment of the data packet */
        mpPacket2->msgData.dataReq.txOptions = gMacTxOptionsAck_c;
        /* Give the data packet a handle. The handle is
        returned in the MCPS-Data Confirm message. */
        mpPacket2->msgData.dataReq.msduHandle = mMsduHandle++;
        mpPacket2->msgData.dataReq.securityLevel = gMacSecurityNone_c;
        /* Send the Data Request to the MCPS */
        (void)NWK_MCPS_SapHandler(mpPacket2, mMacInstance);
        /* Prepare for another data buffer */
        mcPendingPackets++;
    }
}
    
    
    
    else if (pulseCounter>=pulsePeriod) {
      //Turn this else-if statement into an if statement for multiple
      //samples in one interrupt.
      
      //REPLACE THIS WITH MEASUREMENT FUNCTION
      //PRINTF("\tPulse measurement\r\n");
      numPulseSamplesTakenCounter++;
      if(numPulseSamplesTakenCounter>=numPulseSamplesPerPeriod){
        pulseCounter=pulseCounter-pulsePeriod;
        numPulseSamplesTakenCounter=0;
      }
    }
      
    LED1_TOGGLE;
   // PRINTF("micCounter: %d, accelerometerCounter: %d\r\n", micCounter, 
   //        accelerometerCounter);
    
    
    
    
 
   
}

/******************************************************************************
* The App_ReceiveData() function will check if it is possible to send out an
* MLME-Poll request in order to receive data from the coordinator. If its 
* permitted then a poll request is created and sent.
* 
* The function uses the coordinator information gained during the Active Scan
* for building the MLME-Poll Request message. The message is sent to the MLME
* service access point in the MAC.
*
******************************************************************************/
static void App_ReceiveData( void )
{
    /* Check if we are permitted, and if it is time to send a poll request.
    The poll interval is adjusted dynamically to the current band-width
    requirements. */
    if( mWaitPollConfirm == FALSE )
    {  
        /* This is an MLME-POLL.req command. */
        mlmeMessage_t *pMlmeMsg = MSG_AllocType( mlmeMessage_t );
        if( pMlmeMsg )
        {
            /* Create the Poll Request message data. */
            pMlmeMsg->msgType = gMlmePollReq_c;
            /* Use the coordinator information we got from the Active Scan. */
            FLib_MemCpy(&pMlmeMsg->msgData.pollReq.coordAddress, &mCoordInfo.coordAddress, 8);
            FLib_MemCpy(&pMlmeMsg->msgData.pollReq.coordPanId, &mCoordInfo.coordPanId, 2);
            pMlmeMsg->msgData.pollReq.coordAddrMode = mCoordInfo.coordAddrMode;
            pMlmeMsg->msgData.pollReq.securityLevel = gMacSecurityNone_c;
            
            /* Send the Poll Request to the MLME. */
            if( NWK_MLME_SapHandler( pMlmeMsg, mMacInstance ) == gSuccess_c )
            {
                /* Do not allow another Poll request before the confirm is received. */
                mWaitPollConfirm = TRUE;
            }
        }
    }
}


/******************************************************************************
* The App_Idle(void) function will check for network or serial terminal interface activity .
*
* The function may return either of the following values:
*   TRUE			: No activity detected
*   FALSE			: Network or serial terminal interface activity detected
*
******************************************************************************/
static bool_t App_Idle( void )
{
    if( (!MSG_Pending(&mMlmeNwkInputQueue)) &&
        (!MSG_Pending(&mMcpsNwkInputQueue)) &&
        (mWaitPollConfirm == FALSE)&&
        (TMR_AreAllTimersOff()) )
    {
        return TRUE;      
    }	 
    else
    {
        return FALSE;  
    }
}


/******************************************************************************
* The App_UpdateLEDs(uint8_t val) function will switch on/off the LEDs.
*
******************************************************************************/
static void App_UpdateLEDs( uint8_t displayMsg )
{
    if( displayMsg & LED1 ) Led1On(); else Led1Off();
    if( displayMsg & LED2 ) Led2On(); else Led2Off();
    if( displayMsg & LED3 ) Led3On(); else Led3Off();
    if( displayMsg & LED4 ) Led4On(); else Led4Off();
}


/******************************************************************************
* The App_IntervalTimeoutHandler(uint8_t timerID) function will send an event 
* to the application task after the programmed interval of time expires
*
******************************************************************************/
static void App_IntervalTimeoutHandler(void *pData)
{
   (void)pData;
   OSA_EventSet(&mAppEvent, gAppEvtStartWait_c);                
}

/******************************************************************************
* The App_PollIntervalTimeoutHandler(uint8_t timerID) function will send an event 
* to the application task after the programmed interval of time expires to send
* a poll interval
******************************************************************************/
static void App_PollIntervalTimeoutHandler( void *pData )
{
    (void)pData;
    OSA_EventSet(&mAppEvent, gAppEvtSendPollReq_c );
}

/******************************************************************************
* The App_SensorIntervalTimeoutHandler function will send an event 
* to the application task after the programmed interval of time expires
*
******************************************************************************/
static void App_SensorIntervalTimeoutHandler( void *pData )
{
    (void)pData;
    OSA_EventSet(&mAppEvent, gAppEvtSendSensorData_c );                
}

/*****************************************************************************
* Function to handle a generic key press. Called for all keys.
*****************************************************************************/
static void App_HandleGenericKey(void)
{
    if( gState == stateInit )
    {
        LED_StopFlashingAllLeds();

        OSA_EventSet(&mAppEvent, gAppEvtDummyEvent_c);        
    }
    else if(gState == stateListen)
    {
        /* Send a predefined message to Coordinator each time user presses
        a switch. */      
#if (gLpmIncluded_d == 1) 
        if (flag == 0)
        {
            flag = 1;
            PWR_DisallowDeviceToSleep(); 
        }
#endif 
        OSA_EventSet(&mAppEvent, gAppEvtSendMessage_c);
    } 
}

/*****************************************************************************
* The App_HandleKeys(key_event_t events) function can handle different
* key events. It waits for user to push a button in order to start 
* the application.
*
* Interface assumptions:
*     None
*
* Return value: 
*     None
*****************************************************************************/
static void App_HandleKeys( key_event_t events )
{
#if gKBD_KeysCount_c > 0 
    switch ( events ) 
    { 
    case gKBD_EventSW1_c:
    case gKBD_EventSW2_c:
    case gKBD_EventSW3_c:
    case gKBD_EventSW4_c:
    case gKBD_EventLongSW1_c:
    case gKBD_EventLongSW2_c:
    case gKBD_EventLongSW3_c:
    case gKBD_EventLongSW4_c:
        App_HandleGenericKey();
        break;
    default:
        break;  
    }
#endif
}


/******************************************************************************
* The following functions are called by the MAC to put messages into the
* Application's queue. They need to be defined even if they are not used
* in order to avoid linker errors.
*
******************************************************************************/
resultType_t MLME_NWK_SapHandler( nwkMessage_t* pMsg, instanceId_t instanceId )
{
    /* Put the incoming MLME message in the applications input queue. */    
    MSG_Queue( &mMlmeNwkInputQueue, pMsg );
    OSA_EventSet(&mAppEvent, gAppEvtMessageFromMLME_c );
    return gSuccess_c;
}

resultType_t MCPS_NWK_SapHandler( mcpsToNwkMessage_t* pMsg, instanceId_t instanceId )
{
    /* Put the incoming MCPS message in the applications input queue. */
    MSG_Queue( &mMcpsNwkInputQueue, pMsg );
    OSA_EventSet(&mAppEvent, gAppEvtMessageFromMCPS_c );
    return gSuccess_c;
}
