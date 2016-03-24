
/* Define to prevent recursive inclusion -------------------------------------*/  
#ifndef __SAMPLE_SERVICE
#define __SAMPLE_SERVICE

/* Includes ------------------------------------------------------------------*/
#include "cube_hal.h"
#include "hal_types.h"
#include "bluenrg_gatt_server.h"
#include "bluenrg_gap.h"
#include "string.h"
#include "bluenrg_gap_aci.h"
#include "bluenrg_gatt_aci.h"
#include "hci_const.h"
#include "gp_timer.h"
#include "bluenrg_hal_aci.h"
#include "bluenrg_aci_const.h"   
#include "hci.h"
#include "hal.h"
#include "sm.h"
#include "debug.h"

#include <stdlib.h>

#define IDB04A1 0
#define IDB05A1 1

typedef int i32_t;


tBleStatus Add_Sample_Service(void);
tBleStatus Sample_Characteristic_Update(uint8_t value);
void       setConnectable(void);
void       enableNotification(void);
void       GAP_ConnectionComplete_CB(uint8_t addr[6], uint16_t handle);
void       GAP_DisconnectionComplete_CB(void);
void       HCI_Event_CB(void *pckt);

#endif
