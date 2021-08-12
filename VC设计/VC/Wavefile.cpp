/****************************ͷ�ļ�����*************************************/
#include "iostream.h"
#include "stdafx.h"
#include "WaveFile.h"
#include "Resource.h"
#include "fourier.h"
#include "RecordDlg.h"
#include "windows.h "
/****************************ͷ�ļ�����*************************************/

/****************************ϵͳ����*************************************/
CRecordDlg * pDlg;
#pragma comment(lib,"winmm.lib")  //���߱�����Ҫ����winmm��
#define BUF_NUMS 5
#define BUF_SIZE 512
#define BUF2_SIZE 512*1024
double inReal[512],inImg[512];
double outReal[512],outImg[512];
double outAmp[512];
double domfreq;
extern double FindDominantFrequency (int numSamples,
									 int windowSize,
									 double *real,
									 double *imag,
									 double *ampl,
									 double samplingRate );

extern CDialog *g_pDlg;
FILE *g_fpWaveFile=NULL;
HWAVEIN hwi;
HWAVEOUT hwo=NULL;
WAVEHDR wh[BUF_NUMS];
WAVEHDR wh2;
WAVEFORMATEX wfm;
WriteParams params;
int recording=0;
int nChanels,nBits,nSpeed;
extern CDC *g_pDC;
extern CRect g_Rect;
extern BOOL g_bVisual;
/****************************ϵͳ����*************************************/

   

/****************************�˲����Ķ���*************************************/
class IIR_I
{
     private:
	 double *m_pNum;
	 double *m_pDen;
	 double *m_px;

	  double *m_py;
	  int m_num_order;
	  int m_den_order;
      public:
     IIR_I();
    void reset();
    void setPara(double num[], int num_order, double den[], int den_order);
    void resp(double data_in[], int m, double data_out[], int n);
    double filter(double data);
    void filter(double data[], int len);
    void filter(double data_in[], double data_out[], int len);
};
class CFilter_FIR 
 {
    public:	
	CFilter_FIR();
	virtual ~CFilter_FIR();
    private:
	double *m_pB; 	
	double *m_pdata_buf; 
	int m_nB_order; 	
	int m_nFirIndex;public:	void reset();	
	void setPara(double B[], int nB_order); 
	double filter(double data);	
 };
/****************************�˲����Ķ���*************************************/
                              
/*****************************¼��׼������****************************************/
BOOL CreateWaveFile(LPSTR filename,WriteParams *pParams)
{

	MMRESULT result;
	HMMIO hmmio;
	pParams->mmciRiff=new MMCKINFO;
	pParams->mmciFmt=new MMCKINFO;
	pParams->mmciData=new MMCKINFO;

	//Create waveform file
	hmmio = mmioOpen(filename, NULL,
		MMIO_CREATE | MMIO_WRITE | MMIO_ALLOCBUF);
	pParams->hmmio=hmmio;

	//Create Global chunk
	memset(pParams->mmciRiff,0,sizeof(MMCKINFO));
	pParams->mmciRiff->fccType=mmioFOURCC('W', 'A', 'V', 'E');
	result=mmioCreateChunk(hmmio,pParams->mmciRiff,MMIO_CREATERIFF);
	if(result != MMSYSERR_NOERROR)
		return FALSE;

	//Create format chunk
	memset(pParams->mmciFmt,0,sizeof(MMCKINFO));
	pParams->mmciFmt->ckid=mmioFOURCC('f', 'm', 't', ' ');
	result=mmioCreateChunk(hmmio,pParams->mmciFmt,0);
	if(result != MMSYSERR_NOERROR)
		return FALSE;

	//Write waveform format
	mmioWrite(hmmio, (char*)pParams->pwf,
        sizeof(PCMWAVEFORMAT));

	//Adjust format chunk size
	mmioAscend(hmmio,pParams->mmciFmt,0);

	//Create data chunk
	memset(pParams->mmciData,0,sizeof(MMCKINFO));
	pParams->mmciData->ckid=mmioFOURCC('d', 'a', 't', 'a');
	result=mmioCreateChunk(hmmio,pParams->mmciData,0);
	if(result != MMSYSERR_NOERROR)
		return FALSE;

	char strFile[MAX_PATH];
	sprintf(strFile,"%s.dat",filename);
	g_fpWaveFile=fopen(strFile,"w");

	return TRUE;
}

BOOL WriteWaveFile(WriteParams *pParams,LPSTR pData,LONG length)
{
	fwrite(pData,length,1,g_fpWaveFile);


	MMRESULT result;
	if(pParams->hmmio == NULL)
		return FALSE;

	//Write data to file
	result=mmioWrite(pParams->hmmio,pData,length);

	if(result != MMSYSERR_NOERROR)
		return FALSE;


	return TRUE;
}

BOOL CloseWaveFile(WriteParams *pParams)
{

	MMRESULT result;
	if(pParams->hmmio == NULL)
		return FALSE;

	//Adjust data chunk
	mmioAscend(pParams->hmmio,pParams->mmciData,0);

	//Adjust RIFF chunk
	mmioAscend(pParams->hmmio,pParams->mmciRiff,0);

	//Close waveform file
	result=mmioClose(pParams->hmmio,0);//mmsystem���ļ����ر���mmioOpen�򿪵��豸�ļ�.
	if(result != MMSYSERR_NOERROR)
		return FALSE;

	pParams->hmmio=NULL;//���ָ��
	delete pParams->mmciRiff;//�ͷ��ڴ�
	delete pParams->mmciFmt;//�ͷ��ڴ�
	delete pParams->mmciData;//�ͷ��ڴ�

	if(g_fpWaveFile != NULL)
	{
		fclose(g_fpWaveFile);//�ر���CreateWaveFile�����д򿪵��ļ� g_fpWaveFile=fopen(strFile,"w");
		g_fpWaveFile=NULL;//���ָ��
	}

	return TRUE;
}


