#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�Q�[���p�b�h����
//-------------------------------------------------------------------
#include "BChara.h"

namespace  Caution
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("����");		//�O���[�v��
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
		string name_image_caution;	//�摜���\�[�X
		string name_font;			//�t�H���g
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
		bool transition_scene;		//�V�[���؂�ւ��t���O
		int rate_close_up;			//�`��g�嗦
		int add_rate_close_up;		//�`��g�嗦������
		int speed_rate_close_up;	//�`��g�呬�x
		int time_flag;				//�J�ڃt���O�𔽓]�����鎞��
		int time_erase;				//�t���O���]��A���ł܂ł̎���
		string name_pad;			//�p�b�h�錾
		string text_skip;			//�X�L�b�v�\��
		ML::Vec2 pos_font;			//�t�H���g���W
		ML::Color color_font;		//�t�H���g�F
	public:
		//�R���X�g���N�^
		Object();					
		//��ʐ؂�ւ��t���O�Ǘ�
		bool Transition_Scene();
	};
}