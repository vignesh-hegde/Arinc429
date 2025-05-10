#include "Arinc429.hpp"

int main()
{
    cArinc429 *pA429 = new cArinc429();

    UInt64 data = 0x123456789ABCDEF0;
    UInt8 label[5] = {1, 2, 3, 4, 5};
    UInt8 sdi[5] = {00, 01, 10, 11};
    UInt8 ssm[5] = {11, 10, 01, 00};

    pA429->ClearBuffer();
    pA429->SetSUSIF64Data(&data, ssm, sdi, label);
    pA429->StartTX();
    return 0;
}