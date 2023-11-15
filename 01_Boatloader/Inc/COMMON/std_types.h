/*************************************************************************/
// Author        : Sherif Ashraf Khadr									 
// Project       : STM32F401xC_Drivers
// File          : main.c
// Date          : Sep 8, 2023
// GitHub        : https://github.com/sherifkhadr             		     
/*************************************************************************/

#ifndef COMMON_STD_TYPES_H_
#define COMMON_STD_TYPES_H_

typedef unsigned char                        u8     ;
typedef unsigned short int                   u16    ;
typedef unsigned long  int                   u32    ;

typedef unsigned char                        uint8_t     ;
typedef unsigned short int                   uint16_t    ;
typedef unsigned long  int                   uint32_t    ;
typedef signed   char                        sint8_t     ;
typedef signed   short int                   sint16_t    ;
typedef signed   long  int                   sint32_t    ;
typedef float                                float32_t   ;
typedef double                               float64_t   ;
typedef long double                          float96_t   ;


typedef volatile unsigned char               vuint8_t    ;
typedef volatile unsigned short int          vuint16_t   ;
typedef volatile unsigned long  int          vuint32_t   ;
typedef volatile signed   char               vsint8_t    ;
typedef volatile signed   short int          vsint16_t   ;
typedef volatile signed   long  int          vsint32_t   ;
typedef volatile float                       vfloat32_t  ;
typedef volatile double                      vfloat64_t  ;
typedef volatile long double                 vfloat96_t  ;


#ifndef TRUE
#define TRUE   1
#endif


#ifndef FALSE
#define FALSE 	0
#endif


#ifndef	STR_NULL
#define STR_NULL	'\0'
#endif

#ifndef	PTR_NULL
#define PTR_NULL	(void*)0
#endif

#ifndef	NULL
#define NULL		(void*)0
#endif

#endif /* COMMON_STD_TYPES_H_ */
