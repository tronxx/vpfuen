/*
   This is part of the VPG4S version for LINUX.
   This section takes care of the binding of fields and parameters
  by Franz J Fortuny April 29 199
*/
#include <stdio.h>

#include "cli0core.h"
#include "cli0defs.h"
//#include "cli0env.h"
#include "cli0ext1.h"
#include "archisq.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

  extern HDBC hdbc[10];
  extern HENV hEnv;
  extern ARCHI *ar;
  extern int ik;
  extern SDWORD actsize;
void err(char *, ...);
void vbindpar(int aa,HSTMT thest);

RETCODE vChk(RETCODE sqres)

{
  char buf1[256],buf2[256],buf3[256];
  SDWORD sdw1;
  SWORD sw1;
  extern int aa;
  extern int ik;
  if(sqres != SQL_SUCCESS && sqres != SQL_SUCCESS_WITH_INFO) {
   sqres = SQLError(hEnv,
                    ar[aa].hdbc,
                    ar[aa].hstmt[ik],
                    buf1, /* SQL State */
                    &sdw1, /* native error code */
                    buf3, /* error message text */
                    sizeof(buf3), /* size of buffer to receive the error */
                    &sw1); /* variable to receive the value of the message */

   err("Connection to %s is returning error\n\
State: %s -- Native Error code %d\n\
Error text:\n%s f%d k%d\n",ar[aa].tname,buf1,sdw1,buf3,aa,ik);
   }

return sqres;

}


void vsqdelete(int aa)
{
  char *sql;
  UWORD icol;
  UCHAR cname[60];
  SWORD cnamersz,dType,ctype,cscale,ifnul;
  UDWORD cprecis,bufsz=0,thisz;
  char *bptr;
  int bind,i,longvar=0,rcol=0;

 sql = (char*) calloc(200,sizeof(char));

 sprintf(sql,"delete from %s where current of %s",ar[aa].tname,
             ar[aa].curname[ik]);
   vChk(SQLAllocStmt(ar[aa].hdbc,&ar[aa].stdel[ik]));
   vChk(SQLPrepare(ar[aa].stdel[ik],sql,SQL_NTS));
   free(sql);
}
void vsqupdate(int aa)
{
 /* this function prepares de insert statement */
  char *sql;
  UWORD icol;
  UCHAR cname[60];
  SWORD cnamersz,dType,ctype,cscale,ifnul;
  UDWORD cprecis,bufsz=0,thisz;
  char *bptr;
  int bind,i,longvar=0,rcol=0;

 sql = (char*) calloc(2000,sizeof(char));

 sprintf(sql,"update %s set ",ar[aa].tname);
  icol = 1;
  while ( SQLDescribeCol(ar[aa].hstmt[ik],
                        icol,
                        cname,
                        (SWORD) sizeof(cname),
                        &cnamersz,
                        &dType,
                        &cprecis,
                        &cscale,
                        &ifnul) == SQL_SUCCESS ) {
      if(dType != SQL_LONGVARBINARY && dType != SQL_LONGVARCHAR)  {
        if(icol > 1)
           strcat(sql,",");
        strcat(sql,cname);
        strcat(sql," = ?");
        rcol++;
      }
      icol++;
   }
   --icol;
   strcat(sql," where current of "); /* insert statement ready */
   strcat(sql,ar[aa].curname[ik]);
   vChk(SQLAllocStmt(ar[aa].hdbc,&ar[aa].stupd[ik]));
   vChk(SQLPrepare(ar[aa].stupd[ik],sql,SQL_NTS));
   free(sql);
   vbindpar(aa,ar[aa].stupd[ik]);
}
void vsqinsert(int aa)
{
 /* this function prepares de insert statement */
  char *sql;
  UWORD icol;
  UCHAR cname[60];
  SWORD cnamersz,dType,ctype,cscale,ifnul;
  UDWORD cprecis,bufsz=0,thisz;
  char *bptr;
  int bind,i,longvar=0,rcol=0;

 sql = (char*) calloc(2000,sizeof(char));

 sprintf(sql,"insert into %s (",ar[aa].tname);
  icol = 1;
  while ( SQLDescribeCol(ar[aa].hstmt[ik],
                        icol,
                        cname,
                        (SWORD) sizeof(cname),
                        &cnamersz,
                        &dType,
                        &cprecis,
                        &cscale,
                        &ifnul) == SQL_SUCCESS ) {
      if(dType != SQL_LONGVARBINARY && dType != SQL_LONGVARCHAR)  {
        if(icol > 1)
           strcat(sql,",");
        strcat(sql,cname);
        rcol++;
      }
      icol++;
   }
   --icol;
   strcat(sql,") values ("); /* insert statement ready */
   for(i=0;i<rcol;i++) {
    if(i>0) strcat(sql,",");
    strcat(sql," ?");
   }
   strcat(sql,")");
   vChk(SQLAllocStmt(ar[aa].hdbc,&ar[aa].stins));
   vChk(SQLPrepare(ar[aa].stins,sql,SQL_NTS));
   free(sql);
   vbindpar(aa,ar[aa].stins);
}

