/*   CLIPS Version 6.10  04/09/97 */

/*
 * This file is the general header file for all parts of the MicroEMACS
 * display editor.  It contains definitions used by everyone, and it contains
 * the stuff you have to edit to create a version of the editor for a
 * specific operating system and terminal.
 */

#ifndef _EDITOR_HEADER_
#define _EDITOR_HEADER_

#include <stdio.h>
#define _STDIO_INCLUDED_
#include <stdlib.h>
#include <string.h>

#include "argacces.h"
#include "constant.h"
#include "constrct.h"
#include "memalloc.h"
#include "evaluatn.h"
#include "router.h"
#include "scanner.h"
#include "symbol.h"

#if     VMS
#define ANSI    1                       /* Normally used for VAX VMS    */
#define VT52    0                       /* VT52 terminal (Zenith).      */
#define TERMCAP 0                       /* Use TERMCAP                  */
#define IBM_PC  0			/* Standard IBM PC BIOS         */
#endif

#if     UNIX_7 || UNIX_V
#define ANSI    0                       /* Normally used for VAX VMS    */
#define VT52    0                       /* VT52 terminal (Zenith).      */
#define TERMCAP 1                       /* Use TERMCAP                  */
#define IBM_PC  0			/* Standard IBM PC BIOS         */
#endif

#if     IBM_MSC || IBM_TBC || IBM_ZTC || IBM_ICB
#define ANSI    0                       /* Normally used for VAX VMS    */
#define VT52    0                       /* VT52 terminal (Zenith).      */
#define TERMCAP 0                       /* Use TERMCAP                  */
#define IBM_PC  1			/* Standard IBM PC BIOS         */
#endif

#if     IBM_GCC
#define ANSI    0                       /* Normally used for VAX VMS    */
#define VT52    0                       /* VT52 terminal (Zenith).      */
#define TERMCAP 1                       /* Use TERMCAP                  */
#define IBM_PC  0                       /* Standard IBM PC BIOS         */
#endif


#define CVMVAS  1                       /* C-V, M-V arg. in screens.    */

#define VERSION_NUM "3.0"               /* Emacs editor Version number  */

#define NFILEN  256                     /* # of bytes, file name        */
#define NBUFN   16                      /* # of bytes, buffer name      */
#define NLINE   256                     /* # of bytes, line             */
#define NKBDM   256                     /* # of strokes, keyboard macro */
#define NPAT    80                      /* # of bytes, pattern          */
#define HUGE    1000                    /* Huge number                  */

#define AGRAVE  0x60                    /* M- prefix,   Grave (LK201)   */
#define METACH  0x1B                    /* M- prefix,   Control-[, ESC  */
#define CTMECH  0x1C                    /* C-M- prefix, Control-\       */
#define EXITCH  0x1D                    /* Exit level,  Control-]       */
#define COTLCH  0x1E                    /* C- prefix,   Control-^       */
#define HELPCH  0x1F                    /* Help key,    Control-_       */
#define DEL_KEY 0x7F                    /* Del key, most keyboards      */

#define COTL    0x0100                  /* Control flag, or'ed in       */
#define META    0x0200                  /* Meta flag, or'ed in          */
#define CTLX    0x0400                  /* ^X flag, or'ed in            */

#define ABORT   2                       /* Death, ^G, abort, etc.       */

#define FIOSUC  0                       /* File I/O, success.           */
#define FIOFNF  1                       /* File I/O, file not found.    */
#define FIOEOF  2                       /* File I/O, end of file.       */
#define FIOERR  3                       /* File I/O, error.             */

#define CFCPCN  0x0001                  /* Last command was C-P, C-N    */
#define CFKILL  0x0002                  /* Last command was a kill      */

/*
 * There is a window structure allocated for every active display window. The
 * windows are kept in a big list, in top to bottom screen order, with the
 * listhead at "wheadp". Each window contains its own values of dot and mark.
 * The flag field contains some bits that are set by commands to guide
 * redisplay; although this is a bit of a compromise in terms of decoupling,
 * the full blown redisplay is just too expensive to run for every input
 * character.
 */
