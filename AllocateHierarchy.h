/*!
 * \file AllocateHierarchy.h
 *
 * \author puppet_master
 * \date 九月 2015
 *
 * \微软SDK自带的关于骨骼动画的类，用于骨骼动画的创建以及绘制更新
 * \注：程序不直接使用该类，而是将此类二次封装后再使用。
 */
 
#ifndef __ALLOCATEHIERARCHY_H_
#define __ALLOCATEHIERARCHY_H_
#include <d3d9.h>
#include <d3dx9.h>
#include "D3DUtil.h"
 
//--------------------------------------------------------------------------------------
// Name: struct D3DXFRAME_DERIVED
// Desc: 
//--------------------------------------------------------------------------------------
struct D3DXFRAME_DERIVED : public D3DXFRAME
{
	D3DXMATRIXA16 CombinedTransformationMatrix;
};
 

 
//--------------------------------------------------------------------------------------
// Name: struct D3DXMESHCONTAINER_DERIVED
// Desc: Structure derived from D3DXMESHCONTAINER so we can add some app-specific
//       info that will be stored with each mesh
//--------------------------------------------------------------------------------------
struct D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER
{
	//纹理信息
	LPDIRECT3DTEXTURE9* ppTextures;       //纹理数组  
 
	//网格信息             
	LPD3DXMESH pOrigMesh;					//原始网格
	LPD3DXATTRIBUTERANGE pAttributeTable;	//属性表
	DWORD NumAttributeGroups;				//属性组数量（子网格数量）
	DWORD NumInfl;							//每个顶点最多受几个骨骼影响
	LPD3DXBUFFER pBoneCombinationBuf;		//骨骼结合缓存
	D3DXMATRIX** ppBoneMatrixPtrs;			//骨骼组合变换矩阵
	D3DXMATRIX* pBoneOffsetMatrices;		//骨骼初始变换矩阵
	DWORD NumPaletteEntries;				//骨骼数量上限
	bool UseSoftwareVP;						//是否使用软件顶点处理
	DWORD iAttributeSW;						// used to denote the split between SW and HW if necessary for non-indexed skinning
};
 
//--------------------------------------------------------------------------------------
// Name: class CAllocateHierarchy
// Desc: Custom version of ID3DXAllocateHierarchy with custom methods to create
//       frames and meshcontainers.
//用来从.X文件中加载网格以及动画数据
//--------------------------------------------------------------------------------------
class CAllocateHierarchy : public ID3DXAllocateHierarchy
{
private:
	HRESULT AllocateName( LPCSTR Name, LPSTR* pNewName );
	HRESULT GenerateSkinnedMesh( IDirect3DDevice9* pd3dDevice, D3DXMESHCONTAINER_DERIVED* pMeshContainer );
public:
	STDMETHOD( CreateFrame )( THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame );
	STDMETHOD( CreateMeshContainer )( THIS_
		LPCSTR Name,
		CONST D3DXMESHDATA *pMeshData,
		CONST D3DXMATERIAL *pMaterials,
		CONST D3DXEFFECTINSTANCE *pEffectInstances,
		DWORD NumMaterials,
		CONST DWORD *pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER *ppNewMeshContainer );
	STDMETHOD( DestroyFrame )( THIS_ LPD3DXFRAME pFrameToFree );
	STDMETHOD( DestroyMeshContainer )( THIS_ LPD3DXMESHCONTAINER pMeshContainerBase );
 
	CAllocateHierarchy()
	{
	}
};
 
#endif
