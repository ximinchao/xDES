#ifndef	XDES_KEYMANAGER_H
#define	XDES_KEYMANAGER_H

#define	XDES_KEYMANAGER_COUNT_ROUND	16	//the count of round

#define	XDES_KEYMANAGER_SETMODE_UNSET	-1	//haven't been set
#define	XDES_KEYMANAGER_SETMODE_NORMAL	0	//action of set key only copy the key value to the buffer
#define	XDES_KEYMANAGER_SETMODE_PRECALC	1	//after setting the key, the output keys are calculated and stored in the m_ucOutAllKeyBuffer

#define	XDES_KEYMANAGER_SIZE_ORIGKEYBUFFER	8	//buffer size to store the input key
#define	XDES_KEYMANAGER_SIZE_OUTKEYBUFFER	6	//buffer size to store the output key of this round
#define	XDES_KEYMANAGER_SIZE_INNERBUFFER	7	//buffer size to store the key between each round

#define	XDES_KEYMANAGER_SIZE_OUTALLKEYBUFFER	XDES_KEYMANAGER_SIZE_OUTKEYBUFFER	//the buffer size to store tempory precalculated output keys
#define	XDES_KEYMANAGER_SIZE_OUTALLKEYCOUNT	XDES_KEYMANAGER_COUNT_ROUND	//the count of round

#define	XDES_KEYMANAGER_SIZE_TABLE_PC1	XDES_KEYMANAGER_SIZE_INNERBUFFER*8
#define	XDES_KEYMANAGER_SIZE_TABLE_PC2	XDES_KEYMANAGER_SIZE_OUTKEYBUFFER*8

class xDESKeyManager
{
public:
	xDESKeyManager();
	~xDESKeyManager();
	bool setKey(const unsigned char *const pucKey, const unsigned int uiKeyLen, const unsigned int uiSetMode);
	bool calcNextKey(unsigned char *const pucOutKey, unsigned int *const puiOutKeyLen);
	bool getCurKey(unsigned char *const pucOutKey, unsigned int *const puiOutKeyLen);
	bool getKeyRound(unsigned int *const puiKeyRount);
private:
	bool calcPC(const unsigned char *const pucInKey, const unsigned int uiInKeyLen, const unsigned int uiType, unsigned char *const pucOutKey, unsigned int *const puiOutKeyLen);
	bool setBit(const unsigned char *const pucInChar, const unsigned int uiInBitPos, unsigned char *const pucOutChar, const unsigned int uiOutBitPos);
	bool exchangeBytes(const unsigned char *const pucInData, const unsigned int uiInDataLen, unsigned char *const pucOutData, unsigned int *const puiOutDataLen, const unsigned int *const pucExchangeTable, const unsigned int uiExchangeTableLen);
	unsigned char m_pucOrigKeyBuffer[XDES_KEYMANAGER_SIZE_ORIGKEYBUFFER];
	unsigned char m_pucOutKeyBuffer[XDES_KEYMANAGER_SIZE_OUTKEYBUFFER];
	unsigned char m_pucInnerBuffer[XDES_KEYMANAGER_SIZE_INNERBUFFER];
	unsigned char m_pucOutAllKeyBuffer[XDES_KEYMANAGER_SIZE_OUTALLKEYCOUNT][XDES_KEYMANAGER_SIZE_OUTALLKEYBUFFER];
	unsigned char m_uiSetMode;
	unsigned char m_uiCalcRound;

	static const unsigned int m_puiPC1[XDES_KEYMANAGER_SIZE_TABLE_PC1];
	static const unsigned int m_puiPC2[XDES_KEYMANAGER_SIZE_TABLE_PC2];
};

#endif
