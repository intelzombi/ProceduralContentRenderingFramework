#include <d3d10.h>
#include <d3dx10.h>

namespace SG {
typedef struct _BufferInfo {
	// Vertex Buffer
	D3D10_BUFFER_DESC* vdb;
	ID3D10Buffer*	pVtxBff;
	int				VertexCount;
	int				VtxBffStart;
	//IndexBuffer
	D3D10_BUFFER_DESC* ibd;
	ID3D10Buffer*	pIdxBff;
	int				IndexCount;
	int				IdxBffStart;
}* BufferInfo;

class SceneItem {
public:
	SceneItem();
	~SceneItem();
	BufferInfo siTBuffers;
	BufferInfo siCBuffers;
	ID3D10InputLayout* pILO;
	ID3D10EffectTechnique* pET;
	ID3D10ShaderResourceView* pRV;
	//effect;
	//material;

};

class SceneGraph {
public:
	SceneGraph(int itemCount);
	~SceneGraph();
	SceneItem *m_sceneGraph;
private:
	SceneGraph(){};
};
};