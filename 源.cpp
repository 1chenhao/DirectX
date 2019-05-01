//*****************************************************************************************
//
//【Visual C++】游戏开发笔记系列配套源码 四十一 浅墨DirectX教程之九 为三维世界添彩：纹理映射技术（一）
//		 VS2010版
// 2012年 1月20日  Create by 浅墨 
//图标及图片素材： 《仙剑奇侠传五前传》 凌波
//更多内容请访问我的博客： http://blog.csdn.net/zhmxy555 
//此刻心情：如果你看到了前面的黑暗，不要担心，那是因为你的背后有阳光。  
//
//***************************************************************************************** 
 
 
 
 
//*****************************************************************************************
// Desc: 头文件定义部分  
//*****************************************************************************************                                                                                       
#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>
#include   <time.h> 
 
 
 
 
//*****************************************************************************************
// Desc: 库文件定义部分  
//***************************************************************************************** 
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib, "winmm.lib ")
 
 
//*****************************************************************************************
// Desc: 宏定义部分   
//*****************************************************************************************
#define SCREEN_WIDTH	800						//为窗口宽度定义的宏，以方便在此处修改窗口宽度
#define SCREEN_HEIGHT	600							//为窗口高度定义的宏，以方便在此处修改窗口高度
#define WINDOW_TITLE	_T("【Visual C++游戏开发笔记】博文配套demo之四十一 浅墨DirectX教程之九 为三维世界添彩：纹理映射技术（一）") //为窗口标题定义的宏
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }      //自定义一个SAFE_RELEASE()宏,便于资源的释放
 
 
 
//*****************************************************************************************
// 【顶点缓存、索引缓存绘图四步曲之一】：设计顶点格式
//*****************************************************************************************
//--------------------------------------------------------------------------------------
// 【纹理绘制四步曲之一】：顶点的定义	
//--------------------------------------------------------------------------------------
 
struct CUSTOMVERTEX 
{
	FLOAT _x, _y, _z;               // 顶点的位置
	FLOAT _u, _v;                   // 纹理坐标
	CUSTOMVERTEX(FLOAT x, FLOAT y, FLOAT z, FLOAT u, FLOAT v)
		: _x(x), _y(y), _z(z), _u(u), _v(v) {}
};
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_TEX1)
 
 
 
 
//*****************************************************************************************
// Desc: 全局变量声明部分  
//*****************************************************************************************
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; //Direct3D设备对象
ID3DXFont*				g_pFont=NULL;    //字体COM接口
float					g_FPS = 0.0f;       //一个浮点型的变量，代表帧速率
wchar_t					g_strFPS[50];    //包含帧速率的字符数组
LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;    //顶点缓存对象
LPDIRECT3DINDEXBUFFER9  g_pIndexBuffer  = NULL;    // 索引缓存对象
LPDIRECT3DTEXTURE9      g_pTexture   = NULL;    // 纹理接口对象
 