typedef struct  WINDOW {
        struct  WINDOW *w_wndp;         /* Next window                  */
        struct  BUFFER *w_bufp;         /* Buffer displayed in window   */
        struct  LINE *w_linep;          /* Top line in the window       */
        struct  LINE *w_dotp;           /* Line containing "."          */
        short   w_doto;                 /* Byte offset for "."          */
        struct  LINE *w_markp;          /* Line containing "mark"       */
        short   w_marko;                /* Byte offset for "mark"       */
        char    w_toprow;               /* Origin 0 top row of window   */
        char    w_ntrows;               /* # of rows of text in window  */
        char    w_force;                /* If NZ, forcing row.          */
        char    w_flag;                 /* Flags.                       */
}       WINDOW;

#define WFFORCE 0x01                    /* Window needs forced reframe  */
#define WFMOVE  0x02                    /* Movement from line to line   */
#define WFEDIT  0x04                    /* Editing within a line        */
#define WFHARD  0x08                    /* Better to a full display     */
#define WFMODE  0x10                    /* Update mode line.            */

/*
 * Text is kept in buffers. A buffer header, described below, exists for every
 * buffer in the system. The buffers are kept in a big list, so that commands
 * that search for a buffer by name can find the buffer header. There is a
 * safe store for the dot and mark in the header, but this is only valid if
 * the buffer is not being displayed (that is, if "b_nwnd" is 0). The text for
 * the buffer is kept in a circularly linked list of lines, with a pointer to
 * the header line in "b_linep".
 */
typedef struct  BUFFER {
        struct  BUFFER *b_bufp;         /* Link to next BUFFER          */
        struct  LINE *b_dotp;           /* Link to "." LINE structure   */
        short   b_doto;                 /* Offset of "." in above LINE  */
        struct  LINE *b_markp;          /* The same as the above two,   */
        short   b_marko;                /* but for the "mark"           */
        struct  LINE *b_linep;          /* Link to the header LINE      */
        char    b_nwnd;                 /* Count of windows on buffer   */
        char    b_flag;                 /* Flags                        */
        char    b_fname[NFILEN];        /* File name                    */
        char    b_bname[NBUFN];         /* Buffer name                  */
}       BUFFER;

#define BFTEMP  0x01                    /* Internal temporary buffer    */
#define BFCHG   0x02                    /* Changed since last write     */

/*
 * The starting position of a region, and the size of the region in
 * characters, is kept in a region structure.  Used by the region commands.
 */
typedef struct  {
        struct  LINE *r_linep;          /* Origin LINE address.         */
        short   r_offset;               /* Origin LINE offset.          */
        long    r_size;                 /* Length in characters.        */
}       REGION;

/*
 * All text is kept in circularly linked lists of "LINE" structures. These
 * begin at the header line (which is the blank line beyond the end of the
 * buffer). This line is pointed to by the "BUFFER". Each line contains a the
 * number of bytes in the line (the "used" size), the size of the text array,
 * and the text. The end of line is not stored as a byte; it's implied. Future
 * additions will include update hints, and a list of marks into the line.
 */
typedef struct  LINE {
        struct  LINE *l_fp;             /* Link to the next line        */
        struct  LINE *l_bp;             /* Link to the previous line    */
        short   l_size;                 /* Allocated size               */
        short   l_used;                 /* Used size                    */
        char    l_text[1];              /* A bunch of characters.       */
}       LINE;

#define lforw(lp)       ((lp)->l_fp)
#define lback(lp)       ((lp)->l_bp)
#define lgetc(lp, n)    ((lp)->l_text[(int) (n)]&0xFF)
#define lputc(lp, n, c) ((lp)->l_text[(int) (n)]= (char) (c))
#define llength(lp)     ((lp)->l_used)

/*
 * The editor communicates with the display using a high level interface. A
 * "TERM" structure holds useful variables, and indirect pointers to routines
 * that do useful operations. The low level get and put routines are here too.
 * This lets a terminal, in addition to having non standard commands, have
 * funny get and put character code too. The calls might get changed to
 * "termp->t_field" style in the future, to make it possible to run more than
 * one terminal type.
 */


