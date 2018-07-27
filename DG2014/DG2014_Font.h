#pragma warning(disable:4996)
#pragma once
#pragma warning( disable : 4005 )
#include <d3dx10.h>
#include <d3dCompiler.h>
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d10.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3dCompiler.lib")
#pragma warning( default : 4005 )


#include <string>

namespace DG_
{
	//-----------------------------------------------------------------------
	//2D�����`��N���X
	class Font
	{
	public:
		static  bool  dt;	//�����`����s�����ۂ�true�A
	//-----------------------------------------------------------------------
	public:
		typedef shared_ptr<Font>   SP;
		typedef weak_ptr<Font>     WP;
	//-----------------------------------------------------------------------
	private:
		Font( );
	//-----------------------------------------------------------------------
	private:
		bool  Initialize(
				const string&		fname_,
				WORD				width_,
				WORD				height_,
				WORD				weight_,
				BYTE				charSet_);
	//-----------------------------------------------------------------------
	private:
		ID3DX10Font*	font;
	//-----------------------------------------------------------------------
	public:
		~Font( );
	//-----------------------------------------------------------------------
	public:
		//��������
		static SP Create(	
				const string&	fname_,
				WORD			width_,
				WORD			height_, 
				WORD			weight_ = 500,
				BYTE			charSet_ = SHIFTJIS_CHARSET);
	//-----------------------------------------------------------------------
	private:
		//�`��
		void DrawS(
				const ML::Box2D&		draw_,
				const string&			tex_,
				ML::Color				color_,
				UINT					uFormat_);
	//-----------------------------------------------------------------------
	public:
		//�`��
		void Draw(
				const ML::Box2D&		draw_,	//	�`���
				const string&			tex_,
				ML::Color				color_ = ML::Color(1,1,1,1),
				UINT					uFormat_ = DT_LEFT);
		//	�t���[���t���������\������
		enum FRAME{ x1, x2, x4};
		void DrawF(
				const ML::Box2D&		draw_,	//	�`���
				const string&			tex_,
				FRAME					mode_,
				ML::Color				color_ = ML::Color(1,1,1,1),
				ML::Color				fColor_ = ML::Color(1,0,0,0),
				UINT					uFormat_ = DT_LEFT);
	//-----------------------------------------------------------------------
	private:
		static  unordered_map<string, WP>   rep;
	};
}
