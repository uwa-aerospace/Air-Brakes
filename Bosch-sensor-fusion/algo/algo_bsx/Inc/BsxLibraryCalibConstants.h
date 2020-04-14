#ifndef __BSXLIBRARYCALIBCONSTANTS_H__
#define __BSXLIBRARYCALIBCONSTANTS_H__
/*!
* @section LICENCE
* $license$
*
*
* (C) Copyright 2011 - 2015 Bosch Sensortec GmbH All Rights Reserved
*------------------------------------------------------------------------------
* Disclaimer
*
* Common: Bosch Sensortec products are developed for the consumer goods
* industry. They may only be used within the parameters of the respective valid
* product data sheet.  Bosch Sensortec products are provided with the express
* understanding that there is no warranty of fitness for a particular purpose.
* They are not fit for use in life-sustaining, safety or security sensitive
* systems or any system or device that may lead to bodily harm or property
* damage if the system or device malfunctions. In addition, Bosch Sensortec
* products are not fit for use in products which interact with motor vehicle
* systems.	The resale and/or use of products are at the purchaser's own risk
* and his own responsibility. The examination of fitness for the intended use
* is the sole responsibility of the Purchaser.
*
* The purchaser shall indemnify Bosch Sensortec from all third party claims,
* including any claims for incidental, or consequential damages, arising from
* any product use not covered by the parameters of the respective valid product
* data sheet or not approved by Bosch Sensortec and reimburse Bosch Sensortec
* for all costs in connection with such claims.
*
* The purchaser must monitor the market for the purchased products,
* particularly with regard to product safety and inform Bosch Sensortec without
* delay of all security relevant incidents.
*
* Engineering Samples are marked with an asterisk (*) or (e). Samples may vary
* from the valid technical specifications of the product series. They are
* therefore not intended or fit for resale to third parties or for use in end
* products. Their sole purpose is internal client testing. The testing of an
* engineering sample may in no way replace the testing of a product series.
* Bosch Sensortec assumes no liability for the use of engineering samples. By
* accepting the engineering samples, the Purchaser agrees to indemnify Bosch
* Sensortec from all claims arising from the use of engineering samples.
*
* Special: This software module (hereinafter called "Software") and any
* information on application-sheets (hereinafter called "Information") is
* provided free of charge for the sole purpose to support your application
* work. The Software and Information is subject to the following terms and
* conditions:
*
* The Software is specifically designed for the exclusive use for Bosch
* Sensortec products by personnel who have special experience and training. Do
* not use this Software if you do not have the proper experience or training.
*
* This Software package is provided `` as is `` and without any expressed or
* implied warranties, including without limitation, the implied warranties of
* merchantability and fitness for a particular purpose.
*
* Bosch Sensortec and their representatives and agents deny any liability for
* the functional impairment of this Software in terms of fitness, performance
* and safety. Bosch Sensortec and their representatives and agents shall not be
* liable for any direct or indirect damages or injury, except as otherwise
* stipulated in mandatory applicable law.
*
* The Information provided is believed to be accurate and reliable. Bosch
* Sensortec assumes no responsibility for the consequences of use of such
* Information nor for any infringement of patents or other rights of third
* parties which may result from its use.
*

* @file      bsxlibrarycalibconstants.h
* @date      2013/02/12 created
*
* @brief
* This file provides constants definition for calibration modules
*
*
*/

/************************************************************************************************************/
/*												INCLUDES													*/
/************************************************************************************************************/

#include "BsxLibraryDataTypes.h"

/************************************************************************************************************/
/*											CONSTANT DEFINITIONS										 	*/
/************************************************************************************************************/

/**\name Calibration modes */

#define BSX_CALIB_SLEEP     			(0U)		/**< sleep mode for calibration */
#define BSX_CALIB_MONITORING  			(1U)		/**< Calibration monitoring is active*/
#define BSX_CALIB_MODEOFFSETACTIVE		(2U)		/**< Calibration offsets estimation is active */

/**\name Data correction modes */

#define BSX_DATACORRECTION_SLEEP		(0U)       /**< 0 => No Data correction */
#define BSX_DATACORRECTION_OFFSET		(1U)		/**<  1 = Only offset correction is included */
#define BSX_DATACORRECTION_SENSDOFFSET	(2U)       /**<   2 = offset + diagonal sensitivity correction are included*/
#define BSX_DATACORRECTION_SENSFOFFSET	(3U)       /**<  3 = offset + full sensitivity correction are used for data correction */

/**\name Calibration Source types */

#define BSX_CALIBSOURCE_NONE     		(0U)		/**< no Calibration source*/
#define BSX_CALIBSOURCE_CLASSIC  		(1U)		/**< Calibration source is classical*/
#define BSX_CALIBSOURCE_FAST			(2U) 		/**< Calibration source is FMC*/

/**\name Gyro calibration modes */

#define BSX_GYROCALIB_SLEEP     		(0U)      /**<  0= Gyroscope Calibration is not Active */
#define BSX_GYROCALIB_GYRODATA			(1U)      /**<   1= Gyroscope Calibration based on (Gyroscope) data only */
#define BSX_GYROCALIB_GYROACC  			(2U)      /**<   2= Gyroscope Calibration based on  (Gyroscope+Accelerometer) Data */
#define BSX_GYROCALIB_GYROACCMAG		(3U)      /**<   3= Gyroscope Calibration based on (Gyroscope+Accelerometer+Magnetometer) Data */

#endif






































