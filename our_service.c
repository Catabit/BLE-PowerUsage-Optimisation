
#include <stdint.h>
#include <string.h>
#include "nrf_gpio.h"
#include "our_service.h"
#include "ble_srv_common.h"
#include "app_error.h"
#include "boards.h"
#include "SEGGER_RTT.h"


// ALREADY_DONE_FOR_YOU: Declaration of a function that will take care of some housekeeping of ble connections related to our service and characteristic
void ble_our_service_on_ble_evt(ble_os_t * p_our_service, ble_evt_t * p_ble_evt)
{
    // OUR_JOB: Step 3.D Implement switch case handling BLE events related to our service. 
		int8_t rssi=0;
		switch (p_ble_evt->header.evt_id)
		{
    case BLE_GAP_EVT_CONNECTED:
        p_our_service->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
				sd_ble_gap_rssi_start(p_our_service->conn_handle, 5, 5); 
        break;
    case BLE_GAP_EVT_DISCONNECTED:
        p_our_service->conn_handle = BLE_CONN_HANDLE_INVALID;
        break;
		
		case BLE_GAP_EVT_RSSI_CHANGED:
						rssi = p_ble_evt->evt.gap_evt.params.rssi_changed.rssi;
						p_our_service->lastRSSI = rssi;
						if (rssi<-80) {	
							SEGGER_RTT_printf(0, "RSSI is bad, %d!\n", rssi);
							rssi = 'C';						
							nrf_gpio_pin_set(LED_3);
							nrf_gpio_pin_set(LED_4);		
							
						}
						else if (rssi<-65) {
							SEGGER_RTT_printf(0, "RSSI is ok, %d!\n", rssi);
							rssi = 'B';
							nrf_gpio_pin_set(LED_3);
							nrf_gpio_pin_clear(LED_4);						
							
						}
						else if (rssi<-55) {
							SEGGER_RTT_printf(0, "RSSI is good, %d!\n", rssi);
							rssi = 'A';
							nrf_gpio_pin_clear(LED_3);
							nrf_gpio_pin_clear(LED_4);
						}
						
						
						//our_termperature_characteristic_update(p_our_service, &rssi);
						
						break;

    default:
        // No implementation needed.
        break;
		}
}

/**@brief Function for adding our new characterstic to "Our service" that we initiated in the previous tutorial. 
 *
 * @param[in]   p_our_service        Our Service structure.
 *
 */
static uint32_t our_char_add(ble_os_t * p_our_service)
{
    // OUR_JOB: Step 2.A, Add a custom characteristic UUID
		uint8_t err_code;
		ble_uuid_t char_uuid;
		ble_uuid128_t base_uuid = BLE_UUID_OUR_BASE_UUID;
		char_uuid.uuid = BLE_UUID_OUR_CHARACTERISTC_UUID;
		err_code = sd_ble_uuid_vs_add(&base_uuid, &char_uuid.type);
		APP_ERROR_CHECK(err_code);
    
    // OUR_JOB: Step 2.F Add read/write properties to our characteristic
    ble_gatts_char_md_t char_md;
    memset(&char_md, 0, sizeof(char_md));
		char_md.char_props.read = 1;
		char_md.char_props.write = 0;

    
    // OUR_JOB: Step 3.A, Configuring Client Characteristic Configuration Descriptor metadata and add to char_md structure
    ble_gatts_attr_md_t cccd_md;
    memset(&cccd_md, 0, sizeof(cccd_md));
		BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
		BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
		cccd_md.vloc = BLE_GATTS_VLOC_STACK;
		char_md.p_cccd_md = &cccd_md;
		char_md.char_props.notify = 1;
   
    
    // OUR_JOB: Step 2.B, Configure the attribute metadata
    ble_gatts_attr_md_t attr_md;
    memset(&attr_md, 0, sizeof(attr_md)); 
		attr_md.vloc = BLE_GATTS_VLOC_STACK;
    
    
    // OUR_JOB: Step 2.G, Set read/write security levels to our characteristic
		BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
		//BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    
    
    // OUR_JOB: Step 2.C, Configure the characteristic value attribute
    ble_gatts_attr_t    attr_char_value;
    memset(&attr_char_value, 0, sizeof(attr_char_value));
		attr_char_value.p_attr_md = &attr_md;
		attr_char_value.p_uuid = &char_uuid;
    
    // OUR_JOB: Step 2.H, Set characteristic length in number of bytes
		attr_char_value.max_len = 12;
		attr_char_value.init_len = 12;
		attr_char_value.p_value =(uint8_t*) &p_our_service->xyz;

    // OUR_JOB: Step 2.E, Add our new characteristic to the service
		err_code = sd_ble_gatts_characteristic_add((*p_our_service).service_handle, &char_md, &attr_char_value, &(*p_our_service).char_handles);
		APP_ERROR_CHECK(err_code);


    return NRF_SUCCESS;
}


/**@brief Function for initiating our new service.
 *
 * @param[in]   p_our_service        Our Service structure.
 *
 */
void our_service_init(ble_os_t * p_our_service, app_timer_id_t sendTimer)
{
    uint32_t   err_code; // Variable to hold return codes from library and softdevice functions
		
		p_our_service->timesPressed = 0;
		p_our_service->lastRSSI = 0;
		p_our_service->nextInterval = APP_TIMER_TICKS(1000, 0);
		p_our_service->nextRSSITreshold = -45;
		p_our_service->sendTimer = sendTimer;
		

    // FROM_SERVICE_TUTORIAL: Declare 16-bit service and 128-bit base UUIDs and add them to the BLE stack
    ble_uuid_t        service_uuid;
    ble_uuid128_t     base_uuid = BLE_UUID_OUR_BASE_UUID;
    service_uuid.uuid = BLE_UUID_OUR_SERVICE_UUID;
    err_code = sd_ble_uuid_vs_add(&base_uuid, &service_uuid.type);
    APP_ERROR_CHECK(err_code);    
    
    // OUR_JOB: Step 3.B, Set our service connection handle to default value. I.e. an invalid handle since we are not yet in a connection.
		(*p_our_service).conn_handle = BLE_CONN_HANDLE_INVALID;
	
	
    // FROM_SERVICE_TUTORIAL: Add our service
    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                        &service_uuid,
                                        &p_our_service->service_handle);
    
    APP_ERROR_CHECK(err_code);
    
    // OUR_JOB: Call the function our_char_add() to add our new characteristic to the service. 
    our_char_add(p_our_service);
}

// ALREADY_DONE_FOR_YOU: Function to be called when updating characteristic value
void notify_sensor_data(ble_os_t *p_our_service, int32_t *data)
{
    // OUR_JOB: Step 3.E, Update characteristic value
	if (p_our_service->conn_handle != BLE_CONN_HANDLE_INVALID)
	{	
			uint16_t len = 12;
			ble_gatts_hvx_params_t hvx_params;
			memset(&hvx_params, 0, sizeof(hvx_params));
		
			hvx_params.handle = (*p_our_service).char_handles.value_handle;
			hvx_params.type = BLE_GATT_HVX_NOTIFICATION;
			hvx_params.offset = 0;
			hvx_params.p_len = &len;
			hvx_params.p_data = (uint8_t*) data;
		
		
			
			sd_ble_gatts_hvx((*p_our_service).conn_handle, &hvx_params);
		
	}

}
