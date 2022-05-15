/*
 * @Description: 
 * @Date: 2022-05-15 20:07:45
 * @Author: chenjingyu
 * @LastEditTime: 2022-05-15 20:08:08
 * @FilePath: \Developmental-Networks\IHDR\include\IHDR.h
 */
#pragma once

#if defined (_MSC_VER) || defined (_WIN32) || defined (_WIN64)
#ifndef API_EXPORTS
#define  API __declspec(dllexport)
#else
#define  API __declspec(dllimport)
#endif // API
#else // defined (windows)
#define API
#endif

class IHDRTree {
public:
    /// <summary>
    /// 构建一颗IHDR树
    /// </summary>
    /// <param name="treeType">树的类型</param>
    /// <param name="nX">输入维度</param>
    /// <param name="nY">输出维度</param>
    /// <param name="l">每个节点的叶子数</param>
    /// <param name="fw">冻结权重</param>
    API IHDRTree(int treeType, int nX, int nY, int l, int fw);

    /// 析构函数
    API ~IHDRTree();

    /// <summary>
    /// 更新树
    /// </summary>
    /// <param name="fX">输入数据</param>
    /// <param name="fY">输入标签</param>
    API void UpdateTree(float *fX, float *fY);

    /// <summary>
    /// 检索树
    /// </summary>
    /// <param name="fX">输入数据</param>
    /// <param name="fY">输出标签</param>
    /// <param name="dis">检索距离</param>
    API void RetriveTree(float *fX, float *fY, float & dis);

    /// <summary>
    /// 保存树
    /// </summary>
    /// <param name="name">树文件保存路径</param>
    API void SaveTree(const char *name);

    /// <summary>
    /// 载入树
    /// </summary>
    /// <param name="name">树文件载入路径</param>
    API void LoadTree(const char *name);

private:
    class Impl;
    Impl* pImpl_;
};

