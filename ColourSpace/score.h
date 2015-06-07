#ifndef _SCORE_H_
#define _SCORE_H_

#include "object2d.h"
#include "settings.h"
#include "Rendering\renderer.h"

const int SCORE_LENGTH = 4;

class Score : public Object2D
{
public:
	Score() {}
	~Score() {}

	bool Initialise(ID3D11Device*, ID3D11DeviceContext*, int, int, float, float, float, float, WCHAR*);

	void Render(ID3D11DeviceContext*, D3DXMATRIX*, D3DXMATRIX*, Renderer*);

	void AddScore(int);
	void SetScore(int, int);
	int GetRunningScore() { return m_runningScore; }

	int GetTotalScore() { return m_totalScore; }
	void SetTotalScore(int amount) { m_totalScore = amount; }

	static bool LoadNumberTextures(ID3D11Device*);

private:
	int Power(int, int);
private:

	Object2D m_Numbers[SCORE_LENGTH];
	int m_score[SCORE_LENGTH];

	//what it should be
	int m_totalScore;

	//what it is
	int m_runningScore;


	static ID3D11ShaderResourceView* m_texture0;
	static ID3D11ShaderResourceView* m_texture1;
	static ID3D11ShaderResourceView* m_texture2;
	static ID3D11ShaderResourceView* m_texture3;
	static ID3D11ShaderResourceView* m_texture4;
	static ID3D11ShaderResourceView* m_texture5;
	static ID3D11ShaderResourceView* m_texture6;
	static ID3D11ShaderResourceView* m_texture7;
	static ID3D11ShaderResourceView* m_texture8;
	static ID3D11ShaderResourceView* m_texture9;

	static ID3D11ShaderResourceView* m_greenTexture0;
	static ID3D11ShaderResourceView* m_greenTexture1;
	static ID3D11ShaderResourceView* m_greenTexture2;
	static ID3D11ShaderResourceView* m_greenTexture3;
	static ID3D11ShaderResourceView* m_greenTexture4;
	static ID3D11ShaderResourceView* m_greenTexture5;
	static ID3D11ShaderResourceView* m_greenTexture6;
	static ID3D11ShaderResourceView* m_greenTexture7;
	static ID3D11ShaderResourceView* m_greenTexture8;
	static ID3D11ShaderResourceView* m_greenTexture9;
	
	static ID3D11ShaderResourceView* m_blueTexture0;
	static ID3D11ShaderResourceView* m_blueTexture1;
	static ID3D11ShaderResourceView* m_blueTexture2;
	static ID3D11ShaderResourceView* m_blueTexture3;
	static ID3D11ShaderResourceView* m_blueTexture4;
	static ID3D11ShaderResourceView* m_blueTexture5;
	static ID3D11ShaderResourceView* m_blueTexture6;
	static ID3D11ShaderResourceView* m_blueTexture7;
	static ID3D11ShaderResourceView* m_blueTexture8;
	static ID3D11ShaderResourceView* m_blueTexture9;

	static ID3D11ShaderResourceView* m_pinkTexture0;
	static ID3D11ShaderResourceView* m_pinkTexture1;
	static ID3D11ShaderResourceView* m_pinkTexture2;
	static ID3D11ShaderResourceView* m_pinkTexture3;
	static ID3D11ShaderResourceView* m_pinkTexture4;
	static ID3D11ShaderResourceView* m_pinkTexture5;
	static ID3D11ShaderResourceView* m_pinkTexture6;
	static ID3D11ShaderResourceView* m_pinkTexture7;
	static ID3D11ShaderResourceView* m_pinkTexture8;
	static ID3D11ShaderResourceView* m_pinkTexture9;

	static bool m_texturesLoaded;
};

#endif