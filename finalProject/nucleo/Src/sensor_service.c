/**
  ******************************************************************************
  * @file    sensor_service.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    04-July-2014
  * @brief   Add a sample service using a vendor specific profile.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
#include "sensor_service.h"
#include <stdio.h>
#include <inttypes.h>

/* Private variables ---------------------------------------------------------*/
volatile int connected = FALSE;
volatile uint8_t set_connectable = 1;
volatile uint16_t connection_handle = 0;
volatile uint8_t notification_enabled = FALSE;
volatile AxesRaw_t axes_data = {0, 0, 0};

uint16_t acc2ServHandle, accCharHandle;

/* ACCELEROMETER */
uint16_t accServHandle, accRollCharHandle, accPitchCharHandle;

/* TEMPERATURE */
uint16_t tempServHandle, tempCharHandle;

/* DOUBLE TAP */
uint16_t tapServHandle, tapCharHandle;

/* LED */
uint16_t ledServHandle, ledDirCharHandle, ledOnCharHandle;



/** @defgroup SENSOR_SERVICE_Private_Macros
 * @{
 */
/* Private macros ------------------------------------------------------------*/
#define COPY_UUID_128(uuid_struct, uuid_15, uuid_14, uuid_13, uuid_12, uuid_11, uuid_10, uuid_9, uuid_8, uuid_7, uuid_6, uuid_5, uuid_4, uuid_3, uuid_2, uuid_1, uuid_0) \
do {\
    uuid_struct[0] = uuid_0; uuid_struct[1] = uuid_1; uuid_struct[2] = uuid_2; uuid_struct[3] = uuid_3; \
        uuid_struct[4] = uuid_4; uuid_struct[5] = uuid_5; uuid_struct[6] = uuid_6; uuid_struct[7] = uuid_7; \
            uuid_struct[8] = uuid_8; uuid_struct[9] = uuid_9; uuid_struct[10] = uuid_10; uuid_struct[11] = uuid_11; \
                uuid_struct[12] = uuid_12; uuid_struct[13] = uuid_13; uuid_struct[14] = uuid_14; uuid_struct[15] = uuid_15; \
}while(0)

  #define COPY_ACC2_SERVICE_UUID(uuid_struct) 	COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,0x00,0x00,0x00,0x01)
  #define COPY_ACC_UUID(uuid_struct)          	COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,0x00,0x00,0x00,0x02)

	/* ACCELEROMETER */
	#define COPY_ACC_SERVICE_UUID(uuid_struct)  	COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,0x00,0x00,0x00,0x03)
	#define COPY_ACC_ROLL_UUID(uuid_struct)  			COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,0x00,0x00,0x00,0x04)
  #define COPY_ACC_PITCH_UUID(uuid_struct) 			COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,0x00,0x00,0x00,0x05)

	/* TEMPERATURE */
	#define COPY_TEMP_SERVICE_UUID(uuid_struct)  	COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,0x00,0x00,0x00,0x06)
	#define COPY_TEMP_UUID(uuid_struct)  					COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,0x00,0x00,0x00,0x07)

	/* DOUBLE TAP */
	#define COPY_TAP_SERVICE_UUID(uuid_struct)  	COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,0x00,0x00,0x00,0x08)
	#define COPY_TAP_UUID(uuid_struct)  					COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,0x00,0x00,0x00,0x09)

	/* LED */
	#define COPY_LED_SERVICE_UUID(uuid_struct)  	COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,0x00,0x00,0x00,0x10)
	#define COPY_LED_DIR_UUID(uuid_struct)  			COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,0x00,0x00,0x00,0x11)
  #define COPY_LED_ON_UUID(uuid_struct) 				COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,0x00,0x00,0x00,0x12)


/* Store Value into a buffer in Little Endian Format */
#define STORE_LE_16(buf, val)    ( ((buf)[0] =  (uint8_t) (val)    ) , \
                                   ((buf)[1] =  (uint8_t) (val>>8) ) )



