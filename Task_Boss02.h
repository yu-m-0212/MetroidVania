#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�{�X01
//-------------------------------------------------------------------
#include "BChara.h"

namespace  Task_Boss02
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�{�X");	//�O���[�v��
	const  string  defName("NoName");		//�^�X�N��
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
	//�ύX������������������������������������������������������
		//���L����ϐ��͂����ɒǉ�����
		string name_image;	//�摜���\�[�X
	};
	//-------------------------------------------------------------------
	class  Object : public  BChara
	{
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//�������� ������true�Ń^�X�N�V�X�e���֎����o�^
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();		//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate();			//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF();	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	//�ύX������������������������������������������������������
	public:
		//��ԊǗ�
		enum State_Boss
		{
			Non,	//���w��
			Base,	//�y��
			Common	//�ʏ�
		};
	private:
		State_Boss state_boss;	//�{�X�p��ԊǗ�
		int interval_shake;		//�Ԋu���E�h��
		float std_pos_x;		//��l�������W
		float speed_shake;		//���x�h��
		float max_speed_fall;	//�ő嗎�����x
	public:
		//�ڐG���̉��������i�K���󂯐g�̏����Ƃ��Ď�������j
		//����	�F	(�U����,�U�����,�^���G����)
		void Received(BChara* from_, AttackInfo at_, const int&);
		//�v�l���󋵔��f(�X�e�[�^�X����j
		void  Think();
		//���[�V�����ɑΉ���������
		void  Move();
		//�A�j���[�V��������
		BChara::DrawInfo  Anim();

		//�A�N�Z�T

		void Set_State_Boss(const State_Boss&);
		void Set_Std_Pos_X(const float&);
		void Set_Hit_Body(const ML::Box2D&);
	};
}