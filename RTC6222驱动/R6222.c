/********************************************************************
 * 文件说明： RTC6222 驱动文件；
 * 移植修改：实现i2c通讯，实现毫秒延时函数；
 * 日期：2022-12-10
 * 版本： V0.1 --初始化版本
 * 作者：B站《含羞草的367朵花》 主页： https://space.bilibili.com/488330245
 * 免责声明：1，本文件仅用于测试
 *           2，如直接将本代码用于生产而且造成损失，本作者不承担任何责任。
 *
 * ********************************************************************/

#include "./radio/R6222.h"
#include "./radio/sim_i2c.h"

#include "debug.h"
#include "VTM8811_12864.H"


uint16 fm_frq = 8750 ;
uint8 fm_vol = 2 ;

void R6222DelayMs(uint16 dx_ms)
{
	Delay_Ms( dx_ms );
}

/*******************************************************************************
* Function: 写寄存器
* Description: 
* Input: void
* Return: void
*******************************************************************************/
void R6222WriteReg(uint8 reg_addr_prm,uint16 reg_data_prm)
{
	SimI2cStart();
	SimI2cSend8bits(0xc8);	
	SimI2cRecvACK();
	SimI2cSend8bits(reg_addr_prm);	
	SimI2cRecvACK();
	SimI2cSend8bits((uint8)(reg_data_prm>>8));
	SimI2cRecvACK();
	SimI2cSend8bits((uint8)reg_data_prm);
	SimI2cRecvACK();
	SimI2cStop();	
}

/*******************************************************************************
* Function: 读寄存器
* Description: 
* Input: void
* Return: void
*******************************************************************************/
uint16 R6222ReadReg(uint8 reg_addr_prm)
{
	uint16 read_temp;
	SimI2cStart();
	SimI2cSend8bits(0xc8);	
	SimI2cRecvACK();
	SimI2cSend8bits(reg_addr_prm);	
	SimI2cRecvACK();

	SimI2cStart();
	SimI2cSend8bits(0xc9);	
	SimI2cRecvACK();

	read_temp=SimI2cRecv8bits();	
	SimI2cSendACK();
	read_temp = read_temp<<8;
	read_temp+=SimI2cRecv8bits();	
	SimI2cSendNACK();
	SimI2cStop();	

	return(read_temp);
}

/*******************************************************************************
* Function: RTC6222 Fm 初始化
* Description:
* Input: void
* Return: void
*******************************************************************************/
void R6222FmInit(void)
{
    SimI2cInit();
    R6222DelayMs(50);
	R6222WriteReg(0x00 , 0x16AA); //Power off
	R6222DelayMs(10);
	R6222WriteReg(0x00 ,0x96AA);//Power on
	R6222DelayMs(20);
	R6222WriteReg(0x02,0x4008);//vol = 0a ,disable mute
	//R6222WriteReg(0x04,0x1100);
	R6222WriteReg(0x04,0x1900);//100K
    R6222WriteReg(0x05,RSSI_LOW_MONO_TH);//set seek noise th and spike th ,0x01h=0010bin,00=80(noise_th),01=144(spike_th),and rssi th=0x0c.
	R6222WriteReg(0x06,0x3000);//bit15:ENABLE IC ,bit12:enable check same channal, bit:2-0set rlck choice one from  32k~38.4M
	//R6222WriteReg(0x07,0xb400);
	R6222WriteReg(0x08,0x043C);//set seek band of freq  start_freq0x043C=1084. x100khz
    R6222WriteReg(0x09 ,0x035c);//set seek start_freq 0x0280=640, x100khz
    R6222WriteReg(0x11,0x0B10); //rclk force 0x0Bh= bin1011
	R6222WriteReg(0x20,0x1108);//1008 hopping open is better than 1018 nohopping //0x1108 confirmed by RD20160330


	R6222WriteReg(0x06 ,(R6222ReadReg(0x06)|0x8000));	  // Write Power On

	R6222DelayMs(10);
	R6222WriteFrq(fm_frq);
	R6222DelayMs(10);
	R6222WriteVol(1);
	R6222DelayMs(10);
}
/*******************************************************************************
* Function: 写音量
* Description: 
* Input: 音量值 0～15
* Return: void
*******************************************************************************/
void R6222WriteVol(uint8 dat)
{
	R6222WriteReg(0x02 ,(0x4000|((dat)&0x0f)));
	LcdShowVol(dat);
}


/*******************************************************************************
* Function: 写频率
* Description: 
* Input: 电台频点：6400(64.0M ~ 10800(108.0MHz)
* Return: void
*******************************************************************************/
void R6222WriteFrq( uint16 frq)
{
    /*Frequency (KHz) = CSR0_CH *10kHz*/
    R6222WriteReg(0x03 , 0X0000);
    R6222DelayMs(5);
    R6222WriteReg(0x03 ,0x8000|frq);

    LcdShowFrq(frq);
}

/*******************************************************************************
* Function: 关机
* Description: 
* Input: void
* Return: void
*******************************************************************************/
void R6222Off(void)
{
    R6222WriteReg(0x00 , 0x16AA);
}


/*******************************************************************************
* Function: 判断电台是否为有效电台
* Description:
* Input: void
* Return: 1:true radio. 0:no radio.
*******************************************************************************/
u8 R6222_Just_Radio(void)
{
    uint16_t  reg0b_data;
    uint16_t  rssi = 0;
    uint16_t  seek_status ;
    reg0b_data  = R6222ReadReg(0X0B);
    seek_status = ((0xFC00 & reg0b_data)>>10);
    rssi = reg0b_data&0X00FF;
    if((rssi >= R6222_JUST_RADIO_FM_RSSI)&&(!seek_status))
        return 1 ;
    return 0 ;
}


/*******************************************************************************
* Function: 搜台
* Description: 
* Input: 搜台方式设定
* Return: SEEK_RE_FINISH ：搜索到有效台；SEEK_RE_FAIL：未发现有效台 ；
*******************************************************************************/
_SEEK_RE_ERROR R6222Seek(uint16 fm_frq_t , _SEEK_PATH seek_path)
{
    volatile uint16 fm_frq_old ;
    fm_frq_old = fm_frq_t ; //记录开始频点

    do{
        if(seek_path == SEEK_PATH_UP)
        {
            fm_frq_t += FM_FRQ_STEP ;
            if(fm_frq_t > FM_FRQ_MAX)
            {
                fm_frq_t = FM_FRQ_MIN ;
            }
        }
        else {
            fm_frq_t -= FM_FRQ_STEP ;
            if(fm_frq_t < FM_FRQ_MIN)
            {
                fm_frq_t = FM_FRQ_MAX ;
            }
        }
        R6222WriteFrq(fm_frq_t);

        R6222DelayMs(50);
        if(R6222_Just_Radio())
        {
            if(R6222_Just_Radio())
            {
                fm_frq = fm_frq_t ;
                R6222WriteFrq(fm_frq);
                return SEEK_RE_FINISH ;
            }
        }

        if(fm_frq_t == fm_frq_old)
        {
            fm_frq = fm_frq_t ;
            R6222WriteFrq(fm_frq);
            return SEEK_RE_FAIL ;
        }

    }while(1);



    return SEEK_RE_FINISH ;


}



