/*
   archisq.h ------ header for the
   structure that VPG programs use for accessing
   multiple tables and cursors from one single program
   By Franz J Fortuny ----  April 28 1997
*/

#define MAXSTMT 20

typedef struct archi {
  HDBC hdbc;
  HSTMT hstmt[20],stins,stdel[20],stupd[20];
  SDWORD iRow[20];
  int ik;
  UWORD fRSt[2];
  char *a_buf;
  char *a_dat;
  char *tname;
  UDWORD a_sz;
  SDWORD cRow[20];
  char modo;
  int bound[20];
  SDWORD *szs;
  UCHAR curname[32][20];
  char prede[20];
  char premo[20];
  char prein[20];
  char pre[20];
  char *pun[20];
  } ARCHI;
