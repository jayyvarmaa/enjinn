#pragma once
#include <enjinnConfig.h>


namespace enjinn
{

	namespace assert
	{

		//arguments don't do anything here
		inline void terminate(...);

		void assertFunctionDevelopment(
			const char *expression, 
			const char *file,
			int line, 
			const char *comment = nullptr);

		void assertFunctionProduction
		(
			const char *expression,
			const char *file,
			int line,
			const char *comment = nullptr
		);

		void assertFunctionToLog(
			const char *expression,
			const char *file,
			int line,
			const char *comment = nullptr);

	}

}



#define ENJINN_PERMA_ASSERT(expression, comment) (void)(			\
			(!!(expression)) ||									\
			(ENJINN_INTERNAL_CURRENT_ASSERT_FUNCTION(#expression,	\
				__FILE__, __LINE__, comment), 0)				\
)


#ifdef ENJINN_DEVELOPMENT

#define ENJINN_DEVELOPMENT_ONLY_ASSERT(expression, comment) (void)(			\
			(!!(expression)) ||												\
			(enjinn::assert::assertFunctionDevelopment(#expression,			\
				__FILE__, __LINE__, comment), 0)							\
)

#elif defined(ENJINN_PRODUCTION)

#define ENJINN_DEVELOPMENT_ONLY_ASSERT(expression, comment)

#endif
