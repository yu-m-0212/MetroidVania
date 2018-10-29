#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�Q�����z��}�b�v
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

namespace  Map2D
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�t�B�[���h");	//�O���[�v��
	const  string  defName("�}�b�v");			//�^�X�N��
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
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate();		//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF();	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	//�ύX������������������������������������������������������
		int					cnt_Quake;		//�J�E���^��ʗh��
		int					dist_Quake;		//��ʗh�ꕝ
		int					limit_Quake;	//��ʂ�h�炷����
	public:
		string				imageName;		//�}�b�v�̓��\�[�X���L���Ȃ�
		int					arr[100][100];	//�}�b�v�z��
		int					sizeY, sizeX;	//�}�b�v�T�C�Y
		ML::Box2D			hitBase;		//�s�N�Z���P�ʂ̃}�b�v�T�C�Y������
		ML::Box2D			chip[16];
		//�}�b�v�t�@�C���ǂݍ���(���͉������j
		bool Load(const  string&  fpath_);
		//�����蔻��
		bool CheckHit(const  ML::Box2D&  h_);
		//�}�b�v�O�������Ȃ��悤�ɃJ�������ʒu��������
		void  AjastCameraPos();

		//�A�N�Z�T
		//��ʂ̗h�ꕝ���w�肷��
		void Set_Dist_Quake(const int&);
		//��ʂ�h�炷���Ԃ��w�肷��
		void Set_Limit_Quake(const int&);
	};
}