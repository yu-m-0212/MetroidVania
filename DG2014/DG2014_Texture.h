#pragma warning(disable:4996)
#pragma once
#pragma warning( disable : 4005 )
#include <d3dx10.h>
#include <d3dCompiler.h>
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d10.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3dCompiler.lib")
#pragma warning( default : 4005 )

#include "../myLib.h"
#include <string>

namespace DG_
{
	//-----------------------------------------------------------------------
	//�e�N�X�`���⏕�N���X
	class Texture
	{
	//-----------------------------------------------------------------------
	public:
		typedef shared_ptr<Texture>  SP;
		typedef weak_ptr<Texture>    WP;
	//-----------------------------------------------------------------------
	private:
		Texture( );
	//-----------------------------------------------------------------------
	private:
		bool  Initialize(const  string&  fpath_);
	//-----------------------------------------------------------------------
	public:
		ID3D10ShaderResourceView*	tex_srv;//�e�N�X�`���[���\�[�X�r���[
		D3D10_TEXTURE2D_DESC		desc;
	//-----------------------------------------------------------------------
	public:
		~Texture( );
	//-----------------------------------------------------------------------
	public:
		//��������
		static SP Create(const string&  fpath_);
		//�d������摜��ʓr�ǂݍ���
		static SP CreateRep(const string&  fpath_);
	//-----------------------------------------------------------------------
	public:
		//�s�N�Z���V�F�[�_�ɓo�^����
		void  PSSet(UINT  slotNum);
		//�s�N�Z���V�F�[�_�ŗ��p����e�N�X�`���𖳌���������
		static  void  Empty_PSSet(UINT  slotNum_);	
	//-----------------------------------------------------------------------
	public:
		const  D3D10_TEXTURE2D_DESC&  Desc( );
	//-----------------------------------------------------------------------
	private:
		static unordered_map<string, WP>   rep;
	};
}
