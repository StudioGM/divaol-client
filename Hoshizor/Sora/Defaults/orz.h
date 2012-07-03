#ifdef WIN32

#include "windows.h"

#ifdef     IN
#undef     IN
#endif
#define    IN

#ifdef     OUT
#undef     OUT
#endif
#define    OUT

#ifdef     OPTIONAL
#undef     OPTIONAL
#endif
#define    OPTIONAL


#define    VT_READ_PTR     1
#define    VT_WRITE_PTR    1 << 1


typedef    __int64    INT64, *PINT64, *LPINT64;


int
DelayExecutionAndWaitForTimeElapse (
    IN PINT64 pDesiredFps,
    IN OUT PINT64 pTimeStampOldValue
);

INT64
GetCurrentSystemTime (
    void
);

INT64
GetCurrentSystemTimeN (
    void
);

void
DelayExecution (
    IN unsigned int Milliseconds
);

void
DelayThreadExecutionN (
    IN unsigned int Milliseconds
);

void
RaisePriority (
    void
);

INT64
GetSystemTimePrecision (
    void
);

INT64
GetSystemTimePrecisionN (
    void
);

void
RaiseThreadPriorityN (
    void
);

void
LowerPriority (
    void
);

void
LowerThreadPriorityN (
    void
);

int
VerifyPointer (
    IN int VerificationType,
    IN void *pDataPointer,
    IN unsigned int SizeOfData
);

int
VerifyReadPtr (
    IN void *pDataPointer,
    IN unsigned int SizeOfData
);

int
VerifyWritePtr (
    IN void *pDataPointer,
    IN unsigned int SizeOfData
);

int
VerifyReadablePointerN (
    IN void *pDataPointer,
    IN unsigned int SizeOfData
);

int
VerifyWritablePointerN (
    IN void *pDataPointer,
    IN unsigned int SizeOfData
);

#endif // OS_WIN32