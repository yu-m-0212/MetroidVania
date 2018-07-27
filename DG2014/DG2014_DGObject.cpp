#include "DG2014_Ver3_51.h"
//#include "atlstr.h"


//����������������������������������������������������������������������������������������������
//���摜����n�@�\�Q																		�@��
//����������������������������������������������������������������������������������������������
namespace DG_
{
	DGObject::WP DGObject::winst;	//	�P�������݂ł��Ȃ��C���X�^���X

	//------------------------------------------------------------
	DGObject::DGObject( )
	{
		this->effectState = nullptr;
		this->sprite = nullptr;
		this->effect = nullptr;
		this->renderTargetView = nullptr;
		this->depthView = nullptr;
		this->depthBuffer = nullptr;
		this->swapChain = nullptr;
		this->d3dDevice = nullptr;
	}
	//------------------------------------------------------------
	DGObject::~DGObject( )
	{
		ML::SafeDelete(&this->effectState);
		ML::SafeRelease(&this->sprite);
		ML::SafeRelease(&this->effect);
		//
		ML::SafeRelease(&this->renderTargetView);
		ML::SafeRelease(&this->depthView);
		ML::SafeRelease(&this->depthBuffer);
		ML::SafeRelease(&this->swapChain);
		ML::SafeRelease(&this->d3dDevice);
	}
	//------------------------------------------------------------
	ID3D10Device&				DGObject::Device( ){			return  *this->d3dDevice;		}	//	�f�o�C�X�I�u�W�F�N�g
	IDXGISwapChain&				DGObject::SChain( ){		return  *this->swapChain;		}	//�X���b�v�`�F�C��
	ID3D10RenderTargetView&		DGObject::RTView( ){		return  *this->renderTargetView;	}	//�����_�[�^�[�Q�b�g�r���[
	ID3D10EffectTechnique&		DGObject::ETechnique( ){	return  *this->effectTechnique;	}	
	ID3D10Effect&				DGObject::Effect( ){		return  *this->effect;			}	

