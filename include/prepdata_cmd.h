#ifndef __prepdata_cmd__
#define __prepdata_cmd__
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
  /***** -ebpp: Raw data in Efflesberg-Berkeley Pulsar Processor format */
  char ebppP;
  /***** -pad0: Pad the data to the next power-of-2 with zeros */
  char pad0P;
  /***** -padavg: Pad the data to the next power-of-2 with the average value */
  char padavgP;
  /***** -numout: Output this many values.  If there are not enough values in the original data file, will pad the output file with the average value */
  char numoutP;
  int numout;
  int numoutC;
  /***** -nobary: Do not barycenter the data */
  char nobaryP;
  /***** -DE405: Use the DE405 ephemeris for barycentering instead of DE200 (the default) */
  char de405P;
  /***** -dm: The dispersion measure to de-disperse (cm^-3 pc) */
  char dmP;
  double dm;
  int dmC;
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

