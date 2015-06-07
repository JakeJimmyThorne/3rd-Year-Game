#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include <D3DX11.h>
#include "settings.h"

#include "Modules/input.h"
#include "Modules/quad.h"
#include "Modules/sound.h"
#include "Modules/texture.h"
#include "Modules/transform.h"


//extern "C" { typedef INT32(__cdecl* DUMMY)(INT32); }
//typedef float(WINAPI* CS_VECTOR)(D3DXVECTOR3, D3DXVECTOR3);
//typedef float(CALLBACK* CS_FLOAT)(float, float);

//#include "MathFunctions.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	void Shutdown();
	void Frame(float);

	//adding modules
	bool AddInput();
	bool AddQuad(ID3D11Device*, int, int);
	bool AddQuad(ID3D11Device*, int, int, float, float, float, float);
	bool AddSound(const char*,HWND*);
	bool AddTexture(int,int,ID3D11Device*, ID3D11DeviceContext*, WCHAR*);
	bool AddTransform();

	bool LoadTexture(ID3D11Device*, WCHAR*);

	//get modules
	Input*     GetInput()     { return m_Input;     }
	Quad*      GetQuad()      { return m_Quad;      }
	Sound*     GetSound()     { return m_Sound;     }
	Texture*   GetTexture()   { return m_Texture;   }
	Transform* GetTransform() { return m_Transform; }

	static HINSTANCE* s_hinstance;
	static HWND* s_hwnd;

protected:
	

private:
	//available modules
	Input* m_Input;
	Quad* m_Quad;
	Sound* m_Sound;
	Texture* m_Texture;
	Transform* m_Transform;

	
	
};

#endif