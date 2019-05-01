//*****************************************************************************************
//
//��Visual C++����Ϸ�����ʼ�ϵ������Դ�� ��ʮһ ǳīDirectX�̳�֮�� Ϊ��ά������ʣ�����ӳ�似����һ��
//		 VS2010��
// 2012�� 1��20��  Create by ǳī 
//ͼ�꼰ͼƬ�زģ� ���ɽ���������ǰ���� �貨
//��������������ҵĲ��ͣ� http://blog.csdn.net/zhmxy555 
//�˿����飺����㿴����ǰ��ĺڰ�����Ҫ���ģ�������Ϊ��ı��������⡣  
//
//***************************************************************************************** 
 
 
 
 
//*****************************************************************************************
// Desc: ͷ�ļ����岿��  
//*****************************************************************************************                                                                                       
#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>
#include   <time.h> 
 
 
 
 
//*****************************************************************************************
// Desc: ���ļ����岿��  
//***************************************************************************************** 
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib, "winmm.lib ")
 
 
//*****************************************************************************************
// Desc: �궨�岿��   
//*****************************************************************************************
#define SCREEN_WIDTH	800						//Ϊ���ڿ�ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڿ��
#define SCREEN_HEIGHT	600							//Ϊ���ڸ߶ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڸ߶�
#define WINDOW_TITLE	_T("��Visual C++��Ϸ�����ʼǡ���������demo֮��ʮһ ǳīDirectX�̳�֮�� Ϊ��ά������ʣ�����ӳ�似����һ��") //Ϊ���ڱ��ⶨ��ĺ�
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }      //�Զ���һ��SAFE_RELEASE()��,������Դ���ͷ�
 
 
 
//*****************************************************************************************
// �����㻺�桢���������ͼ�Ĳ���֮һ������ƶ����ʽ
//*****************************************************************************************
//--------------------------------------------------------------------------------------
// ����������Ĳ���֮һ��������Ķ���	
//--------------------------------------------------------------------------------------
 
struct CUSTOMVERTEX 
{
	FLOAT _x, _y, _z;               // �����λ��
	FLOAT _u, _v;                   // ��������
	CUSTOMVERTEX(FLOAT x, FLOAT y, FLOAT z, FLOAT u, FLOAT v)
		: _x(x), _y(y), _z(z), _u(u), _v(v) {}
};
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_TEX1)
 
 
 
 
//*****************************************************************************************
// Desc: ȫ�ֱ�����������  
//*****************************************************************************************
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; //Direct3D�豸����
ID3DXFont*				g_pFont=NULL;    //����COM�ӿ�
float					g_FPS = 0.0f;       //һ�������͵ı���������֡����
wchar_t					g_strFPS[50];    //����֡���ʵ��ַ�����
LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;    //���㻺�����
LPDIRECT3DINDEXBUFFER9  g_pIndexBuffer  = NULL;    // �����������
LPDIRECT3DTEXTURE9      g_pTexture   = NULL;    // ����ӿڶ���
 
