#include <Windows.h>
#include <cvAutoTrack.h>
#include <fmt/format.h>
#include <fstream>
#include <iostream>
#include <vector>
#ifdef _DEBUG
    #include <filesystem>
#endif // _DEBUG

int TEST()
{
    char version_buff[256] = { 0 };
    if (GetCompileVersion(version_buff, 256))
        fmt::print("版本号       : {}\n", version_buff);
    else
        fmt::print("错误码       : {}\n", GetLastErr());

    char version_time_buff[256] = { 0 };
    if (GetCompileTime(version_time_buff, 256))
        fmt::print("编译时间     : {}\n", version_time_buff);
    else
        fmt::print("错误码       : {}\n", GetLastErr());

    std::cout << "测试完成\n";
    return 0;
}

int TEST_init_and_uninit()
{
    std::cout << "测试 init 与 uninit\n";

    InitResource();

    Sleep(1000);

    UnInitResource();

    Sleep(1000);

    InitResource();

    Sleep(1000);

    UnInitResource();

    Sleep(1000);

    InitResource();

    Sleep(1000);

    UnInitResource();

    Sleep(1000);

    std::cout << "测试完成\n";
    return 0;
}

void Run_SetDx()
{
    // 设置Dx截图
    if (SetUseGraphicsCaptureMode())
        fmt::print("设置Dx截图成功\n");
    else
        fmt::print("错误码       : {}\n", GetLastErr());
}

void Run_SetBit()
{
    // 设置Bitblt截图
    if (SetUseBitbltCaptureMode())
    {
        std::cout << "设置Bitblt截图成功"
                  << "\n";
    }
    else
    {
        std::cout << "错误码       : "
                  << " \n"
                  << GetLastErr() << " "
                  << "\n";
    }
}
void Run_GetTrans()
{
    double x = 0;
    double y = 0;
    double a = 0;
    int map_id = 0;
    if (GetTransformOfMap(x, y, a, map_id))
        fmt::print("坐标和角度   : {} {} {} {}\n", x, y, a, map_id);
    else
        fmt::print("错误码       : {}\n", GetLastErr());
}

void Run_SetUseLocalPictureMode()
{
    if (SetUseLocalPictureMode())
    {
        auto imgFs = std::fstream("test_scene.png", std::ios::in | std::ios::binary);
        size_t size = imgFs.seekg(0, std::ios::end).tellg();
        char* data = new char[size];
        imgFs.seekg(0, std::ios::beg);
        imgFs.read(data, size);
        SetScreenSourceImage(data, size);
        delete[] data;
        fmt::print("设置本地图片模式成功\n");
    }
    else
        fmt::print("错误码       : {}\n", GetLastErr());
}

void Run_GetDir()
{
    double a2 = 0;
    if (GetDirection(a2))
        fmt::print("角度         : {}\n", a2);
    else
        fmt::print("错误码       : {}\n", GetLastErr());
}
void Run_GetRot()
{
    double aa2 = 0;
    if (GetRotation(aa2))
        fmt::print("视角朝向     : {}\n", aa2);
    else
        fmt::print("错误码       : {}\n", GetLastErr());
}
void Run_GetAll()
{
    double x, y, a, r;
    int mapId, uid;
    std::string mapType;
    double time_beg = GetTickCount();
    if (GetAllInfo(x, y, mapId, a, r, uid))
    {
        double time_end = GetTickCount();
        switch (mapId)
        {
            case 0: mapType = "提瓦特大陆"; break;
            case 1: mapType = "渊下宫"; break;
            case 2: mapType = "地下矿区"; break;
        }
        std::cout << fmt::format(R"(
全部信息：
地区:{}
坐标:x = {:6.2f}; y = {:6.2f}
朝向:角色 = {:4.2f}; 相机 = {:4.2f}
UID:{:d}
UseTime:{:4.2f}ms
----------------
)",
                                 mapType, x, y, a, r, uid, time_end - time_beg);
    }
    else
    {
        fmt::print("错误码       : {}\n", GetLastErr());
    }
}

void Run_GetUID()
{
    int uid = 0;
    if (GetUID(uid))
        fmt::print("UID          : {}\n", uid);

    else
        fmt::print("错误码       : {}\n", GetLastErr());
}
void Run_GetStars()
{
	fmt::print("GetStar deleted!\n");
}
void Run_Capture()
{
    // 设置Dx截图
    if (DebugCapture())
        fmt::print("截图成功\n");
    else
        fmt::print("错误码       : {}\n", GetLastErr());
}

void Run_GetPosit()
{
    int mapid = 0;
    double x2 = 0;
    double y2 = 0;
    if (GetPositionOfMap(x2, y2, mapid))
        fmt::print("坐标         : {} {} {}\n", x2, y2, mapid);
    else
        fmt::print("错误码       : {}\n", GetLastErr());
}