	//------------------------------------------------------------
	bool DGObject::Initialize(
					HWND     hw_,
					int      w_,
					int      h_,
					DWORD    ms_, 
					bool     sm_,
					int      w2D_,
					int      h2D_)
	{
		screenWidth  =  w_;
		screenHeight  =  h_;

		// �f�o�C�X�ƃX���b�v�`�F�[���̍쐬
		DXGI_SWAP_CHAIN_DESC  sd;
		ZeroMemory( &sd, sizeof(sd) );
		sd.BufferCount = 1;
		sd.BufferDesc.Width = screenWidth;
		sd.BufferDesc.Height = screenHeight;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator	  = 60;		//���t���b�V�����[�g�i����j
		sd.BufferDesc.RefreshRate.Denominator = 1;		//���t���b�V�����[�g�i���q�j
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.OutputWindow = hw_;
		sd.SampleDesc.Count = ms_;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = !sm_;


		HRESULT hr = D3D10CreateDeviceAndSwapChain(
				NULL,	//�r�f�I�J�[�h�I�����Ȃ�
				D3D10_DRIVER_TYPE_HARDWARE,	//
				NULL,
				0,
				D3D10_SDK_VERSION,
				&sd,
				&this->swapChain,
				&this->d3dDevice);
		if(FAILED(hr)){ return false;}

		//�o�b�N�o�b�t�@�������_�����O�^�[�Q�b�g�Ƃ��邽�߂̏���
		ID3D10Texture2D* pBackBuffer;
		hr = this->swapChain->GetBuffer(0,  __uuidof(ID3D10Texture2D),  (LPVOID*)&pBackBuffer);
		if(FAILED(hr)){ return false;}

		hr = this->d3dDevice->CreateRenderTargetView(
				pBackBuffer,	//�r���[����ăA�N�Z�X���郊�\�[�X�i�o�b�N�o�b�t�@���e�N�X�`���j
				NULL,			//�r���[��`
				&this->renderTargetView);	//���������r���[
		ML::SafeRelease(&pBackBuffer);	//�o�b�N�o�b�t�@�ւ̃A�N�Z�X��������
		if(FAILED(hr)){ return false;}
		this->d3dDevice->OMSetRenderTargets(1, &this->renderTargetView, NULL);

		//�[�x�E�X�e���V���e�N�X�`���̐���
		D3D10_TEXTURE2D_DESC  depthD;
		depthD.Width = screenWidth;		depthD.Height = screenHeight;
		depthD.MipLevels = 1;	depthD.ArraySize = 1;
//		depthD.Format = DXGI_FORMAT_D16_UNORM;
		depthD.Format = DXGI_FORMAT_D32_FLOAT;
		depthD.SampleDesc.Count = ms_;
		depthD.SampleDesc.Quality = 0;
		depthD.Usage = D3D10_USAGE_DEFAULT;
		depthD.BindFlags = D3D10_BIND_DEPTH_STENCIL;
		depthD.CPUAccessFlags = 0;
		depthD.MiscFlags = 0;
		hr = this->d3dDevice->CreateTexture2D(&depthD, NULL, &this->depthBuffer);

		if(FAILED(hr)){ return false;}
		//�[�x�E�X�e���V���r���[�̐���
		D3D10_DEPTH_STENCIL_VIEW_DESC  depthViewD;
		depthViewD.Format = depthD.Format;
		depthViewD.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2DMS;
		depthViewD.Texture2D.MipSlice = 0;
		this->d3dDevice->CreateDepthStencilView(
			this->depthBuffer,
			&depthViewD,
			&this->depthView);
		if(FAILED(hr)){ return false;}

		//�r���[�̓o�^
		this->d3dDevice->OMSetRenderTargets(1,  &this->renderTargetView,  this->depthView);

		//�r���[�|�[�g�ݒ�
		D3D10_VIEWPORT	vp[1];
		vp[0].TopLeftX = 0;	vp[0].TopLeftY = 0;
		vp[0].Width = screenWidth;		vp[0].Height = screenHeight;
		vp[0].MinDepth = 0.0f;	vp[0].MaxDepth = 1.0f;
		this->d3dDevice->RSSetViewports(_countof(vp), vp);

		//�G�t�F�N�g�̐���
		ID3D10Blob*		blobErr    = NULL;
		hr = D3DX10CreateEffectFromFile(
				"data/shader/MyStdShader.fx", NULL, NULL, "fx_4_0", 
				D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_DEBUG, 0,
				this->d3dDevice, NULL, NULL, &this->effect, &blobErr, NULL );
		if(FAILED(hr)){
			if(blobErr){
				char*  err  =  (char*)(blobErr->GetBufferPointer( ));
				MessageBox(NULL,  err,  "�V�F�[�_�R���p�C���G���[", MB_OK);
			}
			return false;
		}
		//�e�N�j�b�N�𓾂�
		this->effectTechnique = this->effect->GetTechniqueByName( "MyStdShaderTec" );


		//���X�^���C�Y�ݒ�
		D3D10_RASTERIZER_DESC rdc;
		ZeroMemory(&rdc,sizeof(rdc));
		rdc.CullMode=D3D10_CULL_NONE;
		rdc.FillMode=D3D10_FILL_SOLID;
	
		ID3D10RasterizerState* pIr=NULL;
		this->d3dDevice->CreateRasterizerState(&rdc,&pIr);
		this->d3dDevice->RSSetState(pIr);


		//�X�v���C�g����
		hr =  D3DX10CreateSprite(this->d3dDevice, spriteBufferMax, &this->sprite);
		if(FAILED(hr)){ return false;}

		return true;	
	}