/*************************************************************
* Implication of recording                                   *
*************************************************************/
BOOL RecordStart()
{
	int i;

	if(hwi != NULL)
		return FALSE;

	//Buffer size
	DWORD bufsize=BUF_SIZE;
	UINT uID;
	MMRESULT result;

	//Is there any device can use
	uID=waveInGetNumDevs();
	if(uID == 0)
		goto ERROR_END;
	uID=WAVE_MAPPER;

	WAVEINCAPS wic;
	//Get device caps
	result=waveInGetDevCaps(uID,&wic,sizeof(WAVEINCAPS));
	if(result != MMSYSERR_NOERROR)
		goto ERROR_END;

	//Set wave formats
	wfm.nChannels=1;
	wfm.nSamplesPerSec=(DWORD)22050;
	wfm.wBitsPerSample=8;
	wfm.wFormatTag=WAVE_FORMAT_PCM;
	wfm.nBlockAlign=1;
	wfm.nAvgBytesPerSec=(DWORD)22050;
	wfm.cbSize=sizeof(WAVEFORMATEX);

	//Open device
	result=waveInOpen(&hwi,
					  uID,
					  &wfm,
					  (DWORD)waveInProc,
					  (DWORD)AfxGetInstanceHandle(),
					  CALLBACK_FUNCTION);
	if(result != MMSYSERR_NOERROR)
		goto ERROR_END;

	for(i=0;i<BUF_NUMS;i++)//BUF_NUMS=5
	{
		//Alloc buffers
		memset(wh+i,0,sizeof(WAVEHDR));
		wh[i].lpData=(char *)GlobalAlloc(GMEM_FIXED | GMEM_SHARE,bufsize);
		wh[i].dwBufferLength=bufsize;

		//Prepare buffers
		result=waveInPrepareHeader(hwi,wh+i,sizeof(WAVEHDR));
		if(result != MMSYSERR_NOERROR)
			goto ERROR_END;

		//Add buffers
		result=waveInAddBuffer(hwi,wh+i,sizeof(WAVEHDR));
		if(result != MMSYSERR_NOERROR)
			goto ERROR_END;
	}

	//Start recording
	result=waveInStart(hwi);
	if(result != MMSYSERR_NOERROR)
		goto ERROR_END;
	recording=1;

	return TRUE;

ERROR_END:
	for(i=0;i<BUF_NUMS;i++)
	{
		if(wh[i].lpData != NULL)
		{
			waveInUnprepareHeader(hwi,wh+i,sizeof(WAVEHDR));
			GlobalFree(wh[i].lpData);
		}
	}
	if(hwi != NULL)
	{
		waveInClose(hwi);
		hwi=NULL;
	}
	return FALSE;
}

//End recording
void RecordEnd()
{
	int i;

	if(hwi == NULL)//hwiδ����¼���豸
		return;

	MMRESULT result;
	
	//Stop
	result=waveInStop(hwi);//mmsystem�еĺ�����ֹͣ�����豸����˷磩���ɹ�����0
	recording=0;//���½�¼����������Ϊ��¼����������һ��¼��

	//Reset buffers
	result=waveInReset(hwi);//waveInReset:mmsystem�еĺ���,�����������룬�豸��ֹ����, λ����0; ����δ����Ļ����������ظ�����
                            //Ϊ�´�¼��׼��      
	for(i=0;i<BUF_NUMS;i++)
	{
		//Unprepare and release buffers
		waveInUnprepareHeader(hwi,wh+i,sizeof(WAVEHDR));
		GlobalFree(wh[i].lpData);
	}

	//Close device
	result=waveInClose(hwi);//�ر�ָ���Ĳ��������豸,�ɹ����� 0
	hwi=NULL;//ȡ��hwi��ָ��¼���豸����¼��ʱ������ָ��
}

//Callback function
void CALLBACK waveInProc(HWAVEIN hwi,
						 UINT uMsg,
						 DWORD dwInstance,
						 DWORD dwParam1,
						 DWORD dwParam2)
{
	if(uMsg == WIM_DATA)
	{
		WAVEHDR *pwh;

		pwh=(WAVEHDR *)dwParam1;
		
		//Save waveform data
		WriteWaveFile(&params,(LPSTR)(pwh->lpData),pwh->dwBytesRecorded);
		if(g_bVisual)
			ShowWave(g_pDC,&g_Rect,(LPSTR)(pwh->lpData),pwh->dwBytesRecorded);
		else
			ShowFreq(g_pDC,&g_Rect,(LPSTR)(pwh->lpData),pwh->dwBytesRecorded);
		
		//Reuse the buffers
		if(recording != 0)
		{
			waveInPrepareHeader(hwi,pwh,sizeof(WAVEHDR));
			waveInAddBuffer(hwi,pwh,sizeof(WAVEHDR));
		}
	}
}

/*****************************¼��׼������****************************************/