void vSQLBind(int aa) /* get desc. and bind columns */
{
  UWORD icol;
  UCHAR cname[60];
  SWORD cnamersz,dType,ctype,cscale,ifnul;
  UDWORD cprecis,bufsz=0,thisz;
  char *bptr;
  int bind;

  icol = 1;
  while ( SQLDescribeCol(ar[aa].hstmt[ik],
                        icol,
                        cname,
                        (SWORD) sizeof(cname),
                        &cnamersz,
                        &dType,
                        &cprecis,
                        &cscale,
                        &ifnul) == SQL_SUCCESS ) {
    switch(dType) {
     case SQL_BIT: thisz = 1; break;

     case SQL_LONGVARBINARY:
     case SQL_LONGVARCHAR: thisz = 10; break;

     case SQL_DOUBLE:
     case SQL_FLOAT:
     case SQL_DECIMAL:
     case SQL_NUMERIC: thisz = sizeof(double); break;

     case SQL_REAL: thisz = sizeof(float); break;

     case SQL_INTEGER: thisz = sizeof(long); break;

     case SQL_SMALLINT: thisz = sizeof(short int); break;

     case SQL_TINYINT: thisz = sizeof(unsigned char); break;

     case SQL_DATE: thisz = sizeof(DATE_STRUCT); break;
     case SQL_TIME: thisz = sizeof(TIME_STRUCT); break;
     case SQL_TIMESTAMP: thisz = sizeof(TIMESTAMP_STRUCT); break;

	  case SQL_BIGINT:
	  case SQL_BINARY:
     case SQL_VARBINARY:
     case SQL_CHAR:
     case SQL_VARCHAR: thisz = cprecis + 1; break;
     default: thisz = cprecis + 1; break;
    }
    bufsz = bufsz + thisz;
    icol++;
  }

  if(ar[aa].a_sz < bufsz) {
   if(ar[aa].a_buf != NULL)
     { free(ar[aa].a_buf); ar[aa].a_buf = NULL; }
    if((ar[aa].a_buf = (char *) calloc(bufsz,sizeof(char))) == NULL)
     err("Can't get memory for table # %d, key %d, (%s)",aa,ik,ar[aa].tname);
   if(ar[aa].szs != NULL)
     { free(ar[aa].szs); ar[aa].szs = NULL; }
    if((ar[aa].szs = (long *) calloc(icol,sizeof(long))) == NULL)
     err("Can't get memory for array of sizes table %s\n",ar[aa].tname);
    ar[aa].a_sz = bufsz;
    ar[aa].a_dat = ar[aa].a_buf;
  }

  bptr = ar[aa].a_buf;
  icol = 1;
  while (SQLDescribeCol(ar[aa].hstmt[ik],
                        icol,
                        cname,
                        (SWORD) sizeof(cname),
                        &cnamersz,
                        &dType,
                        &cprecis,
                        &cscale,
                        &ifnul) == SQL_SUCCESS ) {
    bind = 1;
    switch(dType) {
     case SQL_BIT: thisz = 1; ctype = SQL_C_CHAR; break;

     case SQL_LONGVARBINARY:
     case SQL_LONGVARCHAR: thisz = 10; bind = 0; break;

     case SQL_DOUBLE:
     case SQL_FLOAT:
     case SQL_DECIMAL:
     case SQL_NUMERIC:
      thisz = sizeof(double);
      ctype = SQL_C_DOUBLE;
      break;

     case SQL_REAL:
      thisz = sizeof(float);
      ctype = SQL_C_FLOAT;
      break;

     case SQL_INTEGER:
      thisz = sizeof(long);
      ctype = SQL_C_SLONG;
      break;

     case SQL_SMALLINT:
      thisz = sizeof(short int);
      ctype = SQL_C_SSHORT;
      break;

     case SQL_TINYINT: thisz = sizeof(unsigned char);
      ctype = SQL_C_CHAR;
      break;

     case SQL_DATE: thisz = sizeof(DATE_STRUCT);
      ctype = SQL_C_DATE;
      break;

     case SQL_TIME: thisz = sizeof(TIME_STRUCT);
      ctype = SQL_C_TIME;
      break;

     case SQL_TIMESTAMP: thisz = sizeof(TIMESTAMP_STRUCT);
      ctype = SQL_C_TIMESTAMP;
      break;

	  case SQL_BIGINT:
	  case SQL_BINARY:
     case SQL_VARBINARY:
     case SQL_CHAR:
     case SQL_VARCHAR:
      thisz = cprecis + 1;
      ctype = SQL_C_CHAR;
      break;

     default:
      thisz = cprecis + 1;
      ctype = SQL_C_CHAR;
      break;
    }

    if(bind) {
     vChk(SQLBindCol(ar[aa].hstmt[ik],
                   icol,
                   ctype,
                   bptr,
                   thisz,
                   &actsize));
    }
    bptr = bptr + thisz;
/*    printf("Columna # %3d tama�o %3d %s\n",icol,thisz,cname);*/
    icol++;

   }
  /* while(SQLFetch(ar[aa].hstmt[ik]) == SQL_SUCCESS)
    printf("%4d %s\n",*(int*)ar[aa].a_buf,ar[aa].a_buf+4);*/
}

