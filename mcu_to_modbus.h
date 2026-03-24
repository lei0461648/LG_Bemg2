#ifndef _GET_SCREEN_H
#define _GET_SCREEN_H

#define InitializeParam		0x00
#define HandModeParam		0x01
#define PulseModeParam		0x02
#define Mode_4_20MAParam	0x03
#define BatchPulseParam		0x04
#define CyclicDosingParam	0x05




void SendDataToBoard(void);
void TxDataHeader(void);
void CreateCRC_TX(unsigned int arraySize);

#endif
