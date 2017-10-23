#pragma once
#include <windows.h>
#include <iostream>
#include <exception>

class CSEHException:public std::exception
{
	private:
		unsigned int m_exceptionCode;
		EXCEPTION_POINTERS* m_exceptionPointers;
		std::string m_what;
	public:
		CSEHException(
			unsigned int exceptionCode,
			EXCEPTION_POINTERS* exceptionPointers,
			std::string& what)
			:std::exception(what.c_str()),
			m_exceptionCode(exceptionCode ),
			m_exceptionPointers(exceptionPointers),
			m_what(what){}

		virtual ~CSEHException(){}
		//virtual const char* what()const override;
	static void TransferSEHtoCEH ( unsigned int exceptionCode, PEXCEPTION_POINTERS exceptionPointers );
};

