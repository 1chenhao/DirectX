/*!
 * \file D3DXAnimation.h
 *
 * \author puppet_master
 * \date 九月 2015
 *
 * \封装了微软自带的骨骼动画相关功能，提供一个简单的接口供使用
 */
 
#ifndef __D3DXANIMATION_H_
#define __D3DXANIMATION_H_
#include <d3d9.h>
#include <d3dx9.h>
#include "D3DUtil.h"
#include "AllocateHierarchy.h"
 
class CD3DXAnimation
{
private:
	IDirect3DDevice9*	m_pDevice;					//D3D设备对象
	CAllocateHierarchy* m_pAllocateHier;			//骨骼动画网格模型指针
	LPD3DXFRAME			m_pFrameRoot;				//帧
	LPD3DXANIMATIONCONTROLLER m_pAnimController;	//动画控制器
	D3DXMATRIX*			m_pBoneMatrix;				//骨骼矩阵
private:
	//一些微软自带函数，关于骨骼动画加载与绘制更新的函数，将其封装，不使用这些接口
 
	void DrawMeshContainer( IDirect3DDevice9* pd3dDevice, LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase );
	void DrawFrame( IDirect3DDevice9* pd3dDevice, LPD3DXFRAME pFrame );
	HRESULT SetupBoneMatrixPointers( LPD3DXFRAME pFrameBase, LPD3DXFRAME pFrameRoot );
	void UpdateFrameMatrices( LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix );
public:
	CD3DXAnimation(IDirect3DDevice9* device);
	~CD3DXAnimation(void);
 
	//提供给外界的接口
 
	//创建骨骼动画
	bool Init(LPCTSTR filename);
 
	//通过名字设置要播放的骨骼动画
	void SetAnimationByName(LPCTSTR name);
 
	//更新位置
	void SetMatrix(LPD3DXMATRIX mtrix);
 
	//更新动画
	void UpdateAnimation(double timeDelay);
 
	//绘制骨骼动画
	void Render();
};
 
#endif
