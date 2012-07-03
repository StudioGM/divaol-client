#include "orz.h"

#ifdef WIN32

#include "stdio.h"

int
DelayExecutionAndWaitForTimeElapse (
    IN PINT64 pDesiredFps,
    IN OUT PINT64 pTimeStampOldValue
)
{
    int bPriorityRaised = 0;
    INT64 TimeInterval, TimePrecision;
    
    if (VerifyPointer(VT_READ_PTR, pDesiredFps, sizeof(INT64)))
        return -1;

    if (VerifyPointer(VT_READ_PTR | VT_WRITE_PTR, pTimeStampOldValue, sizeof(INT64)))
        return -1;
        
    if (*pDesiredFps <= 0)
        return -1;

    TimePrecision = GetSystemTimePrecision();
   
    if (TimePrecision == 0)
        return -2;
        
    TimeInterval = TimePrecision / *pDesiredFps;
    
    if (GetCurrentSystemTime() - *pTimeStampOldValue >= TimeInterval)
    {
        LowerPriority();
    }
    else
    {        
        for (;;)
        {
            if (GetCurrentSystemTime() - *pTimeStampOldValue >= (TimeInterval - (TimePrecision / 1000 * 2)))
            {
                if (!bPriorityRaised)
                    LowerPriority();
                
                for (;;)
                {
                    if (GetCurrentSystemTime() - *pTimeStampOldValue >= TimeInterval)
                        break;
                }
            
                break;
            }
            else
            {
                if (!bPriorityRaised)
                {
                    RaisePriority();
                    
                    bPriorityRaised = 1;
                }
                
                DelayExecution(1);
            }
        }
    }

    TimePrecision = GetCurrentSystemTime();
    
    if (*pTimeStampOldValue <= (TimePrecision - TimeInterval) && *pTimeStampOldValue >= (TimePrecision - (TimeInterval * 2)))
        *pTimeStampOldValue += TimeInterval;
    else
        *pTimeStampOldValue = TimePrecision;
    
    return 0;
}

INT64
GetCurrentSystemTime (
    void
)
{
    return GetCurrentSystemTimeN();
}

INT64
GetCurrentSystemTimeN (
    void
)
{
    LARGE_INTEGER Counter;
    
    if (!QueryPerformanceCounter(&Counter))
        return 0;
        
    return (INT64)Counter.QuadPart;
}

void
DelayExecution (
    IN unsigned int Milliseconds
)
{
    DelayThreadExecutionN(Milliseconds);
}
    
void
DelayThreadExecutionN (
    IN unsigned int Milliseconds
)
{
    Sleep(Milliseconds);
}

void
RaisePriority (
    void
)
{
    RaiseThreadPriorityN();
}

INT64
GetSystemTimePrecision (
    void
)
{
    return GetSystemTimePrecisionN();
}

INT64
GetSystemTimePrecisionN (
    void
)
{
    LARGE_INTEGER Frequency;
    
    if (!QueryPerformanceFrequency(&Frequency))
        return 0;
        
    return (INT64)Frequency.QuadPart;
}

void
RaiseThreadPriorityN (
    void
)
{
    int CurrentPriority = GetThreadPriority(GetCurrentThread());

    if (CurrentPriority < THREAD_PRIORITY_HIGHEST)
        SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
}

void
LowerPriority (
    void
)
{
    LowerThreadPriorityN();
}

void
LowerThreadPriorityN (
    void
)
{
    int CurrentPriority = GetThreadPriority(GetCurrentThread());

    if (CurrentPriority > THREAD_PRIORITY_NORMAL)
        SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
}

int
VerifyPointer (
    IN int VerificationType,
    IN void *pDataPointer,
    IN unsigned int SizeOfData
)
{
    int CallResult = 0;

    if ((VerificationType & VT_READ_PTR) != 0)
        CallResult |= VerifyReadPtr(pDataPointer, SizeOfData);

    if ((VerificationType & VT_WRITE_PTR) != 0)
        CallResult |= VerifyWritePtr(pDataPointer, SizeOfData);

    return CallResult;
}


int
VerifyReadPtr (
    IN void *pDataPointer,
    IN unsigned int SizeOfData
)
{
    return VerifyReadablePointerN(pDataPointer, SizeOfData);
}

int
VerifyWritePtr (
    IN void *pDataPointer,
    IN unsigned int SizeOfData
)
{
    return VerifyWritablePointerN(pDataPointer, SizeOfData);
}

int
VerifyReadablePointerN (
    IN void *pDataPointer,
    IN unsigned int SizeOfData
)
{
    if (IsBadReadPtr(pDataPointer, SizeOfData))
        return -1;

    return 0;
}

int
VerifyWritablePointerN (
    IN void *pDataPointer,
    IN unsigned int SizeOfData
)
{
    if (IsBadWritePtr(pDataPointer, SizeOfData))
        return -1;

    return 0;
}

#endif