#include "pch.h"
#include "genshin.handle.h"
#include "genshin.include.h"

namespace TianLi::Genshin
{
	tianli::global::GenshinHandle func_get_handle(HWND &in)
	{
		static tianli::global::GenshinHandle out;
		if (in == 0)
		{
			get_genshin_handle(out);
		}
		else
		{
			update_genshin_handle(in, out);
		}
		return out;
	}

	void get_genshin_handle(tianli::global::GenshinHandle &genshin_handle)
	{
		if (genshin_handle.config.is_auto_find_genshin)
		{
			auto& giHandle = genshin_handle.handle;

			auto now_class = tianli::global::GenshinWindowClass::Unity;
			for (auto& [genshin_window_name, genshin_window_class] : genshin_handle.config.genshin_window_name_list)
			{
				if (genshin_window_class == tianli::global::GenshinWindowClass::Unity)
				{
					giHandle = FindWindowW(L"UnityWndClass", genshin_window_name.c_str());
				}
				else
				{
					giHandle = FindWindowW(nullptr, genshin_window_name.c_str());
				}
				if (giHandle)
				{
					now_class = genshin_window_class;
					break;
				}
			}

			//窗口投影（源） - 云·原神
			if (now_class == tianli::global::GenshinWindowClass::Obs || now_class == tianli::global::GenshinWindowClass::None)
			{
				genshin_handle.config.is_force_used_no_alpha = true;
			}
			else
			{
				genshin_handle.config.is_force_used_no_alpha = false;
			}
			
		}
		else
		{
			genshin_handle.handle = genshin_handle.config.genshin_handle;
		}
		if (genshin_handle.handle != 0)
		{
			genshin_handle.is_exist = true;
		}
		else
		{
			genshin_handle.is_exist = false;
			return;
		}
		// 判断窗口是否存在标题栏
		if (GetWindowLong(genshin_handle.handle, GWL_STYLE) & WS_CAPTION)
		{
			genshin_handle.is_exist_title_bar = true;
		}
		else
		{
			genshin_handle.is_exist_title_bar = false;
		}
		// 获取窗口大小
		GetWindowRect(genshin_handle.handle, &genshin_handle.rect);
		// 获取除标题栏区域大小
		GetClientRect(genshin_handle.handle, &genshin_handle.rect_client);
		// 获取缩放比例
		HMONITOR hMonitor = MonitorFromWindow(genshin_handle.handle, MONITOR_DEFAULTTONEAREST);
		UINT dpiX, dpiY;
		GetDpiForMonitor(hMonitor, MDT_EFFECTIVE_DPI, &dpiX, &dpiY);
		genshin_handle.scale = dpiX / 96.0;

		GetUiRects(genshin_handle);
	}

