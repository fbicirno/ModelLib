//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_VERTEX_H
#define MAGOS_VERTEX_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Misc.h"


//+-----------------------------------------------------------------------------
//| Vertex structure
//+-----------------------------------------------------------------------------
struct VERTEX
{
	VERTEX()
	{
		Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Color = 0xFFFFFFFF;
		TexturePosition = D3DXVECTOR2(0.0f, 0.0f);

		FORMAT = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1;
	
	}

	D3DXVECTOR3 Position;
	D3DXVECTOR3 Normal;
	D3DCOLOR Color;
	D3DXVECTOR2 TexturePosition;

	DWORD FORMAT;

};


//+-----------------------------------------------------------------------------
//| Line vertex structure
//+-----------------------------------------------------------------------------
struct LINE_VERTEX
{
	LINE_VERTEX()
	{
		Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Color = 0xFFFFFFFF;

		FORMAT = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	}

	D3DXVECTOR3 Position;
	D3DCOLOR Color;

	DWORD FORMAT;
	
};


//+-----------------------------------------------------------------------------
//| Particle vertex structure
//+-----------------------------------------------------------------------------
struct PARTICLE_VERTEX
{
	PARTICLE_VERTEX()
	{
		Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Color = 0xFFFFFFFF;
		TexturePosition = D3DXVECTOR2(0.0f, 0.0f);

        FORMAT = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1;

	}

	D3DXVECTOR3 Position;
	D3DXVECTOR3 Normal;
	D3DCOLOR Color;
	D3DXVECTOR2 TexturePosition;

    DWORD FORMAT;

};


//+-----------------------------------------------------------------------------
//| Ground vertex structure
//+-----------------------------------------------------------------------------
struct GROUND_VERTEX
{
	GROUND_VERTEX()
	{
		Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Color = 0xFFFFFFFF;
		TexturePosition = D3DXVECTOR2(0.0f, 0.0f);

        FORMAT = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1;

	}

	D3DXVECTOR3 Position;
	D3DXVECTOR3 Normal;
	D3DCOLOR Color;
	D3DXVECTOR2 TexturePosition;


	DWORD FORMAT;

};


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif
