#pragma warning(disable:4996)
#pragma once

#include "DG2014_DGObject.h"

#include "DG2014_IndexBuffer.h"
#include "DG2014_InputLayout.h"
#include "DG2014_VertexBuffer.h"
#include "DG2014_VertexContainer.h"

#include "DG2014_Texture.h"
#include "DG2014_Mesh.h"
#include "DG2014_Image.h"
#include "DG2014_Font.h"
#include "DG2014_Effect.h"

namespace
{
	enum{	spriteBufferMax = 4096};
}


namespace DG
{
	//������
	bool  Initialize(
		HWND		hw_,	//	�E�B���h�E�n���h��
		int			w_,		//	����
		int			h_,		//	�c��
		DWORD		ms_,	//	�}���`�T���v��
		bool		sm_,	//	�X�N���[�����[�h
		int			w2D_,	//	2D�`��𑜓x�i����
		int			h2D_);	//	2D�`��𑜓x�i����	
	//���
	void  Finalize();

	//	��ʕ`��i�J�n�j
	BOOL  Begin(ML::Color color_);
	//	3D�`��̋��𓾂�
	void  Begin_3D();
	//	3D�`��̂��I������
	void  End_3D();
	//	2D�`��̋��𓾂�
	void  Begin_2D();
	//	2D�`��̂��I������
	void  End_2D();
	//	��ʕ`��i�I���j
	void  End();

	//�G�t�F�N�g�ݒ�
	DG_::EffectState&	EffectState();
	//�X�v���C�g
	ID3DX10Sprite&			Sprite();
	//DG�I�u�W�F�N�g
	DG_::DGObject&		Object();
	//�f�o�C�X
	ID3D10Device&	Device();
	//�G�t�F�N�g�e�N�j�b�N
	ID3D10EffectTechnique&	EffectTec();


//---------------------------------------------------------------------------
	//�ǂݍ��݁�����ւ�
	void  Image_Create(
		const string name_,
		const string fileName_);
	//�\��
	void  Image_Draw(
		const string name_,
		ML::Box2D  draw_,
		ML::Box2D  src_,
		const ML::Color&  c_ = ML::Color(1, 1, 1, 1));
	//��]�i�ݒ�j
	void  Image_Rotation(
		const string name_,
		float  r_,	//	��]�ʁi���W�A���j
		const  ML::Vec2&  c_);	//	��]���idraw���΍��W�j
	//���ݐ������Ă���摜�����ׂĔj��
	void  Image_Clear();
	//�w�肵���摜��j��
	void  Image_Erase(const string name_);
	//�T�C�Y���擾����
	POINT Image_Size(const string name_);
//-----------------------------------------------------------------------
	//�t�H���g�𐶐�����
	void  Font_Create(
		const string	name_,
		const string	fontName_,
		WORD			width_,
		WORD			height_,
		WORD			weight_ = 500,
		BYTE			charSet_ = SHIFTJIS_CHARSET);
	//�e�L�X�g��\������
	void  Font_Draw(const string name_,
		const ML::Box2D&	draw_,	//	�`���
		const string&		tex_,
		ML::Color				color_ = ML::Color(1, 1, 1, 1),
		UINT					uFormat_ = DT_LEFT);
	//�t���[���t���e�L�X�g��\������
	enum FRAME{ x1, x2, x4 };
	void  Font_DrawF(
		const string			name_,
		const ML::Box2D&		draw_,	//	�`���
		const string&			tex_,
		FRAME					mode_,
		ML::Color				color_ = ML::Color(1, 1, 1, 1),
		ML::Color				fColor_ = ML::Color(1, 0, 0, 0),
		UINT					uFormat_ = DT_LEFT);
	//���ݐ������Ă���t�H���g�����ׂĔj��
	void  Font_Clear();
	//�w�肵���t�H���g��j��
	void  Font_Erase(const string name_);
//-----------------------------------------------------------------------
	//���b�V���𐶐�����iSOB�t�@�C����ǂݍ����
	void  Mesh_CreateFromSOBFile(
		const string	name_,
		const  string&  fpath_);
	//���b�V����`�悷��
	void Mesh_Draw(const string	name_);
	//���b�V���ƃ��C�̐ڐG������s��
	bool  Mesh_HitRay(
		const string	name_,
		const  ML::Vec3&  pos_,
		const  ML::Vec3&  dir_,
		float&  dist_,
		bool  qmode_ = false);
	//���b�V���ƃ��C���ރo�E���f�B���O�{�b�N�X�̐ڐG������s��
	bool  Mesh_HitRayBB(
		const string	name_,
		const  ML::Vec3&  pos_,
		const  ML::Vec3&  dir_,
		float&  dist_);
	//���ݐ������Ă��郁�b�V�������ׂĔj��
	void  Mesh_Clear();
	//�w�肵�����b�V����j��
	void  Mesh_Erase(const string name_);
//-----------------------------------------------------------------------
	void  Line_Draw(
		const ML::Vec3&	posS_,
		const ML::Vec3&	posE_);
//-----------------------------------------------------------------------
	//�e�N�X�`���𐶐�����
	void  Texture_Create(
		const string	name_,
		const  string&  fpath_);
	//�s�N�Z���V�F�[�_�ɓo�^����
	void  Texture_PSSet(const string	name_, UINT  slotNum);
	//���ݐ������Ă���e�N�X�`�������ׂĔj��
	void  Texture_Clear();
	//�w�肵���e�N�X�`����j��
	void  Texture_Erase(const string name_);
}