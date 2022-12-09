# STMKeys
	因为cubemx修改配置导致所有生成文件还原，因此代码需要放在自建文件中。只修改生成文件中的main.c。
	cubemx生成的模板为HID或者CustomHID.自建文件都在zian1文件夹中。
	Keycode.h为宏定义。
	Functions.h为函数和变量定义。
	ws2812b.c为操作ws2812b。
	Function.c为后台函数实现。
	BluePill.c为前端函数实现。

	usb_keyboard.c为【USBD_ClassTypeDef usbCustomHid】替换。Hal库也只需要替换这个class（仿C++定义的class）
	就可以满足自定义HID Device的需求了。

	为了避免干扰。替换的函数都改了名字。替换核心在以下代码：
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

	//////////////////////////////////////////////
	把usbCustomHid注册到hUsbDevice里面。
	hUsbDevice.pClass = &usbCustomHid;
	//////////////////////////////////////////////

	HAL库是封装了LL库。我们只需要写应用层就行了。
	usbGetFSCfgDesc为获取汇报符。
	usbEP0RxReady自定义句柄处理函数，这里用不到。
	usbGetDeviceQualifierDesc没啥用
	usbInit0主要功能是开启endpoint
	usbDeInit主要功能是关闭endpoint
	usbSetup主要定义endpoint0的中断函数。
	usbDataIn主要定义输入到PC的中断函数。

	usbDataOut主要定义输出到单片机的中断函数。LL库里面为了稳定，一旦启用这个函数，
	会导致usbSetup不会处理字段长度大于0的dataout中断。统一回复stall。这就会导致
	键盘灯等信号无法接收，需要重新初始化一下hUsbDevice。                                   
	/////////////////////////////////////////////
	WS2812b依靠DMA+PWM实现，需要注意的是L151似乎有中断干扰了数组赋值。
