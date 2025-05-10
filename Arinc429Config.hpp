/******************************************************************************
 *  @file       Arinc429Config.hpp
 *  @brief      Arinc 429 protocol configuation 
 *
 *  @details    This file contains Arinc 429 specific frame configutaion    
 *              to be sent over any other protocol. 
 * 
 *  @author     Vignesh Hegde
 *  @date       2025-05-09
 *  @version    1.0
 *
 *  @copyright  (c) 2025 Vignesh Hegde
 *              All rights reserved.
 *
 *  @note       
 *
 ******************************************************************************/
#ifndef ARINC429_CONFIG_H
#define ARINC429_CONFIG_H
#include <stdint.h>
#include <byteswap.h>

#define NULL_PTR 0

#define A429_SDI_00 AllSystem
#define A429_SDI_01 System01
#define A429_SDI_10 System02
#define A429_SDI_11 System03

#define A429_SSM_00 NoComputedData
#define A429_SSM_01 DataPositive
#define A429_SSM_10 FunctionalTest
#define A429_SSM_11 DataNegative

#define A429_DATA_BITS (524287u)

typedef int8_t    Int8;
typedef uint8_t   UInt8;
typedef int16_t   Int16;
typedef uint16_t  UInt16;
typedef int32_t   Int32;
typedef uint32_t  UInt32;
typedef int64_t   Int64;
typedef uint64_t  UInt64;   
typedef float     Float32;
typedef double    Float64;


// Be considerate using 
#define USE_A429_RESIZABLE_ARRAY (false)

#if USE_A429_RESIZABLE_ARRAY
#include <vector>
#else
#define USE_A429_ARRAY
#define A429_ARRAY_SIZE (10)
#endif

#endif