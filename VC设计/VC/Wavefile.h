#ifndef __WAVEFILE_H__
#define __WAVEFILE_H__

#include <mmsystem.h>
struct WriteParams
{
	HMMIO hmmio;
	MMCKINFO *mmciRiff;
	MMCKINFO *mmciFmt;
	MMCKINFO *mmciData;
	WAVEFORMATEX *pwf;
};

BOOL CreateWaveFile(LPSTR filename,WriteParams *pParams);
BOOL WriteWaveFile(WriteParams *pParams,LPSTR pData,LONG length);
BOOL CloseWaveFile(WriteParams *pParams);

BOOL RecordStart();
void RecordEnd();
void CALLBACK waveInProc(HWAVEIN hwi,UINT uMsg,DWORD dwInstance,DWORD dwParam1,DWORD dwParam2);
void filewrite(int data);


BOOL PlayStart(LPCSTR file,WAVEFORMATEX *pwf,LPSTR *pData,LONG *pLen);
BOOL PlayStart1(LPCSTR file,WAVEFORMATEX *pwf,LPSTR *pData,LONG *pLen);
BOOL PlayStart2(LPCSTR file,WAVEFORMATEX *pwf,LPSTR *pData,LONG *pLen);


void ShowWave(CDC *pDC,CRect *pRect,LPSTR pData,long len);
void ShowFreq(CDC *pDC,CRect *pRect,LPSTR pData,long len);

#endif