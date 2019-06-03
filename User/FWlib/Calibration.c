#include "calibration.h"
#include "BK5863.h"

/**************************************************
FunctionName:ReadCalibCoef6
Describe: 配置系数6：上电后预取的第六个配置系数，该系数的
          的应用软件定义，可用作Calib_Conf使用
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
Describe: 启动 Calibration 过程
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
Describe: Calibration是否完成
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
Describe: 读取Calibration结果
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
Describe: 校准值配置
InputParameter: 输入校准值
OutputParameter: 输出执行结果
Notice:
    对校准值进行了过滤，认为高5bit为全0或者全1无效
**************************************************/
char CalibrationConf(unsigned int conf)
{
	/* 若bit11-bit15的值为0x00或0x1F则认为无效 */
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
Describe: 执行校准，并获取校准值
InputParameter:
OutputParameter: 返回校准值
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
Describe:  配置系数6：上电后预取的第六个配置系数，该系数的
           的应用软件定义.
InputParameter:
OutputParameter:
Notice:
**************************************************/
unsigned int ReadCalibrationConf7(void)
{
	return REG_Calib_Coef7;
}





/***********************  END OF FILES  ***********************/
