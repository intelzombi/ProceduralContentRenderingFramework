#include "SceneGraph.h"


namespace SG {
SceneGraph::SceneGraph(int ItemCount){
	m_sceneGraph = new SceneItem[ItemCount];
	for(int i = 0; i < ItemCount; i++) {
		m_sceneGraph[i].siCBuffers = new _BufferInfo();
	}
}
SceneGraph::~SceneGraph(){
	delete []m_sceneGraph;
}
SceneItem::SceneItem(){
	
}
SceneItem::~SceneItem(){
	
		delete siCBuffers;
	
}
};