typedef struct  {
        short   t_nrow;                 /* Number of rows.              */
        short   t_ncol;                 /* Number of columns.           */
        VOID    (*t_open)(void);        /* Open terminal at the start.  */
        VOID    (*t_close)(void);       /* Close terminal at end.       */
        int     (*t_getchar)(void);     /* Get character from keyboard. */
        VOID    (*t_putchar)(int);      /* Put character to display.    */
        VOID    (*t_flush)(void);       /* Flush output buffers.        */
        VOID    (*t_move)(int,int);     /* Move the cursor, origin 0.   */
        VOID    (*t_eeol)(void);        /* Erase to end of line.        */
        VOID    (*t_eeop)(void);        /* Erase to end of page.        */
        VOID    (*t_beep)(void);        /* Beep.                        */
}       TERM;


#if IBM_PC || IBM_GCC
/*
 *  This section defines the code returned by all the special keys on
 *  a PC numeric Keypad. It could also be used to define the function
 *  keys if desired.
 */

#define UP_ARROW	256
#define DOWN_ARROW	257
#define RIGHT_ARROW	258
#define LEFT_ARROW	259
#define HOME_KEY	260
#define END_KEY  	261
#define PGUP_KEY	262
#define PGDN_KEY	263

#define COTL_RIGHT_ARROW	264
#define COTL_LEFT_ARROW	 	265

#define COTL_AT_SIGN	266

#endif

#if IBM_PC || VMS

#define BADKEY		999

#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _EDBASIC_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

LOCALE int gotobol(int,int);
LOCALE int backchar(int,int);
LOCALE int gotoeol(int,int);
LOCALE int forwchar(int,int);
LOCALE int gotobob(int,int);
LOCALE int gotoeob(int,int);
LOCALE int forwline(int,int);
LOCALE int backline(int,int);
LOCALE int getgoal(LINE *);
LOCALE int forwpage(int,int);
LOCALE int backpage(int,int);
LOCALE int setmark(int,int);
LOCALE int swapmark(int,int);
LOCALE int wrapword(void);
LOCALE int backword(int,int);
LOCALE int forwword(int,int);
LOCALE int upperword(int,int);
LOCALE int lowerword(int,int);
LOCALE int capword(int,int);
LOCALE int delfword(int,int);
LOCALE int delbword(int,int);
LOCALE int inword(void);
LOCALE int killregion(int,int);
LOCALE int copyregion(int,int);
LOCALE int upperregion(int,int);
LOCALE int lowerregion(int,int);
LOCALE int getregion(REGION *);
LOCALE int fileread(int,int);
LOCALE int filevisit(int,int);
LOCALE int filevisit_guts(char []);
LOCALE int readin(char []);
LOCALE int makename(char [],char []);
LOCALE int filewrite(int,int);
LOCALE int filesave(int,int);
LOCALE int writeout(char *);
LOCALE int filename(int,int);
LOCALE int ffropen(char *);
LOCALE int ffwopen(char *);
LOCALE int ffclose(void);
LOCALE int ffputline(char [],int);
LOCALE int ffgetline(char [],int);
LOCALE VOID ttopen(void);
LOCALE VOID ttclose(void);
LOCALE VOID ttputc(int);
LOCALE VOID ttflush(void);
LOCALE int ttgetc(void);

#if VMS
LOCALE int parse_esc_seq(void);
#endif

#undef LOCALE
#ifdef _EDMAIN_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

LOCALE VOID edinit(char []);
LOCALE int execute(int,int,int);
LOCALE int getkey(void);
LOCALE int getctl(void);
LOCALE int quickexit(int,int);
LOCALE int edquit(int,int);
LOCALE int temp_quit(int,int);
LOCALE int ctlxlp(int,int);
LOCALE int ctlxrp(int,int);
LOCALE int ctlxe(int,int);
LOCALE int ctrlg(int,int);
LOCALE VOID full_cleanup(void);
LOCALE int kill_all_buffers(BUFFER **);
LOCALE int kill_all_windows(void);
LOCALE int spec_clear(BUFFER *);
LOCALE VOID EditCommand(void);
LOCALE VOID EditorFunctionDefinition(void);

#ifndef _EDMAIN_SOURCE_

extern int     currow;
extern int     curcol;
extern int     fillcol;
extern int     thisflag;
extern int     lastflag;
extern int     curgoal;
extern BUFFER  *curbp;
extern WINDOW  *curwp;
extern BUFFER  *bheadp;
extern WINDOW  *wheadp;
extern BUFFER  *blistp;
extern short   kbdm[NKBDM];
extern short   *kbdmip;
extern short   *kbdmop;
extern char    pat[NPAT];
extern char    lastbufn[NBUFN];
extern BUFFER *CompileBufferp;

