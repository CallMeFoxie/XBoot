/*
 * usb.c
 *
 * Created: 29.12.2012 17:25:47
 *  Author: Ondra
 */ 

#include "usb.h"
#include "cdc/cdc.h"
//#ifdef DEBUG
#include "../../uart/uart.h"
//#endif

void USB_Init()
{
	Clock_Set_USB(32); // 32MHz
	PMIC_Enable(PMIC_HILVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_LOLVLEN_bm);
	USB_Set_Registers();
	USB_Reset();
	#ifdef DEBUG
	uart_println("USB drv: init done");
	#endif
}

void USB_Reset()
{
	USB_Reset_Everything();
	USB_Set_Address(0);
	USB_State = Powered;
}

void USB_Reset_Everything()
{	
	USB_Endpoint_Reset(0);
	USB_Endpoint_Reset(1);
	USB_Endpoint_Reset(2);
	USB_Endpoint_Reset(3);
	USB_Endpoint_Reset(4);
	USB_Endpoint_Reset(5);
	
	USB_Endpoint_Set_CTRL(0, USB_EP_TYPE_CONTROL_gc | USB_EP_BUFSIZE_64_gc);
	USB_Endpoint_Set_CTRL(1, USB_EP_TYPE_CONTROL_gc | USB_EP_BUFSIZE_64_gc); // no AZLP nor auxdata -- we are doing it manually due to sillicon errata on A1U chips
	USB_Endpoint_Set_CTRL(2, USB_EP_TYPE_DISABLE_gc);
	USB_Endpoint_Set_CTRL(3, USB_EP_TYPE_BULK_gc | USB_EP_BUFSIZE_64_gc);
	USB_Endpoint_Set_CTRL(4, USB_EP_TYPE_BULK_gc | USB_EP_BUFSIZE_64_gc);
	USB_Endpoint_Set_CTRL(5, USB_EP_TYPE_BULK_gc | USB_EP_BUFSIZE_64_gc); // actually inter	rupt, but they are the same for XMega
	
	USB_Set_DATA(EP_CTRL_OUT, usb_ram.payload[0]);
	USB_Set_DATA(EP_DATA_OUT, usb_ram.payload[1]);
	USB_Set_DATA(EP_NOTIF_OUT, usb_ram.payload[2]);
	
}

void USB_Set_Registers()
{
	USB.CTRLA = 0;
	USB.CTRLB = 0;
	
	USB.CTRLA = XBOOT_ENDPOINTS;
	USB.CTRLA |= USB_SPEED_bm;
	USB.CTRLA |= USB_ENABLE_bm;
	USB.CTRLA |= USB_STFRNUM_bm;
	USB.EPPTR = (u16)usb_ram.endpoint;
	USB.CTRLA |= USB_FIFOEN_bm;
	USB.FIFORP = 0xFF;
	USB.INTCTRLA = USB_INTLVL_HI_gc | /*USB_SOFIE_bm | */USB_BUSEVIE_bm/* | USB_BUSERRIE_bm | USB_STALLIE_bm*/;
	USB.INTCTRLB = USB_TRNIE_bm | USB_SETUPIE_bm;
	
	//USB.CAL0 = 0xCF;
	//USB.CAL1 = 0x07;
	
	//USB.CTRLB = USB_PULLRST_bm;
	//USB.CTRLA = USB_ENABLE_bm | USB_SPEED_bm | (XBOOT_ENDPOINTS) | USB_FIFOEN_bm;
	
}

void USB_Attach()
{
	
	USB_Ack_Suspend();
	USB_Ack_Resume();

	USB.CTRLB |= USB_ATTACH_bm;

	
}

void USB_Detach()
{
	USB.CTRLB &= ~USB_ATTACH_bm;
}