/**
 * @brief  Add an accelerometer service using a vendor specific profile.
 *
 * @param  None
 * @retval tBleStatus Status
 */
tBleStatus Add_Acc2_Service(void)
{
  tBleStatus ret;

  uint8_t uuid[16];
  
  COPY_ACC2_SERVICE_UUID(uuid);
  ret = aci_gatt_add_serv(UUID_TYPE_128,  uuid, PRIMARY_SERVICE, 6,
                          &acc2ServHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;    
  
  
  COPY_ACC_UUID(uuid);  
  ret =  aci_gatt_add_char(acc2ServHandle, UUID_TYPE_128, uuid, 6,
                           CHAR_PROP_NOTIFY|CHAR_PROP_READ,
                           ATTR_PERMISSION_NONE,
                           GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP,
                           16, 0, &accCharHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;
  
  PRINTF("Service ACC added. Handle 0x%04X, Acc Charac handle: 0x%04X\n",acc2ServHandle, accCharHandle);	
  return BLE_STATUS_SUCCESS; 
  
fail:
  PRINTF("Error while adding ACC service.\n");
  return BLE_STATUS_ERROR ;
    
}


/**
 * @brief  Update acceleration characteristic value.
 *
 * @param  Structure containing acceleration value in mg
 * @retval Status
 */
tBleStatus Acc_Update(AxesRaw_t *data)
{  
  tBleStatus ret;    
  uint8_t buff[6];
    
  STORE_LE_16(buff,data->AXIS_X);
  STORE_LE_16(buff+2,data->AXIS_Y);
  STORE_LE_16(buff+4,data->AXIS_Z);
	
  ret = aci_gatt_update_char_value(acc2ServHandle, accCharHandle, 0, 6, buff);
	
  if (ret != BLE_STATUS_SUCCESS){
    PRINTF("Error while updating ACC characteristic.\n") ;
    return BLE_STATUS_ERROR ;
  }
  return BLE_STATUS_SUCCESS;	
}

/******************************************************************************************************/
/*************************************** ACCELEROMETER ************************************************/
/******************************************************************************************************/

/**
 * @brief  Add an accelerometer service
 *
 * @param  None
 * @retval tBleStatus Status
 */
tBleStatus Add_Acc_Service(void)
{
  tBleStatus ret;

  uint8_t uuid[16];
  
  COPY_ACC_SERVICE_UUID(uuid);
  ret = aci_gatt_add_serv(UUID_TYPE_128,  uuid, PRIMARY_SERVICE, 7,
                          &accServHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;    
  
  COPY_ACC_ROLL_UUID(uuid);  
  ret =  aci_gatt_add_char(accServHandle, UUID_TYPE_128, uuid, 2,
                           CHAR_PROP_NOTIFY|CHAR_PROP_READ,
                           ATTR_PERMISSION_NONE, 0,
                           16, 0, &accRollCharHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;
	
	COPY_ACC_PITCH_UUID(uuid);  
  ret =  aci_gatt_add_char(accServHandle, UUID_TYPE_128, uuid, 2,
                           CHAR_PROP_NOTIFY|CHAR_PROP_READ,
                           ATTR_PERMISSION_NONE, 0,
                           16, 0, &accPitchCharHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;
  
  PRINTF("Service ACC added. Handle 0x%04X, Roll Charac handle: 0x%04X, Pitch Charac handle: 0x%04X\n",accServHandle, accRollCharHandle, accPitchCharHandle);	
  return BLE_STATUS_SUCCESS; 
  
fail:
  PRINTF("Error while adding ACC service.\n");
  return BLE_STATUS_ERROR ;
    
}


/**
 * @brief  Update Roll angle
 *
 * @param  roll angle in degrees
 * @retval Status
 */
tBleStatus Roll_Update(i32_t roll)
{  
  tBleStatus ret;    
  uint8_t buff[2];
    
  STORE_LE_16(buff,roll);
	
  ret = aci_gatt_update_char_value(accServHandle, accRollCharHandle, 0, 2, buff);
	
  if (ret != BLE_STATUS_SUCCESS){
    PRINTF("Error while updating ACC characteristic.\n") ;
    return BLE_STATUS_ERROR ;
  }
  return BLE_STATUS_SUCCESS;	
}

/**
 * @brief  Update Roll angle
 *
 * @param  roll angle in degrees
 * @retval Status
 */
tBleStatus Pitch_Update(i32_t pitch)
{  
  tBleStatus ret;    
  uint8_t buff[2];
    
  STORE_LE_16(buff, pitch);
	
  ret = aci_gatt_update_char_value(accServHandle, accPitchCharHandle, 0, 2, buff);
	
  if (ret != BLE_STATUS_SUCCESS){
    PRINTF("Error while updating ACC characteristic.\n") ;
    return BLE_STATUS_ERROR ;
  }
  return BLE_STATUS_SUCCESS;	
}


/******************************************************************************************************/
/*************************************** TEMPERATURE **************************************************/
/******************************************************************************************************/

/**
 * @brief  Add an temperature service
 *
 * @param  None
 * @retval tBleStatus Status
 */
tBleStatus Add_Temp_Service(void)
{
  tBleStatus ret;

  uint8_t uuid[16];
  
  COPY_TEMP_SERVICE_UUID(uuid);
  ret = aci_gatt_add_serv(UUID_TYPE_128,  uuid, PRIMARY_SERVICE, 4,
                          &tempServHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;    
  
  
  COPY_TEMP_UUID(uuid);  
  ret =  aci_gatt_add_char(tempServHandle, UUID_TYPE_128, uuid, 2,
                           CHAR_PROP_NOTIFY|CHAR_PROP_READ,
                           ATTR_PERMISSION_NONE,
                           0,
                           16, 0, &tempCharHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;
  
  PRINTF("Service TEMP added. Handle 0x%04X, Temp Charac handle: 0x%04X\n",tempServHandle, tempCharHandle);	
  return BLE_STATUS_SUCCESS; 
  
fail:
  PRINTF("Error while adding TEMP service.\n");
  return BLE_STATUS_ERROR ;
    
}


/**
 * @brief  Update temperature characteristic value.
 *
 * @param  temperature
 * @retval Status
 */
tBleStatus Temp_Update(i32_t temp)
{  
  tBleStatus ret;    
  uint8_t buff[2];
    
  STORE_LE_16(buff, temp);
	
  ret = aci_gatt_update_char_value(tempServHandle, tempCharHandle, 0, 2, buff);
	
  if (ret != BLE_STATUS_SUCCESS){
    PRINTF("Error while updating TEMP characteristic.\n") ;
    return BLE_STATUS_ERROR ;
  }
  return BLE_STATUS_SUCCESS;	
}

//TODO implement function that notify led characteristic for over temperature

/******************************************************************************************************/
/****************************************** DOUBLE TAP ************************************************/
/******************************************************************************************************/

/**
 * @brief  Add an double tap service
 *
 * @param  None
 * @retval tBleStatus Status
 */
tBleStatus Add_Tap_Service(void)
{
  tBleStatus ret;

  uint8_t uuid[16];
  
  COPY_TAP_SERVICE_UUID(uuid);
  ret = aci_gatt_add_serv(UUID_TYPE_128,  uuid, PRIMARY_SERVICE, 4,
                          &tapServHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;    
  
  
  COPY_TAP_UUID(uuid);  
  ret =  aci_gatt_add_char(tapServHandle, UUID_TYPE_128, uuid, 2,
                           CHAR_PROP_NOTIFY,
                           ATTR_PERMISSION_NONE,
                           GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP,
                           16, 0, &tapCharHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;
  
  PRINTF("Service TAP added. Handle 0x%04X, Tap Charac handle: 0x%04X\n",tapServHandle, tapCharHandle);	
  return BLE_STATUS_SUCCESS; 
  
fail:
  PRINTF("Error while adding TAP service.\n");
  return BLE_STATUS_ERROR ;
    
}


// TODO implement taptap() that notify the characteristic
 


/******************************************************************************************************/
/************************************************* LED ************************************************/
/******************************************************************************************************/

/**
 * @brief  Add an accelerometer service using a vendor specific profile.
 *
 * @param  None
 * @retval tBleStatus Status
 */
tBleStatus Add_Led_Service(void)
{
  tBleStatus ret;

  uint8_t uuid[16];
  
  COPY_LED_SERVICE_UUID(uuid);
  ret = aci_gatt_add_serv(UUID_TYPE_128,  uuid, PRIMARY_SERVICE, 5,
                          &ledServHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;    
  
  
  COPY_LED_DIR_UUID(uuid);  
  ret =  aci_gatt_add_char(ledServHandle, UUID_TYPE_128, uuid, 2,
                           CHAR_PROP_WRITE,
                           ATTR_PERMISSION_NONE,
                           GATT_NOTIFY_WRITE_REQ_AND_WAIT_FOR_APPL_RESP,
                           16, 0, &ledDirCharHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;
	
	COPY_LED_ON_UUID(uuid);  
  ret =  aci_gatt_add_char(ledServHandle, UUID_TYPE_128, uuid, 2,
                           CHAR_PROP_WRITE,
                           ATTR_PERMISSION_NONE,
                           GATT_NOTIFY_WRITE_REQ_AND_WAIT_FOR_APPL_RESP,
                           16, 0, &ledOnCharHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;
  
  PRINTF("Service LED added. Handle 0x%04X, Dir Charac handle: 0x%04X, On Charac handle:0x%04X\n",ledServHandle, ledDirCharHandle, ledOnCharHandle);	
  return BLE_STATUS_SUCCESS; 
  
fail:
  PRINTF("Error while adding LED service.\n");
  return BLE_STATUS_ERROR ;
    
}



/**
 * @brief  Puts the device in connectable mode.
 *         If you want to specify a UUID list in the advertising data, those data can
 *         be specified as a parameter in aci_gap_set_discoverable().
 *         For manufacture data, aci_gap_update_adv_data must be called.
 * @param  None 
 * @retval None
 */
/* Ex.:
 *
 *  tBleStatus ret;    
 *  const char local_name[] = {AD_TYPE_COMPLETE_LOCAL_NAME,'B','l','u','e','N','R','G'};    
 *  const uint8_t serviceUUIDList[] = {AD_TYPE_16_BIT_SERV_UUID,0x34,0x12};    
 *  const uint8_t manuf_data[] = {4, AD_TYPE_MANUFACTURER_SPECIFIC_DATA, 0x05, 0x02, 0x01};
 *  
 *  ret = aci_gap_set_discoverable(ADV_IND, 0, 0, PUBLIC_ADDR, NO_WHITE_LIST_USE,
 *                                 8, local_name, 3, serviceUUIDList, 0, 0);    
 *  ret = aci_gap_update_adv_data(5, manuf_data);
 *
 */
void setConnectable(void)
{  
  tBleStatus ret;
  
  const char local_name[] = {AD_TYPE_COMPLETE_LOCAL_NAME,'B','L','E','-','G','0','1'};
  
  /* disable scan response */
  hci_le_set_scan_resp_data(0,NULL);
  PRINTF("General Discoverable Mode.\n");
  
  ret = aci_gap_set_discoverable(ADV_IND, 0, 0, PUBLIC_ADDR, NO_WHITE_LIST_USE,
                                 sizeof(local_name), local_name, 0, NULL, 0, 0);
  if (ret != BLE_STATUS_SUCCESS) {
    PRINTF("Error while setting discoverable mode (%d)\n", ret);    
  }  
}

/**
 * @brief  This function is called when there is a LE Connection Complete event.
 * @param  uint8_t Address of peer device
 * @param  uint16_t Connection handle
 * @retval None
 */
void GAP_ConnectionComplete_CB(uint8_t addr[6], uint16_t handle)
{  
  connected = TRUE;
  connection_handle = handle;
  
  PRINTF("Connected to device:");
  for(int i = 5; i > 0; i--){
    PRINTF("%02X-", addr[i]);
  }
  PRINTF("%02X\n", addr[0]);
}

/**
 * @brief  This function is called when the peer device gets disconnected.
 * @param  None 
 * @retval None
 */
void GAP_DisconnectionComplete_CB(void)
{
  connected = FALSE;
  PRINTF("Disconnected\n");
  /* Make the device connectable again. */
  set_connectable = TRUE;
  notification_enabled = FALSE;
}

/**
 * @brief  Write request callback.
 * @param  uint16_t Handle of the attribute
 * @param  uint8_t Length of the value
 * @param  uint8_t Value
 * @retval None
 */
void Write_Request_CB(uint16_t attr_handle, uint8_t att_val_len, uint8_t *att_val){
	
	if(attr_handle == ledOnCharHandle+1) {
    //TODO do something with the incoming data
		printf("callback led with value: %i\n", att_val[0]);
	}
  if(attr_handle == ledDirCharHandle+1) {
    //TODO
  }

//EXIT:
  if(connection_handle != 0){}
    aci_gatt_write_response(connection_handle, attr_handle, 0x00, 0, att_val_len, att_val);
}

//TODO Implement Notify_Request_CB()

/**
 * @brief  Read request callback.
 * @param  uint16_t Handle of the attribute
 * @retval None
 */
void Read_Request_CB(uint16_t handle)
{  
  if(handle == accCharHandle + 1){
    Acc_Update((AxesRaw_t*)&axes_data);
		printf("callback acc");
  }  
    if(handle == accRollCharHandle + 1){
    //Roll_Update(/*TODO*/);
    printf("callback roll");
  }  
    if(handle == accPitchCharHandle + 1){
    //Pitch_Update(/*TODO*/);
    printf("callback pitch");
  }  
    if(handle == tempCharHandle + 1){
    //Temp_Update(/*TODO*/;
    printf("callback temp");
  }  

  
  //EXIT:
  if(connection_handle != 0)
    aci_gatt_allow_read(connection_handle);
}

/**
 * @brief  Callback processing the ACI events.
 * @note   Inside this function each event must be identified and correctly
 *         parsed.
 * @param  void* Pointer to the ACI packet
 * @retval None
 */
void HCI_Event_CB(void *pckt)
{
	
	
  hci_uart_pckt *hci_pckt = pckt;
  /* obtain event packet */
  hci_event_pckt *event_pckt = (hci_event_pckt*)hci_pckt->data;
  
  if(hci_pckt->type != HCI_EVENT_PKT)
    return;
	
  
  switch(event_pckt->evt){
    
  case EVT_DISCONN_COMPLETE:
    {
      GAP_DisconnectionComplete_CB();
    }
    break;
    
  case EVT_LE_META_EVENT:
    {
      evt_le_meta_event *evt = (void *)event_pckt->data;
      
      switch(evt->subevent){
      case EVT_LE_CONN_COMPLETE:
        {
          evt_le_connection_complete *cc = (void *)evt->data;
          GAP_ConnectionComplete_CB(cc->peer_bdaddr, cc->handle);
        }
        break;
      }
    }
    break;
    
  case EVT_VENDOR:
    {
			
			
      evt_blue_aci *blue_evt = (void*)event_pckt->data;
      switch(blue_evt->ecode){

      case EVT_BLUE_GATT_READ_PERMIT_REQ:
        {
          evt_gatt_read_permit_req *pr = (void*)blue_evt->data;                    
          Read_Request_CB(pr->attr_handle);                    
        }
        break;
      
			case EVT_BLUE_GATT_WRITE_PERMIT_REQ:
        {
          evt_gatt_write_permit_req *pr = (void*)blue_evt->data;                    
          Write_Request_CB(pr->attr_handle, pr->data_length, pr->data);                    
        }
        break;
      }
    }
    break;
  }    
}

 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
