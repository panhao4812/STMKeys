#include "Functions.h"
#include "usbd_customhid.h"

/////
/* 配置CubeMX中的 RCC\USRT(SWD)\USB_HID\Timer\TIM2\DMA\GPIO\
 * 新增文件 Keycode.h
 * 新增文件 Functions.h
 * 新增文件 Functions.c
 * 新增文件 ws2812.c
 * 新增文件 usb_keyboard.c//替换原有的USBD_ClassTypeDef
 * 新增文件 BluePill.c
 * 修改文件 main.c //添加init_main() 禁用默认void MX_USB_DEVICE_Init(void)
 */
USBD_HandleTypeDef hUsbDevice;//usb传值工具

static uint8_t KeyboardReport[] = {
		0x05, 0x01, // Usage Page (Generic Desktop),
		0x09, 0x06,          // Usage (Keyboard),
		0xA1, 0x01,          // Collection (Application),
		0x75, 0x01,          //   Report Size (1),
		0x95, 0x08,          //   Report Count (8),
		0x05, 0x07,          //   Usage Page (Key Codes),
		0x19, 0xE0,          //   Usage Minimum (224),
		0x29, 0xE7,          //   Usage Maximum (231),
		0x15, 0x00,          //   Logical Minimum (0),
		0x25, 0x01,          //   Logical Maximum (1),
		0x81, 0x02,        //   Input (Data, Variable, Absolute), ;Modifier byte
		0x95, 0x01,          //   Report Count (1),
		0x75, 0x08,          //   Report Size (8),
		0x81, 0x03,        //   Input (Constant),                 ;Reserved byte
		0x95, 0x05,          //   Report Count (5),
		0x75, 0x01,          //   Report Size (1),
		0x05, 0x08,          //   Usage Page (LEDs),
		0x19, 0x01,          //   Usage Minimum (1),
		0x29, 0x05,          //   Usage Maximum (5),
		0x91, 0x02,          //   Output (Data, Variable, Absolute), ;LED report
		0x95, 0x01,          //   Report Count (1),
		0x75, 0x03,          //   Report Size (3),
		0x91, 0x03,  //   Output (Constant),                 ;LED report padding
		0x95, 0x06,          //   Report Count (6),
		0x75, 0x08,          //   Report Size (8),
		0x15, 0x00,          //   Logical Minimum (0),
		0x26, 0xFF, 0x00,    //   Logical Maximum(255),
		0x05, 0x07,          //   Usage Page (Key Codes),
		0x19, 0x00,          //   Usage Minimum (0),
		0x29, 0xFF,          //   Usage Maximum (255),
		0x81, 0x00,          //   Input (Data, Array),
		0xc0                 // End Collection
		};