//*****************************************************************************************
// Desc: 全局函数声明部分 
//***************************************************************************************** 
LRESULT CALLBACK	WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
HRESULT				Direct3D_Init(HWND hwnd);
HRESULT				Objects_Init();
void				Direct3D_Render( HWND hwnd);
void				Direct3D_CleanUp( );
float				Get_FPS();
VOID				Matrix_Set();
 
 
//*****************************************************************************************
// Name: WinMain( )
// Desc: Windows应用程序入口函数
//*****************************************************************************************
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nShowCmd)
{
 
	//开始设计一个完整的窗口类
	WNDCLASSEX wndClass = { 0 };				//用WINDCLASSEX定义了一个窗口类，即用wndClass实例化了WINDCLASSEX，用于之后窗口的各项初始化    
	wndClass.cbSize = sizeof( WNDCLASSEX ) ;	//设置结构体的字节数大小
	wndClass.style = CS_HREDRAW | CS_VREDRAW;	//设置窗口的样式
	wndClass.lpfnWndProc = WndProc;				//设置指向窗口过程函数的指针
	wndClass.cbClsExtra		= 0;
	wndClass.cbWndExtra		= 0;
	wndClass.hInstance = hInstance;				//指定包含窗口过程的程序的实例句柄。
	wndClass.hIcon=(HICON)::LoadImage(NULL,_T("icon.ico"),IMAGE_ICON,0,0,LR_DEFAULTSIZE|LR_LOADFROMFILE); //从全局的::LoadImage函数从本地加载自定义ico图标
	wndClass.hCursor = LoadCursor( NULL, IDC_ARROW );    //指定窗口类的光标句柄。
	wndClass.hbrBackground=(HBRUSH)GetStockObject(GRAY_BRUSH);  //为hbrBackground成员指定一个灰色画刷句柄
	wndClass.lpszMenuName = NULL;						//用一个以空终止的字符串，指定菜单资源的名字。
	wndClass.lpszClassName = _T("ForTheDreamOfGameDevelop");		//用一个以空终止的字符串，指定窗口类的名字。
 
	if( !RegisterClassEx( &wndClass ) )				//设计完窗口后，需要对窗口类进行注册，这样才能创建该类型的窗口
		return -1;		
 
	HWND hwnd = CreateWindow( _T("ForTheDreamOfGameDevelop"),WINDOW_TITLE,			//喜闻乐见的创建窗口函数CreateWindow
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, SCREEN_WIDTH,
		SCREEN_HEIGHT, NULL, NULL, hInstance, NULL );
 
 
	//Direct3D资源的初始化，调用失败用messagebox予以显示
	if (!(S_OK==Direct3D_Init (hwnd)))
	{
		MessageBox(hwnd, _T("Direct3D初始化失败~！"), _T("浅墨的消息窗口"), 0); //使用MessageBox函数，创建一个消息窗口 
	}
 
 
 
	MoveWindow(hwnd,200,50,SCREEN_WIDTH,SCREEN_HEIGHT,true);   //调整窗口显示时的位置，窗口左上角位于屏幕坐标（200，50）处
	ShowWindow( hwnd, nShowCmd );    //调用Win32函数ShowWindow来显示窗口
	UpdateWindow(hwnd);  //对窗口进行更新，就像我们买了新房子要装修一样
 
	
 
	
 
	//消息循环过程
	MSG msg = { 0 };  //初始化msg
	while( msg.message != WM_QUIT )			//使用while循环
	{
		if( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )   //查看应用程序消息队列，有消息时将队列中的消息派发出去。
		{
			TranslateMessage( &msg );		//将虚拟键消息转换为字符消息
			DispatchMessage( &msg );		//该函数分发一个消息给窗口程序。
		}
		else
		{
			Direct3D_Render(hwnd);			//调用渲染函数，进行画面的渲染
		}
	}
 
	UnregisterClass(_T("ForTheDreamOfGameDevelop"), wndClass.hInstance);
	return 0;  
}
 
 
 
//*****************************************************************************************
// Name: WndProc()
// Desc: 对窗口消息进行处理
//*****************************************************************************************
LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )   //窗口过程函数WndProc
{
	switch( message )				//switch语句开始
	{
	case WM_PAINT:					 // 客户区重绘消息
		Direct3D_Render(hwnd);          //调用Direct3D_Render函数，进行画面的绘制
		ValidateRect(hwnd, NULL);   // 更新客户区的显示
		break;									//跳出该switch语句
 
	case WM_KEYDOWN:                // 键盘按下消息
		if (wParam == VK_ESCAPE)    // ESC键
			DestroyWindow(hwnd);    // 销毁窗口, 并发送一条WM_DESTROY消息
		break;
	case WM_DESTROY:				//窗口销毁消息
		Direct3D_CleanUp();     //调用Direct3D_CleanUp函数，清理COM接口对象
		PostQuitMessage( 0 );		//向系统表明有个线程有终止请求。用来响应WM_DESTROY消息
		break;						//跳出该switch语句
 
	default:						//若上述case条件都不符合，则执行该default语句
		return DefWindowProc( hwnd, message, wParam, lParam );		//调用缺省的窗口过程来为应用程序没有处理的窗口消息提供缺省的处理。
	}
 
	return 0;					//正常退出
}
 
 
//*****************************************************************************************
// Name: Direct3D_Init( )
// Desc: 初始化Direct3D
// Point:【Direct3D初始化四步曲】
//		1.初始化四步曲之一，创建Direct3D接口对象
//		2.初始化四步曲之二，获取硬件设备信息
//		3.初始化四步曲之三，填充结构体
//		4.初始化四步曲之四，创建Direct3D设备接口
//*****************************************************************************************
 
