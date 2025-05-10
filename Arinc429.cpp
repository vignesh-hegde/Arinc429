#include "Arinc429.hpp"
#include "Arinc429Config.hpp"
#include<stdio.h>

void Arinc429::ClearBuffer(void)
{
#if USE_A429_RESIZABLE_ARRAY
    mA429Frame.clear(); 
#else
    mA429FrameSize = 0;
#endif
    mpCurrentA429Frame = NULL_PTR;
}

void Arinc429::SetPosSsm() { mpCurrentA429Frame->mSsm = A429_SSM_01; }
void Arinc429::SetNegSsm() { mpCurrentA429Frame->mSsm = A429_SSM_11; }
void Arinc429::SetNcdSsm() { mpCurrentA429Frame->mSsm = A429_SSM_00; }
void Arinc429::SetFTSsm() { mpCurrentA429Frame->mSsm = A429_SSM_10; }

void Arinc429::SetSDI(UInt32 Sdi) { mpCurrentA429Frame->mSsm = Sdi; }


void Arinc429::SetSUSS16Data(void *pData, UInt8 *pSsmArray, UInt8 *pSdiArray, UInt8 *pLabelArray )
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

void Arinc429::SetSUSIF32Data(void *pData, UInt8 *pSsmArray, UInt8 *pSdiArray, UInt8 *pLabelArray )
{
#if __BYTE_ORDER == __BIG_ENDIAN
    ByteSwap32(pData);
#endif
    
    UInt16 *pDataWord1 = (UInt16 *)(pData);
    UInt16 *pDataWord2 = pDataWord1++;
    SetSUSS16Data(pDataWord1, pSsmArray, pSdiArray, pLabelArray);
    SetSUSS16Data(pDataWord2, ++pSsmArray, ++pSdiArray, ++pLabelArray);
    printf("1: %d %d %d %x\n", mpCurrentA429Frame->mLabel, mpCurrentA429Frame->mSdi, mpCurrentA429Frame->mSsm, mpCurrentA429Frame->mData);
}
void Arinc429::SetSUSIF64Data(void *pData, UInt8 *pSsmArray, UInt8 *pSdiArray, UInt8 *pLabelArray )
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

    printf("1: %d %d %d %d\n", mpCurrentA429Frame->mLabel, mpCurrentA429Frame->mSdi, mpCurrentA429Frame->mSsm, mpCurrentA429Frame->mData);
}

void Arinc429::StartTX()
{
    UInt32 FrameIndex = 0;
    UInt32 FrameEnd = 0;
    sA429Frame *pA429Iter = NULL_PTR;
    UInt32 DataBuff;

    
#if USE_A429_RESIZABLE_ARRAY
    FrameEnd = mA429Frame.size();
    pA429Iter;
#else
    FrameEnd = mA429FrameSize;
    pA429Iter;
#endif

    printf("2: %d %d %d %d\n", mpCurrentA429Frame->mLabel, mpCurrentA429Frame->mSdi, mpCurrentA429Frame->mSsm, mpCurrentA429Frame->mData);
    
    for(; FrameIndex < FrameEnd; ++FrameIndex )
    {
        pA429Iter = GetA429Frame(FrameIndex);

        printf("%d %d %d %0x\n", pA429Iter->mLabel, pA429Iter->mSdi, pA429Iter->mSsm, pA429Iter->mData);

        mPackedA429Frame.mLabel = pA429Iter->mLabel;
        mPackedA429Frame.mSdi = pA429Iter->mSdi;
        mPackedA429Frame.mSsm = pA429Iter->mSsm;
        memcpy((void *)&DataBuff, (void *)(pA429Iter->mData), sizeof(UInt8) * 3);
        mPackedA429Frame.mParity = 1;
        mPackedA429Frame.mData = 0;
        mPackedA429Frame.mData |= DataBuff;
        
        memcpy((void *)&DataBuff, (void *)(&mPackedA429Frame), sizeof(UInt32));
        
        printf("%d %d %d %d\n", mPackedA429Frame.mLabel, mPackedA429Frame.mSdi, mPackedA429Frame.mSsm, mPackedA429Frame.mData);
        printf("0x%08X\n", DataBuff);
        pA429Iter++;
    }
    
}

int main()
{
    Arinc429 A1;

    UInt64 data = 0x123456789ABCDEF1;
    UInt8 label[5] = {1, 2, 3, 4, 5};
    UInt8 sdi[5] = {00, 01, 10, 11};
    UInt8 ssm[5] = {11, 10, 01, 00};

    A1.ClearBuffer();
    A1.SetSUSIF64Data(&data, ssm, sdi, label);
    A1.StartTX();
    return 0;
}