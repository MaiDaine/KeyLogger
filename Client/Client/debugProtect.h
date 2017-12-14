#ifndef DEBUGPROTECT_H_
# define DEBUGPROTECT_H_

#include <windows.h>

class debugProtect
{
public:
	debugProtect();
	~debugProtect();
	LONG UnhandledExcepFilter(PEXCEPTION_POINTERS pExcepPointers)const;
	void ProtectMe(void)const;
	bool HideThread(HANDLE hThread)const;
private:
	void DebugSelf()const;
};

template<debugProtect &dbg>
LONG WINAPI UnhandledExcepFilter(PEXCEPTION_POINTERS pExcepPointers)
{
	return (dbg.UnhandledExcepFilter(pExcepPointers));
}

#endif /* !DEBUGPROTECT_H_ */
