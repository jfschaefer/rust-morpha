/*! \defgroup stemmer A Library for Stemming Words
    Basically, this library is a small wrapper around
    the (therefore slightly modified) morpha stemmer
    from the University of Sussex.
    @file
*/

#ifndef NORMALIZER_H_INCLUDED
#define NORMALIZER_H_INCLUDED
#include <stdio.h>

//used as an interface to morpha:
//(I've redirected the reading and writing from stdin/stdout to these streams)
FILE *morpha_instream;
FILE *morpha_outstream;
char *morpha_instream_buff_ptr;
char *morpha_outstream_buff_ptr;

/*! Initializes the stemmer */
void m_init();

/*! closes the stemmer */
void m_close();

/*! stems a sentence */
char * m_stem(const char *input);

/*! stems input until it doesn't change anymore */
char * m_full_stem(const char *input);

#endif
