#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�v���C���̏o���e
//-------------------------------------------------------------------
//#include "GameEngine_Ver3_7.h"
#include "BChara.h"

namespace  Shot00
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�e�i�v���C���j");	//�O���[�v��
	const  string  defName("NoName");	//�^�X�N��
	//-------------------------------------------------------------------//���\�[�X
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
		string imageName;
	};
	//-------------------------------------------------------------------//�I�u�W�F�N�g
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
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate();		//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF();	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	//�ύX������������������������������������������������������
	//�ϐ�

		//�ǂ�G�ɏՓ˂����Ƃ��A���ł��邩�ۂ�
		//shot�͏�����Amelee�͏����Ȃ�
		bool eraseFlag;
		//���ł���܂ł̎���
		int cntLimit;
	public:
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����

		//�U�����Ɏ�����݂���ۂɎg�p
		//����	�F	�i�����j
		void Set_Limit(const int&);
		//�ǂ�G�ɏՓ˂����Ƃ��A�����邩�ۂ����w�肷��
		//����	�F	�i0��1�j
		void Set_Erase(const int&);
		//��Ԃ��Ƃɍs�����w�肷��
		void Move();
	};
}