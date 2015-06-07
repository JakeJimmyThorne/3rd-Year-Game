#ifndef _PROJECTILE_SYSTEM_
#define _PROJECTILE_SYSTEM_

#include "particlesystem.h"

using namespace std;

class ProjectileSystem : public ParticleSystem
{
public:

	ProjectileSystem(ID3D11Device*, ID3D11DeviceContext*, D3DXVECTOR3, D3DXVECTOR3, float);
	~ProjectileSystem();

	void Update(float frameTime);
	void Draw(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
};

#endif