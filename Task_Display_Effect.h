#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//��ʌ���
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

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
		float transparency;			//�����x
		bool flag_in_out;			//0=in 1=out
		int cnt_transition;			//��ʑJ�ڃJ�E���^
		int time_create_next_task;	//���p���^�X�N�𐶐�����^�C�~���O
		int time_start_fade_out;	//�t�F�[�h�A�E�g���n�߂�^�C�~���O
		int next_scene;				//���p���^�X�N���w�肷��ϐ�
	public:
		//���p���^�X�N���w�肷��
		//����	�F	�i�����j
		//0=Game	1=Retry		2=Ending	3=Title
		void Set_Next_Scene(const int&);
	};
}