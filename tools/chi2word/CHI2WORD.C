/* Chi2word.c */
/* Author: Nikolaj Grigoriev (grig@iitp.ru), 1996-1998 */

/* The whole thing was compiled by Visual C++ 5.0 */
/* Compiler flags: /Od /G3 /TC /ML */

/* This program converts ChiWriter 3.1x files to Rich Text Format.   */
/* Information about ChiWriter fonts is written in .WIN files. */
/* Headers and footers are ignored, except for the default and */
/* first-page ones. */
/* ChiWriter tables are not converted. */
/* In this version, no symbol merging is done; if more than one symbol */
/* is present in different rows of the same column, they are written */
/* one after another, preceded by QUOTATION_PREFIX and followed by */
/* QUOTATION_SUFFIX (see #defines below). Anyone willing to implement */
/* this feature is welcome. */

/* Most variables are named using Hungarian (Szimonyi) notation. */
/* The following prefixes have been used: */
/*   i -  integer  */
/*   fp - file pointer */
/*   b -  boolean */
/*   c -  unsigned char */
/*   n - any enum type */
/*   pX - pointer to X */
/*   aX - array of X */
/*   l  - long (and fpos_t) */

#include <stdio.h>
//#include <io.h>
#include <string.h>
#include <stdlib.h>
//#include <conio.h>
#include <ctype.h>

#include <cstring>

/* Uncomment this for Turbo C 2.0 or other antiques ;-) */
/* #include <alloc.h> */

/* Logical values for BOOL type */
#define TRUE   (BOOL) 1
#define FALSE  (BOOL) 0

/* Max number of footnotes */
#define NUM_NOTES  224
/* Strings to embrace remnants of composed symbols in the RTF output  */
#define QUOTATION_PREFIX    "{\\plain\\f0 <!}"
#define QUOTATION_SUFFIX    "{\\plain\\f0 !>}"

/* Macros for attribute bitwise flags */
#define A_NONE		0x00
#define A_ITALIC 	0x01
#define A_BOLD   	0x02
#define A_UNDERLINED	0x04
#define A_STRIKED	0x08
#define A_OUTLINED	0x10
#define A_SHADOWED	0x20
#define A_NONBREAKING	0x40

/* Macros for checking character attributes */
#define IsItalic(x) 	((x)&A_ITALIC)
#define IsBold(x)   	((x)&A_BOLD)
#define IsUnderlined(x) ((x)&A_UNDERLINED)
#define IsStriked(x)  	((x)&A_STRIKED)
#define IsOutlined(x)   ((x)&A_OUTLINED)
#define IsShadowed(x)   ((x)&A_SHADOWED)
#define IsNonbreaking(x) ((x)&A_NONBREAKING)

/* Macro for service char value */
#define SPECIAL_SYMBOL ((char)'\001')
#define SOFT_SPACE     ((char)'\002')

/* Macro translating ChiWriter font identifier into its number */
#define FontIdToNumber(x)  (UCHAR) ( ((x)=='0') ? 10 : \
				       ( ('1'<=(x) && (x)<='9') ? ((x)-'0') : \
				       ( ('!'<=(x) && (x)<='*') ? ((x)-'!'+11) : 0) ) )

/* Macro translating ChiWriter footnote identifier into its number */
#define NoteIdToNumber(x) (int) ((x)-'!')

/* Macro that checks a match of the input line with a certain template */
#define BeginsWith(x) (strncmp(szInputBuffer,(x),strlen((x)))==0)
#define EndsWith(x) (strcmp(szInputBuffer+strlen(szInputBuffer)-strlen((x)),(x))==0)
#define IsBlankString(x) (strspn((x), " \t\n") == strlen ((x)))

/* Auxiliary typedefs */
typedef unsigned char BOOL;
typedef unsigned char UCHAR;
typedef signed char SCHAR;

/* Essential typedefs */
typedef unsigned short int SPACING; /* number-of-rows values */
typedef UCHAR * ROW;                /* just an alias for string of chars */
typedef enum {SUCCESS, FAIL, ABORT} ERRCODE; /* return values */
typedef enum {S_HEADER, S_FOOTER, S_NOTE, S_SEPARATOR, S_MAINTEXT} SECT_TYPE; /* auxiliary section markers */
typedef enum {J_LEFT, J_RIGHT, J_CENTERED, J_JUSTIFIED} JUSTIFICATION; /* justification types for paragraphs */

typedef struct tagLINE
	    {
	    int iNumSuperRows;
	    int iNumSubRows;
	    ROW rMainRow;
	    ROW *pSuperRow;
	    ROW *pSubRow;
	    } LINE;

typedef struct tagPARAGRAPH
	    {
	    SPACING iSpacing;
	    SPACING iNumRowsBefore;
	    SPACING iNumRowsAfter;
	    JUSTIFICATION nJust;
	    int iNumLines;
	    LINE * pLine;
	    } PARAGRAPH;

typedef struct tagSECTION
	    {
	    SECT_TYPE nSectType;
	    UCHAR cSectId;
	    int iNumPars;
	    PARAGRAPH * pParagraph;
	    } SECTION;

typedef struct tagCHARINFO
	    {
	    UCHAR cWinFontId;
	    UCHAR cAttr;
	    UCHAR cSize;
	    UCHAR cIndexSize;
	    UCHAR cValue;
	    UCHAR cChiFontId;
	    } CHARINFO;

typedef struct tagFONTINFO
	    {
	    char szName[10];
	    BOOL bIsInitialized;
	    CHARINFO ciSym[256];
	    } FONTINFO;

typedef struct tagWINFONTINFO
	    {
	    char szName[64];
	    char szFamily[32];
	    char szCharset[32];
	    } WINFONTINFO;

/* Functions */
void ReadOneMoreLine (void);
void ErrorMessage (char * szMessage);
void PutProlog (char * szOrigName);
void PutEpilog (void);
void PutParagraphAttr (PARAGRAPH * pPar);
void NormalizeFontName (char * pszNormalizedName, char * pszFontName);
ERRCODE PutSpecialSection (SECTION *pSect);
ERRCODE GetFontList (void);
ERRCODE GetFontInfo (FONTINFO * pFont);
ERRCODE GetSettings (void);
ERRCODE GetSection (SECTION *pSect);
ERRCODE FreeSection (SECTION *pSect);
ERRCODE GetParagraph (PARAGRAPH *pPar);
ERRCODE FreeParagraph (PARAGRAPH *pPar);
ERRCODE AnalyzeParagraph (PARAGRAPH *pPar);
ERRCODE RecodeParagraph (PARAGRAPH *pPar);
ERRCODE GetLine (LINE *pLine);
ERRCODE FreeLine (LINE *pLine);
ERRCODE RecodeLine (LINE *pLine);
ERRCODE GetRow (ROW * pRow);
ERRCODE FreeRow (ROW * pRow);
SECT_TYPE DetermineSectionType (void);
int CountParagraphs (void);
int CountLines (void);
int CountSuperRows (void);
int CountSubRows (void);
UCHAR RegisterFont (char * pszFontName);
UCHAR GetAttr (char * pszAttrList);
void ExtractFirstChar (CHARINFO * pSym, ROW * pRow);
void PutCharacterAttr (CHARINFO * pSym, int iPos);
void GetFontOnEnd (ROW rRow);

