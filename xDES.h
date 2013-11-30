#ifndef X_DES_H
#define X_DES_H4

#define	XALG_DES_BLOCKLEN	64
#define	XALG_DES_KEYLEN		64

#define	XALG_DES_KEYINNERLEN	48
#define	XALG_DES_KEYDATALEN	56

#define	XALG_DES_ROUNDCOUNT	16

#define	XALG_DES_SIZE_TABLE_IP	64
#define	XALG_DES_SIZE_TABLE_RIP	64
#define	XALG_DES_SIZE_TABLE_E	48
#define	XALG_DES_SIZE_TABLE_S	8
#define	XALG_DES_SIZE_TABLE_S1	4
#define	XALG_DES_SIZE_TABLE_S2	16
#define	XALG_DES_SIZE_TABLE_P	32

class xDES
{
public:
	bool calcDES(const unsigned char *pucKey, const unsigned int uiKeyLen, const unsigned char *pucPlainData, const unsigned int uiPlainDataLen, unsigned char *pucCipherData, unsigned int *puiCipherDataLen);

private:
	bool calcIP(const unsigned char *pucInData, const unsigned int uiInDataLen, unsigned char *pucOutData, unsigned int *puiOutDataLen);
	bool calcRIP(const unsigned char *pucInData, const unsigned int uiInDataLen, unsigned char *pucOutData, unsigned int *puiOutDataLen);
	bool calcRound(const unsigned char *pucInData, const unsigned int uiInDataLen, const unsigned char *pucKey, const unsigned int uiKeyLen, unsigned char *pucOutData, unsigned int *puiOutDataLen);

	bool calcF(const unsigned char *pucKey, const unsigned int uiKeyLen, const unsigned char* pucInData, const unsigned int uiInDataLen, unsigned char* pucOutData, unsigned int *puiOutDataLen);
	bool calcE(const unsigned char *pucInData, const unsigned int uiInDataLen, unsigned char *pucOutData, unsigned int *puiOutDataLen);
	bool calcSBox(const unsigned int uiBoxIndex, const unsigned char *pucInData, const unsigned int uiInDataLen, unsigned char *pucOutData, unsigned int *puiOutDataLen);
	bool calcP(const unsigned char *pucInData, const unsigned int uiInDataLen, unsigned char *pucOutData, unsigned int *puiOutDataLen);

	bool calcEncryptKeyRound(const unsigned int uiIndex, const unsigned char *pucInData, const unsigned int uiInDataLen, unsigned char *pucOutKey, unsigned int *puiOutKeyLen, unsigned char *pucOutData, unsigned int *puiOutDataLen);
	bool calcDecryptKeyRound(const unsigned int uiIndex, const unsigned char *pucInData, const unsigned int uiInDataLen, unsigned char *pucOutKey, unsigned int *puiOutKeyLen, unsigned char *pucOutData, unsigned int *puiOutDataLen);
	bool calcPC1(const unsigned char *pucInData, const unsigned int uiInDataLen, unsigned char *pucOutData, unsigned int *puiOutDataLen);
	bool calcPC2(const unsigned char *pucInData, const unsigned int uiInDataLen, unsigned char *pucOutData, unsigned int *puiOutDataLen);
	bool calcLS(const unsigned int uiLSIndex, const unsigned char *pucInData, const unsigned int uiInDataLen, unsigned char *pucOutData, unsigned int *puiOutDataLen);
	bool calcRS(const unsigned int uiRSIndex, const unsigned char *pucInData, const unsigned int uiInDataLen, unsigned char *pucOutData, unsigned int *puiOutDataLen);

private:
	static const unsigned int uiIPTable[XALG_DES_SIZE_TABLE_IP];
	static const unsigned int uiRIPTable[XALG_DES_SIZE_TABLE_RIP];
	static const unsigned int uiETable[XALG_DES_SIZE_TABLE_E];
	static const unsigned int uiSTable[XALG_DES_SIZE_TABLE_S][XALG_DES_SIZE_TABLE_S1][XALG_DES_SIZE_TABLE_S2];
	static const unsigned int uiPTable[XALG_DES_SIZE_TABLE_P];
};

#endif
