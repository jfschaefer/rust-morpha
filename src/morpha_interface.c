#define interactive   /*required for morpha to work properly*/
#include <ctype.h>
#include "morpha_interface.h"
#include "../morpha/morpha.yy.c"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int STEMMER_IS_INITIALIZED = 0;

void m_init() {
  if (STEMMER_IS_INITIALIZED) return;
  STEMMER_IS_INITIALIZED = 1;
  /* Initialize options */
  UnSetOption(print_affixes);
  SetOption(change_case);
  UnSetOption(tag_output);
  UnSetOption(fspec);

  state = any;

  read_verbstem("third-party/morpha/verbstem.list");
}


//based on example from http://stackoverflow.com/questions/779875/what-is-the-function-to-replace-string-in-c
char *str_replace(char *orig, char *rep, char *with) {
  char *result; // the return string
  char *ins;    // the next insert point
  char *tmp;    // varies
  int len_rep;  // length of rep
  int len_with; // length of with
  int len_front; // distance between rep and end of last rep
  int count;    // number of replacements

  if (!orig)
    return NULL;
  if (!rep)
    rep = "";
  len_rep = strlen(rep);
  if (!with)
    with = "";
  len_with = strlen(with);

  ins = orig;
  for (count = 0; tmp = strstr(ins, rep); ++count) {
    ins = tmp + len_rep;
  }

  // first time through the loop, all the variable are set correctly
  // from here on,
  //    tmp points to the end of the result string
  //    ins points to the next occurrence of rep in orig
  //    orig points to the remainder of orig after "end of rep"
  tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

  if (!result)
    return NULL;

  while (count--) {
    ins = strstr(orig, rep);
    len_front = ins - orig;
    tmp = strncpy(tmp, orig, len_front) + len_front;
    tmp = strcpy(tmp, with) + len_with;
    orig += len_front + len_rep; // move to next "end of rep"
  }
  strcpy(tmp, orig);
  //free(orig);
  return result;
}


char *word_replace(char *orig, char *rep, char *with) {
  size_t size;
  char *outstring;
  FILE *stream = open_memstream(&outstring, &size);
  char *tmp = orig;

  //now process word by word
  int found_end = 0;
  while (!found_end) {
    while (*tmp != ' ' && *tmp != '\0') {
      tmp++;
    }
    if (*tmp == '\0') found_end = 1;
    *tmp = '\0';
    if (strcmp(orig, rep)) {  //if word doesn't have to be replaced
      fprintf(stream, "%s", orig);
    } else {  //word has to be replaced
      fprintf(stream, "%s", with);
    }
    if (!found_end) *tmp = ' ';
    putc(*tmp, stream);
    orig = ++tmp;   //next word
  }
  fclose(stream);
  return outstring;
}

char * m_stem(const char *sentence) {
  char *x0;
  char **stemmed = &x0;
  if (!STEMMER_IS_INITIALIZED) m_init();
  char *tmp;
  size_t insize, outsize;   //we're not interested in the size
  morpha_instream = open_memstream(&morpha_instream_buff_ptr, &insize);
  morpha_outstream = open_memstream(&morpha_outstream_buff_ptr, &outsize);

  yyout = morpha_outstream;
  yyin = morpha_instream;

  sentence = word_replace(sentence, "'s", "'S");   //otherwise the s gets removed

  fprintf(morpha_instream, "%s", sentence);

  free(sentence);

  int c;
  while ((c = getc(morpha_instream)) != EOF) {
    ungetc(c, morpha_instream);
    BEGIN(state);
    yylex();
  }
    
  BEGIN(state);
  yylex();

  fclose(morpha_outstream);
  //*stemmed = morpha_outstream_buff_ptr;
  *stemmed = word_replace(morpha_outstream_buff_ptr, "formulum", "formula");

  free(morpha_outstream_buff_ptr);
  tmp = word_replace(*stemmed, "vium", "via");
  free(*stemmed);
  *stemmed = tmp;

  //size_t outsize;
  //morpha_outstream = open_memstream(&morpha_outstream_buff_ptr, &outsize);
  fclose(morpha_instream);
  free(morpha_instream_buff_ptr);
  return *stemmed;
}

char * m_full_stem(const char *input) {
  char *x0;
  char **stemmed = &x0;
  *stemmed = m_stem(input);
  if (strcmp(input, *stemmed)) {
    input = *stemmed;
    *stemmed = m_stem(input);
    while (strcmp(input, *stemmed)) {
      free(input);
      input = *stemmed;
      *stemmed = m_stem(input);
    }
    free(input);
  }
  return *stemmed;
}

void m_close() {
  //fclose(morpha_instream);
  //free(morpha_instream_buff_ptr);
  //fclose(morpha_outstream);
  //free(morpha_outstream_buff_ptr);
  yy_delete_buffer(yy_current_buffer);
  int i;
  for (i=0; i<verbstem_n; i++) {
    free(verbstem_list[i]);
  }
  free(verbstem_list);
  STEMMER_IS_INITIALIZED = 0;
}
