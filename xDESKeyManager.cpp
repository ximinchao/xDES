#include "xDESKeyManager.h"

#include <cstring>
using std::memcpy;

const unsigned int xDESKeyManager::m_puiPC1[XDES_KEYMANAGER_SIZE_TABLE_PC1] =
{
	56, 48, 40, 32, 24, 16, 8, 0,
	57, 49, 41, 33, 25, 17, 9, 1,
	58, 50, 42, 34, 26, 18, 10, 2,
	59, 51, 43, 35, 62, 54, 46, 38,
	30, 22, 14, 6, 61, 53, 45, 37,
	29, 21, 13, 5, 60, 52, 44, 36,
	28, 20, 12, 4, 27, 19, 11, 3
};

const unsigned int xDESKeyManager::m_puiPC2[XDES_KEYMANAGER_SIZE_TABLE_PC2] = 
{
	13, 16, 10, 23, 0, 4, 3, 27,
	14, 5, 20, 9, 22, 18, 11, 3,
	25, 7, 15, 6, 26, 19, 12, 1,
	40, 51, 30, 36, 46, 54, 29, 39,
	50, 44, 32, 47, 43, 48, 38, 55,
	33, 52, 45, 41, 49, 35, 28, 31
};

xDESKeyManager::xDESKeyManager()
{
	m_uiSetMode = XDES_KEYMANAGER_SETMODE_UNSET;
	m_uiCalcRound = 0;
}

xDESKeyManager::~xDESKeyManager()
{
}

bool xDESKeyManager::setKey(const unsigned char *const pucKey, const unsigned int uiKeyLen, const unsigned int uiSetMode)
{
	bool bRtn = false;

	unsigned int uiInnerLen = 0, i = 0;

	if (!pucKey || !uiKeyLen || (uiKeyLen != XDES_KEYMANAGER_SIZE_ORIGKEYBUFFER))
	{
		bRtn = false;
		goto END;
	}

	if ((uiSetMode != XDES_KEYMANAGER_SETMODE_NORMAL) && (uiSetMode != XDES_KEYMANAGER_SETMODE_PRECALC))
	{
		bRtn = false;
		goto END;
	}

	if (uiSetMode == XDES_KEYMANAGER_SETMODE_NORMAL)
	{
		memcpy(m_pucOrigKeyBuffer, pucKey, uiKeyLen);
		uiInnerLen = sizeof(m_pucInnerBuffer);

		if(!calcPC(m_pucOrigKeyBuffer, sizeof(m_pucOrigKeyBuffer), 1, m_pucInnerBuffer, &uiInnerLen))
		{
			bRtn = false;
			goto END;
		}

		m_uiCalcRound = 0;
	}
	else if (uiSetMode == XDES_KEYMANAGER_SETMODE_PRECALC)
	{
		//todo
		memcpy(m_pucOrigKeyBuffer, pucKey, uiKeyLen);
		uiInnerLen = sizeof(m_pucInnerBuffer);
		if(!calcPC(m_pucOrigKeyBuffer, sizeof(m_pucOrigKeyBuffer), 1, m_pucInnerBuffer, &uiInnerLen))
		{
			bRtn = false;
			goto END;
		}

		for(i=0; i<XDES_KEYMANAGER_COUNT_ROUND; i++)
		{
			uiInnerLen = sizeof(m_pucInnerBuffer);
			if(!calcNextKey(m_pucInnerBuffer, &uiInnerLen))
			{
				bRtn = false;
				goto END;
			}
			
			uiInnerLen = sizeof(m_pucOutAllKeyBuffer[i]);
			if(!calcPC(m_pucInnerBuffer, sizeof(m_pucInnerBuffer), 2, m_pucOutAllKeyBuffer[i], &uiInnerLen))
			{
				bRtn = false;
				goto END;
			}
		}
		
		m_uiCalcRound = i;
	}

	m_uiSetMode = uiSetMode;
	bRtn = true;
END:
	return bRtn;
}

bool xDESKeyManager::setBit(const unsigned char *const pucInChar, const unsigned int uiInBitPos, unsigned char *const pucOutChar, const unsigned int uiOutBitPos)
{
	bool bRtn = false;
	unsigned char ucInResult = 0;

	if (!pucInChar || !pucOutChar)
	{
		bRtn = false;
		goto END;
	}

	ucInResult = (*pucInChar) & (0x80 >> uiInBitPos);
	if (ucInResult)
	{
		(*pucOutChar) |= (0x80 >> uiOutBitPos);
	}
	else
	{
		(*pucOutChar) &= (~(0x80 >> uiOutBitPos));
	}

	bRtn = true;
END:
	return bRtn;
}