//*****************************************************************************************
// Desc: ȫ�ֺ����������� 
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
// Desc: WindowsӦ�ó�����ں���
//*****************************************************************************************
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nShowCmd)
{
 
	//��ʼ���һ�������Ĵ�����
	WNDCLASSEX wndClass = { 0 };				//��WINDCLASSEX������һ�������࣬����wndClassʵ������WINDCLASSEX������֮�󴰿ڵĸ����ʼ��    
	wndClass.cbSize = sizeof( WNDCLASSEX ) ;	//���ýṹ����ֽ�����С
	wndClass.style = CS_HREDRAW | CS_VREDRAW;	//���ô��ڵ���ʽ
	wndClass.lpfnWndProc = WndProc;				//����ָ�򴰿ڹ��̺�����ָ��
	wndClass.cbClsExtra		= 0;
	wndClass.cbWndExtra		= 0;
	wndClass.hInstance = hInstance;				//ָ���������ڹ��̵ĳ����ʵ�������
	wndClass.hIcon=(HICON)::LoadImage(NULL,_T("icon.ico"),IMAGE_ICON,0,0,LR_DEFAULTSIZE|LR_LOADFROMFILE); //��ȫ�ֵ�::LoadImage�����ӱ��ؼ����Զ���icoͼ��
	wndClass.hCursor = LoadCursor( NULL, IDC_ARROW );    //ָ��������Ĺ������
	wndClass.hbrBackground=(HBRUSH)GetStockObject(GRAY_BRUSH);  //ΪhbrBackground��Աָ��һ����ɫ��ˢ���
	wndClass.lpszMenuName = NULL;						//��һ���Կ���ֹ���ַ�����ָ���˵���Դ�����֡�
	wndClass.lpszClassName = _T("ForTheDreamOfGameDevelop");		//��һ���Կ���ֹ���ַ�����ָ������������֡�
 
	if( !RegisterClassEx( &wndClass ) )				//����괰�ں���Ҫ�Դ��������ע�ᣬ�������ܴ��������͵Ĵ���
		return -1;		
 
	HWND hwnd = CreateWindow( _T("ForTheDreamOfGameDevelop"),WINDOW_TITLE,			//ϲ���ּ��Ĵ������ں���CreateWindow
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, SCREEN_WIDTH,
		SCREEN_HEIGHT, NULL, NULL, hInstance, NULL );
 
 
	//Direct3D��Դ�ĳ�ʼ��������ʧ����messagebox������ʾ
	if (!(S_OK==Direct3D_Init (hwnd)))
	{
		MessageBox(hwnd, _T("Direct3D��ʼ��ʧ��~��"), _T("ǳī����Ϣ����"), 0); //ʹ��MessageBox����������һ����Ϣ���� 
	}
 
 
 
	MoveWindow(hwnd,200,50,SCREEN_WIDTH,SCREEN_HEIGHT,true);   //����������ʾʱ��λ�ã��������Ͻ�λ����Ļ���꣨200��50����
	ShowWindow( hwnd, nShowCmd );    //����Win32����ShowWindow����ʾ����
	UpdateWindow(hwnd);  //�Դ��ڽ��и��£��������������·���Ҫװ��һ��
 
	
 
	
 
	//��Ϣѭ������
	MSG msg = { 0 };  //��ʼ��msg
	while( msg.message != WM_QUIT )			//ʹ��whileѭ��
	{
		if( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )   //�鿴Ӧ�ó�����Ϣ���У�����Ϣʱ�������е���Ϣ�ɷ���ȥ��
		{
			TranslateMessage( &msg );		//���������Ϣת��Ϊ�ַ���Ϣ
			DispatchMessage( &msg );		//�ú����ַ�һ����Ϣ�����ڳ���
		}
		else
		{
			Direct3D_Render(hwnd);			//������Ⱦ���������л������Ⱦ
		}
	}
 
	UnregisterClass(_T("ForTheDreamOfGameDevelop"), wndClass.hInstance);
	return 0;  
}
 
 
 
//*****************************************************************************************
// Name: WndProc()
// Desc: �Դ�����Ϣ���д���
//*****************************************************************************************
LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )   //���ڹ��̺���WndProc
{
	switch( message )				//switch��俪ʼ
	{
	case WM_PAINT:					 // �ͻ����ػ���Ϣ
		Direct3D_Render(hwnd);          //����Direct3D_Render���������л���Ļ���
		ValidateRect(hwnd, NULL);   // ���¿ͻ�������ʾ
		break;									//������switch���
 
	case WM_KEYDOWN:                // ���̰�����Ϣ
		if (wParam == VK_ESCAPE)    // ESC��
			DestroyWindow(hwnd);    // ���ٴ���, ������һ��WM_DESTROY��Ϣ
		break;
	case WM_DESTROY:				//����������Ϣ
		Direct3D_CleanUp();     //����Direct3D_CleanUp����������COM�ӿڶ���
		PostQuitMessage( 0 );		//��ϵͳ�����и��߳�����ֹ����������ӦWM_DESTROY��Ϣ
		break;						//������switch���
 
	default:						//������case�����������ϣ���ִ�и�default���
		return DefWindowProc( hwnd, message, wParam, lParam );		//����ȱʡ�Ĵ��ڹ�����ΪӦ�ó���û�д���Ĵ�����Ϣ�ṩȱʡ�Ĵ���
	}
 
	return 0;					//�����˳�
}
 
 
//*****************************************************************************************
// Name: Direct3D_Init( )
// Desc: ��ʼ��Direct3D
// Point:��Direct3D��ʼ���Ĳ�����
//		1.��ʼ���Ĳ���֮һ������Direct3D�ӿڶ���
//		2.��ʼ���Ĳ���֮������ȡӲ���豸��Ϣ
//		3.��ʼ���Ĳ���֮�������ṹ��
//		4.��ʼ���Ĳ���֮�ģ�����Direct3D�豸�ӿ�
//*****************************************************************************************
 