/* Global data */
static FILE *fpIn, *fpOut;	   /* File pointers for input/output */
static UCHAR szInputBuffer[512];   /* Input buffer */
static UCHAR szCountBuffer[512];   /* Used to remember input buffer */
				   /* after count operations */
static FONTINFO fiFont[20];  	   /* List of ChiWriter fonts used */
static WINFONTINFO wfWinFont[50];  /* List of Windows fonts used */
static int iNumWinFonts;	   /* Number of Windows fonts occurred */

int iLeftMargin, iRightMargin;
static SECTION * pDefHeader, * pFirstHeader;
static SECTION * pDefFooter, * pFirstFooter;
static SECTION * pSeparator;
static SECTION * pNote[NUM_NOTES];

struct tagLastCharInfo
    {
	UCHAR cWinFontId;
	UCHAR cAttr;
	UCHAR cSize;
	int   iPos;
	} last;

static UCHAR cCurChiFont;
static BOOL bIsInsideSpecial;

void main (int argc, char **argv)
	{
	char szOutName[50];  /* Output file name (if not specified) */
	short int i;
	short int iNoteNum;
	SECTION * pSect;
	SECT_TYPE nSectType;
	PARAGRAPH parTextPar;

	if (argc!=2 && argc!=3)   /* Wrong argument number */
	    {
	    printf("\nUsage: ");
	    printf("chi2word <source_file> [recoded_file]\n\n");
	    printf("  If no output file name is specified,\n");
	    printf("  a *.RTF extension is added to the source file name\n");
	    return;
	    }

/* Opening input file */
	fpIn=fopen(argv[1],"r");
	if(fpIn==NULL)
	    {
	    printf("\n Can't open input file %s",argv[1]);
	    return;
	    }

/* Opening output file */
	if(argc==2)
	    {
	    strncpy (szOutName, argv[1], 50);
	    if (strchr(szOutName,'.') != NULL)
		strcpy (strchr(szOutName,'.'), ".rtf");
	    else strcat(szOutName,".rtf");
	    while (std::stricmp(szOutName,argv[1])==0)
		{
		printf("\nYour file already has a *.RTF extension.");
		printf("\nSpecify another output file name (or nothing to quit)");
		printf("\nOutput file:");
		scanf("%s", szOutName);
		if (szOutName[0]=='\0') return;
		}
	    }
	else strncpy(szOutName, argv[2], 50);

        if (access(szOutName, 0) == 0) /* Check for the existing file */
            {
            char c;
            printf("File %s already exists, overwrite (Y/N)?", szOutName);
            c=getche();
            putchar ('\n');
            if ( c!='y' && c!='Y')
            	return;
            }

	fpOut=fopen(szOutName,"w");
	if(fpOut==NULL)
	    {
	    printf("\n Can't open file %s", szOutName);
	    return;
	    }

/* Reading the first line */
	ReadOneMoreLine();
	if (!BeginsWith("\\1cw"))
	    {
	    ErrorMessage ("This file does not seem to be a ChiWriter file.");
	    exit (0);
	    }
	printf ("\n ChiWriter version: %s\n", szInputBuffer+4);

/* Retrieving used fonts */
	ReadOneMoreLine();
	if (GetFontList() != SUCCESS)
	    {
	    ErrorMessage ("Cannot retrieve font list");
	    exit (0);
	    }

/* Retrieving ChiWriter settings. Older ChiWriter files have no settings */
	if (GetSettings() != SUCCESS)
	    {
	    ErrorMessage ("Cannot retrieve ChiWriter settings");
	    exit (0);
            }

/* Retrieving all auxiliary sections - headers/footers, notes, etc. */
	pDefHeader = NULL;
	pFirstHeader = NULL;
	pDefFooter = NULL;
	pFirstFooter = NULL;
	pSeparator = NULL;
	for (i=0; i<NUM_NOTES; i++)
	    pNote[i] = NULL;

	while ( (nSectType = DetermineSectionType()) != S_MAINTEXT)
	    {
	    pSect = (SECTION *) malloc (sizeof(SECTION));
	    if (pSect == NULL)
		{
		ErrorMessage ("Not enough memory");
		exit (0);
		}

	    pSect->nSectType = nSectType;
	    pSect->cSectId = szInputBuffer[2];

	    ReadOneMoreLine();
	    if ( GetSection(pSect) != SUCCESS )
		{
		ErrorMessage ("Cannot read a ChiWriter section");
		exit (0);
		}

	    switch (nSectType)
		{
case S_HEADER:
		if (pSect->cSectId == 'D')
		    pDefHeader = pSect;
		else if (pSect->cSectId == '1')
		    pFirstHeader = pSect;
		else FreeSection (pSect);
		break;

case S_FOOTER:
		if (pSect->cSectId == 'D')
		    pDefFooter = pSect;
		else if (pSect->cSectId == '1')
		    pFirstFooter = pSect;
		else FreeSection (pSect);
		break;

case S_NOTE:
		iNoteNum = NoteIdToNumber(pSect->cSectId);
		pNote[iNoteNum] = pSect;
		break;

case S_SEPARATOR:
		pSeparator = pSect;
		break;

default:        ErrorMessage ("Invalid section type");
		exit (0);
		}
	    }

	PutProlog(argv[1]);

	if (PutSpecialSection (pDefHeader) == FAIL)
	    {
	    ErrorMessage ("Error while recoding the default header");
	    exit (0);
	    }
	if (PutSpecialSection (pFirstHeader) == FAIL)
	    {
	    ErrorMessage ("Error while recoding the first-page header");
	    exit (0);
	    }
	if (PutSpecialSection (pDefFooter) == FAIL)
	    {
	    ErrorMessage ("Error while recoding the default footer");
	    exit (0);
	    }
	if (PutSpecialSection (pFirstFooter) == FAIL)
	    {
	    ErrorMessage ("Error while recoding the first-page footer");
	    exit (0);
	    }
	if (PutSpecialSection (pSeparator) == FAIL)
	    {
	    ErrorMessage ("Error while recoding the footnote separator");
	    exit (0);
	    }

	cCurChiFont = 0;
	while(!BeginsWith("\\=") && !feof(fpIn))
	    {
	    if ( GetParagraph(&parTextPar) != SUCCESS )
		{
		ErrorMessage ("Cannot read a ChiWriter paragraph");
		exit (0);
		}
	    RecodeParagraph (&parTextPar);
	    FreeParagraph (&parTextPar);
	    }
	PutEpilog();
	printf ("\nAll Done!\n");

	fclose (fpIn);
	fclose (fpOut);
	return;
	}

