// STM32G0C1KET6N

#include "cortexm0plus/barriers.hpp"

int main()
{
    CortexM0Plus::DataSyncBarrier();

    while (true);
    return 0;
}