static uint8_t MouseReport[] = {
/* mouse */
		0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
		0x09, 0x02,                    // USAGE (Mouse)
		0xa1, 0x01,                    // COLLECTION (Application)
		0x85, REPORT_ID_MOUSE,         //   REPORT_ID (1)
		0x09, 0x01,                    //   USAGE (Pointer)
		0xa1, 0x00,                    //   COLLECTION (Physical)
		// ----------------------------  Buttons
		0x05, 0x09,                    //     USAGE_PAGE (Button)
		0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
		0x29, 0x05,                    //     USAGE_MAXIMUM (Button 5)
		0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
		0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
		0x75, 0x01,                    //     REPORT_SIZE (1)
		0x95, 0x05,                    //     REPORT_COUNT (5)
		0x81, 0x02,                    //     INPUT (Data,Var,Abs)
		0x75, 0x03,                    //     REPORT_SIZE (3)
		0x95, 0x01,                    //     REPORT_COUNT (1)
		0x81, 0x03,                    //     INPUT (Cnst,Var,Abs)
		// ----------------------------  X,Y position
		0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
		0x09, 0x30,                    //     USAGE (X)
		0x09, 0x31,                    //     USAGE (Y)
		0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
		0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
		0x75, 0x08,                    //     REPORT_SIZE (8)
		0x95, 0x02,                    //     REPORT_COUNT (2)
		0x81, 0x06,                    //     INPUT (Data,Var,Rel)
		// ----------------------------  Vertical wheel
		0x09, 0x38,                    //     USAGE (Wheel)
		0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
		0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
		0x35, 0x00,          //     PHYSICAL_MINIMUM (0)        - reset physical
		0x45, 0x00,                    //     PHYSICAL_MAXIMUM (0)
		0x75, 0x08,                    //     REPORT_SIZE (8)
		0x95, 0x01,                    //     REPORT_COUNT (1)
		0x81, 0x06,                    //     INPUT (Data,Var,Rel)
		// ----------------------------  Horizontal wheel
		0x05, 0x0c,                    //     USAGE_PAGE (Consumer Devices)
		0x0a, 0x38, 0x02,              //     USAGE (AC Pan)
		0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
		0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
		0x75, 0x08,                    //     REPORT_SIZE (8)
		0x95, 0x01,                    //     REPORT_COUNT (1)
		0x81, 0x06,                    //     INPUT (Data,Var,Rel)
		0xc0,                          //   END_COLLECTION
		0xc0,                          // END_COLLECTION
		/* system control */
		0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
		0x09, 0x80,                    // USAGE (System Control)
		0xa1, 0x01,                    // COLLECTION (Application)
		0x85, REPORT_ID_SYSTEM,        //   REPORT_ID (2)
		0x15, 0x01,                    //   LOGICAL_MINIMUM (0x1)
		0x26, 0xb7, 0x00,              //   LOGICAL_MAXIMUM (0xb7)
		0x19, 0x01,                    //   USAGE_MINIMUM (0x1)
		0x29, 0xb7,                    //   USAGE_MAXIMUM (0xb7)
		0x75, 0x10,                    //   REPORT_SIZE (16)
		0x95, 0x01,                    //   REPORT_COUNT (1)
		0x81, 0x00,                    //   INPUT (Data,Array,Abs)
		0xc0,                          // END_COLLECTION
		/* consumer */
		0x05, 0x0c,                    // USAGE_PAGE (Consumer Devices)
		0x09, 0x01,                    // USAGE (Consumer Control)
		0xa1, 0x01,                    // COLLECTION (Application)
		0x85, REPORT_ID_CONSUMER,      //   REPORT_ID (3)
		0x15, 0x01,                    //   LOGICAL_MINIMUM (0x1)
		0x26, 0x9c, 0x02,              //   LOGICAL_MAXIMUM (0x29c)
		0x19, 0x01,                    //   USAGE_MINIMUM (0x1)
		0x2a, 0x9c, 0x02,              //   USAGE_MAXIMUM (0x29c)
		0x75, 0x10,                    //   REPORT_SIZE (16)
		0x95, 0x01,                    //   REPORT_COUNT (1)
		0x81, 0x00,                    //   INPUT (Data,Array,Abs)
		0xc0                        // END_COLLECTION
		};
static uint8_t RawReport[] = {
		0x06, 0x31, 0xFF, //Usage Page (Vendor-Defined 50 31FF)
		0x09, 0x74,                        //Usage (Vendor-Defined 116)
		0xA1, 0x01,                        //Collection (Application)
		0x09, 0x75,                        //Usage (Vendor-Defined 117)
		0x15, 0x00,                        //Logical Minimum (0)
		0x26, 0xFF, 0x00,                        //Logical Maximum (255 FF00)
		0x95, 0x08,                        //Report Count (8)
		0x75, 0x08,                        //Report Size (8)
		0x81, 0x02,                //Input (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit)
		0x09, 0x76,                        //Usage (Vendor-Defined 118)
		0x15, 0x00,                        //Logical Minimum (0)
		0x26, 0xFF, 0x00,                        //Logical Maximum (255)
		0x95, 0x08, //Report Count (8)
		0x75, 0x08, //Report Size (8)
		0x91, 0x02, //Output (Data,Var,Abs,NWrp,Lin,Pref,NNul,NVol,Bit)
		0xC0 //End Collection
		};
