#ifndef  WIN_FILE_FOLDER
#define WIN_FILE_FOLDER

#include <string>
#include <io.h>
#include<direct.h>
#include <cerrno>
#include <windows.h>
#include <Shlwapi.h>	//PathFileExists头文件
#include <tchar.h>		//定义了_t的宏，统一unicode和ansi
#include <vector>
#pragma comment(lib,"Shlwapi.lib") /*需要加上此行才可以正确link*/

/************************************************************************/
/*windows下的文件夹和文件操作，unicode/ANSI字符集均可用，不依赖于MFC环境和windows.h，linux不可用      */
/*by xiaojie，20191108*/
/************************************************************************/

/*说明：
*LPCTSTR:Unicode环境下LPCTSTR = LPCWSTR = const wchar_t*;ansi环境下：LPCTSTR=LPCSTR=const char*
*TCHAR:Unicode环境下TCHAR = wchar_t;ansi环境下：TCHAR=char
*/
using namespace std;

namespace WinSupport
{
//定义字符串适配
#ifndef _UNICODE
typedef  string UAString ;
#else
typedef  wstring UAString;
#endif

	//检查给出的路径是否存在，不会判断文件属性，
	//若存在，返回true；否则返回false
	bool IsExist(const string &_path);

	//检查给出路径是否有读权限
	//若可读，返回true；否则返回false
	bool CanRead(const string &_path);

	//检查给出路径是否有写权限
	//若可写，返回true；否则返回false
	bool CanWrite(const string &_path);

	//创建一个空目录
	bool CreateFolder(const string &folder_path, string &err);

	//判断一个目录是否存在
	bool CheckFolder(LPCTSTR folder_path, UAString &err);

	//创建一个目录，若目录已经存在，则不处理
	//is_loop:若上级目录不存在，是否循环创建
	bool CreateFolder(LPCTSTR folder_path, UAString &err, bool is_loop);

	//创建一个目录，若目录已经存在，则不处理
	//is_loop:若上级目录不存在，是否循环创建
	bool CreateFolder(LPCTSTR folder_path, UAString &err, bool is_loop);

	//测试给定路径是否存在（给定路径可以是文件，也可以是目录），true存在，false不存在
	bool IsFileExist(LPCTSTR cstr_file_path);

	//获取的一个目录内的全部文件（不包括子文件夹以及子文件夹内的内容）
	bool GetFileList(LPCTSTR folder_path, vector<UAString> &vec_file_name, UAString &err);

	//删除一个目录，包括目录下的子目录和目录本身
	bool DeleteFolder(LPCTSTR folder_path, UAString &err);

	//请空一个目录，包括目录下的子目录
	bool ClearFolder(LPCTSTR folder_path, UAString &err);

	//修改目录名称，例如将D:\\AA改为D:\\BB,则folder_path = D:\\AA,new_name=D:\\BB
	bool ModifyFolderName(LPCTSTR folder_path, LPCTSTR new_name, UAString &err);

	//将一个目录内的文件全部移动到另一个目录中，目录名不变
	bool CopyFolder(LPCTSTR src, LPCTSTR dst, UAString &err);

	//将一个目录复制到另一个目录中，目录名不变，源目录删除
	bool MoveFolder(LPCTSTR src, LPCTSTR dst, UAString &err);

	//将一个文件复制另一个目录中,isReplace控制是否覆盖同名文件，true，覆盖，false，不覆盖
	bool CopyFileSingle(LPCTSTR src, LPCTSTR dst, BOOL isReplace, UAString &err);

	//以字符串方式返回当前系统的时间,返回格式为：yyyy-mm-dd hh:mm:ss
	string GetSysTimeStandard(void);

	//以字符串方式返回当前系统的时间,返回格式为：yyyymmddhhmmss
	string GetSysTime(void);

	//字符集转换
	bool CharToLPCTSTR(const char *pSrc, int len, TCHAR *dst,int len_dst);
}

#endif