	//------------------------------------------------------------
	//	DG_���C�u�����𐶐�����
	DGObject::SP DGObject::Create(
							HWND        hw_,	//	�E�B���h�E�n���h��
							int         w_,		//	����
							int         h_,		//	�c��
							DWORD       ms_,	//	�}���`�T���v��
							bool        sm_,	//	�X�N���[�����[�h
							int         w2D_,
							int         h2D_)
	{
		if(winst.expired( )){
			if( DGObject::SP sp = DGObject::SP(new DGObject) ){
				if(sp->Initialize(hw_, w_, h_, ms_, sm_, w2D_, h2D_)){
					winst = sp;
					//
					sp->effectState = new DG_::EffectState( );
					return sp;
				}
			}
		}
		return 0;
	}
	//------------------------------------------------------------
	DGObject::SP DGObject::GetInst( )
	{
		if(!winst.expired( )){
			return winst.lock( );
		}
		return 0;
	}
	//------------------------------------------------------------
	//	��ʕ`��i�J�n�j
	BOOL DGObject::Begin(const ML::Color&  c_)
	{
		//�[�x�o�b�t�@��L����
		this->d3dDevice->OMSetRenderTargets(1, &this->renderTargetView, this->depthView);
		Device( ).ClearRenderTargetView(this->renderTargetView, c_.Ptr( ));
	//	�[�x�o�b�t�@�̃N���A
		Device( ).ClearDepthStencilView(this->depthView, D3D10_CLEAR_DEPTH, 1.0f, 0);
		return true;
	}
	//------------------------------------------------------------
	//	3D�`��̋��𓾂�
	void DGObject::Begin3D( )
	{
		//�[�x�o�b�t�@��L����
		this->d3dDevice->OMSetRenderTargets(1, &this->renderTargetView, this->depthView);
		this->effectState->RS_Def3D( );
		this->effectState->DSS_Def3D( );
	}
	//------------------------------------------------------------
	//	3D�`����I������
	void  DGObject::End3D( )
	{
	}
	//------------------------------------------------------------
	//	2D�`��̋��𓾂�
	void  DGObject::Begin2D( )
	{
		Font::dt = false;
		this->effectState->RS_Def2D( );
		this->effectState->DSS_Def2D( );
		//spriteBuffer.clear( );
		//spriteBuffer.reserve(spriteBufferMax);
		//�[�x�o�b�t�@�𖳌���
		this->d3dDevice->OMSetRenderTargets(1, &this->renderTargetView, NULL);
//		this->sprite->Begin(D3DX10_SPRITE_SORT_DEPTH_BACK_TO_FRONT);
//		this->sprite->Begin(D3DX10_SPRITE_SORT_DEPTH_FRONT_TO_BACK);
		this->sprite->Begin(0);
		

		ML::Mat4x4  p, v, s, t;
		s.Scaling( ML::Vec3(2.0f/screenWidth, 2.0f/screenHeight, 1));
		t.Translation( ML::Vec3(-(screenWidth/2.0f), screenHeight/2.0f, 0));
		p = t * s;
		//p.Identity( );
		v.Identity( );
		this->sprite->SetProjectionTransform(&p);
		this->sprite->SetViewTransform(&v);
	}
	//------------------------------------------------------------
	//	2D�`��̂��I������
	void  DGObject::End2D( )
	{
		Flush2D( );
		this->sprite->End( );
	}
	//	2D�`��A�o�^�ς݂̉摜����������
	void DGObject::Flush2D( )
	{
		if(Image::dt == true){
			Image::dt = false;
			//�t�H���g�`�惂�[�h�ł������ꍇ�A�X�v���C�g�\�����[�h�ɍĐݒ�
			if( Font::dt == true){
				Font::dt = false;
				this->effectState->RS_Def2D( );
				this->effectState->DSS_Def2D( );
				ML::Mat4x4  p, v, s, t;
				s.Scaling( ML::Vec3(2.0f/960.0f, 2.0f/544.0f, 1));
				t.Translation( ML::Vec3(-480, +272, 0));
				p = t * s;
				//p.Identity( );
				v.Identity( );
				this->sprite->SetProjectionTransform(&p);
				this->sprite->SetViewTransform(&v);
			}
			//���݃o�b�t�@�ɓo�^����Ă���f�[�^��S�ĕ`�悷��
			this->sprite->Flush( );
			//this->sprite->DrawSpritesImmediate(&spriteBuffer.front( ),  s,  0, 0);
			//spriteBuffer.clear( );
			//spriteBuffer.reserve(spriteBufferMax);
		}
	}
	//------------------------------------------------------------
	//	��ʕ`��i�I���j
	void  DGObject::End( )
	{
		this->swapChain->Present(1, 0);
	}
	//------------------------------------------------------------
	//2D�`��f�[�^���o�b�t�@�֓o�^
	void  DGObject::PushBack_D3DX10_SPRITE(D3DX10_SPRITE  &s_)
	{
//		spriteBuffer.push_back(s_);
		this->sprite->DrawSpritesBuffered(&s_, 1);
	}
	DWORD	DGObject::ScreenWidth( ){		return screenWidth;}
	DWORD	DGObject::ScreenHeight( ){		return screenHeight;}


	//����������������������������������������������������������������
	//�e���v���[�g����
//	template bool DGObject::VSSet_ConstBuffer<CB_SheenInfo::CBuffer>(const CB_SheenInfo::CBuffer&);
	//����������������������������������������������������������������


}