static uint8_t config1_descriptor[CONFIG1_DESC_SIZE] = {
		9, // sizeof(usbDescrInterface): length of descriptor in bytes
		0x02,
		CONFIG1_DESC_SIZE, 0x00, //9+(9+9+7)+(9+9+7+7)
		0x03, // number of interfaces in this configuration
		1, // index of this configuration
		0, // configuration name string index
		0xA0, //SelfPower
		0x32, // max USB current in 2mA units
		//////////////////////////////////////////////////////////////////////////
		//Interface Descriptor 0/0 HID, 1 Endpoint
		9,// sizeof(usbDescrInterface): length of descriptor in bytes
		0x04, // descriptor type
		KEYBOARD_INTERFACE, // index of this interface
		0, // alternate setting for this interface
		0x01, //numEndPoint
		0x03, //hid
		0x01, //Boot
		0x01, //0 null 1 keyboard 2 mouse
		0, // string index for interface
		   //////////////////////////////////////////////////////////////////////////
		   //HID descriptor
		9, // sizeof(usbDescrHID): length of descriptor in bytes
		0x21, // descriptor type: HID
		0x11, 0x01, // BCD representation of HID version
		0x00, // target country code
		0x01, // number of HID Report (or other HID class) Descriptor infos to follow
		0x22, // descriptor type: report
		sizeof(KeyboardReport) & 0xFF, 0x00, // total length of report descriptor
		//////////////////////////////////////////////////////////////////////////
		//endpoint descriptor for endpoint 1
		0x07,// sizeof(usbDescrEndpoint)
		0x05, // descriptor type = endpoint
		0x80 + KEYBOARD_ENDPOINT, // IN endpoint number 1
		0x03, // attrib: Interrupt endpoint
		KEYBOARD_SIZE, 0x00, // maximum packet size
		0x01, // in ms
		//////////////////////////////////////////////////////////////////////////
		//Interface Descriptor 1/0 HID, 1 Endpoints
		0x09,//length of descriptor in bytes
		0x04,  //interface
		MOUSE_INTERFACE,  //interface number
		0x00,  // alternate setting for this interface
		0x01,  //numEndPoint
		0x03, //hid
		0x01, //boot
		0x02, //mouse
		0x00, // string index for interface
		//////////////////////////////////////////////////////////////////////////
		//HID descriptor
		0x09,//length of descriptor in bytes
		0x21,		// descriptor type: HID
		0x11, 0x01, 0x00, 0x01, 0x22, sizeof(MouseReport) & 0xFF, 0x00,
		//////////////////////////////////////////////////////////////////////////
		//endpoint descriptor for endpoint 4
		0x07,// sizeof(usbDescrEndpoint) length of descriptor in bytes
		0x05, // descriptor type = endpoint
		0x80 + MOUSE_ENDPOINT, // IN endpoint number 1
		0x03, // attrib: Interrupt endpoint
		MOUSE_SIZE, 0x00, // maximum packet size
		0x01, // in ms
		//////////////////////////////////////////////////////////////////////////
		//Interface Descriptor 1/0 HID, 2 Endpoints
		0x09,//length of descriptor in bytes
		0x04,		//interface
		RAW_INTERFACE, //interface number
		0x00, // alternate setting for this interface
		0x02, //numEndPoint
		0x03, //hid
		0x00, //no boot
		0x00, //custom CDC
		0x00, // string index for interface
		//////////////////////////////////////////////////////////////////////////
		//HID descriptor
		0x09,//length of descriptor in bytes
		0x21,		// descriptor type: HID
		0x11, 0x01, 0x00, 0x01, 0x22, sizeof(RawReport) & 0xFF, 0x00,
		//////////////////////////////////////////////////////////////////////////
		//endpoint descriptor for endpoint 2
		0x07,// sizeof(usbDescrEndpoint)
		0x05, // descriptor type = endpoint
		0x80 + RAW_ENDPOINT_IN, // IN endpoint number 1
		0x03, // attrib: Interrupt endpoint
		RAW_EPSIZE, 0x00, // maximum packet size
		0x01, // in ms
		//////////////////////////////////////////////////////////////////////////
		//endpoint descriptor for endpoint 3
		0x07,// sizeof(usbDescrEndpoint)
		0x05, // descriptor type = endpoint
		RAW_ENDPOINT_OUT, // IN endpoint number 1
		0x03, // attrib: Interrupt endpoint
		RAW_EPSIZE, 0x00, // maximum packet size
		0x01 // in ms
		};
