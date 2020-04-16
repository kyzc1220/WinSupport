#include "stdafx.h"
#include "WinSupport.h"

using namespace WinSupport;

//��������·���Ƿ���ڣ������ж��ļ����ԣ�
//�����ڣ�����true�����򷵻�false
bool WinSupport::IsExist(const string &_path)
{
	int mode = 0;//0:�Ƿ���ڣ�2���Ƿ��д��4���Ƿ�ɶ���6������дȨ��
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

//������·���Ƿ��ж�Ȩ��
//���ɶ�������true�����򷵻�false
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

//������·���Ƿ���дȨ��
//����д������true�����򷵻�false
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

//����һ����Ŀ¼
bool WinSupport::CreateFolder(const string &folder_path, string &err)
{
	if (-1 == _mkdir(folder_path.c_str()))
	{
		if (EEXIST == errno)
			err = "[" + folder_path + "]���Ѿ�����";
		else if (ENOENT == errno)
			err = "·��[" + folder_path + "]������";
		else
			err = "δ֪����";

		return false;
	}

	return true;
}

//�ж�һ��Ŀ¼�Ƿ����
bool WinSupport::CheckFolder(LPCTSTR folder_path, UAString &err)
{
	DWORD   dwAttr = GetFileAttributes(folder_path);
	if (dwAttr == INVALID_FILE_ATTRIBUTES)
	{
		TCHAR charErr[256] = { 0x00 };
		_stprintf_s(charErr, sizeof(charErr)/sizeof(TCHAR), _T("·��[%s]������"),folder_path);
		err = folder_path;
		return false;
	}
	else
	{
		if (dwAttr & FILE_ATTRIBUTE_DIRECTORY)
			return true;
		else
			err = _T("ָ��·������Ŀ¼");
	}

	return false;
}

//����һ��Ŀ¼����Ŀ¼�Ѿ����ڣ��򲻴���
//is_loop:���ϼ�Ŀ¼�����ڣ��Ƿ�ѭ������
bool WinSupport::CreateFolder(LPCTSTR folder_path, UAString &err, bool is_loop)
{
	//���Ŀ¼�Ƿ���ڣ����Ѿ����ڣ��򷵻�
	if (CheckFolder(folder_path, err))
		return true;

	//����Ŀ¼
	TCHAR errMsg[256] = { 0x00 };
	if (!is_loop)//��ѭ������
	{
		//Ĭ������
		if (0 == CreateDirectoryW(folder_path, NULL))
		{
			DWORD dErr = GetLastError();
			if (dErr == ERROR_ALREADY_EXISTS)
				_stprintf_s(errMsg, sizeof(errMsg) / sizeof(TCHAR), _T("Ŀ¼[%s]�Ѿ�����"), folder_path);
			else if (dErr == ERROR_PATH_NOT_FOUND)
				_stprintf_s(errMsg, sizeof(errMsg) / sizeof(TCHAR), _T("Ŀ¼[%s]���ϼ�Ŀ¼������"), folder_path);
			else
				_stprintf_s(errMsg, sizeof(errMsg) / sizeof(TCHAR), _T("����δ֪����"));
			err = errMsg;

			return false;
		}
	}
	else
	{
		/*
		//if (_tcslen(folder_path) == 2)//�Ѿ����̷��ˣ�����̷��Ƿ����
		//{
		//	bool bRet = false;	//�̷��Ƿ�ƥ��

		//	//��ȡϵͳ��ǰ���õ��̷����͵�ǰ�ַ�����Ƚ�
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
		//		_stprintf_s(errChar,60, L"�̷�'%s'������", folder_path);
		//		err = errChar;
		//		bRet = false;
		//	}
		//}
		*/
		//��֤��һ��Ŀ¼�Ƿ����
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
			//��֤����Ŀ¼�Ƿ����
			if (0 == CreateDirectory(folder_path, NULL))
			{
				DWORD dErr = GetLastError();
				if (dErr == ERROR_ALREADY_EXISTS)
					_stprintf_s(errMsg, sizeof(errMsg) / sizeof(TCHAR), _T("Ŀ¼[%s]�Ѿ�����"), folder_path);
				else if (dErr == ERROR_PATH_NOT_FOUND)
					_stprintf_s(errMsg, sizeof(errMsg) / sizeof(TCHAR), _T("Ŀ¼[%s]���ϼ�Ŀ¼������"), folder_path);
				else
					_stprintf_s(errMsg, sizeof(errMsg) / sizeof(TCHAR), _T("����δ֪����"));
				err = errMsg;

				return false;
			}
		}
		else
			return false;
	}

	return true;
}

//���Ը���·���Ƿ���ڣ�����·���������ļ���Ҳ������Ŀ¼����true���ڣ�false������
bool WinSupport::IsFileExist(LPCTSTR cstr_file_path)
{
	return PathFileExists(cstr_file_path);
}

//��ȡ��һ��Ŀ¼�ڵ�ȫ���ļ������������ļ����Լ����ļ����ڵ����ݣ�
bool WinSupport::GetFileList(LPCTSTR folder_path, vector<UAString> &vec_file_name, UAString &err)
{
	//�жϸ�����·���Ƿ�ΪĿ¼
	if (!CheckFolder(folder_path, err))
		return false;

	//�ж�src_folder_path�Ƿ�Ϊ��
	if (PathIsDirectoryEmpty(folder_path))
		return true;

	//����Ŀ¼�µ������ļ���ɾ��
	TCHAR tcharTemp[MAX_PATH] = { 0x00 };
	HANDLE file = INVALID_HANDLE_VALUE;							//�ļ����
	WIN32_FIND_DATA fileData;									//����һ���ļ�����Ϣ
	TCHAR fullPath[MAX_PATH] = { 0x00 };
	_stprintf_s(fullPath, MAX_PATH, _T("%s\\*.*"), folder_path);

	file = FindFirstFile(fullPath, &fileData);
	if (file == INVALID_HANDLE_VALUE)
	{
		TCHAR charErr[MAX_PATH] = { 0x00 };
		_stprintf_s(charErr, MAX_PATH, _T("ɨ��ʧ�ܣ�������:%d"), GetLastError());
		err = charErr;
		FindClose(file);
		return false;
	}

	//��ȡĿ¼
	do {
		if (_tcscmp(fileData.cFileName, _T(".")) == 0
			|| _tcscmp(fileData.cFileName, _T("..")) == 0)
			continue;

		//�����Ŀ¼�ļ�
		if (!(fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			UAString strTemp = fileData.cFileName;
			vec_file_name.push_back(strTemp);
		}

	} while (FindNextFile(file, &fileData));

	FindClose(file);

	return true;
}

//ɾ��һ��Ŀ¼������Ŀ¼�µ���Ŀ¼��Ŀ¼����
bool WinSupport::DeleteFolder(LPCTSTR folder_path, UAString &err)
{
	//�жϸ�����·���Ƿ�ΪĿ¼
	if (!CheckFolder(folder_path, err))
		return false;

	TCHAR tCharTemp[MAX_PATH] = { 0x00 };
	HANDLE file = INVALID_HANDLE_VALUE;							//�ļ����
	TCHAR fullPath[MAX_PATH] = { 0x00 };
	_stprintf_s(fullPath, MAX_PATH, _T("%s\\*.*"), folder_path);

	WIN32_FIND_DATAW wfd;
	HANDLE h = FindFirstFile(folder_path, &wfd);
	if (h == INVALID_HANDLE_VALUE)
	{
		TCHAR errChar[MAX_PATH] = { 0x00 };
		_stprintf_s(errChar, MAX_PATH, _T("Ŀ¼[%s]��ʧ�ܣ��������:%d"),folder_path, GetLastError());
		err = errChar;
		return false;
	}
	do
	{
		//�ܿ���Ŀ¼�ͱ�Ŀ¼
		if (_tcscmp(wfd.cFileName, _T(".")) == 0 ||
			_tcscmp(wfd.cFileName, _T("..")) == 0)
			continue;

		//������һ����Ŀ¼��ɾ��
		_stprintf_s(tCharTemp, MAX_PATH, _T("%s\\%s"), folder_path, wfd.cFileName);
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			DeleteFolder(tCharTemp, err);
		else
			DeleteFile(tCharTemp);
	} while (FindNextFile(h, &wfd));
	FindClose(h);

	//ɾ���Լ�
	RemoveDirectory(folder_path);
	return true;
}

//���һ��Ŀ¼������Ŀ¼�µ���Ŀ¼
bool WinSupport::ClearFolder(LPCTSTR folder_path, UAString &err)
{
	//�жϸ�����·���Ƿ�ΪĿ¼
	if (!CheckFolder(folder_path, err))
		return false;

	//�ж�src_folder_path�Ƿ�Ϊ��
	if (PathIsDirectoryEmpty(folder_path))
		return true;

	//����Ŀ¼�µ������ļ���ɾ��
	TCHAR tcharTemp[MAX_PATH] = { 0x00 };
	HANDLE file = INVALID_HANDLE_VALUE;							//�ļ����
	WIN32_FIND_DATA fileData;									//����һ���ļ�����Ϣ
	TCHAR fullPath[MAX_PATH] = { 0x00 };
	_stprintf_s(fullPath, MAX_PATH, _T("%s\\*.*"), folder_path);

	file = FindFirstFile(fullPath, &fileData);
	if (file == INVALID_HANDLE_VALUE)
	{
		TCHAR charErr[MAX_PATH] = { 0x00 };
		_stprintf_s(charErr, MAX_PATH, _T("Ŀ¼[%s]ɨ��ʧ�ܣ�������:%d"),folder_path, GetLastError());
		return false;
	}

	//��ȡĿ¼
	do {
		if (_tcscmp(fileData.cFileName, _T(".")) == 0
			|| _tcscmp(fileData.cFileName, _T("..")) == 0)
			continue;

		//������һ����Ŀ¼��ɾ��
		_stprintf_s(tcharTemp, MAX_PATH, _T("%s\\%s"), folder_path, fileData.cFileName);
		if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			DeleteFolder(tcharTemp, err);
		else//�����ļ���ɾ��
			DeleteFile(tcharTemp);

	} while (FindNextFile(file, &fileData));

	FindClose(file);

	return true;
}

//�޸�Ŀ¼���ƣ����罫D:\\AA��ΪD:\\BB,��folder_path = D:\\AA,new_name=D:\\BB
bool WinSupport::ModifyFolderName(LPCTSTR folder_path, LPCTSTR new_name, UAString &err)
{
	if (0 != _trename(folder_path, new_name))
	{
		TCHAR charMsg[MAX_PATH] = { 0x00 };

		if (EACCES == errno)
			_stprintf_s(charMsg, MAX_PATH, _T("���ļ�/Ŀ¼����[%s]�Ѿ�����"), new_name);
		else if (ENOENT == errno)
			_stprintf_s(charMsg, MAX_PATH, _T("Դ�ļ�/Ŀ¼[%s]������"), folder_path);
		else if (EINVAL == errno)
			_stprintf_s(charMsg, MAX_PATH, _T("�ļ�/Ŀ¼���д��ڷǷ��ַ�"));
		else
			_stprintf_s(charMsg, MAX_PATH, _T("δ֪����"));

		err = charMsg;
		return false;
	}

	return true;
}

//��һ��Ŀ¼�ڵ��ļ�ȫ���ƶ�����һ��Ŀ¼�У�Ŀ¼������
bool WinSupport::CopyFolder(LPCTSTR src, LPCTSTR dst, UAString &err)
{
	//CopyFile(,)
	return true;
}

//��һ��Ŀ¼���Ƶ���һ��Ŀ¼�У�Ŀ¼�����䣬ԴĿ¼ɾ��
bool WinSupport::MoveFolder(LPCTSTR src, LPCTSTR dst, UAString &err)
{
	//CopyFile()
	//DeletFile();
	return true;
}

//��һ���ļ�������һ��Ŀ¼��
bool WinSupport::CopyFileSingle(LPCTSTR src, LPCTSTR dst,BOOL isReplace, UAString &err)
{
	if (0 == ::CopyFile(src, dst, !isReplace))
	{
		DWORD dRet = GetLastError();
		TCHAR charErr[MAX_PATH] = { 0x00 };
		_stprintf_s(charErr, MAX_PATH, _T("�����ļ����������룺%d"), GetLastError());
		err = charErr;
		return false;
	}
	return true;
}

//��ȡ��ǰϵͳ��ʱ��
string WinSupport::GetSysTimeStandard(void)
{
	time_t timep;
	time(&timep);
	char tmp[64] = { 0x00 };
	strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&timep));
	return tmp;
}

//���ַ�����ʽ���ص�ǰϵͳ��ʱ��,���ظ�ʽΪ��yyyymmddhhmmss
string WinSupport::GetSysTime(void)
{
	time_t timep;
	time(&timep);
	char tmp[64] = { 0x00 };
	strftime(tmp, sizeof(tmp), "%Y%m%d%H%M%S", localtime(&timep));
	return tmp;
}
//�ַ���ת��
bool WinSupport::CharToLPCTSTR(const char *pSrc, int len, TCHAR *dst, int len_dst)
{
	//������ֽ��ַ��Ĵ�С�����ַ����㡣
	int lenDst = MultiByteToWideChar(CP_ACP, 0, pSrc, len, NULL, 0);
	if (len_dst < lenDst+1)
		return false;

	//���ֽڱ���ת���ɿ��ֽڱ���
	MultiByteToWideChar(CP_ACP, 0, pSrc, len, dst, len_dst);

	dst[len_dst] = 0x00;  //����ַ�����β��ע�ⲻ��len+1
	return true;
}