void USB_Fetch_Descriptor(USB_DeviceRequest_t* Packet) 
{
	switch(Packet->wValue >> 8)
	{
		case USB_Request_Descriptor_DEVICE:
			USB_Send_Data(EP_CTRL_IN, &USB_Descriptor_Device, USB_Descriptor_Device.bLength);
			USB_Ack(EP_CTRL_IN);
			USB_Ack(EP_CTRL_OUT);
			#ifdef DEBUG
			uart_println("Descriptor device");
			#endif
		break;
		case USB_Request_Descriptor_CONFIGURATION:
			USB_Send_Data(EP_CTRL_IN, &USB_Descriptor_Configuration, USB_Descriptor_Configuration.Configuration.wTotalLength)
			USB_Ack(EP_CTRL_IN);
			USB_Ack(EP_CTRL_OUT);
			#ifdef DEBUG
			uart_println("Descriptor configuration");
			#endif
		break;
		case USB_Request_Descriptor_STRING:
			switch(Packet->wValue & 0xFF)
			{
				case USB_STRING_MANUFACTURER:
					USB_Send_Data(EP_CTRL_IN, &USB_Descriptor_String_Manufacturer, USB_Descriptor_String_Manufacturer.bLength);
					USB_Ack(EP_CTRL_IN);
					USB_Ack(EP_CTRL_OUT);
					#ifdef DEBUG
					uart_println("Descriptor string manufacturer");
					#endif
				break;
				case USB_STRING_PRODUCT:
					USB_Send_Data(EP_CTRL_IN, &USB_Descriptor_String_Product, USB_Descriptor_String_Product.bLength);
					USB_Ack(EP_CTRL_IN);
					USB_Ack(EP_CTRL_OUT);
					#ifdef DEBUG
					uart_println("Descriptor string product");
					#endif
				break;
				case USB_STRING_LIST:
					USB_Send_Data(EP_CTRL_IN, &USB_Descriptor_Language, USB_Descriptor_Language.bLength);
					USB_Ack(EP_CTRL_IN);
					USB_Ack(EP_CTRL_OUT);
					#ifdef DEBUG
					uart_println("Descriptor string list");
					#endif
				break;
			}
		break;
	}
	
	
}

void USB_Process_Setup_Packet()
{
	USB_Clear_Stall(EP_CTRL_IN);
	USB_Clear_Stall(EP_CTRL_OUT);
	USB_EP_Ack_Setup(EP_CTRL_OUT);
	
	USB_DeviceRequest_t* Request = (USB_DeviceRequest_t*) ((u8*)usb_ram.endpoint[0].DATAPTR);
	
	USB_Set_AUXDATA(EP_DATA_IN, 0); // clear any pending data
	
	if((Request->bmRequestType & USB_Request_Type_MASK) == USB_Request_Type_CLASS)
	{
		#ifdef DEBUG
		uart_println("CDC Packet");
		#endif
		USB_CDC_Setup_Packet(Request);
		return;
	}
	else if((Request->bmRequestType & USB_Request_Type_MASK) == USB_Request_Type_VENDOR)
	{
		#ifdef DEBUG
		uart_println("?? vendor packet");
		#endif
		// error condition
	}
	else if((Request->bmRequestType & USB_Request_Type_STANDARD) == USB_Request_Type_STANDARD)
	{ // regular setup packet
		switch(Request->bRequest)
		{
			case USB_Request_GET_DESCRIPTOR:
				USB_Fetch_Descriptor(Request);
			break;
			case USB_Request_SET_ADDRESS:
				#ifdef DEBUG
				uart_println("Set address packet");
				#endif
				USB_State = AddressPending;
				USB_New_Addr = Request->wValue & 0x7F;
				//USB_ZLP(EP_CTRL_IN);
				//USB_Ack(EP_CTRL_IN);
				//USB_Ack(EP_CTRL_OUT);
			break;
			case USB_Request_SET_CONFIGURATION:
				#ifdef DEBUG
				uart_println("Set configuration packet");
				#endif
				//USB_ZLP(EP_CTRL_IN);
				//USB_Ack(EP_CTRL_OUT);
				//USB_Ack(EP_CTRL_IN);
				USB_State = Configured;
			break;
		}
	}
	
	if(Request->wLength == 0)
	{ // ZLP
		USB_ZLP(EP_CTRL_IN);
		USB_Ack(EP_CTRL_IN);
		USB_Ack(EP_CTRL_OUT);
	}
	
	//uart_println("This was setup packet. Data: ");
	//memdump(Request, sizeof(USB_DeviceRequest_t));
	//uart_println("\r\n");
}

