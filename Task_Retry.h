#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//���g���C
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include "BChara.h"
#include "Task_Effect.h"
using namespace ML;
namespace  Retry
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("���g���C");	//�O���[�v��
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
		string back_retry;
		string button_retry;
		string name_se_retry;	//���g���C�{�^���v�b�V��
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
		string controllerName;			//�R���g���[���̐錾
		Vec2 pos_dead;					//�O��̎��S�n�_
		BChara::Angle_LR angle_dead;	//�O��̎��S���̌���
		bool flag_transition;			//��ʑJ�ڗp�t���O
		bool title_or_game;				//���p���^�X�N�̑I���t���O
		int cnt_transition;				//�J�E���^�J�ڗp
		int cnt_create_bubble;			//�G�t�F�N�g�̐����J�E���^
		int cnt_available_controll;		//������A������󂯕t����܂ł̃J�E���^
		int time_create_next_task;		//���p���^�X�N�̐����^�C�~���O
		int time_kill_game;				//���g�����ł�����^�C�~���O
		int time_available_controll;	//������A������󂯕t����܂ł̎���
		float interval_anim_back;		//�w�i�A�j������
		float cnt_anim_back;			//�w�i�A�j���J�E���^
		float posY;						//�w�iY�����W
		float posY_std;					//�w�iY�����W��l
		float height_anim_back;			//�w�i�A�j���h�ꕝ
		float init_bubble_pos_y;		//�A��Y���������W
		Task_Effect::Object* eff;		//���\�b�h�Ăяo��
	public:
		//���S�������W��ۑ�����
		//����	�F	�iVec2)
		void Set_Pos_Dead(const Vec2&);
		//�O�񎀖S�������W��Ԃ�
		ML::Vec2 Get_Pos_Dead();
		//���S���̌������w�肷��
		//����	�F	�iBChara::Angle_LR)
		void Set_Angle_Dead(const BChara::Angle_LR&);
		//���S���̌������擾����
		BChara::Angle_LR Get_Angle_Dead();
	};
}