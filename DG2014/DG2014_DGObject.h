#pragma warning(disable:4996)
#pragma once
#pragma warning( disable : 4005 )
#include <d3d10.h>
#include <d3dx10.h>
#include <d3dCompiler.h>
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d10.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3dCompiler.lib")
#pragma warning( default : 4005 )

#include "../myLib.h"
#include <string>

#include "DG2014_Effect.h"
#include "DG2014_VertexContainer.h"
#include "DG2014_InputLayout.h"

namespace DG_
{
	//-----------------------------------------------------------------------
	class DGObject
	{	
	//-----------------------------------------------------------------------
	private:
		ID3D10Device*					d3dDevice;			//�f�o�C�X�I�u�W�F�N�g
		IDXGISwapChain*					swapChain;			//�X���b�v�`�F�C��
		ID3D10RenderTargetView*			renderTargetView;	//�����_�[�^�[�Q�b�g�r���[
		ID3D10Effect*					effect;				//
		ID3D10EffectTechnique*			effectTechnique;	//
		ID3D10Texture2D*				depthBuffer;
		ID3D10DepthStencilView*			depthView;			//�[�x�X�e���V���r���[

		ID3DX10Sprite*					sprite;				//2D�`��X�v���C�g
//		vector<D3DX10_SPRITE>			spriteBuffer;		
		//�e��p�����[�^
		EffectState*					effectState;

		DWORD							screenWidth;
		DWORD							screenHeight;
	//-----------------------------------------------------------------------
	private:
		typedef weak_ptr<DGObject>		WP;
		static DGObject::WP				winst;//�P�������݂ł��Ȃ��C���X�^���X�̃E�B�[�N�|�C���^
	//-----------------------------------------------------------------------
	private:
		DGObject( );
		bool Initialize(HWND  hw_,  int  w_,  int  h_,  DWORD  ms_, bool  sm_,  int  w2D_,  int  h2D_);	
	//-----------------------------------------------------------------------
	public:
		EffectState&		EState( ){	return  *effectState;}
		ID3DX10Sprite&		Sprite( ){	return  *sprite;}
	//-----------------------------------------------------------------------
	public:
		typedef shared_ptr<DGObject> SP;
		~DGObject( );
	//-----------------------------------------------------------------------
	public:
		//	DG_���C�u�����𐶐�����
		static DGObject::SP Create(HWND  hw_,  int  w_,  int  h_,  DWORD  ms_,  bool  sm_,  int  w2D_,  int  h2D_);
		//�A�N�Z�X
		static DGObject::SP GetInst( );
		ID3D10Device&				Device( );		//�f�o�C�X�I�u�W�F�N�g
		IDXGISwapChain&				SChain( );		//�X���b�v�`�F�C��
		ID3D10RenderTargetView&		RTView( );		//�����_�[�^�[�Q�b�g�r���[
		ID3D10EffectTechnique&		ETechnique( );
		ID3D10Effect&				Effect( );
	//-----------------------------------------------------------------------
	public:
	//	��ʕ`��i�J�n�j
		BOOL  Begin(const  ML::Color&  c_ = ML::Color(1,0,0,0));	//	�h��Ԃ��F
	//	3D�`��̋��𓾂�
		void Begin3D( );
	//	3D�`����I������
		void End3D( );
	//	2D�`��̋��𓾂�
		void Begin2D( );
	//	2D�`��A�o�^�ς݂̉摜����������
		void Flush2D( );
	//	2D�`��̂��I������
		void End2D( );
	//	��ʕ`��i�I���j
		void  End( );
	//	�f�o�C�X
	//-----------------------------------------------------------------------
	public:
	//�p�����[�^�̍X�V��Ԃ𒲂ׂēK�X�萔�o�b�t�@�ɓo�^������
		DWORD  ScreenWidth( );
		DWORD  ScreenHeight( );
		//-----------------------------------------------------------------------
	public:
		//2D�`��f�[�^���o�b�t�@�֓o�^
		void  PushBack_D3DX10_SPRITE(D3DX10_SPRITE  &s_);
	};

}
