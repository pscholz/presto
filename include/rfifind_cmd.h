#ifndef __rfifind_cmd__
#define __rfifind_cmd__
/*****
  command line parser interface -- generated by clig 
  (http://wsd.iitb.fhg.de/~kir/clighome/)

  The command line parser `clig':
  (C) 1995,1997,1998,1999,2000 Harald Kirsch (kir@iitb.fhg.de)
*****/

typedef struct s_Cmdline {
  /***** -o: Root of the output file names */
  char outfileP;
  char* outfile;
  int outfileC;
  /***** -pkmb: Raw data in Parkes Multibeam format */
  char pkmbP;
  /***** -ebpp: Raw data in EBPP (Effelsberg) format */
  char ebppP;
  /***** -gbpp: Raw data in GBPP (Green Bank) format */
  char gbppP;
  /***** -xwin: Draw plots to the screen as well as a PS file */
  char xwinP;
  /***** -time: Minutes to integrate for stats and FFT calcs */
  char timeP;
  float time;
  int timeC;
  /***** -sigma: The +/-sigma cutoff to reject a chunk of data */
  char sigmaP;
  float sigma;
  int sigmaC;
  /***** -zapchan: Channels to explicitly remove from analysis */
  char zapchanP;
  int *zapchan;
  int zapchanC;
  /***** -zapints: Intervals to explicitly remove from analysis */
  char zapintsP;
  int *zapints;
  int zapintsC;
  /***** -mask: File containing masking information to use */
  char maskfileP;
  char* maskfile;
  int maskfileC;
  /***** uninterpreted command line parameters */
  int argc;
  /*@null*/char **argv;
  /***** the whole command line concatenated */
  char *full_cmd_line;
} Cmdline;


extern char *Program;
extern void usage(void);
extern /*@shared*/Cmdline *parseCmdline(int argc, char **argv);

extern void showOptionValues(void);

#endif