/*****************************¼�����Ų���****************************************/
BOOL PlayStart(LPCSTR file,WAVEFORMATEX *pwf,LPSTR *pData,LONG *pLen)
{
	HMMIO hmmio=NULL;
	MMCKINFO ckiRiff,cki;
	MMRESULT result;
	LONG len;
	if(hwo != NULL) //�ط��豸�Ѵ�
	{
		result=waveOutReset(hwo);//ֹͣ����
		if(result != MMSYSERR_NOERROR)
			goto END_ERROR;
		result=waveOutClose(hwo);//�رմ򿪵Ļط��豸
		if(result != MMSYSERR_NOERROR)
			goto END_ERROR;
		hwo=NULL;//�رմ򿪵Ļط��豸֮�����豸�ص����б���
	}

	hmmio=mmioOpen((LPSTR)file,NULL,MMIO_READ | MMIO_ALLOCBUF);//Ϊ���������һ����ý���ļ�file
	if(hmmio == NULL)
		return FALSE;

	ckiRiff.fccType=mmioFOURCC('W','A','V','E');        //��ȡ�ܿ����Ϣ
	result=mmioDescend(hmmio,&ckiRiff,NULL,MMIO_FINDRIFF);
	if(result != MMSYSERR_NOERROR)
		goto END_ERROR;

	cki.ckid=mmioFOURCC('f','m','t',' ');     //��ȡ��ʽ��Ϣ
	result=mmioDescend(hmmio,&cki,&ckiRiff,MMIO_FINDCHUNK);
	if(result != MMSYSERR_NOERROR)
		goto END_ERROR;

	len=mmioRead(hmmio,(LPSTR)pwf,sizeof(WAVEFORMATEX));//hmmio���ļ����,����ȡ���ļ��ľ����pwf��ָ��һ��������,����/��Ŵ��ļ���ȡ�����ݡ�sizeof(WAVEFORMATEX)Ҫ���ļ���ȡ���ֽ���
	if(len == -1)
		goto END_ERROR;

    //ͨ����mmioAscend�����Ի��DATA��Ϣ����
	result=mmioAscend(hmmio,&cki,0);
	if(result != MMSYSERR_NOERROR)
		goto END_ERROR;


    //��ȡ���ݿ�
	cki.ckid=mmioFOURCC('d','a','t','a');  //dataID �͸�CKI�ӿ�   ��ȡ������Ϣ 
	result=mmioDescend(hmmio,&cki,&ckiRiff,MMIO_FINDCHUNK);  //�ܿ������������ݿ����Ϣ DATA
	if(result != MMSYSERR_NOERROR)
		goto END_ERROR;
     //������Ϣ���
	*pLen=cki.cksize; //CKIΪ���ݿ���Ϣ  *pLen=cki.cksize������ݳ���  
	*pData=(LPSTR)GlobalAlloc(GMEM_FIXED | GMEM_SHARE,*pLen);//����һ�����ݿ��С�Ŀռ�  �ɶ�ȡ����      //�����ڴ��ַ��*pLen��ָ��Ĵ�С���ڴ棬GMEM_FIXED����̶����ڴ�,����ֵ��һ��ָ��
	//���������óɹ�,�򷵻�һ���·�����ڴ����ľ��,�������û���㹻�Ŀռ���������,���������� NULL
	if(*pData == NULL)
		goto END_ERROR;
	len=mmioRead(hmmio,(LPSTR)*pData,*pLen);//!!�Ӵ򿪵��ļ�hmmio�ж�ȡ����Ϊ*pLen�����ݵ�pData��ָ��ĵ�ַ��  PDATA���Ѿ�������������Ϣ/////////
	if(len == -1)
		goto END_ERROR;
	result=waveOutOpen(&hwo,// ���ӳɹ� �򷵻ز�Ϊ��   //һ��ָ����ղ�����Ƶ���װ�ñ��Ļ������������豸�����ָ��
					  WAVE_MAPPER, //    //��Ҫ���򿪵Ĳ�����Ƶ���װ�õ�ID,ָ��ΪWAVE_MAPPERʱ��������ݸ����Ĳ��θ�ʽѡ����ʵ��豸
					  pwf, //���νṹ��ָ�룬����Ҫ����Ĳ��θ�ʽ
					  NULL,//�ص�������ַ�򴰿�ָ�룬��ʹ�ûص���������NULL
					  (DWORD)AfxGetInstanceHandle(),//���ݵ�CALLBACK���̵��û�ʵ�����ݡ�����Ǵ���CALLBACK���̵Ļ����ò�������
					  CALLBACK_NULL//  Ĭ�ϵ����ã�����CALLBACK����
					  );//���������һ�������Ĳ�����Ƶ���װ�������лط�
	if(result != MMSYSERR_NOERROR)   //��ʧ�� ���������
		goto END_ERROR;
	memset(&wh2,0,sizeof(WAVEHDR));  //�洢������ //WAVEHDR wh2; wh2Ϊȫ�ֱ�����//
    /*typedef struct { 
    LPSTR  lpData; //���λ�������(�����׵�ַ)  ���ݿ���׵�ַ
    DWORD  dwBufferLength; //����������
    DWORD  dwBytesRecorded; //ָ��¼��ʱ����������
    DWORD  dwUser; //�û�����
    DWORD  dwFlags; //�ṩ��������ʾ                                             
    DWORD  dwLoops; //ѭ������
    struct wavehdr_tag * lpNext; //Ԥ��,NULL
    DWORD  reserved; //Ԥ��,0
    } WAVEHDR;*/

	wh2.lpData=*pData;  //����������λ��       //�ɶ�ȡ����
	wh2.dwBufferLength=*pLen;  //�������ݳ�

	result=waveOutPrepareHeader(hwo,&wh2,sizeof(WAVEHDR));//hwo��Ҫ���ŵ��豸  wh2Ϊ����  ׼��һ���������ݿ����ڲ���
	if(result != MMSYSERR_NOERROR)
		goto END_ERROR;
	//::MessageBox(NULL, "msg1", "caption", MB_OK);

	waveOutWrite(hwo,&wh2,sizeof(WAVEHDR));   //�����͸������豸  �����豸��������
	
	//��������,mmsystem����,��һ�����ݿ鷢�͵�һ��ָ���Ĳ�����Ƶ���װ�á��ڲ���������ͬʱ���к���ĳ���
    //hwo:HWAVEOUT������Ƶ���װ�õı���handle��,&wh2: һ��ָ����������ݿ���Ϣ��WAVEHDR�ṹ��ָ��
    //��������������WAVEHDR�ṹ�Ĵ�С����sizeof (WAVEHDR) �Ϳ����ˣ�
	
	//::MessageBox(NULL, "msg2", "caption", MB_OK);

	return TRUE;    //�����������ݳɹ�
END_ERROR:  //����ʧ�ܴ��� 
	if(*pData != NULL)
	{
		GlobalFree(*pData);
		*pData=NULL;
		*pLen=0;
	}
	if(hmmio != NULL)
		mmioClose(hmmio,0);

	if(hwo != NULL)
		waveOutClose(hwo);
}
/*****************************¼�����Ų���****************************************/
	
 

