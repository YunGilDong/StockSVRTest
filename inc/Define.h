//------------------------------------------------------------------------------
//
// Project:		CLSlib
// Target:		General
// Filename:	Define.h
// Version:		
// History:
// 
//------------------------------------------------------------------------------
#ifndef	DefineH
#define	DefineH
//------------------------------------------------------------------------------
// Include File
//------------------------------------------------------------------------------
#include "General.h"
//------------------------------------------------------------------------------
// Constant
//------------------------------------------------------------------------------
// Database
//------------------------------------------------------------------------------
#define DB_USER (char *)"mios"
#define DB_PWD	(char *)"mios"
#define DB_ALIAS (char *)"MIOSLOC"
//------------------------------------------------------------------------------
#define	MAX_APPROACH		4
#define	MAX_PRCTHRC			4
#define	MAX_PRCMTHR			4
#define	MAX_EQUIP			5
#define MAX_STOCKCL			5
//------------------------------------------------------------------------------
// Directory
//------------------------------------------------------------------------------
#define	DIR_HOME				"/home/ygd/test/work/GD/StockSvr"
#define	DIR_LOG					DIR_HOME"/log/"
#define	DIR_BIN					DIR_HOME"/bin/"
//------------------------------------------------------------------------------
// Macro
//------------------------------------------------------------------------------
#define	IN_RANGE(v,a,b)		(((v) >= (a) && (v) <= (b)) ? true : false)
//------------------------------------------------------------------------------
#endif	// DefineH
//------------------------------------------------------------------------------
// End of DefineH.h
//------------------------------------------------------------------------------
