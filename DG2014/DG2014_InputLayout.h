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
	//���̓��C�A�E�g�⏕�N���X
	class InputLayout
	{
	//-----------------------------------------------------------------------
	public:
		ID3D10InputLayout*		layout;//���̓��C�A�E�g
		DWORD					tecNum;//�Ώۃe�N�j�b�N�ԍ�
	//-----------------------------------------------------------------------
	public:
		typedef shared_ptr<InputLayout> SP;
	//-----------------------------------------------------------------------
	private:
		InputLayout( );
		bool Initialize(
				DWORD  tecNum_,
				D3D10_INPUT_ELEMENT_DESC  elem_[],
				UINT  nElem_);
	//-----------------------------------------------------------------------
	public:
		~InputLayout( );
	//-----------------------------------------------------------------------
	public:
		static SP Create(	
				DWORD  tecNum_,
				D3D10_INPUT_ELEMENT_DESC  elem_[],
				UINT  nElem_);
	//-----------------------------------------------------------------------
	public:
		bool IASet( );
	};
}