/*****************************¼��IIR������****************************************/  
BOOL PlayStart1(LPCSTR file,WAVEFORMATEX *pwf,LPSTR *pData,LONG *pLen)
{
	HMMIO hmmio=NULL;
	MMCKINFO ckiRiff,cki;
	MMRESULT result;
	LONG len;
	/*************IIR��������*********************/
    double b[28]={8.99280649946377e-15,2.23820961764432e-13,3.42481598636368e-12,2.44995135290083e-11,1.66437530424446e-10,7.03039404470474e-10,2.73757905233651e-09,7.88531906437129e-09,2.02708179131150e-08,4.19822754338384e-08,7.65721779316664e-08,1.17259332910180e-07,1.57335307449102e-07,1.80800270754844e-07,1.81222276296467e-07,1.56913301907480e-07,1.17679064715048e-07,7.61865521781147e-08,4.23073629463033e-08,2.00338092781749e-08,8.02722865955730e-09,2.66638977564071e-09,7.31020577404706e-10,1.57936747013920e-10,2.64601604516024e-11,3.17243887409946e-12,2.42146160436309e-13,9.13335806453346e-15};
    double a[28]={1,-14.3886025767929,101.523003529445,-466.910633913951,1570.01582097952,-4105.43575502545,8671.32538914623,-15172.3410973321,22382.4363823637,-28191.3698436073,30592.4087119901,-28786.7011146348,23590.7123474927,-16880.9890071552,10559.2131186569,-5771.41640886962,2751.44147590545,-1140.25559622574,408.679546369461,-125.765697265155,32.9033232948691,-7.22038375212792,1.30454242831869,-0.189019448719416,0.0211209061550664,-0.00170891920438011,8.91250932678719e-05,-2.24998609164659e-06};
    IIR_I filter;
    char *shuru_x,*shuchu_y;
    int tt;
	long int i;
   /*************IIR��������*********************/

	if(hwo != NULL) //�ط��豸�Ѵ�
	{
		result=waveOutReset(hwo);//ֹͣ����
		if(result != MMSYSERR_NOERROR)
			goto END_ERROR;
		result=waveOutClose(hwo);//�رմ򿪵Ļط��豸
		if(result != MMSYSERR_NOERROR)
			goto END_ERROR;
		hwo=NULL;//�رմ򿪵Ļط��豸֮�����豸�ص����б���
	}
	hmmio=mmioOpen((LPSTR)file,NULL,MMIO_READ | MMIO_ALLOCBUF);//Ϊ���������һ����ý���ļ�file
	if(hmmio == NULL)
		return FALSE;
	ckiRiff.fccType=mmioFOURCC('W','A','V','E');        //��ȡ�ܿ����Ϣ
	result=mmioDescend(hmmio,&ckiRiff,NULL,MMIO_FINDRIFF);
	if(result != MMSYSERR_NOERROR)
		goto END_ERROR;
	cki.ckid=mmioFOURCC('f','m','t',' ');     //��ȡ��ʽ��Ϣ
	result=mmioDescend(hmmio,&cki,&ckiRiff,MMIO_FINDCHUNK);
	if(result != MMSYSERR_NOERROR)
		goto END_ERROR;
	len=mmioRead(hmmio,(LPSTR)pwf,sizeof(WAVEFORMATEX));//hmmio���ļ����,����ȡ���ļ��ľ����pwf��ָ��һ��������,����/��Ŵ��ļ���ȡ�����ݡ�sizeof(WAVEFORMATEX)Ҫ���ļ���ȡ���ֽ���
	if(len == -1)
		goto END_ERROR;
    //ͨ����mmioAscend�����Ի��DATA��Ϣ����
	result=mmioAscend(hmmio,&cki,0);
	if(result != MMSYSERR_NOERROR)
		goto END_ERROR;
    //��ȡ���ݿ�
	cki.ckid=mmioFOURCC('d','a','t','a');  //dataID �͸�CKI�ӿ�   ��ȡ������Ϣ 
	result=mmioDescend(hmmio,&cki,&ckiRiff,MMIO_FINDCHUNK);  //�ܿ������������ݿ����Ϣ DATA
	if(result != MMSYSERR_NOERROR)
		goto END_ERROR;
     //������Ϣ���
	*pLen=cki.cksize; //CKIΪ���ݿ���Ϣ  *pLen=cki.cksize������ݳ���  
    //*pData=(LPSTR)GlobalAlloc(GMEM_FIXED | GMEM_SHARE,*pLen);//����һ�����ݿ��С�Ŀռ�  �ɶ�ȡ����      //�����ڴ��ַ��*pLen��ָ��Ĵ�С���ڴ棬GMEM_FIXED����̶����ڴ�,����ֵ��һ��ָ��
	//���������óɹ�,�򷵻�һ���·�����ڴ����ľ��,�������û���㹻�Ŀռ���������,���������� NULL
    //if(*pData == NULL)
	//	goto END_ERROR;

	shuru_x=(LPSTR)GlobalAlloc(GMEM_FIXED | GMEM_SHARE,*pLen);
	if(shuru_x==NULL)
	{
	::MessageBox(NULL, "���뽨��ʧ��", "caption", MB_OK);
	return FALSE;
	}

	shuchu_y=(LPSTR)GlobalAlloc(GMEM_FIXED | GMEM_SHARE,*pLen);
	if(shuchu_y==NULL)
	{
	::MessageBox(NULL, "�������ʧ��", "caption", MB_OK);
	return FALSE;
	}

	len=mmioRead(hmmio,shuru_x,*pLen);//!!�Ӵ򿪵��ļ�hmmio�ж�ȡ����Ϊ*pLen�����ݵ�pData��ָ��ĵ�ַ��  PDATA���Ѿ�������������Ϣ/////////
	if(len == -1)
	{
	::MessageBox(NULL, "��������ʧ��", "caption", MB_OK);
    return FALSE;
	}

	/*****************************IIR�����źŴ���*********************************/
    filter.setPara(b, 28, a, 28);	
	for(i=50;i<*pLen;i++)
	{
     tt=int(filter.filter(byte(*(shuru_x+i))));
     if(tt>255)
	 *(shuchu_y+i)=255;
	 else if(tt<0) *(shuchu_y+i)=0;
	 else *(shuchu_y+i)=byte(tt); 

	}
  	/*****************************IIR�����źŴ���*********************************/
	



	result=waveOutOpen(&hwo,// ���ӳɹ� �򷵻ز�Ϊ��   //һ��ָ����ղ�����Ƶ���װ�ñ��Ļ������������豸�����ָ��
					  WAVE_MAPPER, //    //��Ҫ���򿪵Ĳ�����Ƶ���װ�õ�ID,ָ��ΪWAVE_MAPPERʱ��������ݸ����Ĳ��θ�ʽѡ����ʵ��豸
					  pwf, //���νṹ��ָ�룬����Ҫ����Ĳ��θ�ʽ
					  NULL,//�ص�������ַ�򴰿�ָ�룬��ʹ�ûص���������NULL
					  (DWORD)AfxGetInstanceHandle(),//���ݵ�CALLBACK���̵��û�ʵ�����ݡ�����Ǵ���CALLBACK���̵Ļ����ò�������
					  CALLBACK_NULL//  Ĭ�ϵ����ã�����CALLBACK����
					  );//���������һ�������Ĳ�����Ƶ���װ�������лط�
	if(result != MMSYSERR_NOERROR)   //��ʧ�� ���������
		goto END_ERROR;


	memset(&wh2,0,sizeof(WAVEHDR));  //�洢������ //WAVEHDR wh2; wh2Ϊȫ�ֱ�����//
    /*typedef struct { 
    LPSTR  lpData; //���λ�������(�����׵�ַ)  ���ݿ���׵�ַ
    DWORD  dwBufferLength; //����������
    DWORD  dwBytesRecorded; //ָ��¼��ʱ����������
    DWORD  dwUser; //�û�����
    DWORD  dwFlags; //�ṩ��������ʾ                                             
    DWORD  dwLoops; //ѭ������
    struct wavehdr_tag * lpNext; //Ԥ��,NULL
    DWORD  reserved; //Ԥ��,0
    } WAVEHDR;*/

	wh2.lpData=shuru_x;  //����������λ��       //�ɶ�ȡ����
	wh2.dwBufferLength=*pLen;  //�������ݳ���

	result=waveOutPrepareHeader(hwo,&wh2,sizeof(WAVEHDR));//hwo��Ҫ���ŵ��豸  wh2Ϊ����  ׼��һ���������ݿ����ڲ���
	if(result != MMSYSERR_NOERROR)
		goto END_ERROR;

	//::MessageBox(NULL, "msg1", "caption", MB_OK);

	waveOutWrite(hwo,&wh2,sizeof(WAVEHDR));   //�����͸������豸  �����豸��������
	::MessageBox(NULL, "ԭʼ�����źŲ���", "caption", MB_OK);
	//��������,mmsystem����,��һ�����ݿ鷢�͵�һ��ָ���Ĳ�����Ƶ���װ�á��ڲ���������ͬʱ���к���ĳ���
    //hwo:HWAVEOUT������Ƶ���װ�õı���handle��,&wh2: һ��ָ����������ݿ���Ϣ��WAVEHDR�ṹ��ָ��
    //��������������WAVEHDR�ṹ�Ĵ�С����sizeof (WAVEHDR) �Ϳ����ˣ�
	
	//::MessageBox(NULL, "msg2", "caption", MB_OK);

    Sleep(3000);
	wh2.lpData=shuchu_y;  //����������λ��       //�ɶ�ȡ����
	wh2.dwBufferLength=*pLen;  //�������ݳ���
	result=waveOutPrepareHeader(hwo,&wh2,sizeof(WAVEHDR));//hwo��Ҫ���ŵ��豸  wh2Ϊ����  ׼��һ���������ݿ����ڲ���
	if(result != MMSYSERR_NOERROR)
		goto END_ERROR;

	//::MessageBox(NULL, "msg1", "caption", MB_OK);

	waveOutWrite(hwo,&wh2,sizeof(WAVEHDR));   //�����͸������豸  �����豸��������
	::MessageBox(NULL, "����������źŲ���", "caption", MB_OK);
	//��������,mmsystem����,��һ�����ݿ鷢�͵�һ��ָ���Ĳ�����Ƶ���װ�á��ڲ���������ͬʱ���к���ĳ���
    //hwo:HWAVEOUT������Ƶ���װ�õı���handle��,&wh2: һ��ָ����������ݿ���Ϣ��WAVEHDR�ṹ��ָ��
    //��������������WAVEHDR�ṹ�Ĵ�С����sizeof (WAVEHDR) �Ϳ����ˣ�
	
	//::MessageBox(NULL, "msg2", "caption", MB_OK);
	return TRUE;    //�����������ݳɹ�
END_ERROR:  //����ʧ�ܴ��� 
	if(*pData != NULL)
	{
		GlobalFree(*pData);
		*pData=NULL;
		*pLen=0;
	}
	if(hmmio != NULL)
		mmioClose(hmmio,0);

	if(hwo != NULL)
		waveOutClose(hwo);
}
/*****************************¼��IIR������****************************************/ 