ERRCODE GetFontList()
	{
	ERRCODE nRetStatus;
	int iFontNumber;


	for (iFontNumber = 0; iFontNumber<20; iFontNumber++)
	    {
	    fiFont[iFontNumber].szName[0] = '\0';
	    }

	nRetStatus = FAIL;
	iNumWinFonts = 1;  /* Courier is included by default */
	while (BeginsWith ("\\U"))
	    {
	    iFontNumber = FontIdToNumber(szInputBuffer[2])-1;
	    if (IsBlankString (szInputBuffer+3))
		{
		printf ("Font F%02d has no name; using STANDARD instead\n",
				iFontNumber+1);
		strcpy (fiFont[iFontNumber].szName, "STANDARD");
		}
	    else
		{
		strncpy (fiFont[iFontNumber].szName, szInputBuffer+3, 8);
		fiFont[iFontNumber].szName[8] = '\0';
		}

	    if (GetFontInfo(&(fiFont[iFontNumber])) != SUCCESS)
		{
		printf ("No description for ChiWriter font: %s\n",
               			fiFont[iFontNumber].szName);
                printf ("Using LACKING instead\n");
		strcpy (fiFont[iFontNumber].szName, "LACKING");
	    	if (GetFontInfo(&(fiFont[iFontNumber])) != SUCCESS)
		    {
                    printf ("No emergency font defined: exiting\n");
                    exit (0);
                    }
		}

	    nRetStatus = SUCCESS;
	    ReadOneMoreLine();
	    }
	return nRetStatus;
	}

/* Reading information about mapping of the detected ChiWriter fonts */
/* into the TrueType fonts */
ERRCODE GetFontInfo (FONTINFO * pFont)
	{
	static char szFileName[128];
	static char szReadLine[512];
	FILE * fpFontInfo;
	CHARINFO ciCurFont;
	register int i;
	char * ptr;
        char cDefaultSym;

	printf ("Looking for font %s...\n", pFont->szName);
	strcpy (szFileName, pFont->szName);
	strcat (szFileName, ".win");
	fpFontInfo = fopen (szFileName, "r");

	if (fpFontInfo == NULL && getenv("CHIFILES") != NULL)
	    {
	    strcpy (szFileName, getenv ("CHIFILES"));
	    strcat (szFileName, "\\");
	    strcat (szFileName, pFont->szName);
	    strcat (szFileName, ".win");
	    fpFontInfo = fopen (szFileName, "r");
	    }

	if (fpFontInfo == NULL)
	    {
	    printf ("File %s.WIN not found ", pFont->szName);
	    printf
       ("neither in the current nor in the ChiWriter directory.\n");
	    return FAIL;
	    }

	pFont->bIsInitialized = FALSE;
        cDefaultSym = '\0';

	for (i=0; i<256; i++)
	    {
	    pFont->ciSym[i].cValue = i;
	    pFont->ciSym[i].cWinFontId = 0;
	    pFont->ciSym[i].cSize = 12;
	    pFont->ciSym[i].cIndexSize = 12;
	    pFont->ciSym[i].cAttr = A_NONE;
	    }

	while (!feof(fpFontInfo))
	    {
	    fgets (szReadLine, 512, fpFontInfo);
	    if (IsBlankString(szReadLine))
		continue;
	    if (strncmp (szReadLine, "##", 2) == 0)
		continue;

	    if (strnicmp(szReadLine, "FONT", 4) == 0)
		{
		ciCurFont.cWinFontId =
			RegisterFont (szReadLine+4);
		continue;
		}

	    if (strnicmp(szReadLine, "SIZE", 4) == 0)
		{
		ciCurFont.cSize = (UCHAR)
			atoi (strtok(szReadLine+4, " \t\n:-="));
		if (ciCurFont.cSize == 0)
		    ciCurFont.cSize = 12;
		continue;
		}

	    if (strnicmp(szReadLine, "INDEX_SIZE", 10) == 0)
		{
		ciCurFont.cIndexSize = (UCHAR)
			atoi (strtok(szReadLine+10, " \t\n:-="));
		if (ciCurFont.cIndexSize == 0)
		    ciCurFont.cIndexSize = 10;
		continue;
		}

	    if (strnicmp(szReadLine, "ATTR", 4) == 0)
		{
		ciCurFont.cAttr = GetAttr (szReadLine+4);
		continue;
		}

	    if (strnicmp(szReadLine, "ATTRIBUTES", 10) == 0)
		{
		ciCurFont.cAttr = GetAttr (szReadLine+10);
		continue;
		}

	    if (strnicmp(szReadLine, "DEFSYMBOL", 9) == 0)
		{
                ptr = strtok (szReadLine+9, " \t\n=:\'");

		if (ptr == NULL)
		    ErrorMessage ("Error: DEFSYMBOL not defined.");

		else if (strlen (ptr) == 1)
                    cDefaultSym = (*ptr);

                else if (stricmp(ptr, "SPACE")==0 ||
                         stricmp(ptr, "SP")==0)
                    cDefaultSym = ' ';

                else if (stricmp(ptr, "NBSPACE")==0 ||
                         stricmp(ptr, "NBSP")==0 ||
                         stricmp(ptr, "NONBR-SPACE")==0 ||
                         stricmp(ptr, "NONBREAK-SPACE")==0)
                    {
                    cDefaultSym = ' ';
		    ciCurFont.cAttr |= A_NONBREAKING;
                    }

                else if ( strlen(ptr)==3 && isdigit(ptr[0]) &&
                          isdigit(ptr[0]) && isdigit(ptr[0]) )
                    cDefaultSym = (UCHAR)(atoi(ptr));

                else printf
                    ("Error in DEFSYMBOL: wrong target token %s\n", ptr);
		continue;
		}

	    /*  Line belongs to the recoding table */

	    if (!(pFont->bIsInitialized))
		{
		for (i=0; i<256; i++)
		    {
		    pFont->ciSym[i].cWinFontId = ciCurFont.cWinFontId;
		    pFont->ciSym[i].cSize = ciCurFont.cSize;
		    pFont->ciSym[i].cIndexSize = ciCurFont.cIndexSize;
		    pFont->ciSym[i].cAttr = ciCurFont.cAttr;
                    if (cDefaultSym)
                    	pFont->ciSym[i].cValue = cDefaultSym;
		    }
		pFont->bIsInitialized = TRUE;
		}

	    for (ptr = strtok (szReadLine, " \t\n");
		 ptr != NULL;
		 ptr = strtok (NULL, " \t\n"))
		{
		if (strlen (ptr) != 1)
		    {
		    ErrorMessage
      ("Error in the recoding table: only single-char source symbols are permitted!");
		    ErrorMessage (ptr);
		    break;
		    }

		i = (*ptr);
		ptr = strtok (NULL, " \t\n");

		if (ptr == NULL)
		    {
		    ErrorMessage
      ("Error in the recoding table: the number of tokens on one line is odd!");
		    break;
		    }

		if (strlen (ptr) == 1)
                    {
                    pFont->ciSym[i].cValue = (*ptr);
		    pFont->ciSym[i].cAttr = ciCurFont.cAttr;
                    }
                else if (stricmp(ptr, "SPACE")==0 ||
                         stricmp(ptr, "SP")==0)
                    {
                    pFont->ciSym[i].cValue = ' ';
		    pFont->ciSym[i].cAttr = (ciCurFont.cAttr & (~A_NONBREAKING));
                    }
                else if (stricmp(ptr, "NBSPACE")==0 ||
                         stricmp(ptr, "NBSP")==0 ||
                         stricmp(ptr, "NONBR-SPACE")==0 ||
                         stricmp(ptr, "NONBREAK-SPACE")==0)
                    {
                    pFont->ciSym[i].cValue = ' ';
		    pFont->ciSym[i].cAttr = (ciCurFont.cAttr|A_NONBREAKING);
                    }
                else if ( strlen(ptr)==3 && isdigit(ptr[0]) &&
                          isdigit(ptr[0]) && isdigit(ptr[0]) )
                    {
                    pFont->ciSym[i].cValue = (UCHAR)(atoi(ptr));
		    pFont->ciSym[i].cAttr = ciCurFont.cAttr;
                    }

                else
                    {
		    ErrorMessage
      ("Error in the recoding table: wrong target token!");
		    ErrorMessage (ptr);
		    break;
		    }

		pFont->ciSym[i].cWinFontId = ciCurFont.cWinFontId;
		pFont->ciSym[i].cSize = ciCurFont.cSize;
		pFont->ciSym[i].cIndexSize = ciCurFont.cIndexSize;
		}
	    }

	if (!(pFont->bIsInitialized))
	    {
	    for (i=0; i<256; i++)
		{
		pFont->ciSym[i].cWinFontId = ciCurFont.cWinFontId;
		pFont->ciSym[i].cSize = ciCurFont.cSize;
		pFont->ciSym[i].cIndexSize = ciCurFont.cIndexSize;
		pFont->ciSym[i].cAttr = ciCurFont.cAttr;
                if (cDefaultSym)
                    pFont->ciSym[i].cValue = cDefaultSym;
		}
	    pFont->bIsInitialized = TRUE;
	    }

	fclose (fpFontInfo);
	return SUCCESS;
	}

