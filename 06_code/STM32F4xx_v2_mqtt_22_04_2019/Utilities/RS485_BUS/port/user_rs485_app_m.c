/*
 * RS485bus Libary: user callback functions and buffer define in master mode
 * Copyright (C) 2013 Armink <armink.ztl@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: user_rs485_app_m.c,v 1.60 2013/11/23 11:49:05 Armink $
 */
#include "user_rs485_app.h"
#include "rs485_m.h"

#define M_REG_HOLDING_START           0
#define M_REG_HOLDING_NREGS           40
/*-----------------------Master mode use these variables----------------------*/
#if RS485_MASTER_RTU_ENABLED > 0 || RS485_MASTER_ASCII_ENABLED > 0
extern CTPORT_TypeDef CTport;


//Master mode:HoldingRegister variables
USHORT   usMRegHoldStart                            = M_REG_HOLDING_START;
#define MAX_SUPORT_PORT               4
USHORT   usMRegHoldBuf[MAX_SUPORT_PORT][MB_MASTER_TOTAL_SLAVE_NUM][M_REG_HOLDING_NREGS];
// struct   usMRegHoldBufStruct[4] ={
//       UCHAR Port,
//       usMRegHoldBuf[MB_MASTER_TOTAL_SLAVE_NUM][M_REG_HOLDING_NREGS],
// }; 

/**
 * RS485bus slave Check callback function.
 *
 * @param ucPort Port check
 * @param pucBuffer Check buffer
 * @param usLen Length
 *
 * @return result
 */
eRS485ErrorCode eMasterResponseChecksCB( UCHAR ucPort,UCHAR * pucBuffer, UCHAR * ucLen)
{
  eRS485ErrorCode    eStatus = RS485_ENOERR;
	CTport.Port[ucPort].timeout = PORT_TIMEOUT;
	CTport.Port[ucPort].status = PORT_OK;
  if(pucBuffer[0] == RS485_EX_ACK)
  {
    CTport.Port[ucPort].active = PORT_ENABLE;
  }
  else
  {
    CTport.Port[ucPort].active = PORT_DISABLE;
  }
	*ucLen = 1;
  return eStatus;
}

/**
 * RS485bus slave Check callback function.
 *
 * @param ucPort Port check
 * @param pucBuffer Check buffer
 * @param usLen Length
 *
 * @return result
 */
eRS485ErrorCode eMasterFuncPeriodicPingCB( UCHAR ucPort,UCHAR * pucBuffer, UCHAR * ucLen)
{
  eRS485ErrorCode    eStatus = RS485_ENOERR;
	CTport.Port[ucPort].timeout = PORT_TIMEOUT;
	CTport.Port[ucPort].status = PORT_OK;
  if(pucBuffer[0] == RS485_EX_ACK)
  {
    CTport.Port[ucPort].active = PORT_ENABLE;
  }
  else
  {
    CTport.Port[ucPort].active = PORT_DISABLE;
  }
	*ucLen = 1;
  return eStatus;
}

/**
 * RS485bus slave Check callback function.
 *
 * @param ucPort Port check
 * @param pucBuffer Check buffer
 * @param usLen Length
 *
 * @return result
 */
eRS485ErrorCode eMasterFuncNFCDetectCB( UCHAR ucPort,UCHAR * pucBuffer, UCHAR * ucLen)
{
  eRS485ErrorCode    eStatus = RS485_ENOERR;
	CTport.Port[ucPort].timeout = PORT_TIMEOUT;
	CTport.Port[ucPort].status = PORT_OK;
  if(pucBuffer[0] == RS485_EX_ACK)
  {
    CTport.Port[ucPort].event = PORT_AUTH_PASS | PORT_RECEIVE_DATA;
  }
  else
  {
    CTport.Port[ucPort].event = PORT_AUTH_FAIL | PORT_RECEIVE_DATA;
  }
	*ucLen = 1;
  return eStatus;
}

/**
 * RS485bus slave Check callback function.
 *
 * @param ucPort Port check
 * @param pucBuffer Check buffer
 * @param usLen Length
 *
 * @return result
 */
eRS485ErrorCode eMasterFuncErrorDetectCB( UCHAR ucPort,UCHAR * pucBuffer, UCHAR * ucLen)
{
  eRS485ErrorCode    eStatus = RS485_ENOERR;
  return eStatus;
}

/**
 * Modbus master holding register callback function.
 *
 * @param pucRegBuffer holding register buffer
 * @param usAddress holding register address
 * @param usNRegs holding register number
 * @param eMode read or write
 *
 * @return result
 */
eRS485ErrorCode eRS485MasterRegHoldingCB(UCHAR ucPort, USHORT usAddress, UCHAR * pucRegBuffer,
        USHORT usNRegs, eRS485RegisterMode eMode)
{
    eRS485ErrorCode    eStatus = RS485_ENOERR;
    USHORT          iRegIndex;
    USHORT *        pusRegHoldingBuf;
    USHORT          REG_HOLDING_START;
    USHORT          REG_HOLDING_NREGS;
    USHORT          usRegHoldStart;

	pusRegHoldingBuf = usMRegHoldBuf[ucPort][ucRS485MasterGetDestAddress(ucPort) - 1];
    REG_HOLDING_START = M_REG_HOLDING_START;
    REG_HOLDING_NREGS = M_REG_HOLDING_NREGS;
    usRegHoldStart = usMRegHoldStart;
    /* if mode is read, the master will write the received date to buffer. */
    eMode = RS485_REG_WRITE;

    /* it already plus one in modbus function method. Tao dell biet no inc lam gi de roi dec xuong */
    usAddress--;

    if ((usAddress >= REG_HOLDING_START)
            && (usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS))
    {
        iRegIndex = usAddress - usRegHoldStart;
        switch (eMode)
        {
        /* read current register values from the protocol stack. */
        //Swap this case 
        case RS485_REG_WRITE:
            while (usNRegs > 0)
            {
//                *pucRegBuffer++ = (UCHAR) (pusRegHoldingBuf[iRegIndex] >> 8);
//                *pucRegBuffer++ = (UCHAR) (pusRegHoldingBuf[iRegIndex] & 0xFF);
//                iRegIndex++;
                usNRegs--;
            }
            break;
        /* write current register values with new values from the protocol stack. */
        case RS485_REG_READ:
            while (usNRegs > 0)
            {
//                pusRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
//                pusRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
//                iRegIndex++;
               usNRegs--;
            }
            break;
        }
    }
    else
    {
        eStatus = RS485_ENOERR;
    }
    return eStatus;
}
#endif