void vendsql()
{
  int i;
  for(i=0;i<10;i++) {
  // SQLFreeStmt(ar[i].hstmt[ik],SQL_DROP);
   if(hdbc[i]) {
   SQLDisconnect(hdbc[i]);
   SQLFreeConnect(hdbc[i]);
   }
  }
  SQLFreeEnv(hEnv);
}

void vbindpar(int aa,HSTMT thest)
{
 /* this function should bind the parameters for an exec */
  char *sql;
  UWORD icol;
  UCHAR cname[60];
  SWORD cnamersz,dType,ctype,cscale,ifnul;
  UDWORD cprecis,bufsz=0,thisz;
  char *bptr;
  int bind,i,longvar=0,rcol=1;
   bptr = ar[aa].a_buf;
   i=1;
   while(SQLDescribeCol(ar[aa].hstmt[ik],
                        i,
                        cname,
                        (SWORD) sizeof(cname),
                        &cnamersz,
                        &dType,
                        &cprecis,
                        &cscale,
                        &ifnul) == SQL_SUCCESS) {
    bind = 1;
    switch(dType) {
     case SQL_BIT: thisz = 1; ctype = SQL_C_CHAR; break;

     case SQL_LONGVARBINARY:
     case SQL_LONGVARCHAR: thisz = 10; bind = 0; break;

     case SQL_DOUBLE:
     case SQL_FLOAT:
     case SQL_DECIMAL:
     case SQL_NUMERIC:
      thisz = sizeof(double);
      ctype = SQL_C_DOUBLE;
      break;

     case SQL_REAL:
      thisz = sizeof(float);
      ctype = SQL_C_FLOAT;
      break;

     case SQL_INTEGER:
      thisz = sizeof(long);
      ctype = SQL_C_SLONG;
      break;

     case SQL_SMALLINT:
      thisz = sizeof(short int);
      ctype = SQL_C_SSHORT;
      break;

     case SQL_TINYINT: thisz = sizeof(unsigned char);
      ctype = SQL_C_CHAR;
      break;

     case SQL_DATE: thisz = sizeof(DATE_STRUCT);
      ctype = SQL_C_DATE;
      break;

     case SQL_TIME: thisz = sizeof(TIME_STRUCT);
      ctype = SQL_C_TIME;
      break;

     case SQL_TIMESTAMP: thisz = sizeof(TIMESTAMP_STRUCT);
      ctype = SQL_C_TIMESTAMP;
      break;

	  case SQL_BIGINT:
	  case SQL_BINARY:
     case SQL_VARBINARY:
     case SQL_CHAR:
     case SQL_VARCHAR:
      thisz = cprecis + 1;
      ctype = SQL_C_CHAR;
      break;

     default:
      thisz = cprecis + 1;
      ctype = SQL_C_CHAR;
      break;
    }
    if(bind)
     { vChk(SQLBindParameter(thest,
       rcol, SQL_PARAM_INPUT, ctype, dType, cprecis, cscale,
       bptr, thisz, &ar[aa].szs[rcol-1]));
       if(ctype != SQL_C_CHAR) ar[aa].szs[rcol-1] = thisz;
       else ar[aa].szs[rcol-1] = SQL_NTS;
       rcol++;
     }
     bptr = bptr + thisz;
     i++;
   }
}