ERRCODE GetSettings()
	{
	iLeftMargin = 0;
	iRightMargin = 0;

	while (BeginsWith ("\\p"))
	    {
	    if (BeginsWith ("\\pLM"))
		iLeftMargin = atoi (szInputBuffer+5);
	    else if (BeginsWith ("\\pRM"))
		iRightMargin = atoi (szInputBuffer+5);

            ReadOneMoreLine();
	    }
	return SUCCESS;
	}

SECT_TYPE DetermineSectionType()
	{
	if (BeginsWith("\\H")) return S_HEADER;
	if (BeginsWith("\\F")) return S_FOOTER;
	if (BeginsWith("\\N")) return S_NOTE;
	if (BeginsWith("\\S")) return S_SEPARATOR;
	return S_MAINTEXT;
	}

/* ErrorMessage - a wrapper routine for error messages */
void ErrorMessage (char * szMessage)
	{
	puts (szMessage);
	}

ERRCODE GetSection (SECTION *pSect)
	{
	int i;

	pSect->iNumPars = CountParagraphs();
	if (pSect->iNumPars == 0)
	    {
	    ErrorMessage ("Cannot count paragraphs in section");
	    return FAIL;
	    }

	pSect->pParagraph = (PARAGRAPH *)
			calloc (pSect->iNumPars, sizeof(PARAGRAPH));
	if (pSect->pParagraph == NULL)
	    {
	    ErrorMessage ("Not enough memory");
	    return FAIL;
	    }

	for (i=0; i<pSect->iNumPars; i++)
	    {
	    if (GetParagraph (&(pSect->pParagraph[i])) != SUCCESS)
		return FAIL;
	    }

	ReadOneMoreLine();
	return SUCCESS;
	}

ERRCODE GetParagraph (PARAGRAPH *pPar)
	{
	int i;

	pPar->iNumLines = CountLines();
	if (pPar->iNumLines == 0)
	    {
	    ErrorMessage ("Cannot count lines in paragraph");
	    return FAIL;
	    }
	pPar->pLine = (LINE *) calloc (pPar->iNumLines, sizeof(LINE));
	if (pPar->pLine == NULL)
	    {
	    ErrorMessage ("Not enough memory");
	    return FAIL;
	    }


	for (i=0; i<pPar->iNumLines; i++)
	    {
	    if (GetLine (&(pPar->pLine[i])) != SUCCESS)
		return FAIL;
	    }
	if ( AnalyzeParagraph (pPar) != SUCCESS )
	    return FAIL;
	return SUCCESS;
	}


ERRCODE GetLine (LINE *pLine)
	{
	int i;

	pLine->iNumSuperRows = CountSuperRows();

	if (pLine->iNumSuperRows > 0)
	    {
	    pLine->pSuperRow = (ROW *)
			calloc (pLine->iNumSuperRows, sizeof(ROW));
	    if (pLine->pSuperRow == NULL)
		{
		ErrorMessage ("Not enough memory");
		return FAIL;
		}

	    for (i=0; i<pLine->iNumSuperRows; i++)
		{
		if (GetRow (&(pLine->pSuperRow[i])) != SUCCESS)
		    return FAIL;
		}
	    }

	if (GetRow (&(pLine->rMainRow)) != SUCCESS)
	    return FAIL;

	pLine->iNumSubRows = CountSubRows();

	if (pLine->iNumSubRows > 0)
	    {
	    pLine->pSubRow = (ROW *)
			calloc (pLine->iNumSubRows, sizeof(ROW));
	    if (pLine->pSubRow == NULL)
		    {
		    ErrorMessage ("Not enough memory");
		    return FAIL;
		    }

	    for (i=0; i<pLine->iNumSubRows; i++)
		    {
		    if (GetRow (&(pLine->pSubRow[i])) != SUCCESS)
		        return FAIL;
		    }
	    }

	return SUCCESS;
	}

