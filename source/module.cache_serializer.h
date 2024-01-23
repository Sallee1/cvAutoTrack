#pragma once
// std
#include <fstream>
#include <map>
#include <string>
#include <vector>
// cv
#include <opencv2/opencv.hpp>
// cereal序列化类
#include <cereal/archives/binary.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>

#include "algorithms/algorithms.include.h">
namespace cache
{
    // 基础设置结构体
    struct Setting
    {
        std::string version = "1.0.0";
        float_t hessian_threshold = 1.0;
        int32_t octaves = 1;
        int32_t octave_layers = 1;
        bool extended = false;
        bool up_right = false;
        cv::Rect2i roi = cv::Rect(0, 0, 0, 0); // 坐标的总范围

        template <class Archive> void serialize(Archive& ar) { ar(version, extended, up_right, octaves, octave_layers, hessian_threshold, roi); }
    };

    // 存储地图级别的信息（提瓦特大陆，渊下宫等）
    struct MapInfo
    {
        int32_t map_id = 0;       // 地图id
        std::string name;         // 地图名称
        std::string value;        // 地图的枚举值
        float zoom;               // 地图的缩放
        cv::Rect2i roi;           // 地图的坐标范围
        cv::Point2i fixed_offset; // 地图特征点的偏移量

        template <class Archive> void serialize(Archive& ar) { ar(map_id, map_name, value, zoom, roi, fixed_offset); }
    };

    // 存储区域级别的信息（如鹤观，龙脊雪山等）
    // TODO: 需要实现
    struct AreaInfo
    {
        int32_t area_id = 0; // 区域id
        int32_t map_id = 0;  // 所在的地图id
        std::string name;    // 区域名称
        std::string value;   // 区域的枚举值
        cv::Rect2i roi;      // 区域的roi
        cv::Mat mask;        // 区域的掩膜

        template <class Archive> void serialize(Archive& ar) { ar(area_id, map_id, area_name, value, roi, mask) }
    };

    // 标签信息结构体
    struct LayerInfo
    {
        int32_t layer_id = 0; // 层级id，用于区分洞穴或特殊地区
        int32_t map_id = 0;   // 所在的地图id，用于区分不同的地图（提瓦特大陆，渊下宫等）
        std::string name;     // 层级的名称
        std::string value;    // 层级的枚举值
        float zoom = 0;       // 地图的输出缩放，0为继承MapInfo缩放

        template <class Archive> void serialize(Archive& ar) { ar(layer_id, map_id, zoom, name, value) }
    };

    // 特征点的序列化方法
    template <class Archive> void serialize(Archive& ar, features& feats)
    {
        ar(feats.keypoints, feats.descriptors);
    }

    // 缓存文件整合后的结构体
    struct Cache_File
    {
        Setting setting;
        std::map<int32_t, MapInfo> map_infos;
        std::map<int32_t, AreaInfo> area_infos;
        std::map<int32_t, LayerInfo> layer_infos;
        features all_features;
    };
} // namespace cache