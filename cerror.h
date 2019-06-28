#ifndef CTIERROR_H
#define CTIERROR_H

#include <iostream> //<iostream.h>
#include <string.h>

class CError
{
    char* message;
    UINT  msgnumber;
public:
    CError();
    CError( char* m,  UINT=MB_ICONHAND );
    CError( double n, double m, char* msg, UINT=MB_ICONHAND );
    ~CError();
    void operator = (char*);
    char* GetMessage();
	friend std::ostream& operator << (std::ostream &s, CError &e);
};

#endif