#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�{�X�i�Z���^�[�j
//-------------------------------------------------------------------
#include "BChara.h"
#include "Boss.h"
#include "Task_Shot01.h"

namespace  Boss_Center
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�{�X");		//�O���[�v��
	const  string  defName("�{�X�i�Z���^�[�j");		//�^�X�N��
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
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate();		//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF();	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	//�ύX������������������������������������������������������

		int cnt_move;					//�J�E���^�s��
		int interval_shot;				//�������ԃV���b�g

		int add_un_hit;					//�v���C���ɗ^���閳�G����

		float speed_chase;				//���x�Ǐ]
		ML::Vec2 vec_shot;				//�ړ��ʃV���b�g
		ML::Box2D hit_shot;				//��`�V���b�g

		Boss* boss;						//���\�b�h�Ăяo��
		Shot01::Object* shot;			//���\�b�h�Ăяo��
	public:
		//�R���X�g���N�^
		Object();
		//�v�l&�󋵔��f(�X�e�[�^�X����)
		void Think();
		//���[�V�����ɑΉ���������
		void Move();
		//�ڐG���̉��������i�K���󂯐g�̏����Ƃ��Ď�������j
		//����	�F	�i�U�����̃|�C���^,�U�����,�^���G���ԁj
		void Received(BChara* from_, AttackInfo at_, const int&);
		//�A�j���[�V��������
		BChara::DrawInfo Anim();
	};
}