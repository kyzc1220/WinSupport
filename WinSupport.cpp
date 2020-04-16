#include "stdafx.h"
#include "WinSupport.h"

using namespace WinSupport;

//检查给出的路径是否存在，不会判断文件属性，
//若存在，返回true；否则返回false
bool WinSupport::IsExist(const string &_path)
{
	int mode = 0;//0:是否存在，2：是否可写，4：是否可读，6：读和写权限
	if (-1 == (_access(_path.c_str(), mode)))
	{
		return false;
		/*if (errno == ENOENT)
		{
		int ss = 0;
		ss = ss;
		}*/
	}

	return true;
}

//检查给出路径是否有读权限
//若可读，返回true；否则返回false
bool WinSupport::CanRead(const string &_path)
{
	if (-1 == (_access(_path.c_str(), 4)))
	{
		return false;
		/*if (errno == ENOENT)
		{
		int ss = 0;
		ss = ss;
		}*/
	}

	return true;
}

//检查给出路径是否有写权限
//若可写，返回true；否则返回false
bool WinSupport::CanWrite(const string &_path)
{
	if (-1 == (_access(_path.c_str(), 2)))
	{
		return false;
		/*if (errno == ENOENT)
		{
		int ss = 0;
		ss = ss;
		}*/
	}

	return true;
}

//创建一个空目录
bool WinSupport::CreateFolder(const string &folder_path, string &err)
{
	if (-1 == _mkdir(folder_path.c_str()))
	{
		if (EEXIST == errno)
			err = "[" + folder_path + "]，已经存在";
		else if (ENOENT == errno)
			err = "路径[" + folder_path + "]不存在";
		else
			err = "未知错误";

		return false;
	}

	return true;
}

//判断一个目录是否存在
bool WinSupport::CheckFolder(LPCTSTR folder_path, UAString &err)
{
	DWORD   dwAttr = GetFileAttributes(folder_path);
	if (dwAttr == INVALID_FILE_ATTRIBUTES)
	{
		TCHAR charErr[256] = { 0x00 };
		_stprintf_s(charErr, sizeof(charErr)/sizeof(TCHAR), _T("路径[%s]不存在"),folder_path);
		err = folder_path;
		return false;
	}
	else
	{
		if (dwAttr & FILE_ATTRIBUTE_DIRECTORY)
			return true;
		else
			err = _T("指定路径不是目录");
	}

	return false;
}

//创建一个目录，若目录已经存在，则不处理
//is_loop:若上级目录不存在，是否循环创建
bool WinSupport::CreateFolder(LPCTSTR folder_path, UAString &err, bool is_loop)
{
	//检查目录是否存在，若已经存在，则返回
	if (CheckFolder(folder_path, err))
		return true;

	//创建目录
	TCHAR errMsg[256] = { 0x00 };
	if (!is_loop)//不循环创建
	{
		//默认属性
		if (0 == CreateDirectoryW(folder_path, NULL))
		{
			DWORD dErr = GetLastError();
			if (dErr == ERROR_ALREADY_EXISTS)
				_stprintf_s(errMsg, sizeof(errMsg) / sizeof(TCHAR), _T("目录[%s]已经存在"), folder_path);
			else if (dErr == ERROR_PATH_NOT_FOUND)
				_stprintf_s(errMsg, sizeof(errMsg) / sizeof(TCHAR), _T("目录[%s]的上级目录不存在"), folder_path);
			else
				_stprintf_s(errMsg, sizeof(errMsg) / sizeof(TCHAR), _T("出现未知错误"));
			err = errMsg;

			return false;
		}
	}
	else
	{
		/*
		//if (_tcslen(folder_path) == 2)//已经到盘符了，检查盘符是否存在
		//{
		//	bool bRet = false;	//盘符是否匹配

		//	//获取系统当前可用的盘符并和当前字符逐个比较
		//	TCHAR diskBuf[100] = { 0x00 };
		//	DWORD len = GetLogicalDriveStrings(sizeof(diskBuf) / sizeof(TCHAR), diskBuf);
		//	for (TCHAR* s = diskBuf; *s; s += _tcslen(s) + 1)
		//	{
		//		if(0 == _tcscmp(s,folder_path))
		//		{
		//			bRet = true;
		//			break;
		//		}
		//	}

		//	if (!bRet)
		//	{
		//		TCHAR errChar[60] = { 0x00 };
		//		_stprintf_s(errChar,60, L"盘符'%s'不存在", folder_path);
		//		err = errChar;
		//		bRet = false;
		//	}
		//}
		*/
		//验证上一级目录是否存在
		TCHAR curPath[MAX_PATH] = { 0x00 };
		int len = _tcslen(folder_path);
		for (int i = len - 1; i >= 0; i--)
		{
			if (folder_path[i] == L'\\' && i < len - 1)
			{
				_tcsncpy_s(curPath, sizeof(curPath) / sizeof(TCHAR), folder_path, i);
				break;
			}
		}
		if (CreateFolder(curPath, err, is_loop))
		{
			//验证本级目录是否存在
			if (0 == CreateDirectory(folder_path, NULL))
			{
				DWORD dErr = GetLastError();
				if (dErr == ERROR_ALREADY_EXISTS)
					_stprintf_s(errMsg, sizeof(errMsg) / sizeof(TCHAR), _T("目录[%s]已经存在"), folder_path);
				else if (dErr == ERROR_PATH_NOT_FOUND)
					_stprintf_s(errMsg, sizeof(errMsg) / sizeof(TCHAR), _T("目录[%s]的上级目录不存在"), folder_path);
				else
					_stprintf_s(errMsg, sizeof(errMsg) / sizeof(TCHAR), _T("出现未知错误"));
				err = errMsg;

				return false;
			}
		}
		else
			return false;
	}

	return true;
}

