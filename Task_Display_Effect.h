#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//��ʌ���
//-------------------------------------------------------------------
#include "BChara.h"

namespace  Display_Effect
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("��ʌ���");	//�O���[�v��
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
		string imageName;
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
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();		//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate();			//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF();	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	//�ύX������������������������������������������������������
		enum State_Display_Effect
		{
			Non,							//���w��
			Fade,							//�t�F�[�h�C���A�A�E�g
			Letter_Box						//���^�[�{�b�N�X
		};
		State_Display_Effect state_display;	//��ԊǗ�
		int	num_state;						//�O������w�肷��ۂ̏�Ԕԍ�

		bool flag_switching_slide;			//�X���C�h�C��/�A�E�g�t���O
		int move_letter;					//���^�[�{�b�N�X�A�j���J�E���^
		int add_move_letter;				//���^�[�{�b�N�X�X���C�h���x
		int limit_move_letter;				//���^�[�̈ړ��ʏ��
		int cnt_switch_letter;				//�X���C�h�C��/�A�E�g�̐؂�ւ��Ԋu�J�E���^
		int time_switch_letter;				//�X���C�h�C��/�A�E�g�̐؂�ւ��^�C�~���O

		bool flag_in_out;					//0=in 1=out
		int cnt_transition;					//��ʑJ�ڃJ�E���^
		int time_create_next_task;			//���p���^�X�N�𐶐�����^�C�~���O
		int time_start_fade_out;			//�t�F�[�h�A�E�g���n�߂�^�C�~���O
		float transparency;					//�����x
	public:
		//�O�������Ԃ��w�肷��
		//����	�F	�i�����j
		//0�F�t�F�[�h�C��/�A�E�g
		//1�F���^�[�{�b�N�X
		void Set_Num_State(const int&);
		//��ʑS�̂̌��ʂ𐶐�����
		//����	�F	�i��Ԕԍ��j
		void Create_Display_Effect(const int&);
	};
}