ERRCODE GetRow (ROW *pRow)
	{
	if ( BeginsWith ("\\+") || BeginsWith ("\\-") )
	    {
	    if ( IsBlankString(szInputBuffer+2) )
		    {
		    (*pRow) = NULL;
		    ReadOneMoreLine();
		    return SUCCESS;
		    }

	    *pRow = (ROW) malloc (strlen(szInputBuffer)-1);
	    if (*pRow == NULL)
		    {
		    ErrorMessage ("Not enough memory");
		    return FAIL;
		    }

	    strcpy (*pRow, szInputBuffer+2);
	    ReadOneMoreLine();
	    return SUCCESS;
	    }

	*pRow = (ROW) malloc (strlen(szInputBuffer)+1);
	if (*pRow == NULL)
	    {
	    ErrorMessage ("Not enough memory");
	    return FAIL;
	    }

	strcpy (*pRow, szInputBuffer);
        ReadOneMoreLine();
	return SUCCESS;
	}

int CountParagraphs ()
	{
	static fpos_t lInitFilePos;
	int iCount;
	BOOL bNewPar;

	iCount = 0;
	fgetpos(fpIn, &lInitFilePos);
	strcpy (szCountBuffer, szInputBuffer);

	bNewPar = TRUE;
	while (!BeginsWith("\\="))
	    {
	    if (bNewPar && !BeginsWith("\\+") && !BeginsWith("\\-"))
		{
		iCount++;
		bNewPar = FALSE;
		}
	    if (EndsWith("\\,")) bNewPar = TRUE;
	    ReadOneMoreLine();
	    }

	fsetpos(fpIn, &lInitFilePos);
	strcpy (szInputBuffer, szCountBuffer);
	return iCount;
	}

int CountLines ()
	{
	static fpos_t lInitFilePos;
	int iCount;

	iCount = 0;
	fgetpos(fpIn, &lInitFilePos);
	strcpy (szCountBuffer, szInputBuffer);

	while (!BeginsWith("\\="))
	    {
	    if (!BeginsWith("\\+") && !BeginsWith("\\-")) iCount++;
	    if (EndsWith("\\,")) break;
	    ReadOneMoreLine();
	    }

	fsetpos(fpIn, &lInitFilePos);
	strcpy (szInputBuffer, szCountBuffer);
	return iCount;
	}

int CountSuperRows ()
	{
	static fpos_t lInitFilePos;
	int iCount;

	iCount = 0;
	fgetpos(fpIn, &lInitFilePos);
	strcpy (szCountBuffer, szInputBuffer);

	while (BeginsWith("\\+"))
	    {
	    iCount++;
	    ReadOneMoreLine();
	    }

	fsetpos(fpIn, &lInitFilePos);
	strcpy (szInputBuffer, szCountBuffer);
	return iCount;
	}

int CountSubRows ()
	{
	static fpos_t lInitFilePos;
	int iCount;

	iCount = 0;
	fgetpos(fpIn, &lInitFilePos);
	strcpy (szCountBuffer, szInputBuffer);

	while (BeginsWith("\\-"))
	    {
	    iCount++;
	    ReadOneMoreLine();
	    }

	fsetpos(fpIn, &lInitFilePos);
	strcpy (szInputBuffer, szCountBuffer);
	return iCount;
	}


ERRCODE FreeSection (SECTION *pSect)
	{
	int i;

	for (i=0; i<pSect->iNumPars; i++)
	    FreeParagraph (&(pSect->pParagraph[i]));

	free (pSect->pParagraph);
	pSect->pParagraph = NULL;
	pSect->iNumPars = 0;
	return SUCCESS;
	}

ERRCODE FreeParagraph (PARAGRAPH *pPar)
	{
	int i;

	for (i=0; i<pPar->iNumLines; i++)
	    FreeLine (&(pPar->pLine[i]));

	free (pPar->pLine);
	pPar->pLine = NULL;
	pPar->iNumLines = 0;
	return SUCCESS;
	}

ERRCODE FreeLine (LINE *pLine)
	{
	int i;

	for (i=0; i<pLine->iNumSuperRows; i++)
	    FreeRow (&(pLine->pSuperRow[i]));
	for (i=0; i<pLine->iNumSubRows; i++)
	    FreeRow (&(pLine->pSubRow[i]));
	FreeRow (&(pLine->rMainRow));

	free (pLine->pSuperRow);
	free (pLine->pSubRow);
	pLine->pSuperRow = NULL;
	pLine->pSubRow = NULL;
	pLine->iNumSuperRows = 0;
	pLine->iNumSubRows = 0;
	return SUCCESS;
	}

ERRCODE FreeRow (ROW *pRow)
	{
	if (*pRow != NULL)
	    {
	    free (*pRow);
	    (*pRow) = NULL;
	    }
	return SUCCESS;
	}

ERRCODE AnalyzeParagraph (PARAGRAPH *pPar)
	{
	int i;
	SPACING iCurSpacing;
        SPACING iMinRowsBefore;
        SPACING iMinRowsAfter;

        if (strstr (pPar->pLine[0].rMainRow, "\\^") == NULL)
	    pPar->nJust = J_LEFT;
        else pPar->nJust = J_CENTERED;

	if (pPar->iNumLines == 1)
	    {
	    pPar->iNumRowsAfter = 0;
            pPar->iNumRowsBefore = 0;
	    pPar->iSpacing = pPar->pLine[0].iNumSubRows +
            		     pPar->pLine[0].iNumSuperRows + 1;
	    }
	else
	    {
  	    iMinRowsBefore = pPar->pLine[0].iNumSuperRows;
  	    iMinRowsAfter = pPar->pLine[0].iNumSubRows;
	    pPar->iSpacing = pPar->pLine[0].iNumSuperRows +
            		     pPar->pLine[0].iNumSuperRows + 1;

	    for (i=1; i<pPar->iNumLines; i++)
		{
		iCurSpacing = pPar->pLine[i-1].iNumSubRows +
			      pPar->pLine[i].iNumSuperRows + 1;
		if (pPar->iSpacing > iCurSpacing)
		    pPar->iSpacing = iCurSpacing;

                if (iMinRowsBefore > pPar->pLine[i].iNumSuperRows)
                    iMinRowsBefore = pPar->pLine[i].iNumSuperRows;
                if (iMinRowsAfter > pPar->pLine[i].iNumSubRows)
                    iMinRowsAfter = pPar->pLine[i].iNumSubRows;
		}

	    pPar->iNumRowsBefore =
            	pPar->pLine[0].iNumSuperRows - iMinRowsBefore;
	    pPar->iNumRowsAfter =
            	pPar->pLine[pPar->iNumLines-1].iNumSubRows - iMinRowsAfter;
	    }

	if (pPar->iSpacing == 0)
	    return FAIL;

	return SUCCESS;
	}

