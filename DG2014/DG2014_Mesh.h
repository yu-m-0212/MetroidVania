#pragma warning(disable:4996)
#pragma once
#include "DG2014_Ver3_51.h"

namespace DG_
{
	//�}�e���A���R���e�i
	class  Material{
	public:
		string				name;		//�}�e���A����
		ML::Color			diffuse;	//�}�e���A�����:�f�t���[�Y
		ML::Color			ambient;	//�}�e���A�����:�A���r�G���g
		ML::Color			speculer;	//�}�e���A�����:�X�y�L����
		ML::Color			emissive;	//�}�e���A�����:�G�~�b�V�u
		Texture::SP			tex;		//�e�N�X�`��
	};
	//�����Ƀ����_�����O���镔��
	class  MeshGroup{
	public:
		DWORD				mcNumber;	//�Ή�����}�e���A���ԍ�
		IndexBuffer::SP		index;		//�C���f�b�N�X���
		MeshGroup( ):mcNumber(0){}
	};
	//���b�V��
	class  Mesh
	{
	//-------------------------------------------------------------------
	public:
		typedef  shared_ptr<Mesh>   SP;
		typedef  weak_ptr<Mesh>     WP;
	//-------------------------------------------------------------------
	private:
		static  unordered_map<string,  WP>   rep;
	//-------------------------------------------------------------------
	private:
		BOOL					normalEnable;	//�@������H
		BOOL					uvEnable;		//UV����H
		DG_::VB_Container::SP	vbc;			//�o�[�e�b�N�X�o�b�t�@�R���e�i
		vector<Material>		mateConts;		//�}�e���A��
		vector<MeshGroup>		meshGroups;		//�H
		InputLayout::SP			inputLayout;	//���_���̓��̓��C�A�E�g
		XNA::AxisAlignedBox		bb;				//�o�E���f�B���O�{�b�N�X	
		XNA::Sphere				bs;				//�o�E���f�B���O�X�t�B�A
		//-------------------------------------------------------------------
	private:
		Mesh( );
	public:
		~Mesh( );
	//-------------------------------------------------------------------
	public:
		//	���b�V���𐶐�����(�f�[�^�����O�ŗp�ӂ��鉼�����I�ȃ��m�j
		static  SP  Create(
				Material&				mc_,
				VB_Container::SP&		vbc_,//�o�[�e�b�N�X�o�b�t�@�R���e�i
				Texture::SP&			tex_);//�e�N�X�`��
	private:
		//�@���b�V���𐶐�����
		bool  Initialize(
				Material&				mc_,
				VB_Container::SP&		vbc_,	//�o�[�e�b�N�X�o�b�t�@�R���e�i
				Texture::SP&			tex_);	//�e�N�X�`��			
	//-------------------------------------------------------------------
	public:
		//SOB�t�@�C�����烁�b�V���𐶐�����
		static  SP  Create_FromSOBFile(const  string&  fpath_);
	private:
		//SOB�t�@�C�����烁�b�V���𐶐�����
		static  SP  Create_FromSOBFileSub(const  string&  fpath_);
		//SOB�t�@�C�����烁�b�V���𐶐�����
		bool  Initialize_FromSOBFile(const  string&  fpath_);
	//-------------------------------------------------------------------
	public:
		//X�t�@�C�����烁�b�V���𐶐�����(������
		static  SP  Create_FromXFile(const  string&  fpath_);
	private:
		//X�t�@�C�����烁�b�V���𐶐�����(������
		bool  Initialize_FromXFile(const  string&  fpath_);
	//-------------------------------------------------------------------
	public:
		//	���b�V����\������
		void Draw(	);
	//-------------------------------------------------------------------
	public:
		//	���b�V���ƃ��C�̐ڐG������s���i��̓��b�V���̃��[�J�����W�n�j
		bool  Hit_Ray(const  ML::Vec3&  pos_,  const  ML::Vec3&  dir_,  float&  dist_,  bool  qmode_=false);
	//-------------------------------------------------------------------
	private:
		//�a�a�Ƃa�r�𐶐�����
		void  InitializeBB_BS( );
	//-------------------------------------------------------------------
	public:
		//	�o�E���f�B���O�{�b�N�X�ƃ��C�̐ڐG������s���i���C�̓��b�V���̃��[�J�����W�n�j
		bool  Hit_RayBB(const  ML::Vec3&  pos_,  const  ML::Vec3&  dir_,  float&  dist_);
	};
}
