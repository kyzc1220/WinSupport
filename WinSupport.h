#ifndef  WIN_FILE_FOLDER
#define WIN_FILE_FOLDER

#include <string>
#include <io.h>
#include<direct.h>
#include <cerrno>
#include <windows.h>
#include <Shlwapi.h>	//PathFileExistsͷ�ļ�
#include <tchar.h>		//������_t�ĺ꣬ͳһunicode��ansi
#include <vector>
#pragma comment(lib,"Shlwapi.lib") /*��Ҫ���ϴ��вſ�����ȷlink*/

/************************************************************************/
/*windows�µ��ļ��к��ļ�������unicode/ANSI�ַ��������ã���������MFC������windows.h��linux������      */
/*by xiaojie��20191108*/
/************************************************************************/

/*˵����
*LPCTSTR:Unicode������LPCTSTR = LPCWSTR = const wchar_t*;ansi�����£�LPCTSTR=LPCSTR=const char*
*TCHAR:Unicode������TCHAR = wchar_t;ansi�����£�TCHAR=char
*/
using namespace std;

namespace WinSupport
{
//�����ַ�������
#ifndef _UNICODE
typedef  string UAString ;
#else
typedef  wstring UAString;
#endif

	//��������·���Ƿ���ڣ������ж��ļ����ԣ�
	//�����ڣ�����true�����򷵻�false
	bool IsExist(const string &_path);

	//������·���Ƿ��ж�Ȩ��
	//���ɶ�������true�����򷵻�false
	bool CanRead(const string &_path);

	//������·���Ƿ���дȨ��
	//����д������true�����򷵻�false
	bool CanWrite(const string &_path);

	//����һ����Ŀ¼
	bool CreateFolder(const string &folder_path, string &err);

	//�ж�һ��Ŀ¼�Ƿ����
	bool CheckFolder(LPCTSTR folder_path, UAString &err);

	//����һ��Ŀ¼����Ŀ¼�Ѿ����ڣ��򲻴���
	//is_loop:���ϼ�Ŀ¼�����ڣ��Ƿ�ѭ������
	bool CreateFolder(LPCTSTR folder_path, UAString &err, bool is_loop);

	//����һ��Ŀ¼����Ŀ¼�Ѿ����ڣ��򲻴���
	//is_loop:���ϼ�Ŀ¼�����ڣ��Ƿ�ѭ������
	bool CreateFolder(LPCTSTR folder_path, UAString &err, bool is_loop);

	//���Ը���·���Ƿ���ڣ�����·���������ļ���Ҳ������Ŀ¼����true���ڣ�false������
	bool IsFileExist(LPCTSTR cstr_file_path);

	//��ȡ��һ��Ŀ¼�ڵ�ȫ���ļ������������ļ����Լ����ļ����ڵ����ݣ�
	bool GetFileList(LPCTSTR folder_path, vector<UAString> &vec_file_name, UAString &err);

	//ɾ��һ��Ŀ¼������Ŀ¼�µ���Ŀ¼��Ŀ¼����
	bool DeleteFolder(LPCTSTR folder_path, UAString &err);

	//���һ��Ŀ¼������Ŀ¼�µ���Ŀ¼
	bool ClearFolder(LPCTSTR folder_path, UAString &err);

	//�޸�Ŀ¼���ƣ����罫D:\\AA��ΪD:\\BB,��folder_path = D:\\AA,new_name=D:\\BB
	bool ModifyFolderName(LPCTSTR folder_path, LPCTSTR new_name, UAString &err);

	//��һ��Ŀ¼�ڵ��ļ�ȫ���ƶ�����һ��Ŀ¼�У�Ŀ¼������
	bool CopyFolder(LPCTSTR src, LPCTSTR dst, UAString &err);

	//��һ��Ŀ¼���Ƶ���һ��Ŀ¼�У�Ŀ¼�����䣬ԴĿ¼ɾ��
	bool MoveFolder(LPCTSTR src, LPCTSTR dst, UAString &err);

	//��һ���ļ�������һ��Ŀ¼��,isReplace�����Ƿ񸲸�ͬ���ļ���true�����ǣ�false��������
	bool CopyFileSingle(LPCTSTR src, LPCTSTR dst, BOOL isReplace, UAString &err);

	//���ַ�����ʽ���ص�ǰϵͳ��ʱ��,���ظ�ʽΪ��yyyy-mm-dd hh:mm:ss
	string GetSysTimeStandard(void);

	//���ַ�����ʽ���ص�ǰϵͳ��ʱ��,���ظ�ʽΪ��yyyymmddhhmmss
	string GetSysTime(void);

	//�ַ���ת��
	bool CharToLPCTSTR(const char *pSrc, int len, TCHAR *dst,int len_dst);
}

#endif