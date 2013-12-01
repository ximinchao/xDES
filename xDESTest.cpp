#include <iostream>
using std::cout;
using std::endl;

#include <cstring>
using std::memset;

#include "xDESKeyManager.h"


int main(int argc, char **argv)
{
	int iRtn = -1;
	bool bRtn = false;

	unsigned char pucxDESKey[XDES_KEYMANAGER_SIZE_ORIGKEYBUFFER] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8};
	unsigned char pucxDESOutKey[XDES_KEYMANAGER_SIZE_OUTKEYBUFFER] = {0};
	unsigned int uixDESOutKeyLen = 0;

	xDESKeyManager *pKeyManager = new xDESKeyManager();

	bRtn = pKeyManager->setKey(pucxDESKey, sizeof(pucxDESKey), XDES_KEYMANAGER_SETMODE_NORMAL);
	if (!bRtn)
	{
		cout << "setKey error." << endl;
	}

	uixDESOutKeyLen = sizeof(pucxDESOutKey);
	memset(pucxDESOutKey, 0, uixDESOutKeyLen);
	bRtn = pKeyManager->calcNextKey(pucxDESOutKey, &uixDESOutKeyLen);
	if (!bRtn)
	{
		cout << "setKey error." << endl;
	}

	iRtn = 0;

	return iRtn;
}