	// TODO: 需要将获取UI布局的方法分离出来
	void GetUiRects(tianli::global::GenshinHandle &genshin_handle)
	{
		{
			int x = genshin_handle.rect_client.right - genshin_handle.rect_client.left;
			int y = genshin_handle.rect_client.bottom - genshin_handle.rect_client.top;

			double f = 1, fx = 1, fy = 1;

			if (static_cast<double>(x) / static_cast<double>(y) == 16.0 / 9.0)
			{
				genshin_handle.size_frame = cv::Size(1920, 1080);
			}
			else if (static_cast<double>(x) / static_cast<double>(y) > 16.0 / 9.0)
			{

				//高型，以宽为比例

				// x = (y * 16) / 9;
				f = y / 1080.0;
				//将giFrame缩放到1920*1080的比例
				fx = x / f;
				// 将图片缩放
				genshin_handle.size_frame = cv::Size(static_cast<int>(fx), 1080);

			}
			else if (static_cast<double>(x) / static_cast<double>(y) < 16.0 / 9.0)
			{

				//宽型，以高为比例

				// x = (y * 16) / 9;
				f = x / 1920.0;
				//将giFrame缩放到1920*1080的比例
				fy = y / f;
				// 将图片缩放
				genshin_handle.size_frame = cv::Size(1920, static_cast<int>(fy));
			}
			x = genshin_handle.size_frame.width;
			y = genshin_handle.size_frame.height;
			// 派蒙可能性区域计算参数
			int paimon_mayArea_left = 0;
			int paimon_mayArea_top = 0;
			int paimon_mayArea_width = static_cast<int>(x * 0.10);
			int paimon_mayArea_height = static_cast<int>(y * 0.10);
			// 派蒙可能性区域
			cv::Rect Area_Paimon_mayArea(
				paimon_mayArea_left,
				paimon_mayArea_top,
				paimon_mayArea_width,
				paimon_mayArea_height);
			genshin_handle.rect_paimon_maybe = Area_Paimon_mayArea;

			// 小地图标定可能性区域计算参数
			int miniMap_Cailb_mayArea_left = static_cast<int>(x * 0.08);
			int miniMap_Cailb_mayArea_top = 0;
			int miniMap_Cailb_mayArea_width = static_cast<int>(x * 0.10);
			int miniMap_Cailb_mayArea_height = static_cast<int>(y * 0.10);
			// 小地图标定可能性区域
			cv::Rect Area_MiniMap_Cailb_mayArea(
				miniMap_Cailb_mayArea_left,
				miniMap_Cailb_mayArea_top,
				miniMap_Cailb_mayArea_width,
				miniMap_Cailb_mayArea_height);
			genshin_handle.rect_minimap_cailb_maybe = Area_MiniMap_Cailb_mayArea;

			// 小地图可能性区域计算参数
			int miniMap_mayArea_left = 0;
			int miniMap_mayArea_top = 0;
			int miniMap_mayArea_width = static_cast<int>(x * 0.18);
			int miniMap_mayArea_height = static_cast<int>(y * 0.22);
			// 小地图可能性区域
			cv::Rect Area_MiniMap_mayArea(
				miniMap_mayArea_left,
				miniMap_mayArea_top,
				miniMap_mayArea_width,
				miniMap_mayArea_height);
			genshin_handle.rect_minimap_maybe = Area_MiniMap_mayArea;

			// UID可能性区域计算参数
			int UID_mayArea_left = static_cast<int>(x * 0.88);
			int UID_mayArea_top = static_cast<int>(y * 0.97);
			int UID_mayArea_width = x - UID_mayArea_left;
			int UID_mayArea_height = y - UID_mayArea_top;
			// UID可能性区域
			cv::Rect Area_UID_mayArea(
				UID_mayArea_left,
				UID_mayArea_top,
				UID_mayArea_width,
				UID_mayArea_height);
			genshin_handle.rect_uid_maybe = Area_UID_mayArea;

			int UID_Rect_x = cvCeil(x - x * (1.0 - 0.865));
			int UID_Rect_y = cvCeil(y - 1080.0 * (1.0 - 0.9755));
			int UID_Rect_w = cvCeil(1920 * 0.11);
			int UID_Rect_h = cvCeil(1920 * 0.0938 * 0.11);
			genshin_handle.rect_uid = cv::Rect(UID_Rect_x, UID_Rect_y, UID_Rect_w, UID_Rect_h);

			// 左侧已获取物品可能性区域计算参数
			int leftGetItems_mayArea_left = static_cast<int>(x * 0.570);
			int leftGetItems_mayArea_top = static_cast<int>(y * 0.250);
			int leftGetItems_mayArea_width = static_cast<int>(x * 0.225);
			int leftGetItems_mayArea_height = static_cast<int>(y * 0.500);
			// 左侧已获取物品可能性区域
			cv::Rect Area_LeftGetItems_mayArea(
				leftGetItems_mayArea_left,
				leftGetItems_mayArea_top,
				leftGetItems_mayArea_width,
				leftGetItems_mayArea_height);
			genshin_handle.rect_left_give_items_maybe = Area_LeftGetItems_mayArea;

			// 右侧可捡取物品可能性区域计算参数
			int rightGetItems_mayArea_left = static_cast<int>(x * 0.050);
			int rightGetItems_mayArea_top = static_cast<int>(y * 0.460);
			int rightGetItems_mayArea_width = static_cast<int>(x * 0.160);
			int rightGetItems_mayArea_height = static_cast<int>(y * 0.480);
			// 右侧可捡取物品可能性区域
			cv::Rect Area_RightGetItems_mayArea(
				rightGetItems_mayArea_left,
				rightGetItems_mayArea_top,
				rightGetItems_mayArea_width,
				rightGetItems_mayArea_height);
			genshin_handle.rect_right_pick_items_maybe = Area_RightGetItems_mayArea;
		}
	}

	void update_genshin_handle(const HWND &old_handle, tianli::global::GenshinHandle &out_genshin_handle)
	{
		static unsigned char tick_count = 0;
		if (IsWindowVisible(old_handle))
		{
			if (tick_count < 30)
			{
				tick_count++;
			}
			else
			{
				tick_count = 0;
				get_genshin_handle(out_genshin_handle);
			}
		}
		else
		{
			get_genshin_handle(out_genshin_handle);
		}
		return;
	}

}