void USB_Vect_TRNCOMPL()
{
	USB_Ack_TRNCOMPL();
	
	if(USB_Is_Setup_Packet())
	{
		USB_Ack_Setup_Packet();
		USB_Process_Setup_Packet();
		return;
	}
	
	//u8 EP = (u8)USB_Get_EP_From_FIFO();
	u8 EP, ep_index;
	u8 i_fifo;
	u16 ad;
	u16 *p_ad;
	u8 rp;
	rp = USB.FIFORP;
	i_fifo = 2 * (1 + ~rp);
	ad = ((u16) usb_ram.endpoint) - i_fifo;
	p_ad = (u16 *)ad;
	ep_index = (((u16) * p_ad - ((u16)usb_ram.endpoint)) >> 3);
	EP = ep_index;
	
	u16 cnt;
	u16 aux;
	
	switch(EP)
	{
		case EP_CTRL_OUT: // PC => device
		// let's hope for now that PC won't send >64 bytes of data on control endpoints
		//uart_println("TRNCOMPL on ctrl out FIFO");
		//memdump(usb_ram.payload[EP_CTRL_OUT], XBOOT_DATASIZE);
		if(USB_Get_CNT(EP_CTRL_OUT) != 0)
		{
			#ifdef DEBUG
			uart_println("Received data on EP_CTRL_OUT: ");
			memdump(USB_Get_DATA(EP_CTRL_OUT), USB_Get_CNT(EP_CTRL_OUT));
			uart_println("");
			if(CDC_Callback != NULL)
			{
				(*CDC_Callback)();
			}
			#endif
			USB_Ack(EP_CTRL_OUT);
			USB_ZLP(EP_CTRL_IN);
			USB_Ack(EP_CTRL_IN);
		}
		break;
		
		case EP_CTRL_IN: // device => PC
		//uart_println("TRNCOMPL on ctrl in FIFO");
		// manage auxiliary data + ZLP
		cnt = USB_Get_CNT(EP_CTRL_IN);
		aux = USB_Get_AUXDATA(EP_CTRL_IN);
		
		if(cnt == aux && aux == XBOOT_DATASIZE) // aux != 0 for packets with 0 length in the beginning.
		{
			USB_ZLP(EP_CTRL_IN);
			USB_Ack(EP_CTRL_IN);
			#ifdef DEBUG
			uart_println("Sending ZLP");
			#endif
		}
		else if (cnt < aux)
		{
			aux -= cnt; //"cnt" sent
			USB_Set_AUXDATA(EP_CTRL_IN, aux); // set new auxiliary data
			u16 addr = USB_Get_DATA(EP_CTRL_IN);
			#ifdef DEBUG
			uart_send("Orig addr: ");
			memdump(&addr, 2);
			#endif
			addr += cnt;
			#ifdef DEBUG
			uart_send("=>");
			memdump(&addr, 2);
			uart_println("");
			#endif
			USB_Set_DATA(EP_CTRL_IN, addr); // move ptr by cnt further. Might not have been full XBOOT_DATASIZE (for example only the first 8B of device descriptor for backward compatibility)
			USB_Set_CNT(EP_CTRL_IN, MIN(aux, cnt)); // now send either aux or cnt - what is smaller. If aux is smaller than max data, then send aux, otherwise max data (cnt)
			USB_Ack(EP_CTRL_IN);
			#ifdef DEBUG
			uart_println("Sending another CNT AUX:");
			//usart_u16(cnt);
			memdump(&cnt, 2);
			//usart_u16(aux);
			memdump(&aux, 2);
			uart_println("");
			#endif
		}
		else if (cnt == aux && aux == 0)
		{
			// nothing, last packet was ZLP.
		}
		else if(cnt != aux) // regular packet <XBOOT_DATASIZE
		{
			// error, shouldn't happen?!
			#ifdef DEBUG
			uart_println("error! cnt > aux!");
			memdump(&cnt, 2);
			memdump(&aux, 2);
			uart_println("");
			#endif
		}
		
		if(USB_State == AddressPending)
		{
			USB_Set_Address(USB_New_Addr);
			USB_State = Addressed;
			#ifdef DEBUG
			uart_println("Setting new address");
			#endif
		}
		break;
		
		default: // CDC packet or error condition
			USB_CDC_Data_Packet(EP);
		return;
		break;
	}
}

ISR(USB_TRNCOMPL_vect)
{
	USB_Vect_TRNCOMPL();
}

void USB_Vect_BUSEVENT()
{
	/*if(USB.INTFLAGSASET & USB_SOFIF_bm)
	{
		USB.INTFLAGSACLR = USB_SOFIF_bm;
		return;
	}*/
	
	if(USB.INTFLAGSASET & USB_RSTIF_bm)
	{
		USB.INTFLAGSACLR = USB_RSTIF_bm;
		#ifdef DEBUG
		uart_println(" >> Resetting.");
		#endif
		USB_Detach();
		USB_Reset();
		USB_Attach();
		return;
	}

	USB.INTFLAGSACLR = 0xFF; // clear all the USB A flags
}

ISR(USB_BUSEVENT_vect)
{
	USB_Vect_BUSEVENT();
}