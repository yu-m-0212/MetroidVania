#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�G�l�~�[�̔��������锻���`(�e�j
//-------------------------------------------------------------------
#include "BChara.h"
#include "Task_Effect.h"

namespace  Shot01
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�e(�G�l�~�[)");	//�O���[�v��
	const  string  defName("NoName");				//�^�X�N��
	//-------------------------------------------------------------------//���\�[�X
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
		string imageName;
	};
	//-------------------------------------------------------------------//�I�u�W�F�N�g
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
		void  Render2D_AF();	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	//�ύX������������������������������������������������������
		bool flag_Erase;			//�ǂ�G�ɏՓ˂����Ƃ��A���ł��邩�ۂ�
		bool flag_reflect;			//���˂����e���ۂ�
		int cnt_anim;				//�A�j���J�E���^
		int speed_anim;				//�A�j���X�s�[�h
		int limit_Erase;			//���ł���܂ł̎���
		int power;					//�U����
		int rate_reflect_power;		//���˂����e�ɗ^����U���͔{��
		int add_un_hit_player;		//�v���C���ɗ^���閳�G����
		int add_un_hit_boss;		//�{�X�ɗ^���閳�G����
		float angle;				//�p�x
		float rate_speed_reflect;	//���˂����ہA�^��������{��
		Task_Effect::Object* eff;	//���\�b�h�Ăяo��
	public:
		//�R���X�g���N�^
		Object();
		//��Ԃ��Ƃɍs�����w�肷��
		void Move();
		//���ł���ہA��Ԃɉ����ăG�t�F�N�g�𐶐�
		//����	�F	�i�G�t�F�N�g�𐶐�������W�j
		void Effect_Hit(const ML::Vec2&);
		//�V���b�g�𐶐�����
		//����	�F	�i���W,�ړ���,��`,���Ŏ���,�U����,�ǂɓ��������������邩�ۂ��j
		void Create_Shot(const ML::Vec2&, const ML::Vec2&, const ML::Box2D&, const int&, const int&, const bool&);

		//�A�N�Z�T
		//�U�����Ɏ�����݂���ۂɎg�p
		//����	�F	�i�����j
		void Set_Limit(const int&);
		//�ǂ�G�ɏՓ˂����Ƃ��A�����邩�ۂ����w�肷��
		//����	�F	�i0��1�j
		void Set_Erase(const int&);
		//�O�����琶������ہA�U���͂��w��
		//����	�F	�i�����l�j
		void Set_Power(const int&);
		//�p�x���w�肷��
		//����	�F	�i�p�x�j
		void Set_Angle(const float&);
	};
}