HRESULT Direct3D_Init(HWND hwnd)
{
 
	//--------------------------------------------------------------------------------------
	// ��Direct3D��ʼ���Ĳ���֮һ�����ӿڡ�������Direct3D�ӿڶ���, �Ա��ø�Direct3D���󴴽�Direct3D�豸����
	//--------------------------------------------------------------------------------------
	LPDIRECT3D9  pD3D = NULL; //Direct3D�ӿڶ���Ĵ���
	if( NULL == ( pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) ) //��ʼ��Direct3D�ӿڶ��󣬲�����DirectX�汾Э��
 			return E_FAIL;
 
	//--------------------------------------------------------------------------------------
	// ��Direct3D��ʼ���Ĳ���֮��,ȡ��Ϣ������ȡӲ���豸��Ϣ
	//--------------------------------------------------------------------------------------
	D3DCAPS9 caps; int vp = 0;
	if( FAILED( pD3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps ) ) )
		{
			return E_FAIL;
		}
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;   //֧��Ӳ���������㣬���ǾͲ���Ӳ���������㣬���׵�
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING; //��֧��Ӳ���������㣬����ֻ�ò��������������
 
	//--------------------------------------------------------------------------------------
	// ��Direct3D��ʼ���Ĳ���֮���������ݡ������D3DPRESENT_PARAMETERS�ṹ��
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
	// ��Direct3D��ʼ���Ĳ���֮�ģ����豸��������Direct3D�豸�ӿ�
	//--------------------------------------------------------------------------------------
	if(FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
		hwnd, vp, &d3dpp, &g_pd3dDevice)))
		return E_FAIL;
 
 
	if(!(S_OK==Objects_Init())) return E_FAIL;
 
 
 
	SAFE_RELEASE(pD3D) //LPDIRECT3D9�ӿڶ����ʹ����ɣ����ǽ����ͷŵ�
 
	return S_OK;
}
 
 
HRESULT Objects_Init()
{
	//��������
	if(FAILED(D3DXCreateFont(g_pd3dDevice, 30, 0, 0, 1, FALSE, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("����"), &g_pFont)))
		return E_FAIL;
 
 
 
	//--------------------------------------------------------------------------------------
	// �����㻺�桢���������ͼ�Ĳ���֮�������������㻺�����������
	//--------------------------------------------------------------------------------------
		//�������㻺��
		if( FAILED( g_pd3dDevice->CreateVertexBuffer( 24*sizeof(CUSTOMVERTEX),
			0, D3DFVF_CUSTOMVERTEX,
			D3DPOOL_DEFAULT, &g_pVertexBuffer, NULL ) ) )
		{
			return E_FAIL;
		}
		// ������������
	if( FAILED( 	g_pd3dDevice->CreateIndexBuffer(36* sizeof(WORD), 0, 
		D3DFMT_INDEX16, D3DPOOL_DEFAULT, &g_pIndexBuffer, NULL)) )
		{
		return E_FAIL;
 
		}
		//--------------------------------------------------------------------------------------
		// �����㻺�桢���������ͼ�Ĳ���֮���������ʶ��㻺�����������
		//--------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------
		// ����������Ĳ���֮����������ķ���	
		//--------------------------------------------------------------------------------------
		//��䶥�㻺��
		CUSTOMVERTEX* pVertices;
		if( FAILED( g_pVertexBuffer->Lock( 0, sizeof(CUSTOMVERTEX), (void**)&pVertices, 0 ) ) )
			return E_FAIL;
 
		// ���涥������
		pVertices[0] = CUSTOMVERTEX(-10.0f,  10.0f, -10.0f, 0.0f, 0.0f);
		pVertices[1] = CUSTOMVERTEX( 10.0f,  10.0f, -10.0f, 1.0f, 0.0f);
		pVertices[2] = CUSTOMVERTEX( 10.0f, -10.0f, -10.0f, 1.0f, 1.0f);
		pVertices[3] = CUSTOMVERTEX(-10.0f, -10.0f, -10.0f, 0.0f, 1.0f);
 
		// ���涥������
		pVertices[4] = CUSTOMVERTEX( 10.0f,  10.0f, 10.0f, 0.0f, 0.0f);
		pVertices[5] = CUSTOMVERTEX(-10.0f,  10.0f, 10.0f, 1.0f, 0.0f);
		pVertices[6] = CUSTOMVERTEX(-10.0f, -10.0f, 10.0f, 1.0f, 1.0f);
		pVertices[7] = CUSTOMVERTEX( 10.0f, -10.0f, 10.0f, 0.0f, 1.0f);
 
		// ���涥������
		pVertices[8]  = CUSTOMVERTEX(-10.0f, 10.0f,  10.0f, 0.0f, 0.0f);
		pVertices[9]  = CUSTOMVERTEX( 10.0f, 10.0f,  10.0f, 1.0f, 0.0f);
		pVertices[10] = CUSTOMVERTEX( 10.0f, 10.0f, -10.0f, 1.0f, 1.0f);
		pVertices[11] = CUSTOMVERTEX(-10.0f, 10.0f, -10.0f, 0.0f, 1.0f);
 
		// ���涥������
		pVertices[12] = CUSTOMVERTEX(-10.0f, -10.0f, -10.0f, 0.0f, 0.0f);
		pVertices[13] = CUSTOMVERTEX( 10.0f, -10.0f, -10.0f, 1.0f, 0.0f);
		pVertices[14] = CUSTOMVERTEX( 10.0f, -10.0f,  10.0f, 1.0f, 1.0f);
		pVertices[15] = CUSTOMVERTEX(-10.0f, -10.0f,  10.0f, 0.0f, 1.0f);
 
		// ����涥������
		pVertices[16] = CUSTOMVERTEX(-10.0f,  10.0f,  10.0f, 0.0f, 0.0f);
		pVertices[17] = CUSTOMVERTEX(-10.0f,  10.0f, -10.0f, 1.0f, 0.0f);
		pVertices[18] = CUSTOMVERTEX(-10.0f, -10.0f, -10.0f, 1.0f, 1.0f);
		pVertices[19] = CUSTOMVERTEX(-10.0f, -10.0f,  10.0f, 0.0f, 1.0f);
 
		// �Ҳ��涥������
		pVertices[20] = CUSTOMVERTEX( 10.0f,  10.0f, -10.0f, 0.0f, 0.0f);
		pVertices[21] = CUSTOMVERTEX( 10.0f,  10.0f,  10.0f, 1.0f, 0.0f);
		pVertices[22] = CUSTOMVERTEX( 10.0f, -10.0f,  10.0f, 1.0f, 1.0f);
		pVertices[23] = CUSTOMVERTEX( 10.0f, -10.0f, -10.0f, 0.0f, 1.0f);
 
		g_pVertexBuffer->Unlock();
 
 
 
		// �����������
		WORD *pIndices = NULL;
		g_pIndexBuffer->Lock(0, 0, (void**)&pIndices, 0);
 
		// ������������
		pIndices[0] = 0; pIndices[1] = 1; pIndices[2] = 2;
		pIndices[3] = 0; pIndices[4] = 2; pIndices[5] = 3;
 
		// ������������
		pIndices[6] = 4; pIndices[7]  = 5; pIndices[8]  = 6;
		pIndices[9] = 4; pIndices[10] = 6; pIndices[11] = 7;
 
		// ������������
		pIndices[12] = 8; pIndices[13] =  9; pIndices[14] = 10;
		pIndices[15] = 8; pIndices[16] = 10; pIndices[17] = 11;
 
		// ������������
		pIndices[18] = 12; pIndices[19] = 13; pIndices[20] = 14;
		pIndices[21] = 12; pIndices[22] = 14; pIndices[23] = 15;
 
		// �������������
		pIndices[24] = 16; pIndices[25] = 17; pIndices[26] = 18;
		pIndices[27] = 16; pIndices[28] = 18; pIndices[29] = 19;
 
		// �Ҳ�����������
		pIndices[30] = 20; pIndices[31] = 21; pIndices[32] = 22;
		pIndices[33] = 20; pIndices[34] = 22; pIndices[35] = 23;
 
		g_pIndexBuffer->Unlock();
 
		//--------------------------------------------------------------------------------------
		// ����������Ĳ���֮����������Ĵ���
		//--------------------------------------------------------------------------------------
		D3DXCreateTextureFromFile(g_pd3dDevice, L"1.jpg", &g_pTexture);
 
		// ���ò���
		D3DMATERIAL9 mtrl;
		::ZeroMemory(&mtrl, sizeof(mtrl));
		mtrl.Ambient  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		mtrl.Diffuse  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		mtrl.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_pd3dDevice->SetMaterial(&mtrl);
 
		// ������Ⱦ״̬
		g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);   //������������
		g_pd3dDevice->SetRenderState(D3DRS_AMBIENT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)); //���û�����
 
 
	return S_OK;
}
 
 
//*****************************************************************************************
// Name:Matrix_Set()
// Desc: �����������
// Point:��Direct3D�Ĵ�任��
//		1.���Ĵ�任֮һ��������任���������
//		2.���Ĵ�任֮������ȡ���任���������
//		3.���Ĵ�任֮������ͶӰ�任���������
//		4.���Ĵ�任֮�ġ����ӿڱ任������
//*****************************************************************************************
VOID Matrix_Set()
{
	//--------------------------------------------------------------------------------------
	//���Ĵ�任֮һ��������任���������
	//--------------------------------------------------------------------------------------
	D3DXMATRIX matWorld, Rx, Ry, Rz;
	D3DXMatrixIdentity(&matWorld);                  // ��λ���������
	D3DXMatrixRotationX(&Rx, D3DX_PI *(::timeGetTime() / 1000.0f));    // ��X����ת
	D3DXMatrixRotationY(&Ry, D3DX_PI *( ::timeGetTime() / 1000.0f/2));    // ��Y����ת
	D3DXMatrixRotationZ(&Rz, D3DX_PI *( ::timeGetTime() / 1000.0f/3));   // ��Z����ת
	matWorld = Rx * Ry * Rz * matWorld;             // �õ����յ���Ͼ���
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);  //��������任����
 
	//--------------------------------------------------------------------------------------
	//���Ĵ�任֮������ȡ���任���������
	//--------------------------------------------------------------------------------------
	D3DXMATRIX matView; //����һ������
	D3DXVECTOR3 vEye(0.0f, 0.0f, -80.0f);  //�������λ��
	D3DXVECTOR3 vAt(0.0f, 0.0f, 0.0f); //�۲���λ��
	D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);//���ϵ�����
	D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &vUp); //�����ȡ���任����
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView); //Ӧ��ȡ���任����
 
	//--------------------------------------------------------------------------------------
	//���Ĵ�任֮������ͶӰ�任���������
	//--------------------------------------------------------------------------------------
	D3DXMATRIX matProj; //����һ������
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4.0f, 1.0f, 1.0f, 1000.0f); //����ͶӰ�任����
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);  //����ͶӰ�任����
 
	//--------------------------------------------------------------------------------------
	//���Ĵ�任֮�ġ����ӿڱ任������
	//--------------------------------------------------------------------------------------
	D3DVIEWPORT9 vp; //ʵ����һ��D3DVIEWPORT9�ṹ�壬Ȼ��������������������ֵ�Ϳ�����
	vp.X      = 0;		//��ʾ�ӿ�����ڴ��ڵ�X����
	vp.Y      = 0;		//�ӿ���ԶԴ��ڵ�Y����
	vp.Width  = SCREEN_WIDTH;	//�ӿڵĿ��
	vp.Height = SCREEN_HEIGHT; //�ӿڵĸ߶�
	vp.MinZ   = 0.0f; //�ӿ�����Ȼ����е���С���ֵ
	vp.MaxZ   = 1.0f;	//�ӿ�����Ȼ����е�������ֵ
	g_pd3dDevice->SetViewport(&vp); //�ӿڵ�����
 
}
 
 
 
