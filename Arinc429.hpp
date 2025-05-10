#include "Arinc429Config.hpp"
#include <memory>
#include <cstring>

class cArinc429{
private:

    typedef struct __attribute__((__packed__))
    {
#if __BYTE_ORDER == __LITTLE_ENDIAN
        UInt32 mLabel  : 8;
        UInt32 mSdi    : 2; 
        UInt32 mData   : 19;
        UInt32 mSsm    : 2;
        UInt32 mParity : 1;
#else
        UInt32 mParity : 1;
        UInt32 mData   : 19;
        UInt32 mSsm    : 2;
        UInt32 mSdi    : 2; 
        UInt32 mLabel  : 8;
#endif
    } sPackedA429Frame;
    

    typedef struct 
    {
        UInt32 mLabel;
        UInt32 mSdi; 
        UInt8 mData[3];
        UInt32 mSsm;
    } sA429Frame;

#if USE_A429_RESIZABLE_ARRAY
    std::vector<std::unique_ptr<sA429Frame>> mA429Frame;
#else
    UInt32 mMaxAllowedBufferSize;
    sA429Frame mA429Frame[A429_ARRAY_SIZE];
    UInt32 mA429FrameSize;
#endif
    sA429Frame *mpCurrentA429Frame;
    sPackedA429Frame mPackedA429Frame;
    UInt32 GetParityBit(UInt32);

#if __BYTE_ORDER == __LITTLE_ENDIAN  
    inline void ByteSwap32(void *pData)
    {
        UInt32 DataBuff;
        memcpy((void *)(&DataBuff), pData, sizeof(UInt32));
        DataBuff = __bswap_32(DataBuff);
        memcpy(pData, (void *)(&DataBuff), sizeof(UInt32));
    }
    inline void ByteSwap64(void *pData)
    {
        UInt64 DataBuff;
        memcpy((void *)(&DataBuff), pData, sizeof(UInt64));
        DataBuff = __bswap_64(DataBuff);
        memcpy(pData, (void *)(&DataBuff), sizeof(UInt64));
    }
    inline void ByteSwap16(void *pData)
    {
        *(UInt16 *)pData = __bswap_16(*(UInt16 *)pData);
    }
#endif
    void SetPosSsm(void);
    void SetNegSsm(void);
    void SetNcdSsm(void);
    void SetFTSsm(void);
    void SetSDI(UInt32 Sdi);
    void SetLabel(uint8_t Label);

    public:
    typedef enum
    {
        A429_SDI_00 = 0b00,
        A429_SDI_01 = 0b01,
        A429_SDI_10 = 0b10,
        A429_SDI_11 = 0b11
    } eSdi;
    
    typedef enum
    {
        A429_SSM_00 = 0b00,
        A429_SSM_01 = 0b01,
        A429_SSM_10 = 0b10,
        A429_SSM_11 = 0b11
    } eSsm;


    cArinc429() : mpCurrentA429Frame(NULL_PTR)
#if USE_A429_RESIZABLE_ARRAY
#else
    , mMaxAllowedBufferSize(A429_ARRAY_SIZE), mA429FrameSize(0)
#endif
    {};
    void GetNewFrame()
    {
#if USE_A429_RESIZABLE_ARRAY
        std::unique_ptr<sA429Frame> pA429Frame = std::make_unique<sA429Frame>(); 
        mA429Frame.push_back(std::move(pA429Frame));
        mpCurrentA429Frame = mA429Frame.back().get();
#else
        mpCurrentA429Frame = &mA429Frame[mA429FrameSize];
        ++mA429FrameSize;
#endif        
    }


    void ClearBuffer(void);
    
    inline sA429Frame *GetA429Frame(UInt32 FrameIndex, bool LastFrame = false)
    {
        sA429Frame *pReturnFrame = NULL_PTR;

        if(LastFrame)
        {
            pReturnFrame = mpCurrentA429Frame;
        }
        else
        {
#if USE_A429_RESIZABLE_ARRAY
            pReturnFrame = mA429Frame[FrameIndex].get();
#else
            pReturnFrame = (FrameIndex < mA429FrameSize) ? &mA429Frame[FrameIndex] : NULL_PTR;
#endif
        }

        return pReturnFrame;
    }
    void SetSUSS16Data(void *Data, UInt8 *pSsmArray, UInt8 *pSdiArray, UInt8 *pLabelArray );
    void SetSUSIF32Data(void *Data, UInt8 *pSsmArray, UInt8 *pSdiArray, UInt8 *pLabelArray );
    void SetSUSIF64Data(void *Data, UInt8 *pSsmArray, UInt8 *pSdiArray, UInt8 *pLabelArray );
    
    // More versetile, pass array of SSM, SDI, Label with data, such as array size is equal to ByteSize
    void SetByteStreamData(void *Data, UInt32 ByteSize, UInt8 *pSsmArray, UInt8 *pSdiArray, UInt8 *pLabelArray);

    void StartTX();
    
};

