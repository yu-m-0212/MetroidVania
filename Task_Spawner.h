#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�X�|�i�[
//-------------------------------------------------------------------
#include "BChara.h"
#include "Task_Game.h"
#include "Task_Effect.h"

namespace  Spawner
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�X�|�i�[");		//�O���[�v��
	const  string  defName("�{�X�X�|�i�[");		//�^�X�N��
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
		string name_bgm_boss;	//�{�XBGM
		string name_image;		//�����`�摜
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
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate();		//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF();	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	//�ύX������������������������������������������������������
		int time_create_boss;			//�{�X�̐����^�C�~���O
		int time_bgm;					//�{�XBGM�̍Đ��J�n����
		bool flag_spawn_boss;			//�{�X�o���t���O
		ML::Vec2 init_pos_boss;			//�{�X�̏������W
		Task_Effect::Object* eff;		//�G�t�F�N�g�C���X�^���X
	public:
		//�{�X�̏o���t���O��Ԃ�
		bool Get_Flag_Spawn_Boss();
	};
}