ERRCODE RecodeParagraph (PARAGRAPH *pPar)
	{
	register int i;


	fputs ("\\pard ", fpOut);
	PutParagraphAttr(pPar);
        fputs ("{\\f0\\plain\\fs24\n", fpOut);

	last.cWinFontId = 0;
	last.cAttr = A_NONE;
	last.cSize = 12;
	last.iPos = 0;

	for (i=0; i<pPar->iNumLines; i++)
	    {
	    if (RecodeLine (pPar->pLine+i) == FAIL)
		    return FAIL;
	    }

	fputs ("\\par }\n", fpOut);
	putchar ('#');
	return SUCCESS;
	}

void ReadOneMoreLine()
	{
	UCHAR *pcNewLine;

	fgets(szInputBuffer, 512, fpIn);
	pcNewLine = strrchr (szInputBuffer, '\n');
	if (pcNewLine != NULL)
	    *pcNewLine = '\0';

	pcNewLine = strrchr (szInputBuffer, '\f');
	if (pcNewLine != NULL)
	    *pcNewLine = '\0';
	}

void PutProlog(char * szOrigName)
	{
	register int i;
        char szFileName[32];
        char *ptr;

        if ( (ptr=strrchr (szOrigName, '\\')) == NULL)
            strcpy (szFileName, szOrigName);
        else strcpy (szFileName, ptr+1);
        strupr(szFileName);

	fputs ("{\\rtf1\\ansi \\deff0\\deflang1024\\linex360\n", fpOut);

	fputs ("{\\fonttbl\n", fpOut);
	fputs ("  {\\f0\\fswiss Courier;}\n", fpOut);
	for (i=1; i<iNumWinFonts; i++)
	    {
	    fprintf (fpOut, "  {\\f%d\\f%s\\fcharset%s %s;}\n",
			i, wfWinFont[i].szFamily, 
                        wfWinFont[i].szCharset, wfWinFont[i].szName);
	    }
	fputs ("}\n", fpOut);

        fputs ("{\\info {\\doccomm Converted from a ChiWriter file ", fpOut);
        fprintf (fpOut, "%s}}\n", szFileName);

	fputs ("\\paperw12240\\paperh15840\n", fpOut);
	fputs ("\\margl1800\\margr1800\\margt1440\\margb1440\\gutter0\n", fpOut);
	fputs ("\\widowctrl\\ftnbj\n", fpOut);
	fputs ("\\pard\\plain\\f0\n", fpOut);

	last.cWinFontId = 0;
	last.cAttr = A_NONE;
	last.cSize = 12;
	last.iPos = 0;
	}

void PutEpilog()
	{
	fputs ("}\n", fpOut);
	}

ERRCODE PutSpecialSection (SECTION *pSect)
	{
	register int i;
	UCHAR cOldChiFont;

	if (pSect == NULL)
	    return SUCCESS;
	if (bIsInsideSpecial)
	    return SUCCESS;

	switch (pSect->nSectType)
	    {
case S_HEADER:
	    if (pSect->cSectId == 'D')
		fputs ("{\\header \n", fpOut);
	    else if (pSect->cSectId == '1')
		fputs ("{\\headerf \n", fpOut);
	    else
		{
		ErrorMessage ("Invalid header type");
		return FAIL;
		}
	    break;

case S_FOOTER:
	    if (pSect->cSectId == 'D')
		fputs ("{\\footer \n", fpOut);
	    else if (pSect->cSectId == '1')
		fputs ("{\\footerf \n", fpOut);
	    else
            	{
		ErrorMessage ("Invalid footer type");
		return FAIL;
		}
	    break;

case S_SEPARATOR:
	    fputs ("{\\ftnsep \n", fpOut);
	    break;

case S_NOTE:
	    fputs ("{\\footnote \n", fpOut);
	    break;

default:    ErrorMessage ("Invalid section type");
            return FAIL;
	    }

    /* Recoding the body of the section */
    /* Font number is preserved in order to treat footnotes correctly */
    cOldChiFont = cCurChiFont;
	cCurChiFont = 0;
	bIsInsideSpecial = TRUE;

	for (i=0; i<pSect->iNumPars; i++)
	    {
	    if (RecodeParagraph (&(pSect->pParagraph[i])) == FAIL)
	        {
	        bIsInsideSpecial = FALSE;
		    return FAIL;
		    }
	    }
	cCurChiFont = cOldChiFont;
	bIsInsideSpecial = FALSE;

	fputs ("}\\f0\\plain\\fs24\n", fpOut);
	FreeSection (pSect);

	last.cWinFontId = 0;
	last.cAttr = A_NONE;
	last.cSize = 12;
	last.iPos = 0;

	return SUCCESS;
	}

UCHAR RegisterFont (char * pszFontName)
	{
	register int i;
	char szNormalizedName[64];
	char szNormalizedCharset[32];
	char *p;

	if ((p = strchr (pszFontName, ',')) != NULL)
	    {
		*(p++) = '\0';
		NormalizeFontName (szNormalizedCharset, p);
	    }
	else strcpy (szNormalizedCharset, "0");
	NormalizeFontName (szNormalizedName, pszFontName);

	for (i=1; i<iNumWinFonts; i++)
	    {
	    if (strcmp (szNormalizedName, wfWinFont[i].szName) == 0 &&
                strcmp (szNormalizedCharset, wfWinFont[i].szCharset) == 0)
		return i;
	    }
	strcpy (wfWinFont[iNumWinFonts].szName, szNormalizedName);
	strcpy (wfWinFont[iNumWinFonts].szCharset, szNormalizedCharset);
	strcpy (wfWinFont[iNumWinFonts].szFamily, "nil");
	return (iNumWinFonts++);
	}

UCHAR GetAttr (char * pszAttrList)
	{
	char *ptr;
	UCHAR cAttr;

	cAttr = A_NONE;
	for (ptr = strtok (pszAttrList, " \t\n:-=");
	     ptr != NULL;
	     ptr = strtok (NULL, " \t\n:-=,;"))
	    {
	    if (stricmp (ptr, "none") == 0)
		{
		return A_NONE;
		}

	    if (stricmp (ptr, "Italic") == 0)
		{
		cAttr |= A_ITALIC;
		continue;
		}

	    if (stricmp (ptr, "Bold") == 0)
		{
		cAttr |= A_BOLD;
		continue;
		}

	    if ( stricmp (ptr, "Underline") == 0 ||
		 stricmp (ptr, "Underlined") == 0 )
		{
		cAttr |= A_UNDERLINED;
		continue;
		}

	    if ( stricmp (ptr, "Outline") == 0 ||
		 stricmp (ptr, "Outlined") == 0 )
		{
		cAttr |= A_OUTLINED;
		continue;
		}

	    if ( stricmp (ptr, "Shadow") == 0 ||
		 stricmp (ptr, "Shadowed") == 0 )
		{
		cAttr |= A_SHADOWED;
		continue;
		}

	    if ( stricmp (ptr, "Strike") == 0  ||
		 stricmp (ptr, "Striked") == 0 ||
		 stricmp (ptr, "Strikethrough") == 0 )
		{
		cAttr |= A_STRIKED;
		continue;
		}

	    if ( stricmp (ptr, "Nonbreaking") == 0 ||
                 stricmp (ptr, "Non-breaking") == 0 ||
                 stricmp (ptr, "Nobreak") == 0 ||
		 stricmp (ptr, "No-break") == 0 )
		{
		cAttr |= A_NONBREAKING;
		continue;
		}

	    ErrorMessage ("Invalid attribute type for font");
	    ErrorMessage (ptr);
	    }

	return cAttr;
	}