/*****************************¼��FIR������****************************************/
BOOL PlayStart2(LPCSTR file,WAVEFORMATEX *pwf,LPSTR *pData,LONG *pLen)
{
	HMMIO hmmio=NULL;
	MMCKINFO ckiRiff,cki;
	MMRESULT result;
	LONG len;
	/*************FIR��������*********************/
    double B[145]={1.75018647069595e-06,2.55863162825405e-06,-7.35029015103877e-06,-3.05553788905629e-05,-5.22590554508151e-05,-4.48246700058963e-05,1.15820714157737e-05,0.000104981322491362,0.000184069040941911,0.000180096237550848,5.19890569215929e-05,-0.000171175910633784,-0.000382425223254448,-0.000440895436289204,-0.000253650260276832,0.000148183699113967,0.000590499639414667,0.000822387783155894,0.000648091544933009,5.95410863397470e-05,-0.000711324689049987,-0.00127063225166704,-0.00125378637850730,-0.000541708882884406,0.000618677842039304,0.00167858609809813,0.00203543209774289,0.00135871772694988,-0.000175797001127152,-0.00189122542530314,-0.00289271672721093,-0.00251891107705612,-0.000740779589628829,0.00171929863814984,0.00365737752773367,0.00396021609146746,0.00221613136265729,-0.000959152991905070,-0.00409841135513349,-0.00553867460626202,-0.00427504260062688,-0.000586112841115091,0.00393229479313007,0.00702402068109182,0.00686742405830994,0.00309145502125111,-0.00283052899549147,-0.00809819960113587,-0.00986422508805442,-0.00671216769107771,
	0.000408154778912492,0.00834410602914845,0.0130653715181467,0.0116261457165139,0.00384567705268102,-0.00718926404924045,-0.0162192999798108,-0.0181766114493111,-0.0108118498919649,0.00368948809533198,0.0190516887047349,0.0273418421371759,0.0226344622271340,0.00435522897662452,-0.0212993553708812,-0.0427877378980623,-0.0472448968129791,-0.0256770281152617,0.0227442985176130,0.0891649642300809,0.157169744644049,0.207963080629717,0.226757924177676,0.207963080629717,0.157169744644049,0.0891649642300809,0.0227442985176130,-0.0256770281152617,-0.0472448968129791,-0.0427877378980623,-0.0212993553708812,0.00435522897662452,0.0226344622271340,0.0273418421371759,0.0190516887047349,0.00368948809533198,-0.0108118498919649,-0.0181766114493111,-0.0162192999798108,-0.00718926404924045,0.00384567705268102,0.0116261457165139,0.0130653715181467,0.00834410602914845,0.000408154778912492,-0.00671216769107771,-0.00986422508805442,-0.00809819960113587,-0.00283052899549147,0.00309145502125111,0.00686742405830994,0.00702402068109182,0.00393229479313007,-0.000586112841115091,-0.00427504260062688,-0.00553867460626202,-0.00409841135513349,-0.000959152991905070,0.00221613136265729,0.00396021609146746,
	0.00365737752773367,0.00171929863814984,-0.000740779589628829,-0.00251891107705612,-0.00289271672721093,-0.00189122542530314,-0.000175797001127152,0.00135871772694988,0.00203543209774289,0.00167858609809813,0.000618677842039304,-0.000541708882884406,-0.00125378637850730,-0.00127063225166704,-0.000711324689049987,5.95410863397470e-05,0.000648091544933009,0.000822387783155894,0.000590499639414667,0.000148183699113967,-0.000253650260276832,-0.000440895436289204,-0.000382425223254448,-0.000171175910633784,5.19890569215929e-05,0.000180096237550848,0.000184069040941911,0.000104981322491362,1.15820714157737e-05,-4.48246700058963e-05,-5.22590554508151e-05,-3.05553788905629e-05,-7.35029015103877e-06,2.55863162825405e-06,1.75018647069595e-06};
	CFilter_FIR filter;
    char *shuru_x,*shuchu_y;
    int tt;
	long int i;
   /*************FIR��������*********************/
	if(hwo != NULL) //�ط��豸�Ѵ�
	{
		result=waveOutReset(hwo);//ֹͣ����
		if(result != MMSYSERR_NOERROR)
			goto END_ERROR;
		result=waveOutClose(hwo);//�رմ򿪵Ļط��豸
		if(result != MMSYSERR_NOERROR)
			goto END_ERROR;
		hwo=NULL;//�رմ򿪵Ļط��豸֮�����豸�ص����б���
	}
	hmmio=mmioOpen((LPSTR)file,NULL,MMIO_READ | MMIO_ALLOCBUF);//Ϊ���������һ����ý���ļ�file
	if(hmmio == NULL)
		return FALSE;
	ckiRiff.fccType=mmioFOURCC('W','A','V','E');        //��ȡ�ܿ����Ϣ
	result=mmioDescend(hmmio,&ckiRiff,NULL,MMIO_FINDRIFF);
	if(result != MMSYSERR_NOERROR)
		goto END_ERROR;
	cki.ckid=mmioFOURCC('f','m','t',' ');     //��ȡ��ʽ��Ϣ
	result=mmioDescend(hmmio,&cki,&ckiRiff,MMIO_FINDCHUNK);
	if(result != MMSYSERR_NOERROR)
		goto END_ERROR;
	len=mmioRead(hmmio,(LPSTR)pwf,sizeof(WAVEFORMATEX));//hmmio���ļ����,����ȡ���ļ��ľ����pwf��ָ��һ��������,����/��Ŵ��ļ���ȡ�����ݡ�sizeof(WAVEFORMATEX)Ҫ���ļ���ȡ���ֽ���
	if(len == -1)
		goto END_ERROR;
    //ͨ����mmioAscend�����Ի��DATA��Ϣ����
	result=mmioAscend(hmmio,&cki,0);
	if(result != MMSYSERR_NOERROR)
		goto END_ERROR;
    //��ȡ���ݿ�
	cki.ckid=mmioFOURCC('d','a','t','a');  //dataID �͸�CKI�ӿ�   ��ȡ������Ϣ 
	result=mmioDescend(hmmio,&cki,&ckiRiff,MMIO_FINDCHUNK);  //�ܿ������������ݿ����Ϣ DATA
	if(result != MMSYSERR_NOERROR)
		goto END_ERROR;
     //������Ϣ���
	*pLen=cki.cksize; //CKIΪ���ݿ���Ϣ  *pLen=cki.cksize������ݳ���  
    //*pData=(LPSTR)GlobalAlloc(GMEM_FIXED | GMEM_SHARE,*pLen);//����һ�����ݿ��С�Ŀռ�  �ɶ�ȡ����      //�����ڴ��ַ��*pLen��ָ��Ĵ�С���ڴ棬GMEM_FIXED����̶����ڴ�,����ֵ��һ��ָ��
	//���������óɹ�,�򷵻�һ���·�����ڴ����ľ��,�������û���㹻�Ŀռ���������,���������� NULL
    //if(*pData == NULL)
	//	goto END_ERROR;
	shuru_x=(LPSTR)GlobalAlloc(GMEM_FIXED | GMEM_SHARE,*pLen);
	if(shuru_x==NULL)
	{
	::MessageBox(NULL, "���뽨��ʧ��", "caption", MB_OK);
	return FALSE;
	}
	shuchu_y=(LPSTR)GlobalAlloc(GMEM_FIXED | GMEM_SHARE,*pLen);
	if(shuchu_y==NULL)
	{
	::MessageBox(NULL, "�������ʧ��", "caption", MB_OK);
	return FALSE;
	}
	len=mmioRead(hmmio,shuru_x,*pLen);//!!�Ӵ򿪵��ļ�hmmio�ж�ȡ����Ϊ*pLen�����ݵ�pData��ָ��ĵ�ַ��  PDATA���Ѿ�������������Ϣ/////////
	if(len == -1)
	{
	::MessageBox(NULL, "��������ʧ��", "caption", MB_OK);
    return FALSE;
	}
	/*****************************FIR�����źŴ���*********************************/
    filter.setPara(B,145);	
	for(i=50;i<*pLen;i++)
	{
     tt=int(filter.filter(byte(*(shuru_x+i))));
     if(tt>255)
	 *(shuchu_y+i)=255;
	 else if(tt<0) *(shuchu_y+i)=0;
	 else *(shuchu_y+i)=byte(tt); 
	}
    /*****************************FIR�����źŴ���*********************************/
	result=waveOutOpen(&hwo,// ���ӳɹ� �򷵻ز�Ϊ��   //һ��ָ����ղ�����Ƶ���װ�ñ��Ļ������������豸�����ָ��
					  WAVE_MAPPER, //    //��Ҫ���򿪵Ĳ�����Ƶ���װ�õ�ID,ָ��ΪWAVE_MAPPERʱ��������ݸ����Ĳ��θ�ʽѡ����ʵ��豸
					  pwf, //���νṹ��ָ�룬����Ҫ����Ĳ��θ�ʽ
					  NULL,//�ص�������ַ�򴰿�ָ�룬��ʹ�ûص���������NULL
					  (DWORD)AfxGetInstanceHandle(),//���ݵ�CALLBACK���̵��û�ʵ�����ݡ�����Ǵ���CALLBACK���̵Ļ����ò�������
					  CALLBACK_NULL//  Ĭ�ϵ����ã�����CALLBACK����
					  );//���������һ�������Ĳ�����Ƶ���װ�������лط�
	if(result != MMSYSERR_NOERROR)   //��ʧ�� ���������
		goto END_ERROR;
	memset(&wh2,0,sizeof(WAVEHDR));  //�洢������ //WAVEHDR wh2; wh2Ϊȫ�ֱ�����//
    /*typedef struct { 
    LPSTR  lpData; //���λ�������(�����׵�ַ)  ���ݿ���׵�ַ
    DWORD  dwBufferLength; //����������
    DWORD  dwBytesRecorded; //ָ��¼��ʱ����������
    DWORD  dwUser; //�û�����
    DWORD  dwFlags; //�ṩ��������ʾ                                             
    DWORD  dwLoops; //ѭ������
    struct wavehdr_tag * lpNext; //Ԥ��,NULL
    DWORD  reserved; //Ԥ��,0
    } WAVEHDR;*/
	wh2.lpData=shuru_x;  //����������λ��       //�ɶ�ȡ����
	wh2.dwBufferLength=*pLen;  //�������ݳ���
	result=waveOutPrepareHeader(hwo,&wh2,sizeof(WAVEHDR));//hwo��Ҫ���ŵ��豸  wh2Ϊ����  ׼��һ���������ݿ����ڲ���
	if(result != MMSYSERR_NOERROR)
		goto END_ERROR;
	//::MessageBox(NULL, "msg1", "caption", MB_OK);
	waveOutWrite(hwo,&wh2,sizeof(WAVEHDR));   //�����͸������豸  �����豸��������
	::MessageBox(NULL, "ԭʼ�����źŲ���", "caption", MB_OK);
	//��������,mmsystem����,��һ�����ݿ鷢�͵�һ��ָ���Ĳ�����Ƶ���װ�á��ڲ���������ͬʱ���к���ĳ���
    //hwo:HWAVEOUT������Ƶ���װ�õı���handle��,&wh2: һ��ָ����������ݿ���Ϣ��WAVEHDR�ṹ��ָ��
    //��������������WAVEHDR�ṹ�Ĵ�С����sizeof (WAVEHDR) �Ϳ����ˣ�
	//::MessageBox(NULL, "msg2", "caption", MB_OK);
    Sleep(3000);
	wh2.lpData=shuchu_y;  //����������λ��       //�ɶ�ȡ����
	wh2.dwBufferLength=*pLen;  //�������ݳ���
	result=waveOutPrepareHeader(hwo,&wh2,sizeof(WAVEHDR));//hwo��Ҫ���ŵ��豸  wh2Ϊ����  ׼��һ���������ݿ����ڲ���
	if(result != MMSYSERR_NOERROR)
		goto END_ERROR;
	//::MessageBox(NULL, "msg1", "caption", MB_OK);
	waveOutWrite(hwo,&wh2,sizeof(WAVEHDR));   //�����͸������豸  �����豸��������
	::MessageBox(NULL, "����������źŲ���", "caption", MB_OK);
	//��������,mmsystem����,��һ�����ݿ鷢�͵�һ��ָ���Ĳ�����Ƶ���װ�á��ڲ���������ͬʱ���к���ĳ���
    //hwo:HWAVEOUT������Ƶ���װ�õı���handle��,&wh2: һ��ָ����������ݿ���Ϣ��WAVEHDR�ṹ��ָ��
    //��������������WAVEHDR�ṹ�Ĵ�С����sizeof (WAVEHDR) �Ϳ����ˣ�

	//::MessageBox(NULL, "msg2", "caption", MB_OK);
	return TRUE;    //�����������ݳ�
END_ERROR:  //����ʧ�ܴ��� 
	if(*pData != NULL)
	{
		GlobalFree(*pData);
		*pData=NULL;
		*pLen=0;
	}
	if(hmmio != NULL)
		mmioClose(hmmio,0);

	if(hwo != NULL)
		waveOutClose(hwo);
}
/*****************************¼��FIR������****************************************/