static uint8_t usbDeviceQualifierDesc[] =
		{
		USB_LEN_DEV_QUALIFIER_DESC,
		USB_DESC_TYPE_DEVICE_QUALIFIER,
		0x00,
		0x02,
		0x00,
		0x00,
		0x00,
		0x40,
		0x01,
		0x00,
		};
/////////////////////class USBD_HID/////////////////////////
static uint8_t  usbInit0(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
	/* Open EP IN */
	  USBD_LL_OpenEP(pdev, (0x80U+KEYBOARD_ENDPOINT), USBD_EP_TYPE_INTR, KEYBOARD_SIZE);
	  pdev->ep_in[(0x80U+KEYBOARD_ENDPOINT) & 0xFU].is_used = 1U;
	  USBD_LL_OpenEP(pdev, (0x80U+MOUSE_ENDPOINT), USBD_EP_TYPE_INTR, MOUSE_SIZE);
	  pdev->ep_in[(0x80U+MOUSE_ENDPOINT) & 0xFU].is_used = 1U;
	  USBD_LL_OpenEP(pdev, (0x80U+RAW_ENDPOINT_IN), USBD_EP_TYPE_INTR, RAW_EPSIZE);
	  pdev->ep_in[(0x80U+RAW_ENDPOINT_IN) & 0xFU].is_used = 1U;
	  USBD_LL_OpenEP(pdev, (RAW_ENDPOINT_OUT), USBD_EP_TYPE_INTR, RAW_EPSIZE);
	  pdev->ep_out[(RAW_ENDPOINT_OUT) & 0xFU].is_used = 1U;

	  pdev->pClassData = USBD_malloc(sizeof(USBD_CUSTOM_HID_HandleTypeDef));
	  if (pdev->pClassData == NULL)
	  {
	    return USBD_FAIL;
	  }
	  ((USBD_CUSTOM_HID_HandleTypeDef *)pdev->pClassData)->state = CUSTOM_HID_IDLE;
	  ((USBD_CUSTOM_HID_ItfTypeDef *)pdev->pUserData)->Init();
	  USBD_LL_PrepareReceive(pdev, RAW_ENDPOINT_OUT, (uint8_t*)(&raw_report_out), 8);

	  return USBD_OK;
}

