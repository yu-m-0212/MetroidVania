#include "DG2014_Ver3_51.h"

//#include "atlstr.h"


//����������������������������������������������������������������������������������������������
//���摜����n�@�\�Q																		�@��
//����������������������������������������������������������������������������������������������
namespace DG_
{

//-----------------------------------------------------------------------
//���̓��C�A�E�g�⏕�N���X
//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	InputLayout::InputLayout( )
	{
	}
	//-----------------------------------------------------------------------
	InputLayout::~InputLayout( )
	{
		ML::SafeRelease(&layout);
	}
	//-----------------------------------------------------------------------
	bool InputLayout::Initialize(
			DWORD  tecNum_,
			D3D10_INPUT_ELEMENT_DESC  elem_[],
			UINT  nElem_)
	{
		auto  dgi = DG_::DGObject::GetInst( ); if(dgi == 0){	return false; }
		D3D10_PASS_DESC  passDesc;
		this->tecNum = tecNum_;
		dgi->ETechnique( ).GetPassByIndex( this->tecNum )->GetDesc( &passDesc );

		HRESULT rtv;
		//���_�C���v�b�g���C�A�E�g���쐬
		rtv = dgi->Device( ).CreateInputLayout(
				elem_, nElem_,
				passDesc.pIAInputSignature, 
				passDesc.IAInputSignatureSize,
				&this->layout );
		if(FAILED(rtv)){	return false;	}	
		return true;
	}
	//-----------------------------------------------------------------------
	InputLayout::SP InputLayout::Create(
			DWORD  tecNum_,
			D3D10_INPUT_ELEMENT_DESC  elem_[],
			UINT  nElem_)
	{
		if( InputLayout::SP ob = InputLayout::SP(new InputLayout( )) ){
			if( ob->Initialize(tecNum_, elem_, nElem_) ){
				return ob;
			}
		}
		return nullptr;
	}
	//-----------------------------------------------------------------------
	bool InputLayout::IASet( )
	{
		auto  dgi = DG_::DGObject::GetInst( ); if(dgi == 0){	return false; }

		//���C�A�E�g�ݒ�
		dgi->Device( ).IASetInputLayout( layout );
		return true;
	}


}