//*****************************************************************************************
// Name: Direct3D_Render()
// Desc: ����ͼ�ε���Ⱦ����
// Point:��Direct3D��Ⱦ�岽����
//		1.��Ⱦ�岽��֮һ����������
//		2.��Ⱦ�岽��֮������ʼ����
//		3.��Ⱦ�岽��֮������ʽ����
//		4.��Ⱦ�岽��֮�ģ���������
//		5.��Ⱦ�岽��֮�壬��ת��ʾ
//*****************************************************************************************
 
void Direct3D_Render(HWND hwnd)
{
 
	//--------------------------------------------------------------------------------------
	// ��Direct3D��Ⱦ�岽��֮һ������������
	//--------------------------------------------------------------------------------------
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(123, 65, 255), 1.0f, 0);
 
	//����һ�����Σ����ڻ�ȡ�����ھ���
	RECT formatRect;
	GetClientRect(hwnd, &formatRect);
 
	//--------------------------------------------------------------------------------------
	// ��Direct3D��Ⱦ�岽��֮��������ʼ����
	//--------------------------------------------------------------------------------------
	g_pd3dDevice->BeginScene();                     // ��ʼ����
 
	Matrix_Set();
 
	// ��ȡ������Ϣ������������Ӧ�����ģʽ
	if (::GetAsyncKeyState(0x31) & 0x8000f)         // �����ּ�1�����£������߿����
		g_pd3dDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
	if (::GetAsyncKeyState(0x32) & 0x8000f)         // �����ּ�2�����£�����ʵ�����
		g_pd3dDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);
 
	//--------------------------------------------------------------------------------------
	// ��Direct3D��Ⱦ�岽��֮��������ʽ���ƣ����ö��㻺�����ͼ��
	//--------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------
	// �����㻺�桢���������ͼ�Ĳ���֮�ġ�������ͼ��
	//--------------------------------------------------------------------------------------
 
	g_pd3dDevice->SetStreamSource( 0, g_pVertexBuffer, 0, sizeof(CUSTOMVERTEX) );//�Ѱ����ļ�������Ϣ�Ķ��㻺�����Ⱦ��ˮ�������
	g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );//ָ������ʹ�õ������ʽ�ĺ�����
	g_pd3dDevice->SetIndices(g_pIndexBuffer);//������������
	//--------------------------------------------------------------------------------------
	// ����������Ĳ���֮�ġ������������
	//--------------------------------------------------------------------------------------
	 g_pd3dDevice->SetTexture(0, g_pTexture);  //��������
 
	g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 12);//��������������϶��㻺�����ͼ��
 
 
	//�ڴ������ϽǴ�����ʾÿ��֡��
	int charCount = swprintf_s(g_strFPS, 20, _T("FPS:%0.3f"), Get_FPS() );
	g_pFont->DrawText(NULL, g_strFPS, charCount , &formatRect, DT_TOP | DT_RIGHT, D3DCOLOR_XRGB(255,239,136));
 
	//--------------------------------------------------------------------------------------
	// ��Direct3D��Ⱦ�岽��֮�ġ�����������
	//--------------------------------------------------------------------------------------
	g_pd3dDevice->EndScene();                       // ��������
	//--------------------------------------------------------------------------------------
	// ��Direct3D��Ⱦ�岽��֮�塿����ʾ��ת
	//--------------------------------------------------------------------------------------
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);  // ��ת����ʾ
	 
}
 
 
//*****************************************************************************************
// Name��Get_FPS��������
// Desc: ���ڼ���֡����
//*****************************************************************************************
float Get_FPS()
{
 
	//�����ĸ���̬����
	static float  fps = 0; //������Ҫ�����FPSֵ
	static int    frameCount = 0;//֡��
	static float  currentTime =0.0f;//��ǰʱ��
	static float  lastTime = 0.0f;//����ʱ��
 
	frameCount++;//ÿ����һ��Get_FPS()������֡������1
	currentTime = timeGetTime()*0.001f;//��ȡϵͳʱ�䣬����timeGetTime�������ص����Ժ���Ϊ��λ��ϵͳʱ�䣬������Ҫ����0.001���õ���λΪ���ʱ��
 
	//�����ǰʱ���ȥ����ʱ�������1���ӣ��ͽ���һ��FPS�ļ���ͳ���ʱ��ĸ��£�����֡��ֵ����
	if(currentTime - lastTime > 1.0f) //��ʱ�������1����
	{
		fps = (float)frameCount /(currentTime - lastTime);//������1���ӵ�FPSֵ
		lastTime = currentTime; //����ǰʱ��currentTime��������ʱ��lastTime����Ϊ��һ��Ļ�׼ʱ��
		frameCount    = 0;//������֡��frameCountֵ����
	}
 
	return fps;
}
 
 
//*****************************************************************************************
// Name: Direct3D_CleanUp()
// Desc: ��Direct3D����Դ���������ͷ�COM�ӿڶ���
//*****************************************************************************************
void Direct3D_CleanUp()
{
	//�ͷ�COM�ӿڶ���
	SAFE_RELEASE(g_pTexture)
	SAFE_RELEASE(g_pIndexBuffer)
	SAFE_RELEASE(g_pVertexBuffer)
	SAFE_RELEASE(g_pFont)
	SAFE_RELEASE(g_pd3dDevice)	
}