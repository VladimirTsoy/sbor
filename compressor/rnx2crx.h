#ifndef RNX2CRX_H
#define RNX2CRX_H

#define VERSION  "ver.4.0.4"

/**** Exit codes are defined here.   ****/
#define EXIT_WARNING 2

#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <QDebug>
#include <QDir>

/*** define macro to  ***/
#define FLUSH_BUFF printf("%s",top_buff), *(p_buff = top_buff)='\0'
#define CLEAR_BUFF *(p_buff = top_buff)='\0'

#define CRX_VERSION1 "1.0"    /* CRINEX version for RINEX 2.x */
#define CRX_VERSION2 "3.0"    /* CRINEX version for RINEX 3.x */
#define PROGNAME "RNX2CRX"
#define MAXSAT     90         /* Maximum number of satellites observed at one epoch */
#define MAXTYPE    50         /* Maximum number of data types for a GNSS system */
#define MAXCLM   1024         /* Maximum columns in one line   (>MAXTYPE*19+3)  */
#define MAX_BUFF_SIZE 131072  /* Muximum size of output buffer (>MAXSAT*(MAXTYPE*19+4)+60 */
#define ARC_ORDER 3           /* order of difference to take    */

/* define data structure for fields of clock offset and obsercvation records */
/* Those data will be handled as integers after eliminating decimal points.  */
/* Since their size may exceeds range between LONG_MIN and LONG_MAX,         */
/* they will be read with being divided properly into upper and lower digits */


class Rnx2Crx
{
public:
    Rnx2Crx(char _pathRnx[]);
    Rnx2Crx();
    typedef struct clock_format{
        long u[ARC_ORDER+1];      /* upper X digits */
        long l[ARC_ORDER+1];      /* lower 8 digits (can be 9-10 digits for deltas)*/
    } clock_format;

    typedef struct data_format{
        long u[ARC_ORDER+1];      /* upper X digits */
        long l[ARC_ORDER+1];      /* lower 5 digits (can be 6-7 digits for deltas) */
        int order;
    } data_format;

    bool compr(std::string str);
    bool  go(int argc, char *argv[]);
    void parse_args(int argc, char *argv[]);
    void header();
    int  get_next_epoch(char *p_line);
    void skip_to_next(char *p_line);
    void initialize_all(char *oldline,int *nsat_old, int count);
    void put_event_data(char *p_line);
    void read_clock(char *line,int shift_cl);
    void process_clock();
    int  set_sat_table(char *p_new, char *p_old, int nsat_old,int *sattbl);
    int  read_more_sat(int n, char *p);
    void data(int *sattbl);
    char *strdiff(char *s1, char *s2, char *ds);
    int  ggetline(data_format *py1, char *flag, char *sat_id, int *ntype_rec);
    void read_value(char *p, long *pu, long *pl);
    void take_diff(data_format *py1, data_format *py0);
    void putdiff(long dddu, long dddl);
    void put_clock(long du, long dl, int clk_order);
    int  read_chk_line(char *line);
    void error_exit(int error_no, char *string);
    void flush_buff();

private:


    long ep_count=0;
    long ep_reset=0;
    long nl_count=0;
    int rinex_version;          /* =2 or 3 */
    int nsat,ntype,ntype_gnss[UCHAR_MAX],ntype_record[MAXSAT],clk_order = -1;
    int exit_status = EXIT_SUCCESS;
    int skip_strange_epoch = 0; /* default : stop with error */

    clock_format clk1,clk0 = {0,0,0,0,0,0,0,0};
    data_format y_0[MAXSAT][MAXTYPE], y_1[MAXSAT][MAXTYPE];
    char flag0[MAXSAT][MAXTYPE*2], flag[MAXSAT][MAXTYPE*2];
    char out_buff[MAX_BUFF_SIZE]= {'x','\0'};  /**** a character is put as a stopper to aviod memory overflow ****/
    char *top_buff=&out_buff[1],*p_buff;       /**** therefore, actual baffer start from the second character ****/

    char oldline[MAXCLM] = {'&','\0'};
    int nsat_old = 0;

    bool exitStatus = true;
};

#endif // RNX2CRX_H