void ShowFreq(CDC *pDC,CRect *pRect,LPSTR pData,long len)
{
	/*int ox,oy,a,x,y;
	long i,k;

	memset(inImg,0,512*sizeof(double));
	for(i=0;i<512;i++)
		inReal[i]=(double)(128-(unsigned char)pData[i]);
	fft_double(512,0,inReal,inImg,outReal,outImg);
	domfreq=FindDominantFrequency(512,4,outReal,outImg,outAmp,11025);

	pDC->FillSolidRect(pRect,RGB(255,255,255));

	ox=pRect->left;
	oy=pRect->top+pRect->Height()-5;

	k=pRect->Width() > len ? len : pRect->Width();
	k/=2;

	/*for(i=0;i<k;i++)
	{
		x=ox+i*2;
		pDC->MoveTo(x,oy);
		if(i>10)
			y=oy-(int)(outAmp[i]/10000);
		else
			y=oy-1;
		pDC->LineTo(x,y);
		if(i%10 == 0)
			pDC->LineTo(x,oy+4);
	}

	g_pDlg->SetDlgItemInt(IDC_DOMFREQ,(int)domfreq);*/
}

void ShowWave(CDC *pDC,CRect *pRect,LPSTR pData,long len)
{
	int ox,oy,data;
	long i,k;

	//pDC->FillSolidRect(pRect,RGB(255,255,255));

	//ox=pRect->left;
	//oy=pRect->top+pRect->Height()/2;

//k=pRect->Width() > len ? len : pRect->Width();

	//pDC->MoveTo(ox,oy);
	for(i=0;i<512;i++)
	{
		data=(unsigned char)pData[i];
		//pDC->LineTo(ox+i,oy+128-data);
		pDlg->scope(data);
		pDlg->filewrite(data);
	}

}