void Run_GetVersion()
{
    char* ver = new char[100];
    GetCompileVersion(ver, 100);
    std::cout << ver << std::endl;
    delete[] ver;
}

int RUN(bool is_off_capture = false, bool is_only_capture = false, int frame_rate = 0)
{
    return 0;
}
int Run()
{
    std::ios::sync_with_stdio(false);
    system("chcp 65001");

#ifdef _DEBUG
    // 如果图像存在，则传入测试图

    if (std::filesystem::exists("map.jpg"))
    {
        DebugLoadMapImagePath("map.jpg");
    }
    else
    {
        std::cout << "载入测试图像\"map.jpg\"失败，将无法可视化特征点" << std::endl;
    }
#endif // _DEBUG

    // 调用循环
    while (1)
    {
        // 显示菜单
        std::cout <<
            R"(
1. 设置Dx截图
2. 设置Bitblt截图
3. 获取坐标和角度
4. 获取坐标
5. 获取角度
6. 获取视角朝向
7. 获取当前UID
8. 获取当前神瞳Json
9. 截图
10.可视化调试【Debug模式】
11. 初始化
12.设置从图片源传入
=====================
-1. 获取版本号
0. 退出
请输入选项: 
)";
        int option = 0;
        std::cin >> option;
        std::cout << "\n";
        switch (option)
        {
            case 11:
                InitResource();
                system("pause");
                break;
            case 1:
                Run_SetDx();
                system("pause");
                break;
            case 2:
                Run_SetBit();
                system("pause");
                break;
            case 3:
                Run_GetTrans();
                system("pause");
                break;
            case 4:
                Run_GetPosit();
                system("pause");
                break;
            case 5:
                Run_GetDir();
                system("pause");
                break;
            case 6:
                Run_GetRot();
                system("pause");
                break;
            case 7:
                Run_GetUID();
                system("pause");
                break;
            case 8:
                Run_GetStars();
                system("pause");
                break;
            case 9:
                Run_Capture();
                system("pause");
                break;
            case 10:
                while (1)
                {
                    // 30ms 内检测到ECS键就退出
                    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
                    {
                        break;
                    }
                    Sleep(100);
                    Run_GetAll();
                }
                break;
            case -1:
                Run_GetVersion();
                system("pause");
                break;
            case 0: return 0; break;
            default: break;
        }
        // 推送io流缓存
        std::cout << std::flush;

        Sleep(30);
        system("cls");
    }
    return 0;
}
void HELP()
{
    /*
    -help : show help text
    -test : run test
    -capture : set capture param
        [--off] : off capture
        [--only] : set run only a capture
        [--t int] : set capture frame rate
    */
    std::cout << "-help      : show help text\n";
    std::cout << "-test      : run test\n";
    std::cout << "-capture   : set capture param\n";
    std::cout << "	[--off]  : off capture\n";
    std::cout << "	[--only] : set run only a capture\n";
    std::cout << "	[--t int] : set capture frame rate\n";
}
int main(int argc, char* argv[])
{
    std::vector<std::string> args;
    for (int i = 0; i < argc; i++)
    {
        args.push_back(argv[i]);
    }

    // 如果输入参数 -test 就执行测试
    if (argc > 1 && strcmp(argv[1], "-test") == 0)
    {
        return TEST();
    }
    else
    {
        // 否则执行正常的程序
        return Run();
    }
}

void Test_video()
{
    // 静态方法调用
    // 初始化
    // init();
    // 准备变量
    float x = 0;
    float y = 0;
    float a = 0;
    double x2 = 0;
    double y2 = 0;
    double a2 = 0;
    double aa2 = 0;
    int uid = 0;
    // 调用循环

    std::vector<std::vector<double>> his;
    char path[256] = { "C:/Users/GengG/source/repos/cvAutoTrack/cvAutoTrack/Picture/001.png" };
    char pathV[256] = { "C:/Users/GengG/source/repos/cvAutoTrack/cvAutoTrack/Video/000.mp4" };

    char pathTxt[256] = { "C:/Users/GengG/source/repos/cvAutoTrack/cvAutoTrack/Video/000.json" };

    // char pathTxt[256] = { "C:/Users/GengG/source/repos/cvAutoTrack/cvAutoTrack/Video/000.txt" };
    /*GetInfoLoadVideo(pathV, pathTxt);
    std::cout << "错误码       : " << " " << GetLastErr() << " " << "\n";*/

    if (InitResource())
    {
        //	Sleep(2000);
    }
    // uninit();
    // Sleep(1000);

	FILE *fptr = NULL;
	fopen_s(&fptr, "./Output.txt", "w+");
    fmt::print("GetInfoLoadPicture deleted!");
    fmt::print("GetInfoLoadPicture deleted!");
    fmt::print("GetStarJson deleted!");
}
