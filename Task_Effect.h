#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�G�t�F�N�g
//-------------------------------------------------------------------
#include "BChara.h"
#include "Effect.h"

using namespace ML;

namespace  Task_Effect
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�G�t�F�N�g");	//�O���[�v��
	const  string  defName("NoName");			//�^�X�N��
	//-------------------------------------------------------------------
	class  Resource
	{
		bool  Initialize();
		bool  Finalize();
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
		//���L����ϐ��͂����ɒǉ�����
		string effectImage;
	};
	//-------------------------------------------------------------------
	class  Object : public  BChara
	{
	//�ύX�s����������������������������������������������������
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//�������� ������true�Ń^�X�N�V�X�e���֎����o�^
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate();		//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF();//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	//�ύX������������������������������������������������������
		//��ԊǗ�
		enum State_Effect
		{
			Non,			//���w��
			Hit_Shot,		//�q�b�g�V���b�g�i�v���C���j
			Barrier,		//�o���A
			Heal,			//��
			Bubble,			//�A
			Appear,			//�o��
			Defeat_Enemy,	//���j�i�G�l�~�[�j
			Debris			//�j��
		};
		State_Effect state_effect;	//��ԊǗ�
		int num_bubble;				//�A�̑傫�����w�肷��
		int interval_bubble;		//�A�̗h�����
		int limit_erase;			//���ł܂ł̎���
		int limit_erase_hit_shot;	//���ԏ��ł܂Ńq�b�g�V���b�g
		int limit_erase_barrier;	//���ԏ��ł܂Ńo���A
		int limit_erase_defeat;		//���ԏ��ł܂Ō��j
		int limit_erase_heal;		//���ԏ��ł܂ŉ�
		int limit_erase_bubble;		//���ԏ��ł܂ŖA
		int limit_erase_appear;		//���ԏ��ł܂œo��
		int limit_erase_debris;		//���ł܂ł̎��Ԕj��
		int choice_state;			//�O�������Ԃ��w�肷��ہA�g�p
		float speed_surfacing;		//�A�̕��㑬�x
		float speed_Debris;			//���x�j��
		float wide_bubble;			//�A�̗h�ꕝ
		float dist;					//���j�G�t�F�N�g�̒��S����̋���
		float angle;				//�\������p�x
		float gravity;				//�d�͉����x
		Vec2 center;				//��]��
		Effect* eff;				//�G�t�F�N�g�N���X
	public:
		Object();
		//�G�t�F�N�g�𐶐�����i�p�x���w�肵�Ȃ��j
		//����	�F	�i���,���W�j
		static void Create_Effect(const int&, const ML::Vec2&);
		//�G�t�F�N�g�𐶐�����i�p�x���w�肷��j
		//����	�F	�i���,���W,�p�x,�����j
		void Create_Effect(const int&, const ML::Vec2&, const float&, const BChara::Angle_LR&);
		//��Ԃ��Ƃɍs�����w�肷��
		void Move();
		//�A�j���[�V����
		BChara::DrawInfo Anim();
	};
}