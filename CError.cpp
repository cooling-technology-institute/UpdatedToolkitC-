#include "stdafx.h"
#include <afxwin.h>
#include "CError.h"

CError::CError()
{
    message = NULL;
    msgnumber = MB_ICONHAND;
}

CError::CError(char* msg, UINT mn)
{
    if (msg)
    {
        message = new char [strlen(msg)+1];
        strcpy(message,msg);
        msgnumber = mn;
    }
    else
    {
        message = NULL;
        msgnumber = MB_ICONHAND;
    }
}

CError::CError(double number,double master,char* msg,UINT nm)
{
    char n[40], m[40];
    sprintf(n,"%10.4f",number);
    sprintf(m,"%10.4f",master);
    message = new char[80];
    strcpy(message,msg);
    msgnumber = nm;
}

CError::~CError()
{
    if (message)
    {
        delete [] message;
    }
    message = NULL;
}

void CError::operator = (char* msg)
{
    if (msg)
    {
        message = new char [strlen(msg)+1];
        strcpy(message,msg);
    }
    else
        message = NULL;
}

char* CError::GetMessage()
{
    return message;
}

std::ostream& operator << (std::ostream& os, CError& ce)
{
    AfxMessageBox(ce.message,ce.msgnumber);
    return os;
}