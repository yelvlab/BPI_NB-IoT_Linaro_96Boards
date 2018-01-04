#ifndef _BC95_USER_H
#define _BC95_USER_H

#include "stm32f10x.h"


#define REQUEST_MANUFACTURER_IDENTIFICATION     "AT+CGMI\r\n"       //请求制造商标识
#define REQUEST_MANUFACTURER_MODLE              "AT+CGMM\r\n"       //请求制造商型号
#define REQUEST_MANUFACTURER_REVISION           "AT+CGMR\r\n"       //请求制造商修订
#define REQUEST_PRODUCT_SERIAL_NUMBER           "AT+CGSN"           //请求产品序列号 Ps:"AT+CGSN=1"返回IMEI "AT+CGSN=2"返回IMEISV "AT+CGSN=3"返回SVN软件版本号



#endif // !_BC95_USER_H