void NormalizeFontName (char * pszNormalizedName, char * pszFontName)
	{
	char * ptr;

	pszNormalizedName[0] = '\0';
	for (ptr = strtok (pszFontName, " \t\n:=");
	     ptr != NULL;
	     ptr = strtok (NULL, " \t\n"))
	    {
	    strlwr(ptr);
	    *ptr = toupper(*ptr);
	    if (pszNormalizedName[0] != '\0')
		strcat (pszNormalizedName, " ");
	    strcat (pszNormalizedName, ptr);
	    }
	}

void PutParagraphAttr (PARAGRAPH * pPar)
	{
	switch (pPar->nJust)
	    {
case J_LEFT:
	    fputs("\\ql", fpOut);
	    break;
case J_RIGHT:
	    fputs("\\qr", fpOut);
	    break;
case J_CENTERED:
	    fputs("\\qc", fpOut);
	    break;
case J_JUSTIFIED:
	    fputs("\\qj", fpOut);
	    break;
	    }
	fprintf (fpOut, "\\sb%d\\sa%d\\sl%d\\slmult1\\widctlpar \n",
			pPar->iNumRowsBefore*120,
			pPar->iNumRowsAfter*120,
			pPar->iSpacing*120);
	last.cWinFontId = 0;
	}

ERRCODE RecodeLine (LINE *pLine)
	{
	int iNumChar;
	BOOL bAllRowsEnded;
	BOOL bStopSearch;
	int i;
	CHARINFO ciMainChar;
	CHARINFO * pSuperChar, * pSubChar;

	pSuperChar = NULL;
	pSubChar = NULL;
	bAllRowsEnded = FALSE;

	if (pLine->iNumSuperRows >0)
	    {
	    pSuperChar = (CHARINFO *)
	        calloc (pLine->iNumSuperRows, sizeof(CHARINFO));
	    if (pSuperChar == NULL)
	        {
	        printf ("\nNot enough memory. \n");
	        return FAIL;
	        }
	    for (i=0; i<pLine->iNumSuperRows; i++)
	        {
	        pSuperChar[i].cChiFontId = cCurChiFont;
	        GetFontOnEnd (pLine->pSuperRow[i]);
	        }
	    }
	ciMainChar.cChiFontId = cCurChiFont;
	GetFontOnEnd(pLine->rMainRow);

    if (pLine->iNumSubRows >0)
	    {
	    pSubChar = (CHARINFO *)
	        calloc (pLine->iNumSubRows, sizeof(CHARINFO));
	    if (pSubChar == NULL)
	        {
	        printf ("\nNot enough memory. \n");
	        free (pSuperChar);
	        return FAIL;
	        }
	    for (i=0; i<pLine->iNumSubRows; i++)
	        {
	        pSubChar[i].cChiFontId = cCurChiFont;
	        GetFontOnEnd (pLine->pSubRow[i]);
	        }
	    }

    while (!bAllRowsEnded)
        {
        ExtractFirstChar (&ciMainChar, &(pLine->rMainRow));
        for (i=0; i<pLine->iNumSuperRows; i++)
            ExtractFirstChar (&(pSuperChar[i]), &(pLine->pSuperRow[i]));
        for (i=0; i<pLine->iNumSubRows; i++)
            ExtractFirstChar (&(pSubChar[i]), &(pLine->pSubRow[i]));

        /* Handling the underlines: if there's a hyphen or an underscore */
        /* in a subscript row, it is treated as an underline to the main */
        /* row symbol */
        if (ciMainChar.cValue != '\0')
            {
            bStopSearch = FALSE;
            iNumChar = 0;
            for (i=0; i<pLine->iNumSubRows && !bStopSearch; i++)
                {
                switch (pSubChar[i].cValue)
                    {
    case (UCHAR)' ':
    case (UCHAR)'\t':
    case (UCHAR)'\0':
                    break;

    case (UCHAR)'-':
    case (UCHAR)'_':
                    pSubChar[i].cValue = ' ';
                    ciMainChar.cAttr |= A_UNDERLINED;

        default:    bStopSearch = TRUE;
                    break;
                    }
                }
            }

        /* Writing the result down to the output */

        bAllRowsEnded = TRUE;
        iNumChar = 0;
        switch (ciMainChar.cValue)
            {
default:    iNumChar++;

case (UCHAR)' ':
case (UCHAR)'\t':
case SOFT_SPACE:
            bAllRowsEnded = FALSE;

case (UCHAR)'\0':
            break;
            }

        for (i=0; i<pLine->iNumSuperRows; i++)
            {
            switch (pSuperChar[i].cValue)
                {
    default:    iNumChar++;

case (UCHAR)' ':
case (UCHAR)'\t':
case SOFT_SPACE:
                bAllRowsEnded = FALSE;

case (UCHAR)'\0':
                break;
                }
            }

        for (i=0; i<pLine->iNumSubRows; i++)
            {
            switch (pSubChar[i].cValue)
                {
    default:    iNumChar++;

case (UCHAR)' ':
case (UCHAR)'\t':
case SOFT_SPACE:
                bAllRowsEnded = FALSE;

case (UCHAR)'\0':
                break;
                }
            }

        if (bAllRowsEnded)
            break;

        if (iNumChar==0 && ciMainChar.cValue == '\0')
           ciMainChar.cValue = ' ';

        if (iNumChar>1)
            {
            fputs (QUOTATION_PREFIX, fpOut);
            }

        if (iNumChar==0 || ciMainChar.cValue != ' ')
            PutCharacterAttr (&ciMainChar, 0);

        for (i=0; i<pLine->iNumSuperRows; i++)
            PutCharacterAttr (&(pSuperChar[i]), pLine->iNumSuperRows-i);
        for (i=0; i<pLine->iNumSubRows; i++)
            PutCharacterAttr (&(pSubChar[i]), -(i+1));
        if (iNumChar >1) fputs (QUOTATION_SUFFIX, fpOut);
        }

	free (pSuperChar);
    free (pSubChar);
	return SUCCESS;
	}

