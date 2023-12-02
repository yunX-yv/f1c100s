

#ifndef __R6222_H__
#define __R6222_H__

#ifndef uint8
#define uint8   unsigned char
#endif

#ifndef uint16
#define uint16   unsigned int
#endif

#ifndef uint32
#define uint32    unsigned long int
#endif

#define	R6222_JUST_RADIO_FM_RSSI	31

#define RSSI_LOW_MONO_TH	    	0X655F

typedef enum{
    SEEK_MODE_LOOP = 0 ,//循环搜台
    SEEK_MODE_HALF,//在最高或最低频点停止
}_SEEK_MODE;

typedef enum{
    SEEK_PATH_DOWN = 0 ,//向下
    SEEK_PATH_UP,//向下
}_SEEK_PATH;

typedef enum{
    SEEK_RE_FINISH = 0 ,
    SEEK_RE_FAIL,
}_SEEK_RE_ERROR;

#define  FM_FRQ_MIN     8750
#define  FM_FRQ_MAX     10800
#define  FM_FRQ_STEP    10
extern uint16 fm_frq ;

#define     VOL_MIN     0
#define     VOL_MAX     15
extern uint8 fm_vol ;

void R6222FmInit(void) ;
void R6222WriteVol(uint8 dat);
void R6222WriteFrq(uint16 frq);
void R6222Off(void);
uint8 R6222Just_Radio(void);
_SEEK_RE_ERROR R6222Seek(uint16 fm_frq_t ,  _SEEK_PATH seek_path) ;

#endif




