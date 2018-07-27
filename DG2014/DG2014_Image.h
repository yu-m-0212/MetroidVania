#pragma warning(disable:4996)
#pragma once
#pragma warning( disable : 4005 )
#include <windows.h>
#include <d3dx10.h>
#include <d3dCompiler.h>
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d10.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3dCompiler.lib")
#pragma warning( default : 4005 )


#include "DG2014_Texture.h"
#include <string>

namespace DG_
{
	//-----------------------------------------------------------------------
	//2D�`��p�e�N�X�`���⏕�N���X
	class Image
	{
	public:
		static  bool  dt;	//�`����s�����ۂ�true�A
	//-----------------------------------------------------------------------
	public:
		typedef shared_ptr<Image>    SP;
		typedef weak_ptr<Image>      WP;
	//-----------------------------------------------------------------------
	private:
		Image( );
	//-----------------------------------------------------------------------
	private:
		bool  Initialize(const  string&  fpath_);
	//-----------------------------------------------------------------------
	private:
		Texture::SP		tex;
		float			angle;
		ML::Vec2		center;
		float			sw;
		float			sh;
	//-----------------------------------------------------------------------
	public:
		~Image( );
		POINT  Size( );
	//-----------------------------------------------------------------------
	public:
		//��������
		static SP Create(const string&  fpath_);
		//�d������摜��ʓr�ǂݍ���
		static SP CreateRep(const string&  fpath_);
	//-----------------------------------------------------------------------
	public:
		//��]
		void  Rotation(float  a_, const ML::Vec2&  c_);
	//-----------------------------------------------------------------------
	public:
		//�`��
		void  Draw(const ML::Box2D&  d_, const ML::Box2D&  s_, const ML::Color&  c_ = ML::Color(1, 1, 1, 1));
//		void  Draw( );
	//-----------------------------------------------------------------------
	private:
		static unordered_map<string, WP> rep;
	};
}
