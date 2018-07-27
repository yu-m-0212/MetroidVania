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
	//�C���f�b�N�X�o�b�t�@�⏕�N���X
	//-----------------------------------------------------------------------
	class IndexBuffer
	{
	//-----------------------------------------------------------------------
	public:
		ID3D10Buffer*	buf;
		UINT			stride;	//�f�[�^1�����̃f�[�^��
		UINT			num;	//�f�[�^����
		void*			memory;	//�A�N�Z�X�p
	//-----------------------------------------------------------------------
	public:
		typedef shared_ptr<IndexBuffer> SP;
		void  Update( );
	//-----------------------------------------------------------------------
	private:
		IndexBuffer( );
		bool Initialize(BYTE da_[], UINT str_, UINT n_);
	//-----------------------------------------------------------------------
	public:
		~IndexBuffer( );
	//-----------------------------------------------------------------------
	public:
		static SP Create(BYTE da_[], UINT str_, UINT n_);
	};
}
