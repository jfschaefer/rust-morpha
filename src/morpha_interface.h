/*! \defgroup stemmer A Library for Stemming Words
    Basically, this library is a small wrapper around
    the (therefore slightly modified) morpha stemmer
    from the University of Sussex.
    @file
*/

#ifndef MORPHA_H_INCLUDED_
#define MORPHA_H_INCLUDED_
#include <stdio.h>

/*! Initializes the stemmer */
void m_init();

/*! closes the stemmer */
void m_close();

/*! stems a sentence */
char * m_stem(const char *input);

/*! stems input until it doesn't change anymore */
char * m_full_stem(const char *input);

#endif
