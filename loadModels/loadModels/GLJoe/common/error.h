/**
 * @file GLJoe/common/error.h
 * 
 * @brief debug and error functions
 * 
 * @author Joachim Valente <joachim.valente@gmail.com>
 * @date 6/11/2013
 */

#ifndef DEF_GLJOE_ERROR_H
#define DEF_GLJOE_ERROR_H

namespace GLJoe
{	

// Error: print a message and exit
inline void Error(const char* format, ...)
{
	va_list argptr;
	va_start(argptr, format);
	fprintf(stderr, "[E] ");
	vfprintf(stderr, format, argptr);
	fprintf(stderr, "\n");
	va_end(argptr);
	
	exit(EXIT_FAILURE);
}

// Warning: print a message
inline void Warning(const char* format, ...)
{
	va_list argptr;
	va_start(argptr, format);
	fprintf(stderr, "[W] ");
	vfprintf(stderr, format, argptr);
	fprintf(stderr, "\n");
	va_end(argptr);
}

// Info: print an information message
inline void Info(const char* format, ...)
{
	va_list argptr;
	va_start(argptr, format);
	fprintf(stderr, "[I] ");
	vfprintf(stderr, format, argptr);
	fprintf(stderr, "\n");
	va_end(argptr);
}

// Debug: print a message for debug
inline void Debug(const char* format, ...)
{
	va_list argptr;
	va_start(argptr, format);
	fprintf(stderr, "[D] ");
	vfprintf(stderr, format, argptr);
	fprintf(stderr, "\n");
	va_end(argptr);
}
	
	
} // namespace GLJoe



#endif // DEF_GLJOE_ERROR_H

