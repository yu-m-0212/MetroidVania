#pragma once
#include "GameEngine_Ver3_7.h"
#include <fstream>
#include <sstream>

//�Q�[�����S��Ŏg�p����\���̂Ȃǂ��`����
//-----------------------------------------------------------------------------------------------
#define SIZE_CHIP 32		//�}�b�v�`�b�v�T�C�Y
#define SIZE_MAP_W 300	//�T�C�Y�}�b�v��
#define SIZE_MAP_H 300	//�T�C�Y�}�b�v�c
//-----------------------------------------------------------------------------------------------
namespace  MyPG
{
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	//----------------------------------------------
	//�J������{�`
	class Camera
	{
		Camera( ){ }
		Camera(
				const ML::Vec3&		tg_,	//	��ʑ̂̈ʒu
				const ML::Vec3&		pos_,	//	�J�����̈ʒu
				const ML::Vec3&		up_,	//	�J�����̏�����������x�N�g���i��̂x�{�Œ�j
				float				fov_,	//	����p
				float				np_,	//	�O�N���b�v���ʁi������O�͉f��Ȃ��j
				float				fp_,	//	��N���b�v���ʁi��������͉f��Ȃ��j
				float				asp_);	//	�A�X�y�N�g��i��ʂ̔䗦�ɍ��킹��@�����c�j			
	public:
	//	�r���[���i�����֘A�j
		ML::Vec3 target;			//	��ʑ̂̈ʒu
		ML::Vec3 pos;				//	�J�����̈ʒu
		ML::Vec3 up;				//	�J�����̏�����������x�N�g���i��̂x�{�Œ�j
	//	�ˉe���i����͈͊֘A�j
		float fov;					//	����p
		float nearPlane;			//	�O�N���b�v���ʁi������O�͉f��Ȃ��j
		float forePlane;			//	��N���b�v���ʁi��������͉f��Ȃ��j
		float aspect;				//	�A�X�y�N�g��i��ʂ̔䗦�ɍ��킹��@�����c�j
	//	�s����
		ML::Mat4x4  matView, matProj;
		~Camera( );
		typedef shared_ptr<Camera> SP;
	//	�J�����𐶐�����
		static SP Create(
				const ML::Vec3&		tg_,	//	��ʑ̂̈ʒu
				const ML::Vec3&		pos_,	//	�J�����̈ʒu
				const ML::Vec3&		up_,	//	�J�����̏�����������x�N�g���i��̂x�{�Œ�j
				float				fov_,	//	����p
				float				np_,	//	�O�N���b�v���ʁi������O�͉f��Ȃ��j
				float				fp_,	//	��N���b�v���ʁi��������͉f��Ȃ��j
				float				asp_);	//	�A�X�y�N�g��i��ʂ̔䗦�ɍ��킹��@�����c�j	
	//	�J�����̐ݒ�
		void UpDate( );
	};
	//----------------------------------------------
	class MyGameEngine : public GameEngine
	{
	public:
		MyGameEngine( );
		//�Q�[���G���W���ɒǉ��������̂̏������ƊJ��
		bool Initialize(HWND wnd_);
		~MyGameEngine( );
		//�Q�[���G���W���ɒǉ��������̂̃X�e�b�v����
		void UpDate( );

		//3DPG1�Ή��ɂ��ǉ�
		//2D�`����̃f�t�H���g�p�����[�^�ݒ�
		void Set2DRenderState(DWORD l_);
		//3D�`����̃f�t�H���g�p�����[�^�ݒ�
		void Set3DRenderState(DWORD l_);

		//�J�����̌X�����x���ɑ΂���p�x�Ŏ擾����
		//X���{������0�x�Ƃ����J�����̌�����Ԃ��B
		float GetCameraDirectionY_XPlus0(UINT cn_);


	//�Q�[���G���W���ɒǉ����������͉̂��ɉ�����
	//----------------------------------------------
		MyPG::Camera::SP	camera[4];	//�J����
		ML::Box2D	camera2D;			// �Q�c�X�N���[������p
		bool clear;						//�N���A�t���O
		bool failure;					//�~�X�t���O
		bool pause;						//�|�[�Y�t���O

	//----------------------------------------------
	};
}
extern MyPG::MyGameEngine* ge;


