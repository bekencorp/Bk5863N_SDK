#include "calibration.h"
#include "BK5863.h"

/**************************************************
FunctionName:ReadCalibCoef6
Describe: ����ϵ��6���ϵ��Ԥȡ�ĵ���������ϵ������ϵ����
          ��Ӧ��������壬������Calib_Confʹ��
InputParameter:
OutputParameter:
Notice:
**************************************************/
unsigned int ReadCalibCoef6(void)
{
	return REG_Calib_Coef6;
}

/**************************************************
FunctionName:StartANA_Calib
Describe: ���� Calibration ����
InputParameter:
OutputParameter:
Notice:
**************************************************/
void StartANA_Calib(void)
{
	REG_Start_ANA_Calib = 0x58a55a; 
}

/**************************************************
FunctionName:CalibrationDone
Describe: Calibration�Ƿ����
InputParameter:
OutputParameter:1:Done
                0:not Done
Notice:
**************************************************/
unsigned char CalibrationDone(void)
{
	return (REG_Calib_DONE&0x01);
}
/**************************************************
FunctionName:ReadCalibrationResult
Describe: ��ȡCalibration���
InputParameter:
OutputParameter:
Notice:
**************************************************/
unsigned int ReadCalibrationResult(void)
{
	return (((REG_Calib_Conf_H&0x1F)<<4)|(REG_Calib_Conf_L&0x0F));
}

/**************************************************
FunctionName:CalibrationConf
Describe: У׼ֵ����
InputParameter: ����У׼ֵ
OutputParameter: ���ִ�н��
Notice:
    ��У׼ֵ�����˹��ˣ���Ϊ��5bitΪȫ0����ȫ1��Ч
**************************************************/
char CalibrationConf(unsigned int conf)
{
	/* ��bit11-bit15��ֵΪ0x00��0x1F����Ϊ��Ч */
	if(((conf&0x1F0)>0)&&((conf&0x1F0)<0x1F0))
	{
	   REG_Calib_Conf = (REG_Calib_Conf&(~(0x1FF<<7)))|((conf&0x1FF)<<7);
		 return 1;
	}
	else
	{
		return 0;
	}
}

/**************************************************
FunctionName: ExecuteCalibAndGetCalibValue
Describe: ִ��У׼������ȡУ׼ֵ
InputParameter:
OutputParameter: ����У׼ֵ
Notice:
**************************************************/
unsigned int ExecuteCalibAndGetCalibValue(void)
{
	/* Start Calibration */
	StartANA_Calib();
	/* Wait Done */
	while(0 == CalibrationDone());
	
	return ReadCalibrationResult();
}

/**************************************************
FunctionName:ReadCalibrationConf7
Describe:  ����ϵ��6���ϵ��Ԥȡ�ĵ���������ϵ������ϵ����
           ��Ӧ���������.
InputParameter:
OutputParameter:
Notice:
**************************************************/
unsigned int ReadCalibrationConf7(void)
{
	return REG_Calib_Coef7;
}





/***********************  END OF FILES  ***********************/
