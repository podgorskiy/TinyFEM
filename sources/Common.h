/*! \file common.h
	\brief Contains common defines and includes
	It contain some usefull logging and assert defines. */

#pragma once

#pragma warning(error:  4239)
#pragma warning(error:  4172)

#ifndef NULL
#define NULL 0
#endif

#define LOG__(...) (void)printf(__VA_ARGS__);
#define LOG_(...) (void)printf(__VA_ARGS__);printf("\n");
/// Macro for logging warnings
#define LOGW(...) printf("W: ");(void)printf(__VA_ARGS__);printf("\n");
/// Macro for profiling
#define LOGP(...) printf("P: ");(void)printf(__VA_ARGS__);printf("\n");
/// Macro for logging errors
/** This macro also prints the file and line of code that prints this message.*/
#define LOGE(...) printf("E: ");(void)printf(__VA_ARGS__);printf("\n");printf("file:%s\nline:%d\n", __FILE__, __LINE__);
/// Macro for logging info
#define LOGI(...) printf("I: ");(void)printf(__VA_ARGS__);printf("\n");

/// Template for safe deleteion of objects
template<typename T>
inline void SafeDelete(T*& p)
{
	if (p != NULL)
	{
		delete p;
		p = NULL;
	}
}

/// Template for safe deleteion of arrays of objects
template<typename T>
inline void SafeDeleteArray(T*& p)
{
	if (p != NULL)
	{
		delete[] p;
		p = NULL;
	}
}
