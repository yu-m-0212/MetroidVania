#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�A�C�e��01(�o���A�A�r���e�B)
//-------------------------------------------------------------------
#include "BChara.h"
#include "Task_Tutorials.h"

namespace  Item01
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�A�C�e��");		//�O���[�v��
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
		string name_image;				//�摜���\�[�X
		string name_se_pick_up_item;	//���ʉ�
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
		void  Render2D_AF();	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	public:
		Object();
	//�ύX������������������������������������������������������
	private:
		int limit_message;					//���b�Z�[�W�̎��Ԑ���
		int time_erase;						//���g�̏��Ń^�C�~���O
		ML::Vec2 center;					//��ʒ������W
		ML::Vec2 init_pos_create_tutorial;	//���˃`���[�g���A���̐������W
		Tutorials::Object* tutorials;		//���\�b�h�Ăяo��
	public:
		//�A�j���[�V��������
		BChara::DrawInfo Anim();
		//�ڐG���̉��������i�K���󂯐g�̏����Ƃ��Ď�������j
		void Recieved(BChara* from_, AttackInfo at_, const int& un_hit_);
		//�A�C�e��01�̐���
		//����	�F	�i�������W�j
		void Create_Item01(const ML::Vec2&);
	};
}