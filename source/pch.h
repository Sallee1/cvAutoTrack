// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#ifndef PCH_H
    #define PCH_H

    // Opencv
    // 如果使用了c++14以上版本，关闭以下警告
    #ifdef _HAS_CXX17
        #pragma warning(disable : 4828)
        #pragma warning(disable : 5054)
        #pragma warning(disable : 6201)
        #pragma warning(disable : 6294)
        #pragma warning(disable : 26451)
        #pragma warning(disable : 26495)
    #endif

    #ifdef SUPPORT_WINRT
        #define WIN32_LEAN_AND_MEAN // 从 Windows 头文件中排除极少使用的内容
        // Windows 头文件
        #include <Unknwn.h>
        #include <inspectable.h>
        #include <windows.h>

        // WinRT
        #include <winrt/Windows.Foundation.h>
        #include <winrt/Windows.Graphics.DirectX.Direct3d11.h>
        #include <winrt/Windows.Graphics.DirectX.h>
        #include <winrt/windows.foundation.metadata.h>

        #include <dwmapi.h>
        #pragma comment(lib, "dwmapi.lib")
    #endif // SUPPORT_WINRT

    // STL
    #include <algorithm>
    #include <atomic>
    #include <chrono>
    #include <filesystem>
    #include <fstream>
    #include <functional>
    #include <future>
    #include <memory>
    #include <mutex>
    #include <numeric>
    #include <random>
    #include <string>

    #if (_MSC_VER && _MSVC_LANG <= 201703L) || (!_MSC_VER && __cplusplus <= 201703L)
        #include <fmt/format.h>
    #else
        #include <format>
    #endif

namespace global
{
    #if (_MSC_VER && _MSVC_LANG <= 201703L) || (!_MSC_VER && __cplusplus <= 201703L)
        #define fmt_namespace_ fmt
    #else
        #define fmt_namespace_ std
    #endif

    template <typename... Args> std::string format(fmt_namespace_::format_string<Args...> fmt, Args&&... args)
    {
        return fmt_namespace_::format(fmt, std::forward<Args>(args)...);
    }
} // namespace global

    // opencv
    #include <opencv2/opencv.hpp>
    #include <opencv2/xfeatures2d.hpp>

    #ifdef SUPPORT_WINDUMP
  // DUMP部分
        #include "DbgHelp.h"
        #include "tchar.h"
inline int GenerateMiniDump(PEXCEPTION_POINTERS pExceptionPointers)
{
    // 定义函数指针
    typedef BOOL(WINAPI * MiniDumpWriteDumpT)(HANDLE, DWORD, HANDLE, MINIDUMP_TYPE, PMINIDUMP_EXCEPTION_INFORMATION, PMINIDUMP_USER_STREAM_INFORMATION, PMINIDUMP_CALLBACK_INFORMATION);
    // 从 "DbgHelp.dll" 库中获取 "MiniDumpWriteDump" 函数
    MiniDumpWriteDumpT pfnMiniDumpWriteDump = NULL;
    HMODULE hDbgHelp = LoadLibrary(_T("DbgHelp.dll"));
    if (NULL == hDbgHelp)
    {
        return EXCEPTION_CONTINUE_EXECUTION;
    }
    pfnMiniDumpWriteDump = (MiniDumpWriteDumpT)GetProcAddress(hDbgHelp, "MiniDumpWriteDump");

    if (NULL == pfnMiniDumpWriteDump)
    {
        FreeLibrary(hDbgHelp);
        return EXCEPTION_CONTINUE_EXECUTION;
    }
    // 创建 dmp 文件件
    TCHAR szFileName[MAX_PATH] = { 0 };
    TCHAR szVersion[] = TEXT("cvAutoTrack");
    SYSTEMTIME stLocalTime;
    GetLocalTime(&stLocalTime);
    wsprintfW(szFileName, L"%s-%04d%02d%02d-%02d%02d%02d.dmp", szVersion, stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay, stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond);
    HANDLE hDumpFile = CreateFile(szFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);
    if (INVALID_HANDLE_VALUE == hDumpFile)
    {
        FreeLibrary(hDbgHelp);
        return EXCEPTION_CONTINUE_EXECUTION;
    }
    // 写入 dmp 文件
    MINIDUMP_EXCEPTION_INFORMATION expParam;
    expParam.ThreadId = GetCurrentThreadId();
    expParam.ExceptionPointers = pExceptionPointers;
    expParam.ClientPointers = FALSE;
    pfnMiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpWithDataSegs, (pExceptionPointers ? &expParam : NULL), NULL, NULL);
    // 释放文件
    CloseHandle(hDumpFile);
    FreeLibrary(hDbgHelp);
    return EXCEPTION_EXECUTE_HANDLER;
}
inline LONG WINAPI ExceptionFilter(LPEXCEPTION_POINTERS lpExceptionInfo)
{
    if (IsDebuggerPresent())
    {
        return EXCEPTION_CONTINUE_SEARCH;
    }
    return GenerateMiniDump(lpExceptionInfo);
}
        #define INSTALL_DUMP_() SetUnhandledExceptionFilter(ExceptionFilter)
    #else
        #define INSTALL_DUMP_()
    #endif // SUPPORT_WINDUMP
    #define INSTALL_DUMP(at_func) \
        INSTALL_DUMP_();          \
        return at_func
#endif // PCH_H

namespace cereal
{
    template <class Archive> inline void serialize(Archive& ar, const cv::Rect2i& rect)
    {
        ar(rect.x, rect.y, rect.width, rect.height);
    }

    template <class Archive> inline void serialize(Archive& ar, const cv::Rect2f& rect)
    {
        ar(rect.x, rect.y, rect.width, rect.height);
    }

    template <class Archive> inline void serialize(Archive& ar, const cv::Rect2d& rect)
    {
        ar(rect.x, rect.y, rect.width, rect.height);
    }

    template <class Archive> inline void serialize(Archive& ar, const cv::Point2i& pt)
    {
        ar(pt.x, pt.y);
    }

    template <class Archive> inline void serialize(Archive& ar, const cv::Point2f& pt)
    {
        ar(pt.x, pt.y);
    }

    template <class Archive> inline void serialize(Archive& ar, const cv::Point2d& pt)
    {
        ar(pt.x, pt.y);
    }

    template <class Archive> inline void serialize(Archive& ar, const cv::KeyPoint kp)
    {
        ar(kp.pt.x, kp.pt.y, kp.size, kp.angle, kp.response, kp.octave, kp.class_id);
    }

    template <class Archive> inline void save(Archive& ar, const cv::Mat& mat)
    {
        std::vector<uchar> buf;
        cv::imencode(".tiff", mat, buf);
        ar(buf);
    }

    template <class Archive> inline void load(Archive& ar, cv::Mat& mat)
    {
        std::vector<uchar> buf;
        ar(buf);
        mat = cv::imdecode(buf, cv::IMREAD_UNCHANGED);
    }

} // namespace cereal