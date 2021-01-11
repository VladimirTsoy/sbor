#ifndef CRX2RNX_H
#define CRX2RNX_H

#define VERSION  "ver.4.0.4"

///**** Exit codes are defined here. ****/
#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif

#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif

#define EXIT_WARNING 2

/* Don't change the lines from here. */
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <QString>
#include <QDir>
#include <QDebug>


/*** define macros ***/
/* #define CHOP_LF(q,p) p = strchr(q,'\n'); *p = '\0'; */
#define CHOP_LF(q,p) p = strchr(q,'\n'); if(p != NULL){if( *(p-1) == '\r' && p>q )p--;*p = '\0';}
#define CHOP_BLANK(q,p) p = strchr(q,'\0');while(*--p == ' ' && p>q);*++p = '\0'

/* define global constants */
#define PROGNAME "CRX2RNX"
#define MAXSAT     90         /* Maximum number of satellites observed at one epoch */
#define MAXTYPE    50         /* Maximum number of data types   */
#define MAXCLM   1024         /* Maximum columns in one line   (>MAXTYPE*19+3)  */
#define MAX_BUFF_SIZE 131072  /* Muximum size of output buffer (>MAXSAT*(MAXTYPE*19+4)+60 */
#define MAX_DIFF_ORDER 5      /* Muximum order of difference to be dealt with */

/* define data structure for fields of clock offset and obsercvation records */
typedef struct clock_format{
    long u[MAX_DIFF_ORDER+1];      /* upper X digits for each diference order */
    long l[MAX_DIFF_ORDER+1];      /* lower 8 digits */
} clock_format;

typedef struct data_format{
    long u[MAX_DIFF_ORDER+1];      /* upper X digits for each diference order */
    long l[MAX_DIFF_ORDER+1];      /* lower 5 digits */
    int  order;
    int  arc_order;
} data_format;

/* define global variables */


class Crx2Rnx
{
public:
    Crx2Rnx(char _pathCrx[]);
    Crx2Rnx();
    ~Crx2Rnx();
    /* declaration of functions */
    bool unCompr(std::string str);
    bool  go(int argc, char *argv[]);
    void fileopen(int argc, char *argv[]);
    void header();
    int  put_event_data(char *dline, char *p_event);
    void skip_to_next(char *dline);
    void process_clock(char *dline);
    void set_sat_table(char *p_new, char *p_old, int nsat1, int *sattbl);
    void data(char *p_sat_lst, int *sattbl, char dflag[][MAXTYPE*2]);
    void repair(char *s, char *ds);
    int  getdiff(data_format *y, data_format *y0, int i0, char *dflag);
    void putfield(data_format *y, char *flag);
    void read_clock(char *dline ,long *yu, long *yl);
    void print_clock(long yu, long yl, int shift_clk);
    int  read_chk_line(char *line);
    void error_exit(int error_no, char *string);

private:

    clock_format clk1,clk0;
    data_format y_1[MAXSAT][MAXTYPE],y_0[MAXSAT][MAXTYPE];
    char flag1[MAXSAT][MAXTYPE*2+1],flag[MAXSAT][MAXTYPE*2+1];

    int rinex_version,crinex_version,ep_top_from,ep_top_to;
    int nsat,ntype,ntype_gnss[UCHAR_MAX],ntype_record[MAXSAT],clk_order = 0, clk_arc_order = 0;
    long nl_count=0;
    int skip = 0;
    int exit_status = EXIT_SUCCESS;

    char out_buff[MAX_BUFF_SIZE],*p_buff;
    std::string     pathCrx;
    bool exitStatus = true;
};

#endif // RNX2CRX_H