static uint8_t  usbDeInit(USBD_HandleTypeDef *pdev,
                              uint8_t cfgidx)
{
	/* Close HID EPs */
		USBD_LL_CloseEP(pdev, (0x80U + KEYBOARD_ENDPOINT));
		pdev->ep_in[(0x80U + KEYBOARD_ENDPOINT) & 0xFU].is_used = 0U;

		USBD_LL_CloseEP(pdev, (0x80U + MOUSE_ENDPOINT));
		pdev->ep_in[(0x80U + MOUSE_ENDPOINT) & 0xFU].is_used = 0U;

		USBD_LL_CloseEP(pdev, (0x80U + RAW_ENDPOINT_IN));
		pdev->ep_in[(0x80U + RAW_ENDPOINT_IN) & 0xFU].is_used = 0U;

		USBD_LL_CloseEP(pdev, (RAW_ENDPOINT_OUT));
		pdev->ep_out[(RAW_ENDPOINT_OUT) & 0xFU].is_used = 0U;
		/* FRee allocated memory */
		if (pdev->pClassData != NULL) {
			((USBD_CUSTOM_HID_ItfTypeDef *)pdev->pUserData)->DeInit();
			USBD_free(pdev->pClassData);
			pdev->pClassData = NULL;
		}

		return USBD_OK;
}
//ep0的中断
static uint8_t  usbSetup(USBD_HandleTypeDef *pdev,
                             USBD_SetupReqTypedef *req)
{
	USBD_CUSTOM_HID_HandleTypeDef *hhid = (USBD_CUSTOM_HID_HandleTypeDef*) pdev->pClassData;
		uint16_t len = 0U;
		uint8_t *pbuf = NULL;
		uint16_t status_info = 0U;
		USBD_StatusTypeDef ret = USBD_OK;
	//	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		switch (req->bmRequest & USB_REQ_TYPE_MASK) {
		case USB_REQ_TYPE_CLASS:
			switch (req->bRequest) {
			case CUSTOM_HID_REQ_GET_PROTOCOL:
				USBD_CtlSendData(pdev, (uint8_t*) (void*) &hhid->Protocol, 1U);
				break;
			case CUSTOM_HID_REQ_GET_IDLE:
				USBD_CtlSendData(pdev, (uint8_t*) (void*) &hhid->IdleState, 1U);
				break;
			case CUSTOM_HID_REQ_GET_REPORT:
					if (req->wIndex == KEYBOARD_INTERFACE) {
						USBD_CtlSendData(pdev , (uint8_t*) &keyboard_report, 8);
					} else if (req->wIndex == MOUSE_INTERFACE) {
						USBD_CtlSendData(pdev, (uint8_t*) (&mouse_report.mouse), 6);
					}
					//to use in boot mode
				break;
			case CUSTOM_HID_REQ_SET_PROTOCOL:
				hhid->Protocol = (uint8_t) (req->wValue);
				break;
			case CUSTOM_HID_REQ_SET_IDLE:
				hhid->IdleState = (uint8_t) (req->wValue >> 8);
				break;
			case CUSTOM_HID_REQ_SET_REPORT:
				if (req->wIndex == KEYBOARD_INTERFACE) {
					hhid->IsReportAvailable = 1U;
	//bmRequest=0x21 capslock LED.Only USBD_CtlPrepareRx function can be used.
					USBD_CtlPrepareRx(pdev, &(keyboard_buffer.keyboard_leds), 1);
				}
				break;
			default:
				USBD_CtlError(pdev, req);
				ret = USBD_FAIL;
				break;
			}
			break;
		case USB_REQ_TYPE_STANDARD:
			switch (req->bRequest) {
			case USB_REQ_GET_STATUS:
				if (pdev->dev_state == USBD_STATE_CONFIGURED) {
					USBD_CtlSendData(pdev, (uint8_t*) (void*) &status_info, 2U);
				} else {
					USBD_CtlError(pdev, req);
					ret = USBD_FAIL;
				}
				break;

			case USB_REQ_GET_DESCRIPTOR:
				if ((req->wValue >> 8 == CUSTOM_HID_REPORT_DESC)
						&& (req->wIndex == KEYBOARD_INTERFACE)) {
					len = MIN(sizeof(KeyboardReport), req->wLength);
					pbuf = KeyboardReport;
				} else if ((req->wValue >> 8 == CUSTOM_HID_DESCRIPTOR_TYPE)
						&& (req->wIndex == KEYBOARD_INTERFACE)) {
					pbuf = config1_descriptor + KEYBOARD_HID_DESC_OFFSET;
					len = MIN(9, req->wLength);
				} else if ((req->wValue >> 8 == CUSTOM_HID_REPORT_DESC)
						&& (req->wIndex == RAW_INTERFACE)) {
					len = MIN(sizeof(RawReport), req->wLength);
					pbuf = RawReport;
				} else if ((req->wValue >> 8 == CUSTOM_HID_DESCRIPTOR_TYPE)
						&& (req->wIndex == RAW_INTERFACE)) {
					pbuf = config1_descriptor + RAW_HID_DESC_OFFSET;
					len = MIN(9, req->wLength);
				} else if ((req->wValue >> 8 == CUSTOM_HID_REPORT_DESC)
						&& (req->wIndex == MOUSE_INTERFACE)) {
					len = MIN(sizeof(MouseReport), req->wLength);
					pbuf = MouseReport;
				} else if ((req->wValue >> 8 == CUSTOM_HID_DESCRIPTOR_TYPE)
						&& (req->wIndex == MOUSE_INTERFACE)) {
					pbuf = config1_descriptor + MOUSE_HID_DESC_OFFSET;
					len = MIN(9, req->wLength);
				} else {
					USBD_CtlError(pdev, req);
					ret = USBD_FAIL;
					break;
				}
				USBD_CtlSendData(pdev, pbuf, len);
				break;

			case USB_REQ_GET_INTERFACE:
				if (pdev->dev_state == USBD_STATE_CONFIGURED) {
					USBD_CtlSendData(pdev, (uint8_t*) (void*) &hhid->AltSetting,
							1U);
				} else {
					USBD_CtlError(pdev, req);
					ret = USBD_FAIL;
				}
				break;

			case USB_REQ_SET_INTERFACE:
				if (pdev->dev_state == USBD_STATE_CONFIGURED) {
					hhid->AltSetting = (uint8_t) (req->wValue);
				} else {
					USBD_CtlError(pdev, req);
					ret = USBD_FAIL;
				}
				break;

			default:
				USBD_CtlError(pdev, req);
				ret = USBD_FAIL;
				break;
			}
			break;

		default:
			USBD_CtlError(pdev, req);
			ret = USBD_FAIL;
			break;
		}
		return ret;
}
static uint8_t  usbDataIn(USBD_HandleTypeDef *pdev,uint8_t epnum)
{
  /* Ensure that the FIFO is empty before a new transfer, this condition could
  be caused by  a new transfer before the end of the previous transfer */
  ((USBD_CUSTOM_HID_HandleTypeDef *)pdev->pClassData)->state = CUSTOM_HID_IDLE;
  return USBD_OK;
}
static uint8_t usbDataOut(USBD_HandleTypeDef *pdev, uint8_t epnum) {
//receive data from PC
	//((USBD_CUSTOM_HID_ItfTypeDef*) pdev->pUserData)->OutEvent(hhid->Report_buf[0], hhid->Report_buf[1]);
	//endpoint4接收信息后endpoint0出现stall，应该是被锁止了。需要在main里重启usb。
	USBD_CUSTOM_HID_HandleTypeDef *hhid = (USBD_CUSTOM_HID_HandleTypeDef*) pdev->pClassData;
	if ((pdev->dev_state == USBD_STATE_CONFIGURED )&&(hhid->state == CUSTOM_HID_IDLE)) {
	if (enableReset!=0){
				enableReset=HAL_PCD_EP_Receive(pdev->pData,RAW_ENDPOINT_OUT,(uint8_t *)&raw_report_out,RAW_EPSIZE);
        }
	}else{return USBD_BUSY;}
	 return USBD_OK;
}
static uint8_t usbEP0RxReady(USBD_HandleTypeDef *pdev)
{
  USBD_CUSTOM_HID_HandleTypeDef *hhid = (USBD_CUSTOM_HID_HandleTypeDef *)pdev->pClassData;
  if (hhid->IsReportAvailable == 1U)
  {

		//((USBD_CUSTOM_HID_ItfTypeDef*) pdev->pUserData)->OutEvent(hhid->Report_buf[0], hhid->Report_buf[1]);
    hhid->IsReportAvailable = 0U;
  }
  return USBD_OK;
}
static uint8_t  *usbGetFSCfgDesc(uint16_t *length){
	 *length = sizeof(config1_descriptor);
	  return config1_descriptor;
}
uint8_t usbConfigured(USBD_HandleTypeDef *pdev) {
	USBD_CUSTOM_HID_HandleTypeDef *hhid = (USBD_CUSTOM_HID_HandleTypeDef*) pdev->pClassData;
	if ((pdev->dev_state != USBD_STATE_CONFIGURED)
			|| (hhid->state != CUSTOM_HID_IDLE)) {
		return 1;
	}
	return 0;
}
static uint8_t  *usbGetDeviceQualifierDesc(uint16_t *length)
{
  *length = sizeof(usbDeviceQualifierDesc);
  return usbDeviceQualifierDesc;
}
uint8_t usbSend(uint8_t endpoint, uint8_t *buffer, uint8_t buffersize,
		uint8_t timeout) {
	uint8_t Counter = timeout;
	USBD_CUSTOM_HID_HandleTypeDef *hhid =
			(USBD_CUSTOM_HID_HandleTypeDef*) (&hUsbDevice)->pClassData;
	while (((&hUsbDevice)->dev_state != USBD_STATE_CONFIGURED)
			|| (hhid->state != CUSTOM_HID_IDLE)) {
		if (Counter < 1) {
			break;
		} else {
			Counter--;
			HAL_Delay(timeout - Counter);
		}
	}
	hhid->state = CUSTOM_HID_BUSY;
	if (Counter < 1) {
		return USBD_BUSY;
	} else {
		return USBD_LL_Transmit(&hUsbDevice, (0x80U + endpoint), buffer,
				(uint16_t) buffersize);
	}
}
uint8_t usbRecv(uint8_t endpoint, uint8_t *buffer, uint8_t buffersize,
		uint8_t timeout) {
	uint8_t Counter = timeout;
	USBD_CUSTOM_HID_HandleTypeDef *hhid =
			(USBD_CUSTOM_HID_HandleTypeDef*) (&hUsbDevice)->pClassData;
	while (((&hUsbDevice)->dev_state != USBD_STATE_CONFIGURED)
			|| (hhid->state != CUSTOM_HID_IDLE)) {
		if (Counter < 1) {
			break;
		} else {
			Counter--;
			//HAL_Delay(timeout - Counter);
		}
	}
	hhid->state = CUSTOM_HID_BUSY;
	if (Counter < 1) {
		return USBD_BUSY;
	} else {
		return HAL_PCD_EP_Receive(hUsbDevice.pData, endpoint, buffer,
				(uint16_t) buffersize);
	}
}
USBD_ClassTypeDef usbCustomHid = {
		usbInit0,
		usbDeInit,
		usbSetup,
		NULL, /*EP0_TxSent*/
		usbEP0RxReady, /*EP0_RxReady*//* STATUS STAGE IN */
		usbDataIn, /*DataIn*/
		usbDataOut,
		NULL, /*SOF */
		NULL,
		NULL,
		usbGetFSCfgDesc,
		usbGetFSCfgDesc,
		usbGetFSCfgDesc,
		usbGetDeviceQualifierDesc,
};
void usbInit(void) {
	/* Init Device Library, add supported class and start the library. */
	if (USBD_Init(&hUsbDevice, &FS_Desc, DEVICE_FS) != USBD_OK) {
		Error_Handler();
	}
	/* link the class to the USB Device handle */
	/*
	 if (USBD_RegisterClass(&hUsbDevice, &USBD_CUSTOM_HID) != USBD_OK)
	  {
	    Error_Handler();
	  }
	  */
	hUsbDevice.pClass = &usbCustomHid;
	if (USBD_CUSTOM_HID_RegisterInterface(&hUsbDevice, &USBD_CustomHID_fops_FS)
			!= USBD_OK) {
		Error_Handler();
	}
	if (USBD_Start(&hUsbDevice) != USBD_OK) {
		Error_Handler();
	}
    clearKeyboard();
  	clearMouse();
  	clearRaw();
  	clearMacro();
}


