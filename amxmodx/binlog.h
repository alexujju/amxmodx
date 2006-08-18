#ifndef _INCLUDE_BINLOG_H
#define _INCLUDE_BINLOG_H

#if defined BINLOG_ENABLED

#include "CString.h"

#define BINLOG_MAGIC	0x414D424C
#define BINLOG_VERSION	0x0200

/**
 * Format of binlog:
 * uint32		magic
 * uint16		version
 * uint8		sizeof(time_t)
 * uint32		num plugins
 * [
 *  uint8		status codes
 *  str[int8]	filename
 *  uint32		num natives
 *  uint32		num publics
 *  [
 *   str[uint8] native name
 *  ]
 *  [
 *   str[uint8] public name
 *  ]
 * ]
 * [
 *  uint8		operation code
 *  time_t		realtime
 *  float		gametime
 *  int32		plugin id
 *  <extra info>
 * ]
 */

enum BinLogOp
{
	BinLog_Start=1,
	BinLog_End,
	BinLog_NativeCall,	//<int32 native id> <int32_t num_params>
	BinLog_NativeError, //<int32 errornum> <str[int16] string>
	BinLog_NativeRet,	//<cell value>
	BinLog_CallPubFunc,	//<int32 public id>
	BinLog_SetLine,		//<int32 line no#>
	BinLog_Registered,	//<string title> <string version>
	BinLog_FormatString, //<int32 param#> <int32 maxlen> <str[int16] string>
	BinLog_NativeParams, //<int32 num> <cell ...>
	BinLog_GetString,	//<cell addr> <string[int16]>
	BinLog_SetString,	//<cell addr> <int maxlen> <string[int16]>
};

class BinLog
{
public:
	BinLog() : m_state(false)
	{
	};
public:
	bool Open();
	void Close();
	void WriteOp(BinLogOp op, int plug, ...);
private:
	void WritePluginDB(FILE *fp);
private:
	String m_logfile;
	bool m_state;
};

extern BinLog g_BinLog;
extern int g_binlog_level;
extern int g_binlog_maxsize;

#endif //BINLOG_ENABLED

#endif //_INCLUDE_BINLOG_H