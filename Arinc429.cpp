#include "Arinc429.hpp"
#include "Arinc429Config.hpp"
#include<stdio.h>

void cArinc429::ClearBuffer(void)
{
#if USE_A429_RESIZABLE_ARRAY
    mA429Frame.clear(); 
#else
    mA429FrameSize = 0;
#endif
    mpCurrentA429Frame = NULL_PTR;
}

void cArinc429::SetPosSsm() { mpCurrentA429Frame->mSsm = A429_SSM_01; }
void cArinc429::SetNegSsm() { mpCurrentA429Frame->mSsm = A429_SSM_11; }
void cArinc429::SetNcdSsm() { mpCurrentA429Frame->mSsm = A429_SSM_00; }
void cArinc429::SetFTSsm() { mpCurrentA429Frame->mSsm = A429_SSM_10; }
void cArinc429::SetSDI(UInt32 Sdi) { mpCurrentA429Frame->mSsm = Sdi; }

void cArinc429::SetSUSS16Data(void *pData, UInt8 *pSsmArray, UInt8 *pSdiArray, UInt8 *pLabelArray )
{
#if __BYTE_ORDER == __BIG_ENDIAN
    ByteSwap16(pData);
#endif
    GetNewFrame();
    memcpy((void *)mpCurrentA429Frame->mData, pData, sizeof(UInt16)); 
    mpCurrentA429Frame->mLabel = pLabelArray[0];
    mpCurrentA429Frame->mSdi = pSdiArray[0];
    mpCurrentA429Frame->mSsm = pSsmArray[0];
}

void cArinc429::SetSUSIF32Data(void *pData, UInt8 *pSsmArray, UInt8 *pSdiArray, UInt8 *pLabelArray )
{
#if __BYTE_ORDER == __BIG_ENDIAN
    ByteSwap32(pData);
#endif
    
    UInt16 *pDataWord1 = (UInt16 *)(pData);
    UInt16 *pDataWord2 = pDataWord1++;
    SetSUSS16Data(pDataWord1, pSsmArray, pSdiArray, pLabelArray);
    SetSUSS16Data(pDataWord2, ++pSsmArray, ++pSdiArray, ++pLabelArray);
}
void cArinc429::SetSUSIF64Data(void *pData, UInt8 *pSsmArray, UInt8 *pSdiArray, UInt8 *pLabelArray )
{
    #if __BYTE_ORDER == __BIG_ENDIAN
    ByteSwap32(pData);
#endif
    UInt16 *pDataWord1 = (UInt16 *)(pData);
    UInt16 *pDataWord4 = pDataWord1++;
    UInt16 *pDataWord3 = pDataWord1++;
    UInt16 *pDataWord2 = pDataWord1++;

    SetSUSS16Data(pDataWord1, pSsmArray, pSdiArray, pLabelArray);
    SetSUSS16Data(pDataWord2, ++pSsmArray, ++pSdiArray, ++pLabelArray);
    SetSUSS16Data(pDataWord3, ++pSsmArray, ++pSdiArray, ++pLabelArray);
    SetSUSS16Data(pDataWord4, ++pSsmArray, ++pSdiArray, ++pLabelArray);
}

void cArinc429::StartTX()
{
    UInt32 FrameIndex = 0;
    sA429Frame *pA429Iter = NULL_PTR;
    UInt32 DataBuff;

    printf("------OUTPUT STARTED------\n");
    
    for(pA429Iter = GetA429Frame(0), FrameIndex = 0; pA429Iter != NULL_PTR; pA429Iter = GetA429Frame(++FrameIndex))
    {
        mPackedA429Frame.mLabel = pA429Iter->mLabel;
        mPackedA429Frame.mSdi = pA429Iter->mSdi;
        mPackedA429Frame.mSsm = pA429Iter->mSsm;
        memcpy((void *)&DataBuff, (void *)(pA429Iter->mData), sizeof(UInt8) * 3);
        mPackedA429Frame.mParity = 1;
        mPackedA429Frame.mData = 0;
        mPackedA429Frame.mData |= DataBuff;
        
        memcpy((void *)&DataBuff, (void *)(&mPackedA429Frame), sizeof(UInt32));
        
        printf("A429 Word [%d]\n", FrameIndex); 
        printf("    LABEL : 0x%X\n", mPackedA429Frame.mLabel); 
        printf("    SSM   : 0x%X\n", mPackedA429Frame.mSsm); 
        printf("    SDI   : 0x%X\n", mPackedA429Frame.mSdi); 
        printf("    DATA  : 0x%X\n", mPackedA429Frame.mData); 
    }
    
}