void ExtractFirstChar (CHARINFO * pSym, ROW * pRow)
    {
    char *p;
    CHARINFO *pFontSym;
    UCHAR cFont;

    if (*pRow == NULL)
        {
        pSym->cValue = '\0';
        return;
        }

    p = *pRow;
    if (*p == '\0')
        {
        *pRow = NULL;
        pSym->cValue = '\0';
        return;
        }

    if (*p==' ')
        {
        pSym->cValue = ' ';
        strcpy (*pRow, ++p);
        return;
        }
    if (strcmp (p, "-\\A") == 0) /* soft hyphen */
    	{
        strcpy (*pRow, p+3);
        ExtractFirstChar (pSym, pRow);
        return;
        }

    if (*p=='\\')
        {
        p++;
        switch (*p)
            {
case (UCHAR)'+':
case (UCHAR)'-':
case (UCHAR)'=':
case (UCHAR)',':
case (UCHAR)'/':
case (UCHAR)'B':
            strcpy (*pRow, ++p);
            ExtractFirstChar (pSym, pRow);
            return;

case (UCHAR)'F':
            p++;
            fputs("{\\cs16\\super \\chftn}", fpOut);
            PutSpecialSection (pNote[NoteIdToNumber(*p)]);
            pSym->cValue = SPECIAL_SYMBOL;
            strcpy (*pRow, ++p);
            return;

case (UCHAR)' ':
case (UCHAR)'^':
            pSym->cValue = SOFT_SPACE;
            strcpy (*pRow, ++p);
            return;

case (UCHAR)']':
            pSym->cValue = ' ';
            strcpy (*pRow, ++p);
            return;


case (UCHAR)'[':
            pSym->cValue = '\t';
            strcpy (*pRow, ++p);
            return;

case (UCHAR)'\\':
	    break;

case (UCHAR)'@':
            fputs("{\\field {\\*\\fldinst {\\cs17  PAGE }}}", fpOut);
            pSym->cValue = SPECIAL_SYMBOL;
            strcpy (*pRow, ++p);
            return;

default:    cFont = FontIdToNumber(*p);
            if (cFont!=0)
                {
                pSym->cChiFontId = cFont-1;
                strcpy (*pRow, ++p);
                ExtractFirstChar (pSym, pRow);
                return;
                }

            strcpy (*pRow, ++p);
            ExtractFirstChar (pSym, pRow);
            return;
            }
        }

        pFontSym = &(fiFont[pSym->cChiFontId].ciSym[*p]);
	pSym->cWinFontId = pFontSym->cWinFontId;
	pSym->cAttr = pFontSym->cAttr;
	pSym->cSize = pFontSym->cSize;
	pSym->cIndexSize = pFontSym->cIndexSize;
	pSym->cValue = pFontSym->cValue;

    strcpy (*pRow, ++p);
    return;
    }

void PutCharacterAttr (CHARINFO * pSym, int iPos)
    {
    UCHAR cSize;
    BOOL bAttrChanged;

    if (pSym->cValue == '\0' ||
        pSym->cValue == SPECIAL_SYMBOL ||
        pSym->cValue == SOFT_SPACE) return;

    if (pSym->cValue == ' '  ||
        pSym->cValue == '\t')  /* Spaces have no attributes */
        {
        if (iPos == 0)  /* Spaces outside the main row are ignored */
            {
            switch (pSym->cValue)
           	{
    case ' ':
    		if (IsNonbreaking(pSym->cAttr))
                    fputs ("\\~", fpOut);
                else fputc (' ', fpOut);
    		break;
    case '\t':	fputs ("\\tab ", fpOut);
    		break;
                }
            }
        return;
        }

    bAttrChanged = FALSE;
    if (iPos != last.iPos)
        {
        bAttrChanged = TRUE;
        if (iPos >= 0) fprintf (fpOut, "\\up%d", iPos*6);
        else fprintf (fpOut, "\\dn%d", (-iPos)*6);
        }
    last.iPos = iPos;

    cSize = ( (iPos==0) ? pSym->cSize : pSym->cIndexSize );
    if (cSize != last.cSize)
        {
        bAttrChanged = TRUE;
        fprintf (fpOut, "\\fs%d", cSize*2);
        }
    last.cSize = cSize;

    if (pSym->cWinFontId != last.cWinFontId)
        {
        bAttrChanged = TRUE;
        fprintf (fpOut, "\\f%d", pSym->cWinFontId);
        }
    last.cWinFontId = pSym->cWinFontId;

    if (pSym->cAttr != last.cAttr)
        {
        bAttrChanged = TRUE;
        if (IsItalic(pSym->cAttr) && !IsItalic(last.cAttr))
            fprintf (fpOut, "\\i");
        if (!IsItalic(pSym->cAttr) && IsItalic(last.cAttr))
            fprintf (fpOut, "\\i0");

        if (IsBold(pSym->cAttr) && !IsBold(last.cAttr))
            fprintf (fpOut, "\\b");
        if (!IsBold(pSym->cAttr) && IsBold(last.cAttr))
            fprintf (fpOut, "\\b0");

        if (IsUnderlined(pSym->cAttr) && !IsUnderlined(last.cAttr))
            fprintf (fpOut, "\\ul");
        if (!IsUnderlined(pSym->cAttr) && IsUnderlined(last.cAttr))
            fprintf (fpOut, "\\ul0");

        if (IsStriked(pSym->cAttr) && !IsStriked(last.cAttr))
            fprintf (fpOut, "\\strike");
        if (!IsStriked(pSym->cAttr) && IsStriked(last.cAttr))
            fprintf (fpOut, "\\strike0");

        if (IsOutlined(pSym->cAttr) && !IsOutlined(last.cAttr))
            fprintf (fpOut, "\\outl");
        if (!IsOutlined(pSym->cAttr) && IsOutlined(last.cAttr))
            fprintf (fpOut, "\\outl0");

        if (IsShadowed(pSym->cAttr) && !IsShadowed(last.cAttr))
            fprintf (fpOut, "\\shad");
        if (!IsShadowed(pSym->cAttr) && IsShadowed(last.cAttr))
            fprintf (fpOut, "\\shad0");
        }

    last.cAttr = pSym->cAttr;

    if (bAttrChanged) fprintf (fpOut, " ");
    if (pSym->cValue == '\\' ||
        pSym->cValue == '{'  ||
        pSym->cValue == '}')
        fputc ('\\', fpOut);
    fputc (pSym->cValue, fpOut);
    }

void GetFontOnEnd (ROW rRow)
    {
    char *p;
    UCHAR cChiFont;
	if (rRow == NULL) return;

    for (p=strchr(rRow, '\\'); p!=NULL; p=strchr(p+1, '\\'))
        {
	if (p[1]=='\\')
	    { 
	    p++;
	    continue;
	    }
        cChiFont = FontIdToNumber(p[1]);
        if (cChiFont != 0)
            cCurChiFont = cChiFont-1;
        }
    }