/*******************************************************IIR�˲������岿��************************************/
void IIR_I::reset()
{
 for(int i = 0; i <= m_num_order; i++)
    {
      m_pNum[i] = 0.0;
 }

  for(i = 0; i <= m_den_order; i++)
    {
        m_pDen[i] = 0.0;
    }
}

IIR_I::IIR_I()
{
  m_pNum=NULL;
  m_pDen=NULL;
  m_px=NULL;
  m_py=NULL;
  m_num_order = -1;
  m_den_order = -1;
};

/** \brief

 * \param num ���Ӷ���ʽ��ϵ������������,num[0] Ϊ������

 * \param m ���Ӷ���ʽ�Ľ���

 * \param den ��ĸ����ʽ��ϵ������������,den[0] Ϊ������

 * \param m ��ĸ����ʽ�Ľ���

 * \return

 */

void IIR_I::setPara(double num[], int num_order, double den[], int den_order)
{
    delete[] m_pNum;
    delete[] m_pDen;
    delete[] m_px;
    delete[] m_py;
    m_pNum = new double[num_order + 1];
    m_pDen = new double[den_order + 1];
    m_num_order = num_order;
    m_den_order = den_order;
    m_px = new double[num_order + 1];
    m_py = new double[den_order + 1];
    for(int i = 0; i <= m_num_order; i++)

    {
        m_pNum[i] = num[i];

        m_px[i] = 0.0;
    }
    for( i = 0; i <= m_den_order; i++)
    {
        m_pDen[i] = den[i];
        m_py[i] = 0.0;
    }
}

