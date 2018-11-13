#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�Q�[���{��
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include  "Task_Shot00.h"
#include  "Task_Shot01.h"
#include  "Task_Effect.h"
#include  "Task_Tutorials.h"
#include  "Task_Player.h"
#include  "Task_Enemy01.h"

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
		ML::Vec2 pos_dead;				//�O��̎��S�n�_
		BChara::Angle_LR angle_dead;	//���S���̌���
		int cnt_transition;				//�J�E���^�J�ڗp
		int time_create_next_task;		//���p���^�X�N�̐����^�C�~���O
		int time_kill_game;				//���g�����ł�����^�C�~���O
		Tutorials::Object* tutorials;	//�|�C���^���b�Z�[�W
		Task_Effect::Object* eff;		//�|�C���^�G�t�F�N�g
	public:
		//���\�[�X���풓������
		Shot00::Resource::SP		shot00_Resource;
		Shot01::Resource::SP		shot01_Resource;
		Task_Effect::Resource::SP	effect_Resource;
		Player::Resource::SP		player_resource;
		Enemy01::Resource::SP		enemy01_resource;
	};
}