#endif


#undef LOCALE
#ifdef _EDMISC_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

LOCALE int compile_region(int,int);
LOCALE int compile_file(int,int);
LOCALE int get_compile(char *,char *);
LOCALE int region_fnd(char *);
LOCALE int region_getc(char *);
LOCALE int region_ungetc(int,char *);
LOCALE int buffer_fnd(char *);
LOCALE int buffer_getc(char *);
LOCALE int buffer_ungetc(int,char *);
LOCALE int query_cmp(char *);
LOCALE int print_cmp(char *,char *);
LOCALE VOID init_cmp_router(void);
LOCALE VOID kill_cmp_router(void);
LOCALE int setfillcol(int,int);
LOCALE int showcpos(int,int);
LOCALE int getccol(int);
LOCALE int getcline(void);
LOCALE int cntlines(void);
LOCALE int gotoline(int,int);
LOCALE int twiddle(int,int);
LOCALE int quote(int,int);
LOCALE int tab(int,int);
LOCALE int openline(int,int);
LOCALE int newline(int,int);
LOCALE int deblank(int,int);
LOCALE int indent(int,int);
LOCALE int forwdel(int,int);
LOCALE int backdel(int,int);
LOCALE int kill_fwd(int,int);
LOCALE int yank(int,int);
LOCALE int forwsearch(int,int);
LOCALE int backsearch(int,int);
LOCALE int bkwrdrpl(int,int);
LOCALE int bkwrdcr(int,int);
LOCALE int frwsr(int,int);
LOCALE int querysr(int,int);
LOCALE int lreplace(char *);
LOCALE int smatchb(int,int);
LOCALE int searchcl(int);
LOCALE int searchop(int);
LOCALE int readpattern(char *);
LOCALE int spawncli(int,int);
LOCALE int spawn(int,int);

#if VMS
LOCALE int sys(char *);
#endif


#undef LOCALE
#ifdef _EDSTRUCT_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

LOCALE int usebuffer(int,int);
LOCALE int killbuffer(int,int);
LOCALE int listbuffers(int,int);
LOCALE int makelist(void);
LOCALE int addline(BUFFER *,char *);
LOCALE int anycb(void);
LOCALE BUFFER *bfind(char *,int,int);
LOCALE int bclear(BUFFER *);
LOCALE LINE *lalloc(int);
LOCALE VOID lfree(LINE *);
LOCALE VOID lchange(int);
LOCALE int linsert(int,int);
LOCALE int lnewline(void);
LOCALE int ldelete(long,int);
LOCALE int ldelnewline(void);
LOCALE VOID kdelete(void);
LOCALE int kinsert(int);
LOCALE int kremove(int);
LOCALE int reposition(int,int);
LOCALE int EditorRefresh(int,int);
LOCALE int nextwind(int,int);
LOCALE int prevwind(int,int);
LOCALE int mvdnwind(int,int);
LOCALE int mvupwind(int,int);
LOCALE int onlywind(int,int);
LOCALE int splitwind(int,int);
LOCALE int enlargewind(int,int);
LOCALE int shrinkwind(int,int);
LOCALE WINDOW *wpopup(void);
LOCALE VOID vtinit(void);
LOCALE VOID vttidy(void);
LOCALE VOID vtmove(int,int);
LOCALE VOID vtputc(int);
LOCALE VOID vteeol(void);
LOCALE VOID update(void);
LOCALE VOID updateline(int,char [],char []);
LOCALE VOID modeline(WINDOW *);
LOCALE VOID movecursor(int,int);
LOCALE VOID mlerase(void);
LOCALE int mlyesno(char *);
LOCALE int mlreply(char *,char *,int);
LOCALE VOID mlwrite(char *,...);
LOCALE VOID mlputs(char *);
LOCALE VOID mlputi(int,int);
LOCALE VOID mlputli(long,int);
LOCALE VOID kill_video_buffers(void);

#ifndef _EDSTRUCT_SOURCE_
extern int     mpresf;
extern int     sgarbf;
#endif

#ifndef _EDTERM_SOURCE_
extern TERM term;
#endif

#endif




