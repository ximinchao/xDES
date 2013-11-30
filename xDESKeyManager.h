#ifdef	XDES_KEYMANAGER_H
#define	XDES_KEYMANAGER_H

#define	XDES_KEYMANAGER_SETMODE_NORMAL	0
#define	XDES_KEYMANAGER_SETMODE_PRECALC	1

#define	XDES_KEYMANAGER_BITSIZE_ORIGKEYBUFFER	8
#define	XDES_KEYMANAGER_BITSIZE_OUTKEYBUFFER	6
#define	XDES_KEYMANAGER_BITSIZE_INNERBUFFER	7

#define	XDES_KEYMANAGER_BITSIZE_OUTALLKEYBUFFER	XDES_KEYMANAGER_SIZE_OUTKEYBUFFER
#define	XDES_KEYMANAGER_SIZE_OUTALLKEYCOUNT	2

class xDESKeyManager
{
public:
	bool setKey(const unsigned char *pucKey, const unsigned int uiKeyLen, const unsigned int uiSetMode);
	bool calcNextKey(unsigned char *pucOutKey, unsigned int *puiOutKeyLen);
	bool getKeyRound(unsigned int *puiKeyRount);
private:
	unsigned char m_uiOrigKeyBuffer[XDES_KEYMANAGER_BITSIZE_ORIGKEYBUFFER];
	unsigned char m_ucOutKeyBuffer[XDES_KEYMANAGER_BITSIZE_OUTKEYBUFFER];
	unsigned char m_ucInnerBuffer[XDES_KEYMANAGER_BITSIZE_INNERBUFFER];
	unsigned char m_ucOutAllKeyBuffer[XDES_KEYMANAGER_SIZE_OUTALLKEYCOUNT][XDES_KEYMANAGER_BITSIZE_OUTALLKEYBUFFER];
	unsigned char m_uiSetMode;
	unsigned char m_uiCalcRound;
};

#endif
