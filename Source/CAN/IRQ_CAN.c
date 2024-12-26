/*----------------------------------------------------------------------------
 * Name:    Can.c
 * Purpose: CAN interface for for LPC17xx with MCB1700
 * Note(s): see also http://www.port.de/engl/canprod/sv_req_form.html
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2009 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <LPC17xx.h>                  /* LPC17xx definitions */
#include "CAN.h"                      /* LPC17xx CAN adaption layer */
#include "../GLCD/GLCD.h"

extern uint8_t icr ; 										//icr and result must be global in order to work with both real and simulated landtiger.
extern uint32_t result;
extern CAN_msg       CAN_TxMsg;    /* CAN message for sending */
extern CAN_msg       CAN_RxMsg;    /* CAN message for receiving */                                

/*----------------------------------------------------------------------------
  CAN interrupt handler
 *----------------------------------------------------------------------------*/
void CAN_IRQHandler (void)  {

  /* check CAN controller 1 */
	icr = 0;
  icr = (LPC_CAN1->ICR | icr) & 0xFF;               /* clear interrupts */
	
  if (icr & (1 << 0)) {                          		/* CAN Controller #1 meassage is received */
		// do nothing, can't receive through can 1
  }
	if (icr & (1 << 1)) {                         /* CAN Controller #1 meassage is transmitted */
		LCD_Clear(Black);//TODO verificare con scheda fisica che effettivamente questa parte di funzione venga chiamata inseguito al click di Key1
		GUI_Text(10, 20, (uint8_t *) "Statistics sent through CAN1!", Red, White);
	}
		
	/* check CAN controller 2 */
	icr = 0;
	icr = (LPC_CAN2->ICR | icr) & 0xFF;             /* clear interrupts */

	if (icr & (1 << 0)) {                          	/* CAN Controller #2 meassage is received */
		CAN_rdMsg (2, &CAN_RxMsg);	                		/* Read the message */
    LPC_CAN2->CMR = (1 << 2);                    		/* Release receive buffer */
		//TODO veriricare con scheda fisica che si ricevano queste informazioni
		//withdraw  statistics
		uint16_t s = CAN_RxMsg.data[0];//first part of score in first byte
		s |= CAN_RxMsg.data[1]<<8;//second part of score in second byte
		uint8_t rl = CAN_RxMsg.data[2];//remaining lives on third byte
		uint8_t rt = CAN_RxMsg.data[3];//remaining time on fourth byte
		
		//print on screen
		GUI_Text(10, 60, (uint8_t *) "Statistics received through CAN2!", Red, White);
		GUI_Text(10, 100, (uint8_t *) "Score:", Red, White);
		char text[5];
		intToCharArray(s,text);
		GUI_Text(60, 100, (uint8_t *) text, Red, White);
		
		GUI_Text(10, 140, (uint8_t *) "Remaining lives:", Red, White);
		intToCharArray(rl,text);
		GUI_Text(100, 140, (uint8_t *) text, Red, White);
		
		GUI_Text(10, 180, (uint8_t *) "Remaining time:", Red, White);
		intToCharArray(rt,text);
		GUI_Text(100, 180, (uint8_t *) text, Red, White);
	}
	if (icr & (1 << 1)) {                         /* CAN Controller #2 meassage is transmitted */
		// do nothing, can't send through can 2
	}
}
