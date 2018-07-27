#pragma	comment(lib,"winmm")	//	�}���`���f�B�A�g���@�\���g�p���邽�߂ɕK�v
#include "MyPG.h"


LRESULT CALLBACK WndProc(HWND wnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_);
HWND MyPG_CreateWindow(HINSTANCE inst_, TCHAR cName_[], TCHAR tName_[], RECT *wSize_, int showCmd_);

//-----------------------------------------------------------------------------
//�E�B�����C��
//�@�\�T�v�F�E�B���h�E�Y�v���O�����̃��C���֐�
//-----------------------------------------------------------------------------
int __stdcall WinMain(	HINSTANCE inst_,	//
						HINSTANCE,			//
						LPSTR,				//
						int showCmd)		//
{
	ge = new MyPG::MyGameEngine( );

	MSG		msg;
	HWND	wnd;								//	�E�C���h�E�n���h��
	RECT	ws = ML::Rect(0, 0, 
		(int)(ge->screenWidth  * ge->viewScaleW),
		(int)(ge->screenHeight * ge->viewScaleH));
	wnd = MyPG_CreateWindow(inst_, ge->windowClassName, ge->windowTitle, &ws, showCmd);
	if(wnd == nullptr){ return 0;}

//	�Q�[�����̏�����
	ge->B_Initialize(wnd);

//	���b�Z�[�W���[�v
	while(1)
	{
	//	���b�Z�[�W�����Ă��邩
		if(PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE))
		{
		//	���b�Z�[�W�̗񋓂��I�����
			if(GetMessage(&msg, nullptr, 0, 0) <= 0)
			{
				break;
			}
		//	���b�Z�[�W�ɉ������������s��
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else if(GetActiveWindow() == wnd)
		{
			ge->B_Step(wnd);		//	�Q�[������
		}
	}

//	�Q�[�����̔j��
  delete ge;
	return 0;
}

//-----------------------------------------------------------------------------
//�E�B���h�E����
//�@�\�T�v�F�E�B���h�E����邽�߂̊e��ݒ���s���A�E�B���h�E�𐶐�����
//-----------------------------------------------------------------------------
HWND MyPG_CreateWindow(HINSTANCE inst_, TCHAR cName_[], TCHAR tName_[], RECT* wSize_, int showCmd_)
{
	WNDCLASSEX wcex;						//	�E�C���h�E�쐬�Ɏg�p
	HWND wnd;								//	�E�C���h�E�n���h��

//	�f�[�^������
	wcex.style			= (CS_HREDRAW | CS_VREDRAW);
	wcex.hIcon			= LoadIcon(inst_, IDI_APPLICATION);
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.hCursor		= LoadCursor(nullptr, IDC_ARROW);
	wcex.hIconSm		= LoadIcon(inst_, IDI_APPLICATION);
	wcex.hInstance		= inst_;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.lpszMenuName	= nullptr;
	wcex.lpszClassName	= cName_;
	wcex.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
//	Windows�ɓo�^
	if(!RegisterClassEx(&wcex))
	{
		MessageBox(nullptr, "�E�C���h�E�o�^�Ɏ��s", nullptr, MB_OK);
		return	nullptr;
	}
//	�E�C���h�E�쐬
	DWORD dws = ge->screenMode ? WS_POPUP : (WS_CAPTION | WS_SYSMENU);
	AdjustWindowRectEx(wSize_, dws, false, WS_EX_APPWINDOW);

	wnd = CreateWindowEx(	WS_EX_APPWINDOW, 
							cName_,
							tName_,
							dws, 0, 0,
							(wSize_->right  - wSize_->left),
							(wSize_->bottom - wSize_->top),
							nullptr, nullptr, inst_, nullptr);
	if(!wnd)
	{
		MessageBox(nullptr, "�E�C���h�E�����Ɏ��s", nullptr, MB_OK);
		return	nullptr;
	}

//	�E�C���h�E�̏�Ԃ��X�V����
	ShowWindow(wnd, showCmd_);
	UpdateWindow(wnd);

	return wnd;	//����
}

//-----------------------------------------------------------------------------
//�E�B���h�E�v���V�W��
//�@�\�T�v�F���b�Z�[�W�ɑ΂��鏈�����s��
//-----------------------------------------------------------------------------
LRESULT CALLBACK WndProc(	HWND	wnd_,		//	�E�B���h�E�n���h��
							UINT	msg_,		//	���b�Z�[�W
							WPARAM	wParam_,	//	�p�����[�^
							LPARAM	lParam_)	//	�p�����[�^
{
	LRESULT ret = (LRESULT)0;
//	Windows���瑗�M���ꂽ���b�Z�[�W�𒲂ׂ�
	switch(msg_){

	case WM_KEYDOWN:
		if(wParam_ == VK_BACK){
			static bool dm = true;
			ge->DebugMode( dm = !dm );
		}
		break;

	//	�E�C���h�E���������ꂽ
		case WM_CREATE:
			break;

	//	���̃E�C���h�E���A�N�e�B�u�ɂȂ���
		case WM_ACTIVATEAPP:
			break;

	//	�~�{�^���������ꂽ
		case WM_CLOSE:
			ge->quitRequest = true;
			break;

	//	�E�C���h�E���j�����ꂽ
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

	//	���̑��̃��b�Z�[�W��Windows�ɏ�����C����
		default:
			ret = DefWindowProc(wnd_, msg_, wParam_, lParam_);
	}
	return ret;
}

