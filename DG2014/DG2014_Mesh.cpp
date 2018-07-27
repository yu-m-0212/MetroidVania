#include "DG2014_Ver3_51.h"
#include "../myLib.h"

#pragma warning(disable:4316)
//#include "atlstr.h"

namespace DG_
{
	//-------------------------------------------------------------------
	Mesh::Mesh( )
	{
	}
	Mesh::~Mesh( )
	{
	}

	//-------------------------------------------------------------------
	//	���b�V���𐶐�����
	bool Mesh::Initialize(	Material&				mc_,
							VB_Container::SP&		vbc_,//�o�[�e�b�N�X�o�b�t�@�R���e�i
							Texture::SP&			tex_)//�e�N�X�`��			
	{
		this->normalEnable  =  false;
		this->uvEnable  =  false;

		mateConts.push_back(mc_);

		MeshGroup  w;
		w.index  =  nullptr;
		w.mcNumber  =  0;
		meshGroups.push_back(w);
		vbc  =  vbc_;
		//BB�EBS����
		this->InitializeBB_BS( );
		return  true;
	}
	//-------------------------------------------------------------------
	//SOB�t�@�C�����烁�b�V���𐶐�����
	Mesh::SP  Mesh::Create_FromSOBFile(	const string&			fpath_)
	{
		//���ۂ̓ǂݍ��݂͕ʃt�@�C���Ŏ���
		Mesh::SP  rtv  =  Mesh::Create_FromSOBFileSub(fpath_);
		if(rtv){
			//BB�EBS����
			rtv->InitializeBB_BS( );
		}
		return rtv;
	}
	//-------------------------------------------------------------------
	//�a�a�Ƃa�r�𐶐�����
	void  Mesh::InitializeBB_BS( )
	{
		//�o�E���f�B���O�{�b�N�X�E�X�t�B�A�𐶐�����
		auto  vb = this->vbc->GetVB(0);
		ML::Vec3  *p = (ML::Vec3*)vb->memory;
		//BB
		//�ŏ��E�ő�_�̌��o
		ML::Vec3  minP(p[0]), maxP(p[0]);
		for (UINT n = 1; n < vb->num; n++){
			minP.x = min(minP.x, p[n].x);
			minP.y = min(minP.y, p[n].y);
			minP.z = min(minP.z, p[n].z);

			maxP.x = max(maxP.x, p[n].x);
			maxP.y = max(maxP.y, p[n].y);
			maxP.z = max(maxP.z, p[n].z);
		}
		this->bb.Center = XMFLOAT3((minP + maxP) / 2);
		this->bb.Extents = XMFLOAT3((maxP - minP) / 2);
		//BS
		//�����Ƃ������_��T��
		ML::Vec3  cen = (minP + maxP) / 2;	//���̒��S�iBB�Ɠ����Ƃ���j
		float  len = 0;
		for (UINT n = 1; n < vb->num; n++){
			ML::Vec3  v = p[n] - cen;
			float  l = v.Length();
			len = max(l, len);
		}
		this->bs.Center = XMFLOAT3(cen);
		this->bs.Radius = len;
	}