HRESULT Direct3D_Init(HWND hwnd)
{
 
	//--------------------------------------------------------------------------------------
	// 【Direct3D初始化四步曲之一，创接口】：创建Direct3D接口对象, 以便用该Direct3D对象创建Direct3D设备对象
	//--------------------------------------------------------------------------------------
	LPDIRECT3D9  pD3D = NULL; //Direct3D接口对象的创建
	if( NULL == ( pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) ) //初始化Direct3D接口对象，并进行DirectX版本协商
 			return E_FAIL;
 
	//--------------------------------------------------------------------------------------
	// 【Direct3D初始化四步曲之二,取信息】：获取硬件设备信息
	//--------------------------------------------------------------------------------------
	D3DCAPS9 caps; int vp = 0;
	if( FAILED( pD3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps ) ) )
		{
			return E_FAIL;
		}
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;   //支持硬件顶点运算，我们就采用硬件顶点运算，妥妥的
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING; //不支持硬件顶点运算，无奈只好采用软件顶点运算
 
	//--------------------------------------------------------------------------------------
	// 【Direct3D初始化四步曲之三，填内容】：填充D3DPRESENT_PARAMETERS结构体
	//--------------------------------------------------------------------------------------
	D3DPRESENT_PARAMETERS d3dpp; 
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth            = SCREEN_WIDTH;
	d3dpp.BackBufferHeight           = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat           = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount            = 2;
	d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality         = 0;
	d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
	d3dpp.hDeviceWindow              = hwnd;
	d3dpp.Windowed                   = true;
	d3dpp.EnableAutoDepthStencil     = true; 
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;
	d3dpp.Flags                      = 0;
	d3dpp.FullScreen_RefreshRateInHz = 0;
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;
 
	//--------------------------------------------------------------------------------------
	// 【Direct3D初始化四步曲之四，创设备】：创建Direct3D设备接口
	//--------------------------------------------------------------------------------------
	if(FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
		hwnd, vp, &d3dpp, &g_pd3dDevice)))
		return E_FAIL;
 
 
	if(!(S_OK==Objects_Init())) return E_FAIL;
 
 
 
	SAFE_RELEASE(pD3D) //LPDIRECT3D9接口对象的使命完成，我们将其释放掉
 
	return S_OK;
}
 
 
HRESULT Objects_Init()
{
	//创建字体
	if(FAILED(D3DXCreateFont(g_pd3dDevice, 30, 0, 0, 1, FALSE, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("宋体"), &g_pFont)))
		return E_FAIL;
 
 
 
	//--------------------------------------------------------------------------------------
	// 【顶点缓存、索引缓存绘图四步曲之二】：创建顶点缓存和索引缓存
	//--------------------------------------------------------------------------------------
		//创建顶点缓存
		if( FAILED( g_pd3dDevice->CreateVertexBuffer( 24*sizeof(CUSTOMVERTEX),
			0, D3DFVF_CUSTOMVERTEX,
			D3DPOOL_DEFAULT, &g_pVertexBuffer, NULL ) ) )
		{
			return E_FAIL;
		}
		// 创建索引缓存
	if( FAILED( 	g_pd3dDevice->CreateIndexBuffer(36* sizeof(WORD), 0, 
		D3DFMT_INDEX16, D3DPOOL_DEFAULT, &g_pIndexBuffer, NULL)) )
		{
		return E_FAIL;
 
		}
		//--------------------------------------------------------------------------------------
		// 【顶点缓存、索引缓存绘图四步曲之三】：访问顶点缓存和索引缓存
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		// 【纹理绘制四步曲之二】：顶点的访问	
		//--------------------------------------------------------------------------------------
		//填充顶点缓存
		CUSTOMVERTEX* pVertices;
		if( FAILED( g_pVertexBuffer->Lock( 0, sizeof(CUSTOMVERTEX), (void**)&pVertices, 0 ) ) )
			return E_FAIL;
 
		// 正面顶点数据
		pVertices[0] = CUSTOMVERTEX(-10.0f,  10.0f, -10.0f, 0.0f, 0.0f);
		pVertices[1] = CUSTOMVERTEX( 10.0f,  10.0f, -10.0f, 1.0f, 0.0f);
		pVertices[2] = CUSTOMVERTEX( 10.0f, -10.0f, -10.0f, 1.0f, 1.0f);
		pVertices[3] = CUSTOMVERTEX(-10.0f, -10.0f, -10.0f, 0.0f, 1.0f);
 
		// 背面顶点数据
		pVertices[4] = CUSTOMVERTEX( 10.0f,  10.0f, 10.0f, 0.0f, 0.0f);
		pVertices[5] = CUSTOMVERTEX(-10.0f,  10.0f, 10.0f, 1.0f, 0.0f);
		pVertices[6] = CUSTOMVERTEX(-10.0f, -10.0f, 10.0f, 1.0f, 1.0f);
		pVertices[7] = CUSTOMVERTEX( 10.0f, -10.0f, 10.0f, 0.0f, 1.0f);
 
		// 顶面顶点数据
		pVertices[8]  = CUSTOMVERTEX(-10.0f, 10.0f,  10.0f, 0.0f, 0.0f);
		pVertices[9]  = CUSTOMVERTEX( 10.0f, 10.0f,  10.0f, 1.0f, 0.0f);
		pVertices[10] = CUSTOMVERTEX( 10.0f, 10.0f, -10.0f, 1.0f, 1.0f);
		pVertices[11] = CUSTOMVERTEX(-10.0f, 10.0f, -10.0f, 0.0f, 1.0f);
 
		// 底面顶点数据
		pVertices[12] = CUSTOMVERTEX(-10.0f, -10.0f, -10.0f, 0.0f, 0.0f);
		pVertices[13] = CUSTOMVERTEX( 10.0f, -10.0f, -10.0f, 1.0f, 0.0f);
		pVertices[14] = CUSTOMVERTEX( 10.0f, -10.0f,  10.0f, 1.0f, 1.0f);
		pVertices[15] = CUSTOMVERTEX(-10.0f, -10.0f,  10.0f, 0.0f, 1.0f);
 
		// 左侧面顶点数据
		pVertices[16] = CUSTOMVERTEX(-10.0f,  10.0f,  10.0f, 0.0f, 0.0f);
		pVertices[17] = CUSTOMVERTEX(-10.0f,  10.0f, -10.0f, 1.0f, 0.0f);
		pVertices[18] = CUSTOMVERTEX(-10.0f, -10.0f, -10.0f, 1.0f, 1.0f);
		pVertices[19] = CUSTOMVERTEX(-10.0f, -10.0f,  10.0f, 0.0f, 1.0f);
 
		// 右侧面顶点数据
		pVertices[20] = CUSTOMVERTEX( 10.0f,  10.0f, -10.0f, 0.0f, 0.0f);
		pVertices[21] = CUSTOMVERTEX( 10.0f,  10.0f,  10.0f, 1.0f, 0.0f);
		pVertices[22] = CUSTOMVERTEX( 10.0f, -10.0f,  10.0f, 1.0f, 1.0f);
		pVertices[23] = CUSTOMVERTEX( 10.0f, -10.0f, -10.0f, 0.0f, 1.0f);
 
		g_pVertexBuffer->Unlock();
 
 
 
		// 填充索引数据
		WORD *pIndices = NULL;
		g_pIndexBuffer->Lock(0, 0, (void**)&pIndices, 0);
 
		// 正面索引数据
		pIndices[0] = 0; pIndices[1] = 1; pIndices[2] = 2;
		pIndices[3] = 0; pIndices[4] = 2; pIndices[5] = 3;
 
		// 背面索引数据
		pIndices[6] = 4; pIndices[7]  = 5; pIndices[8]  = 6;
		pIndices[9] = 4; pIndices[10] = 6; pIndices[11] = 7;
 
		// 顶面索引数据
		pIndices[12] = 8; pIndices[13] =  9; pIndices[14] = 10;
		pIndices[15] = 8; pIndices[16] = 10; pIndices[17] = 11;
 
		// 底面索引数据
		pIndices[18] = 12; pIndices[19] = 13; pIndices[20] = 14;
		pIndices[21] = 12; pIndices[22] = 14; pIndices[23] = 15;
 
		// 左侧面索引数据
		pIndices[24] = 16; pIndices[25] = 17; pIndices[26] = 18;
		pIndices[27] = 16; pIndices[28] = 18; pIndices[29] = 19;
 
		// 右侧面索引数据
		pIndices[30] = 20; pIndices[31] = 21; pIndices[32] = 22;
		pIndices[33] = 20; pIndices[34] = 22; pIndices[35] = 23;
 
		g_pIndexBuffer->Unlock();
 
		//--------------------------------------------------------------------------------------
		// 【纹理绘制四步曲之三】：纹理的创建
		//--------------------------------------------------------------------------------------
		D3DXCreateTextureFromFile(g_pd3dDevice, L"1.jpg", &g_pTexture);
 
		// 设置材质
		D3DMATERIAL9 mtrl;
		::ZeroMemory(&mtrl, sizeof(mtrl));
		mtrl.Ambient  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		mtrl.Diffuse  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		mtrl.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_pd3dDevice->SetMaterial(&mtrl);
 
		// 设置渲染状态
		g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);   //开启背面消隐
		g_pd3dDevice->SetRenderState(D3DRS_AMBIENT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)); //设置环境光
 
 
	return S_OK;
}
 
 
//*****************************************************************************************
// Name:Matrix_Set()
// Desc: 设置世界矩阵
// Point:【Direct3D四大变换】
//		1.【四大变换之一】：世界变换矩阵的设置
//		2.【四大变换之二】：取景变换矩阵的设置
//		3.【四大变换之三】：投影变换矩阵的设置
//		4.【四大变换之四】：视口变换的设置
//*****************************************************************************************
VOID Matrix_Set()
{
	//--------------------------------------------------------------------------------------
	//【四大变换之一】：世界变换矩阵的设置
	//--------------------------------------------------------------------------------------
	D3DXMATRIX matWorld, Rx, Ry, Rz;
	D3DXMatrixIdentity(&matWorld);                  // 单位化世界矩阵
	D3DXMatrixRotationX(&Rx, D3DX_PI *(::timeGetTime() / 1000.0f));    // 绕X轴旋转
	D3DXMatrixRotationY(&Ry, D3DX_PI *( ::timeGetTime() / 1000.0f/2));    // 绕Y轴旋转
	D3DXMatrixRotationZ(&Rz, D3DX_PI *( ::timeGetTime() / 1000.0f/3));   // 绕Z轴旋转
	matWorld = Rx * Ry * Rz * matWorld;             // 得到最终的组合矩阵
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);  //设置世界变换矩阵
 
	//--------------------------------------------------------------------------------------
	//【四大变换之二】：取景变换矩阵的设置
	//--------------------------------------------------------------------------------------
	D3DXMATRIX matView; //定义一个矩阵
	D3DXVECTOR3 vEye(0.0f, 0.0f, -80.0f);  //摄像机的位置
	D3DXVECTOR3 vAt(0.0f, 0.0f, 0.0f); //观察点的位置
	D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);//向上的向量
	D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &vUp); //计算出取景变换矩阵
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView); //应用取景变换矩阵
 
	//--------------------------------------------------------------------------------------
	//【四大变换之三】：投影变换矩阵的设置
	//--------------------------------------------------------------------------------------
	D3DXMATRIX matProj; //定义一个矩阵
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4.0f, 1.0f, 1.0f, 1000.0f); //计算投影变换矩阵
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);  //设置投影变换矩阵
 
	//--------------------------------------------------------------------------------------
	//【四大变换之四】：视口变换的设置
	//--------------------------------------------------------------------------------------
	D3DVIEWPORT9 vp; //实例化一个D3DVIEWPORT9结构体，然后做填空题给各个参数赋值就可以了
	vp.X      = 0;		//表示视口相对于窗口的X坐标
	vp.Y      = 0;		//视口相对对窗口的Y坐标
	vp.Width  = SCREEN_WIDTH;	//视口的宽度
	vp.Height = SCREEN_HEIGHT; //视口的高度
	vp.MinZ   = 0.0f; //视口在深度缓存中的最小深度值
	vp.MaxZ   = 1.0f;	//视口在深度缓存中的最大深度值
	g_pd3dDevice->SetViewport(&vp); //视口的设置
 
}
 
 
 
