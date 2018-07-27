#include "DG2014_Ver3_51.h"


namespace DG_
{
	//-----------------------------------------------------------------------
	//�o�[�e�b�N�X�o�b�t�@�����N���X
	//----------------------------------------------------------------------
	VB_Container::VB_Container( )
	{
	}
	//----------------------------------------------------------------------
	VB_Container::~VB_Container( )
	{
	}	
	//----------------------------------------------------------------------
	bool VB_Container::Initialize(D3D10_PRIMITIVE_TOPOLOGY  pt_)
	{
		priTopo = pt_;
		return true;
	}
	//----------------------------------------------------------------------
	//	��������
	VB_Container::SP  VB_Container::Create(D3D10_PRIMITIVE_TOPOLOGY  pt_)
	{
		if( VB_Container::SP ob = VB_Container::SP(new VB_Container( )) ){
			if( ob->Initialize(pt_) ){
				return ob;
			}
		}
		return nullptr;
	}
	//----------------------------------------------------------------------
	bool  VB_Container::IASet(UINT ofs_)	//ofs_�̓o�b�t�@����stride�{
	{
		auto  dgi = DG_::DGObject::GetInst( );
		if(dgi == 0){	return false; }

		//�v���~�e�B�u�E�g�|���W�[���Z�b�g
		dgi->Device( ).IASetPrimitiveTopology( priTopo );
		auto  it = vbs.begin( );
		for(int c=0; it != vbs.end( ); ++c, ++it)
		{
			//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
			UINT  ofs = ofs_ * (*it)->stride;
			dgi->Device( ).IASetVertexBuffers( c, 1, &(*it)->buf, &(*it)->stride, &ofs);
		}
		return true;
	}
	//----------------------------------------------------------------------
	//���_����Ԃ�
	UINT  VB_Container::Num( )
	{
		auto  it = vbs.begin( );
		if(it != vbs.end( )){
			return  (*it)->num;
		}
		return 0;
	}
	//----------------------------------------------------------------------
	//	�o�[�e�b�N�X����o�^����i�����I�Ƀo�b�t�@�����j
	bool  VB_Container::PushBack(BYTE da_[], UINT str_, UINT n_)
	{
		DG_::VertexBuffer::SP  w = DG_::VertexBuffer::Create(da_, str_, n_);
		if(!w){	return false;	}
		vbs.push_back( w );
		return true;
	}
	//----------------------------------------------------------------------
	//	�w��ԍ��̃o�[�e�b�N�X�o�b�t�@��Ԃ�
	VertexBuffer::SP  VB_Container::GetVB(UINT  n_)
	{
		return  vbs[n_];
	}
}