	//-------------------------------------------------------------------
	//�v���O������Ń��b�V�������i���ʂ͎g��Ȃ��j
	Mesh::SP  Mesh::Create(	Material&				mc_,
							VB_Container::SP&		vbc_,//�o�[�e�b�N�X�o�b�t�@�R���e�i
							Texture::SP&			tex_)//�e�N�X�`��	
	{
		if( Mesh::SP  sp  =  Mesh::SP( new  Mesh( ) ) )
		{
			if( sp->Initialize(mc_,  vbc_,  tex_) ){
				return sp;
			}
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	//	���b�V����\������
	void Mesh::Draw( )
	{
		auto  dgi = DG_::DGObject::GetInst( );	if(dgi == 0){	return;}

		//�e�N�j�b�N�I���H
		dgi->ETechnique( ).GetPassByIndex( 0 )->Apply( 0 );
		//���_�C���v�b�g���C�A�E�g���Z�b�g
		this->inputLayout->IASet( );


		for(	auto  it = meshGroups.begin( );
				it != meshGroups.end( );	it++){
			auto&  mt = mateConts[it->mcNumber];
			if(mt.tex){
				//�e�N�X�`���̓o�^
				mt.tex->PSSet(0);
			}else{
			//	//�e�N�X�`�����g�p���Ȃ�
				Texture::Empty_PSSet(0);
			}
			//���b�V���萔�����X�V
			dgi->EState( ).param.mateDiffuse   =  mt.diffuse;
			dgi->EState( ).param.mateSpeculer  =  mt.speculer;
			dgi->EState( ).param.mateAmbient   =  mt.ambient;
			dgi->EState( ).param.mateEmissive  =  mt.emissive;

			//�v���~�e�B�u�E�g�|���W�[�ƃo�[�e�b�N�X�o�b�t�@�[���Z�b�g
			vbc->IASet(0);
			//�v���~�e�B�u�������_�����O
			//D3D10_TECHNIQUE_DESC  dc;
			//dgi->ETechnique( ).GetDesc( &dc );
			//for( UINT p = 0; p < dc.Passes; ++p )
			//{
			dgi->EState( ).Update_Params( );
				dgi->ETechnique( ).GetPassByIndex( 0 )->Apply( 0 );
				if(it->index){
					dgi->Device( ).IASetIndexBuffer(it->index->buf, DXGI_FORMAT_R32_UINT, 0 );
					dgi->Device( ).DrawIndexed( it->index->num, 0 , 0 );
				}
				else{
					dgi->Device( ).Draw( vbc->Num( ), 0 );
				}
			//}
		}
	}
	//-------------------------------------------------------------------
	//	���b�V���ƃ��C�̐ڐG������s���i���C�̓��b�V���̃��[�J�����W�n�j
	bool  Mesh::Hit_Ray(const  ML::Vec3&  pos_,  const  ML::Vec3&  dir_,  float&  dist_,  bool  qmode_)
	{
		bool  rtv = false;
		//�o�[�e�b�N�X�R���e�i���璸�_���W�̃o�b�t�@�𓾂�
		VertexBuffer::SP  vbPos = this->vbc->GetVB(0);

		//���b�V���͓����ŕ�������Ă���\��������̂Ń��[�v�ŏ���
		for (auto  it = meshGroups.begin();
			it != meshGroups.end();	it++){

			if (it->index){
				//�C���f�b�N�X����i������̉\������j
				rtv = ML::Collsion::HitRayTriangle(pos_, dir_, qmode_, vbPos, 0,
					it->index, dist_);
				if (rtv  &&  qmode_){ break; }
			}
			else{
				//�C���f�b�N�X�Ȃ��i���Ԃ�P��j
				rtv = ML::Collsion::HitRayTriangle(pos_, dir_, qmode_, vbPos, 0, dist_);
				if (rtv  &&  qmode_){ break; }
			}
		}
		return  rtv;
	}
	//-------------------------------------------------------------------
	//	�o�E���f�B���O�{�b�N�X�ƃ��C�̐ڐG������s���i���C�̓��b�V���̃��[�J�����W�n�j
	bool  Mesh::Hit_RayBB(const  ML::Vec3&  pos_, const  ML::Vec3&  dir_, float&  dist_)
	{
		float  len = dir_.Length();
		const  FXMVECTOR  p = XMLoadFloat3(&XMFLOAT3(pos_));
		const  FXMVECTOR  d = XMLoadFloat3(&XMFLOAT3(dir_ / len));

		if (XNA::IntersectRayAxisAlignedBox(p, d, &this->bb, &dist_)){
			dist_ /= len;
			return true;
		}
		return  false;
	}

}