//*****************************************************************************************
// Name: Direct3D_Render()
// Desc: 进行图形的渲染操作
// Point:【Direct3D渲染五步曲】
//		1.渲染五步曲之一，清屏操作
//		2.渲染五步曲之二，开始绘制
//		3.渲染五步曲之三，正式绘制
//		4.渲染五步曲之四，结束绘制
//		5.渲染五步曲之五，翻转显示
//*****************************************************************************************
 
void Direct3D_Render(HWND hwnd)
{
 
	//--------------------------------------------------------------------------------------
	// 【Direct3D渲染五步曲之一】：清屏操作
	//--------------------------------------------------------------------------------------
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(123, 65, 255), 1.0f, 0);
 
	//定义一个矩形，用于获取主窗口矩形
	RECT formatRect;
	GetClientRect(hwnd, &formatRect);
 
	//--------------------------------------------------------------------------------------
	// 【Direct3D渲染五步曲之二】：开始绘制
	//--------------------------------------------------------------------------------------
	g_pd3dDevice->BeginScene();                     // 开始绘制
 
	Matrix_Set();
 
	// 获取键盘消息并给予设置相应的填充模式
	if (::GetAsyncKeyState(0x31) & 0x8000f)         // 若数字键1被按下，进行线框填充
		g_pd3dDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
	if (::GetAsyncKeyState(0x32) & 0x8000f)         // 若数字键2被按下，进行实体填充
		g_pd3dDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);
 
	//--------------------------------------------------------------------------------------
	// 【Direct3D渲染五步曲之三】：正式绘制，利用顶点缓存绘制图形
	//--------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------
	// 【顶点缓存、索引缓存绘图四步曲之四】：绘制图形
	//--------------------------------------------------------------------------------------
 
	g_pd3dDevice->SetStreamSource( 0, g_pVertexBuffer, 0, sizeof(CUSTOMVERTEX) );//把包含的几何体信息的顶点缓存和渲染流水线相关联
	g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );//指定我们使用的灵活顶点格式的宏名称
	g_pd3dDevice->SetIndices(g_pIndexBuffer);//设置索引缓存
	//--------------------------------------------------------------------------------------
	// 【纹理绘制四步曲之四】：纹理的启用
	//--------------------------------------------------------------------------------------
	 g_pd3dDevice->SetTexture(0, g_pTexture);  //启用纹理
 
	g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 12);//利用索引缓存配合顶点缓存绘制图形
 
 
	//在窗口右上角处，显示每秒帧数
	int charCount = swprintf_s(g_strFPS, 20, _T("FPS:%0.3f"), Get_FPS() );
	g_pFont->DrawText(NULL, g_strFPS, charCount , &formatRect, DT_TOP | DT_RIGHT, D3DCOLOR_XRGB(255,239,136));
 
	//--------------------------------------------------------------------------------------
	// 【Direct3D渲染五步曲之四】：结束绘制
	//--------------------------------------------------------------------------------------
	g_pd3dDevice->EndScene();                       // 结束绘制
	//--------------------------------------------------------------------------------------
	// 【Direct3D渲染五步曲之五】：显示翻转
	//--------------------------------------------------------------------------------------
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);  // 翻转与显示
	 
}
 
 
//*****************************************************************************************
// Name：Get_FPS（）函数
// Desc: 用于计算帧速率
//*****************************************************************************************
float Get_FPS()
{
 
	//定义四个静态变量
	static float  fps = 0; //我们需要计算的FPS值
	static int    frameCount = 0;//帧数
	static float  currentTime =0.0f;//当前时间
	static float  lastTime = 0.0f;//持续时间
 
	frameCount++;//每调用一次Get_FPS()函数，帧数自增1
	currentTime = timeGetTime()*0.001f;//获取系统时间，其中timeGetTime函数返回的是以毫秒为单位的系统时间，所以需要乘以0.001，得到单位为秒的时间
 
	//如果当前时间减去持续时间大于了1秒钟，就进行一次FPS的计算和持续时间的更新，并将帧数值清零
	if(currentTime - lastTime > 1.0f) //将时间控制在1秒钟
	{
		fps = (float)frameCount /(currentTime - lastTime);//计算这1秒钟的FPS值
		lastTime = currentTime; //将当前时间currentTime赋给持续时间lastTime，作为下一秒的基准时间
		frameCount    = 0;//将本次帧数frameCount值清零
	}
 
	return fps;
}
 
 
//*****************************************************************************************
// Name: Direct3D_CleanUp()
// Desc: 对Direct3D的资源进行清理，释放COM接口对象
//*****************************************************************************************
void Direct3D_CleanUp()
{
	//释放COM接口对象
	SAFE_RELEASE(g_pTexture)
	SAFE_RELEASE(g_pIndexBuffer)
	SAFE_RELEASE(g_pVertexBuffer)
	SAFE_RELEASE(g_pFont)
	SAFE_RELEASE(g_pd3dDevice)	
}