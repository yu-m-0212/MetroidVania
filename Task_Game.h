#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�Q�[���{��
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include  "Task_Shot00.h"
#include  "Task_Shot01.h"
#include  "Task_Effect.h"

namespace  Game
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�{��");	//�O���[�v��
	const  string  defName("NoName");	//�^�X�N��
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
	};
	//-------------------------------------------------------------------
	class  Object : public  BTask
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
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();		//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate();			//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF();	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	//�ύX������������������������������������������������������
	private:
		ML::Vec2 deadPos;	//�O��̎��S�n�_
		int cnt_Scenes;		//�J�E���^�J�ڗp
	public:
		//���\�[�X���풓������
		Shot00::Resource::SP	shot00_Resource;
		Shot01::Resource::SP	shot01_Resource;
		Effect::Resource::SP	effect_Resource;
		//�`���[�g���A���\���L�����N�^�̐���
		//����	�F	�i���W,���b�Z�[�W�j
		void Create_Message(const ML::Vec2&, const string&);
		//�G�l�~�[�̐���
		//����	�F	�i�������W,�ړ����x,HP�j
		void Create_Enemy01(const ML::Vec2&);
	};
}