bool xDESKeyManager::exchangeBytes(const unsigned char *const pucInData, const unsigned int uiInDataLen, unsigned char *const pucOutData, unsigned int *const puiOutDataLen, const unsigned int *const pucExchangeTable, const unsigned int uiExchangeTableLen)
{
	bool bRtn = false;
	
	unsigned int i = 0, uiInBytePos = 0, uiInBitPos = 0, uiOutBytePos = 0, uiOutBitPos = 0;

	if (!pucInData || !uiInDataLen || !pucOutData || !puiOutDataLen || !pucExchangeTable || !uiExchangeTableLen)
	{
		bRtn = false;
		goto END;
	}

	//The exchange table stores every bit's position of output data
	if (((*puiOutDataLen)*8) < uiExchangeTableLen)
	{
		bRtn = false;
		goto END;
	}

	for (i=0; i<uiExchangeTableLen; i++)
	{
		uiInBytePos = pucExchangeTable[i] / 8;
		uiInBitPos = pucExchangeTable[i] % 8;

		uiOutBytePos = i / 8;
		uiOutBitPos = i % 8;

		if(!setBit(pucInData + uiInBytePos, uiInBitPos, pucOutData + uiOutBytePos, uiOutBitPos))
		{
			bRtn = false;
			goto END;
		}
	}

	*puiOutDataLen = (uiExchangeTableLen / 8);
	bRtn = true;
END:
	return bRtn;
}

bool xDESKeyManager::calcPC(const unsigned char *const pucInKey, const unsigned int uiInKeyLen, const unsigned int uiType, unsigned char *const pucOutKey, unsigned int *const puiOutKeyLen)
{
	bool bRtn = false;

	if (m_uiSetMode == XDES_KEYMANAGER_SETMODE_UNSET)
	{
		bRtn = false;
		goto END;
	}

	if ((uiType != 1) && (uiType != 2))
	{
		bRtn = false;
		goto END;
	}

	if (!pucInKey || !uiInKeyLen || !pucOutKey || !puiOutKeyLen || (*puiOutKeyLen < XDES_KEYMANAGER_SIZE_INNERBUFFER))
	{
		bRtn = false;
		goto END;
	}

	if (((uiType == 1) && (*puiOutKeyLen < XDES_KEYMANAGER_SIZE_INNERBUFFER)) || ((uiType == 2) && (*puiOutKeyLen < XDES_KEYMANAGER_SIZE_OUTKEYBUFFER)))
	{
		bRtn = false;
		goto END;
	}

	if (uiType == 1)
	{
		bRtn = exchangeBytes(pucInKey, uiInKeyLen, pucOutKey, puiOutKeyLen, m_puiPC1, sizeof(m_puiPC1)/sizeof(m_puiPC1[0])); 
	}
	else if (uiType == 2)
	{
		bRtn = exchangeBytes(pucInKey, uiInKeyLen, pucOutKey, puiOutKeyLen, m_puiPC2, sizeof(m_puiPC2)/sizeof(m_puiPC2[0]));
	}
END:
	return bRtn;
}

bool xDESKeyManager::calcNextKey(unsigned char *const pucOutKey, unsigned int *const puiOutKeyLen)
{
	bool bRtn = false;

	if(!pucOutKey || !puiOutKeyLen || (*puiOutKeyLen) < XDES_KEYMANAGER_SIZE_OUTKEYBUFFER || m_uiCalcRound > XDES_KEYMANAGER_COUNT_ROUND)
	{
		bRtn = false;
		goto END;
	}

	//do sth
	memset(m_pucOutKeyBuffer, 0, sizeof(m_pucOutKeyBuffer));

	m_uiCalcRound++;
	memcpy(pucOutKey, m_pucOutKeyBuffer, XDES_KEYMANAGER_SIZE_OUTKEYBUFFER);
	*puiOutKeyLen = XDES_KEYMANAGER_SIZE_OUTKEYBUFFER;
	bRtn = true;
END:
	return bRtn;
}

bool xDESKeyManager::getCurKey(unsigned char *const pucOutKey, unsigned int *const puiOutKeyLen)
{
	bool bRtn = false;

	if(!pucOutKey || !puiOutKeyLen || (*puiOutKeyLen) < XDES_KEYMANAGER_SIZE_OUTKEYBUFFER || m_uiCalcRound < 1 || m_uiCalcRound > XDES_KEYMANAGER_COUNT_ROUND)
	{
		bRtn = false;
		goto END;
	}

	memcpy(pucOutKey, m_pucOutKeyBuffer, XDES_KEYMANAGER_SIZE_OUTKEYBUFFER);
	*puiOutKeyLen = XDES_KEYMANAGER_SIZE_OUTKEYBUFFER;
	bRtn = true;
END:
	return bRtn;
}

bool xDESKeyManager::getKeyRound(unsigned int *const puiKeyRount)
{
	bool bRtn = false;

	bRtn = true;
END:
	return bRtn;
}
