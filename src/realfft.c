/*     Real-Valued Data FFT Program        */
/*          by Scott Ransom                */
/*            Version 3.0                  */

#include <time.h>
#include <sys/times.h>
#include "clk_tck.h"
#include "misc_utils.h"
#include "chkio.h"
#include "ransomfft.h"
#include "vectors.h"
#include "realfft_cmd.h"

#ifdef USEDMALLOC
#include "dmalloc.h"
#endif

#define DEBUG 1

/*  This program calculates the FFT of a file containing    */
/*  a number of single-precision floats representing        */
/*  real numbers.  (i.e. a normal time series)              */
/*  The data is assumed to be located in the directory,     */
/*  specified in the input filename(s).                     */
/*  Input filename(s) must not include '.dat' or '.fft'     */
/*        suffixes. They will be added by the program.      */
/*  Do not end paths in '/'.                                */
/*  Scratch file(s) are the same size as the input file(s). */
/*  If '-inv' is specified, the file to be transformed      */
/*        should end in '.fft'.  Otherwise, it should end   */
/*        in '.dat'.                                        */

int main(int argc, char *argv[])
{
  FILE *datafiles[], *scratchfiles[], *resultfiles[];
  float *data;
  int ii, status, isign, numfiles, issuffix;
  long long numdata;
  char *dir, *lastdir, *filenm, *filenm_root, *filenm_suffix;
  char *datapath, *scratchpath, *resultpath;
  char *datafilenm_root, *scratchfilenm_root, *resultfilenm_root;
  struct tms runtimes;
  double ttim, stim, utim, tott;
  Cmdline *cmd;

  /* Call usage() if we have no command line arguments */

  if (argc == 1) {
    Program = argv[0];
    usage();
    exit(1);
  }

  /* Parse the command line using the excellent program Clig */

  cmd = parseCmdline(argc, argv);

#ifdef DEBUG
  showOptionValues();
#endif

  tott = times(&runtimes) / (double) CLK_TCK;
  printf("\n\n");
  printf("   Real-Valued Data FFT Program v3.0\n");
  printf("        by Scott M. Ransom\n");
  printf("           27 Sept, 2000\n\n");

  
  numfiles = cmd->argc;
  printf("There are %d input files:\n", numfiles);
  datafiles = (FILE *)malloc(sizeof(FILE)*numfiles);
  for (ii=0; ii<numfiles; ii++){
    split_path_file(cmd->argv[ii], &path, &file);
    issuffix = split_root_suffix(file, &root, &suffix);
    if (issuffix==0){
      suffix = (char *)calloc(4, sizeof(char));
      strncpy(suffix, "dat", 3);
    } else {
      if (strcmp(suffix, "dat")==0) isign = -1;
      if (strcmp(suffix, "fft")==0) isign = 1;
    }
    datafiles[ii] = chkfopen("
  }





  if ((argc > 1) && (argc < 7)) {

    /*      Open and check data file.    */

    if (argc == 3) {
      sprintf(datafilenm, "%s.", argv[2]);
      sprintf(scratchfilenm, "%s.tmp", argv[2]);
      sprintf(resultfilenm, "%s.", argv[2]);
    }
    if (argc == 4) {
      sprintf(datafilenm, "%s/%s.", argv[3], argv[2]);
      sprintf(scratchfilenm, "%s/%s.tmp", argv[3], argv[2]);
      sprintf(resultfilenm, "%s/%s.", argv[3], argv[2]);
    }
    if (argc == 5) {
      sprintf(datafilenm, "%s/%s.", argv[3], argv[2]);
      sprintf(scratchfilenm, "%s/%s.tmp", argv[4], argv[2]);
      sprintf(resultfilenm, "%s/%s.", argv[3], argv[2]);
    }
    isign = atoi(argv[1]);

    /*  Add the appropriate suffix to the filenames. */

    if (isign == 1) {
      strcat(datafilenm, "fft");
      strcat(resultfilenm, "dat");
    } else {
      strcat(datafilenm, "dat");
      strcat(resultfilenm, "fft");
    }

    /*  Check the input data set...  */

    printf("Checking data in \"%s\".\n", datafilenm);
    datafile = chkfopen(datafilenm, "rb");

    /* # of real data points */

    numdata = chkfilelen(datafile, sizeof(float));
/*     next2ton = next2_to_n(numdata); */
/*     if (numdata != next2ton) { */
/*       printf("\nNumber of data pts must be an integer power of two,\n"); */
/*       printf("     or data must be single precision floats.\n"); */
/*       printf("Exiting.\n\n"); */
/*       fclose(datafile); */
/*       exit(1); */
/*     } */
    printf("Data OK.  There are %ld points.\n\n", numdata);
    fclose(datafile);
  } else {
    exit(0);
  }

  /*     Start the transform sequence               */

  if (numdata > MAXREALFFT) {

    /*  Perform Two-Pass, Out-of-Core, FFT  */

    printf("Performing Out-of-Core Two-Pass FFT on data.\n\n");
    printf("Result will be stored in the file \"%s\".\n", resultfilenm);

    /*  Initialize files. */

    datafile = chkfopen(datafilenm, "rb+");
    scratchfile = chkfopen(scratchfilenm, "wb+");

    printf("\nTransforming...\n");

    /*     Call Two-Pass routine  */

    if (isign == 1) {
      realfft_scratch_inv(datafile, scratchfile, numdata);
    } else {
      realfft_scratch_fwd(datafile, scratchfile, numdata);
    }

    fclose(scratchfile);

    sprintf(command, "rm -f %s\n", scratchfilenm);
    if ((status = (system(command))) == -1 || status == 127) {
      perror("\nSystem call (rm) failed");
      printf("\n");
      exit(1);
    }
    printf("Finished.\n\n");

    printf("Timing summary:\n");
    tott = times(&runtimes) / (double) CLK_TCK - tott;
    utim = runtimes.tms_utime / (double) CLK_TCK;
    stim = runtimes.tms_stime / (double) CLK_TCK;
    ttim = utim + stim;
    printf("CPU usage: %.3f sec total (%.3f sec user, %.3f sec system)\n", \
	   ttim, utim, stim);
    printf("Total time elapsed:  %.3f sec.\n\n", tott);

  } else {

    /* Perform standard FFT for real functions  */

    printf("Performing in-core FFT for real functions on data.\n\n");
    printf("FFT will be stored in the file \"%s\".\n\n", resultfilenm);

    /* Open the data and fft results files.    */

    datafile = chkfopen(datafilenm, "rb+");

    /* Read data from file to data array   */

    printf("Reading data.\n\n");
    data = gen_fvect(numdata);
    chkfread(data, sizeof(float), (unsigned long) numdata, datafile); 
    chkfileseek(datafile, 0L, sizeof(char), SEEK_SET);

    /* Start and time the transform   */

    printf("Transforming...\n");
    realfft(data, numdata, isign);
    printf("Finished.\n\n");

    /* Write data from FFT array to file  */

    printf("Writing FFT data.\n\n");
    chkfwrite(data, sizeof(float), (unsigned long) numdata, datafile);
    free(data);

    /* Output the timing information */

    printf("Timing summary:\n");
    tott = times(&runtimes) / (double) CLK_TCK - tott;
    utim = runtimes.tms_utime / (double) CLK_TCK;
    stim = runtimes.tms_stime / (double) CLK_TCK;
    ttim = utim + stim;
    printf("CPU usage: %.3f sec total (%.3f sec user, %.3f sec system)\n", \
	   ttim, utim, stim);
    printf("Total time elapsed:  %.3f sec.\n\n", tott);

  }

  sprintf(command, "mv %s %s\n", datafilenm, resultfilenm);
  if ((status = (system(command))) == -1 || status == 127) {
    perror("\nSystem call (mv) failed");
    printf("\n");
    exit(1);
  }
  fclose(datafile);
  /*
    fftw_print_max_memory_usage();
    fftw_check_memory_leaks();
   */
  exit(0);
}