//测试给定路径是否存在（给定路径可以是文件，也可以是目录），true存在，false不存在
bool WinSupport::IsFileExist(LPCTSTR cstr_file_path)
{
	return PathFileExists(cstr_file_path);
}

//获取的一个目录内的全部文件（不包括子文件夹以及子文件夹内的内容）
bool WinSupport::GetFileList(LPCTSTR folder_path, vector<UAString> &vec_file_name, UAString &err)
{
	//判断给定的路径是否为目录
	if (!CheckFolder(folder_path, err))
		return false;

	//判断src_folder_path是否为空
	if (PathIsDirectoryEmpty(folder_path))
		return true;

	//查找目录下的所有文件并删除
	TCHAR tcharTemp[MAX_PATH] = { 0x00 };
	HANDLE file = INVALID_HANDLE_VALUE;							//文件句柄
	WIN32_FIND_DATA fileData;									//保存一个文件的信息
	TCHAR fullPath[MAX_PATH] = { 0x00 };
	_stprintf_s(fullPath, MAX_PATH, _T("%s\\*.*"), folder_path);

	file = FindFirstFile(fullPath, &fileData);
	if (file == INVALID_HANDLE_VALUE)
	{
		TCHAR charErr[MAX_PATH] = { 0x00 };
		_stprintf_s(charErr, MAX_PATH, _T("扫描失败，错误码:%d"), GetLastError());
		err = charErr;
		FindClose(file);
		return false;
	}

	//读取目录
	do {
		if (_tcscmp(fileData.cFileName, _T(".")) == 0
			|| _tcscmp(fileData.cFileName, _T("..")) == 0)
			continue;

		//处理非目录文件
		if (!(fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			UAString strTemp = fileData.cFileName;
			vec_file_name.push_back(strTemp);
		}

	} while (FindNextFile(file, &fileData));

	FindClose(file);

	return true;
}

//删除一个目录，包括目录下的子目录和目录本身
bool WinSupport::DeleteFolder(LPCTSTR folder_path, UAString &err)
{
	//判断给定的路径是否为目录
	if (!CheckFolder(folder_path, err))
		return false;

	TCHAR tCharTemp[MAX_PATH] = { 0x00 };
	HANDLE file = INVALID_HANDLE_VALUE;							//文件句柄
	TCHAR fullPath[MAX_PATH] = { 0x00 };
	_stprintf_s(fullPath, MAX_PATH, _T("%s\\*.*"), folder_path);

	WIN32_FIND_DATAW wfd;
	HANDLE h = FindFirstFile(folder_path, &wfd);
	if (h == INVALID_HANDLE_VALUE)
	{
		TCHAR errChar[MAX_PATH] = { 0x00 };
		_stprintf_s(errChar, MAX_PATH, _T("目录[%s]打开失败，错误代码:%d"),folder_path, GetLastError());
		err = errChar;
		return false;
	}
	do
	{
		//避开父目录和本目录
		if (_tcscmp(wfd.cFileName, _T(".")) == 0 ||
			_tcscmp(wfd.cFileName, _T("..")) == 0)
			continue;

		//碰到下一级子目录，删除
		_stprintf_s(tCharTemp, MAX_PATH, _T("%s\\%s"), folder_path, wfd.cFileName);
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			DeleteFolder(tCharTemp, err);
		else
			DeleteFile(tCharTemp);
	} while (FindNextFile(h, &wfd));
	FindClose(h);

	//删除自己
	RemoveDirectory(folder_path);
	return true;
}

//请空一个目录，包括目录下的子目录
bool WinSupport::ClearFolder(LPCTSTR folder_path, UAString &err)
{
	//判断给定的路径是否为目录
	if (!CheckFolder(folder_path, err))
		return false;

	//判断src_folder_path是否为空
	if (PathIsDirectoryEmpty(folder_path))
		return true;

	//查找目录下的所有文件并删除
	TCHAR tcharTemp[MAX_PATH] = { 0x00 };
	HANDLE file = INVALID_HANDLE_VALUE;							//文件句柄
	WIN32_FIND_DATA fileData;									//保存一个文件的信息
	TCHAR fullPath[MAX_PATH] = { 0x00 };
	_stprintf_s(fullPath, MAX_PATH, _T("%s\\*.*"), folder_path);

	file = FindFirstFile(fullPath, &fileData);
	if (file == INVALID_HANDLE_VALUE)
	{
		TCHAR charErr[MAX_PATH] = { 0x00 };
		_stprintf_s(charErr, MAX_PATH, _T("目录[%s]扫描失败，错误码:%d"),folder_path, GetLastError());
		return false;
	}

	//读取目录
	do {
		if (_tcscmp(fileData.cFileName, _T(".")) == 0
			|| _tcscmp(fileData.cFileName, _T("..")) == 0)
			continue;

		//碰到下一级子目录，删除
		_stprintf_s(tcharTemp, MAX_PATH, _T("%s\\%s"), folder_path, fileData.cFileName);
		if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			DeleteFolder(tcharTemp, err);
		else//碰到文件，删除
			DeleteFile(tcharTemp);

	} while (FindNextFile(file, &fileData));

	FindClose(file);

	return true;
}

//修改目录名称，例如将D:\\AA改为D:\\BB,则folder_path = D:\\AA,new_name=D:\\BB
bool WinSupport::ModifyFolderName(LPCTSTR folder_path, LPCTSTR new_name, UAString &err)
{
	if (0 != _trename(folder_path, new_name))
	{
		TCHAR charMsg[MAX_PATH] = { 0x00 };

		if (EACCES == errno)
			_stprintf_s(charMsg, MAX_PATH, _T("新文件/目录名称[%s]已经存在"), new_name);
		else if (ENOENT == errno)
			_stprintf_s(charMsg, MAX_PATH, _T("源文件/目录[%s]不存在"), folder_path);
		else if (EINVAL == errno)
			_stprintf_s(charMsg, MAX_PATH, _T("文件/目录名中存在非法字符"));
		else
			_stprintf_s(charMsg, MAX_PATH, _T("未知错误"));

		err = charMsg;
		return false;
	}

	return true;
}

//将一个目录内的文件全部移动到另一个目录中，目录名不变
bool WinSupport::CopyFolder(LPCTSTR src, LPCTSTR dst, UAString &err)
{
	//CopyFile(,)
	return true;
}

//将一个目录复制到另一个目录中，目录名不变，源目录删除
bool WinSupport::MoveFolder(LPCTSTR src, LPCTSTR dst, UAString &err)
{
	//CopyFile()
	//DeletFile();
	return true;
}

//将一个文件复制另一个目录中
bool WinSupport::CopyFileSingle(LPCTSTR src, LPCTSTR dst,BOOL isReplace, UAString &err)
{
	if (0 == ::CopyFile(src, dst, !isReplace))
	{
		DWORD dRet = GetLastError();
		TCHAR charErr[MAX_PATH] = { 0x00 };
		_stprintf_s(charErr, MAX_PATH, _T("复制文件出错，错误码：%d"), GetLastError());
		err = charErr;
		return false;
	}
	return true;
}

//获取当前系统的时间
string WinSupport::GetSysTimeStandard(void)
{
	time_t timep;
	time(&timep);
	char tmp[64] = { 0x00 };
	strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&timep));
	return tmp;
}

//以字符串方式返回当前系统的时间,返回格式为：yyyymmddhhmmss
string WinSupport::GetSysTime(void)
{
	time_t timep;
	time(&timep);
	char tmp[64] = { 0x00 };
	strftime(tmp, sizeof(tmp), "%Y%m%d%H%M%S", localtime(&timep));
	return tmp;
}
//字符集转换
bool WinSupport::CharToLPCTSTR(const char *pSrc, int len, TCHAR *dst, int len_dst)
{
	//计算多字节字符的大小，按字符计算。
	int lenDst = MultiByteToWideChar(CP_ACP, 0, pSrc, len, NULL, 0);
	if (len_dst < lenDst+1)
		return false;

	//多字节编码转换成宽字节编码
	MultiByteToWideChar(CP_ACP, 0, pSrc, len, dst, len_dst);

	dst[len_dst] = 0x00;  //添加字符串结尾，注意不是len+1
	return true;
}
