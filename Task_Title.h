#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include  "Task_Effect.h"

namespace  Title
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�^�C�g��");	//�O���[�v��
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
		//���L����ϐ��͂����ɒǉ�����
		string	back_title;
		string	button_title;
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
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate();		//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF();	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	//�ύX������������������������������������������������������
		bool flag_transition;		//��ʑJ�ڃt���O
		int cnt_transition;			//���ŃJ�E���^
		int cnt_create_bubble;		//�G�t�F�N�g�̐����J�E���^
		int cnt_anim_back;			//�w�i�A�j���J�E���^
		int interval_anim_back;		//�w�i�A�j������
		int limit_transparency;		//��ʑJ�ڎ��̏��Ŏ���
		float init_bubble_pos_y;	//�A��Y���������W
		float posY;					//�w�iY�����W
		float posY_std;				//�w�iY�����W��l
		float height_anim_back;		//�w�i�A�j���h�ꕝ
		float cnt_anim_button;		//�{�^���A�j���J�E���^
		float interval_button;		//�{�^���A�j������
		ML::Vec2 pos_button;		//���W�{�^��
		Task_Effect::Object eff;	//���\�b�h�Ăяo��
		string controllerName;		//�R���g���[���[�錾
	};
}