/** \brief �˲�����������ֱ��I�ͽṹ

 * \param data ������������

 * \return �˲���Ľ��
 */

double IIR_I::filter(double data)
{
    m_py[0] = 0.0; // ����˲���Ľ�

    m_px[0] = data;

    for(int i = 0; i <= m_num_order; i++)
    {
     m_py[0] = m_py[0] + m_pNum[i] * m_px[i];
    }
  for( i = 1; i <= m_den_order; i++)
    {

      m_py[0] = m_py[0] - m_pDen[i] * m_py[i];
    }
    for( i = m_num_order; i >= 1; i--)
    {
    m_px[i] = m_px[i-1];
    }
    for(i = m_den_order; i >= 1; i--)
    {
        m_py[i] = m_py[i-1];

    }
    return m_py[0];

}

/** \brief �˲�����������ֱ��I�ͽṹ

 * \param data[] �����������ݣ�����ʱ�����˲���Ľ��

 * \param len data[] ����ĳ���

 * \return

 */

void IIR_I::filter(double data[], int len)

{
    int i, k;
    for(k = 0; k < len; k++)
    {
        m_px[0] = data[k];
        data[k] = 0.0;
        for(i = 0; i <= m_num_order; i++)
        {
         data[k] = data[k] + m_pNum[i] * m_px[i];
        }

        for(i = 1; i <= m_den_order; i++)
        {
            data[k] = data[k] - m_pDen[i] * m_py[i];
        }

       // we get the y value now
        m_py[0] = data[k];

        for(i = m_num_order; i >= 1; i--)
		{
        m_px[i] = m_px[i-1];

        }
        for(i = m_den_order; i >= 1; i--)

        {
            m_py[i] = m_py[i-1];
		}
    }
}

/** \brief �˲�����������ֱ��I�ͽṹ

 * \param data_in[] ��������

 * \param data_out[] �����˲��������

 * \param len ����ĳ���

 * \return
 */

void IIR_I::filter(double data_in[], double data_out[], int len)

{
    int i, k;
    for(k=0; k<len;k++)
	{
        m_px[0] = data_in[k];
        m_py[0] = 0.0;
        for(i=0;i<= m_num_order; i++)
        {
        m_py[0]=m_py[0]+m_pNum[i]*m_px[i];
		}
        for(i=1; i<=m_den_order;i++)
        {
            m_py[0]=m_py[0]-m_pDen[i] * m_py[i];
        }
        for(i = m_num_order; i >= 1; i--)
        {
         m_px[i] = m_px[i-1];
		}
        for(i = m_den_order; i >= 1; i--)

        {
            m_py[i] = m_py[i-1];
        }
        data_out[k] = m_py[0];
    }
}




void IIR_I::resp(double data_in[], int M, double data_out[], int N)
{

    int i, k, il;
    for(k = 0; k < N; k++)
 {
        data_out[k] = 0.0;
        for(i = 0; i <= m_num_order; i++)
        {
            if( k - i >= 0)
            {
                il = ((k - i) < M) ? (k - i) : (M - 1);
                data_out[k] = data_out[k] + m_pNum[i] * data_in[il];

            }
        }
        for(i = 1; i <= m_den_order; i++)
        {
            if( k - i >= 0)
            {
                data_out[k] = data_out[k] - m_pDen[i] * data_out[k - i];
            }
        }
    }

}


void test(void)
{
	double b[5] = {0.001836, 0.007344, 0.011016, 0.007344, 0.001836};
    double a[5] = {1.0, -3.0544, 3.8291, -2.2925, 0.55075};
    double x[100], y[100];
    int len = 100;
    IIR_I filter;
    filter.setPara(b, 4, a, 4);
    for (int i = 0; i < len; i++)
    {
     x[i] = 0.0;
     y[i] = 0.0;
    }
    x[0] = 1.0;
    filter.filter(x, y, 100);
    //filter.reset();
    //filter.filter(x, 100);
    for (i = 0; i < len; i++)
    {
       cout << x[i] <<", " << y[i]<<  endl;
    }
}
/*******************************************************IIR�˲������岿��************************************/

/*******************************************************FIR�˲������岿��************************************/




CFilter_FIR::CFilter_FIR()
{	m_pB = NULL;
	m_pdata_buf = NULL;	
	m_nB_order = 0; 
	m_nFirIndex = 0;
} 

CFilter_FIR::~CFilter_FIR()
{	
	if (m_pdata_buf)
	{	
		delete m_pdata_buf;	
		m_pdata_buf = NULL;
	}	
	if (m_pB)
	{
		delete m_pB;	
		m_pB = NULL;
	}
}

void CFilter_FIR::reset()
{   
	for(int i = 0; i < m_nB_order; i++)   
	{	
		m_pdata_buf[i] = 0.0;  
  }	
	m_nFirIndex = 0;
} 
 void CFilter_FIR::setPara(double B[], int nB_order)
 {	
	 m_nB_order = nB_order; 	
	 if (m_pB)	
	 {		
		 delete m_pB;
	}	
	 if (m_pdata_buf)	
	 {	
		 delete m_pdata_buf;
	}    
	 m_pB = new double[nB_order ]; 
	 m_pdata_buf = new double[nB_order ]; 
	 for(int i = 0; i < nB_order; i++)  
	 {        m_pB[i] = B[i];      
	          m_pdata_buf[i] = 0.0;   
 }
}  
 
 double CFilter_FIR::filter(double data)
 {	
	 int k;	
	 double fOut = 0.0;    
	 int i = 0;
	 m_pdata_buf[m_nFirIndex%m_nB_order] = data;
	 fOut = 0.0;	
	 for (i = 0; i < m_nB_order; ++i)	
	 {		
		 fOut += m_pdata_buf[(m_nFirIndex +i+1)%m_nB_order]*m_pB[i];
	}	
	 m_nFirIndex++;	
	 return fOut;
 }
/*******************************************************FIR�˲������岿��************************************/
