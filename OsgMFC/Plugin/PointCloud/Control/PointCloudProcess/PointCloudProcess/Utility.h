#pragma once

//ʵ�ù�����

#ifndef _UTILITY_H_
#define _UTILITY_H_

#pragma once
#include "export.h"

#include <osg/Camera>
#include <osg/Geode>

class POINTCLOUDPROCESS_API Utility
{
public:
    class SelectBox
    {
    public:
        SelectBox();
        bool Init(osg::Camera* hud);
        bool IsInitialized();
        void Show(osg::Vec2 point1,osg::Vec2 point2);
        void SetInvisible(void);
    private:
        osg::ref_ptr<osg::Geode>  m_selectbox;
        osg::ref_ptr<osg::Camera> m_orthocamera;
        bool m_initialized;
    };

    class DeleteSelectedNodesVisitor : public osg::NodeVisitor
    {
    public:

        DeleteSelectedNodesVisitor(int deletenodemask = 0):_deletenodemask(deletenodemask),
          osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)        
          {
          }

          virtual void apply(osg::Node& node)
          {
              
              if ( (_deletenodemask | node.getNodeMask()) == _deletenodemask)
              {
                  _selectedNodes.push_back(&node);
              }
              else
              {
                  traverse(node);
              }
          }

          void pruneSelectedNodes()
          {
              for(SelectedNodes::iterator itr = _selectedNodes.begin();
                  itr != _selectedNodes.end();
                  ++itr)
              {
                  osg::Node* node = itr->get();
                  osg::Node::ParentList parents = node->getParents();
                  for(osg::Node::ParentList::iterator pitr = parents.begin();
                      pitr != parents.end();
                      ++pitr)
                  {
                      osg::Group* parent = *pitr;
                      parent->removeChild(node);
                  }
              }
          }

          typedef std::vector< osg::ref_ptr<osg::Node> > SelectedNodes;
          SelectedNodes _selectedNodes;
          int _deletenodemask;

    };

public:
    //����Hud�ڵ�
    static osg::Camera* createGlobalHud(int width = 1280,int height = 1024);
    //��ȡȫ��Hud����
    static osg::Camera* getGlobalHud(void) { if(g_Hud.valid()) return g_Hud.get(); else return NULL; }

    static osg::Geode* createPointGeode(osg::ref_ptr<osg::Vec3Array> vertexarray,osg::Vec4 color);

    static osg::Geode* createLineGeode(osg::ref_ptr<osg::Vec3Array> vertexarray,osg::Vec4 color);

    static osg::Geode* createTriGeode(osg::ref_ptr<osg::Vec3Array> vertexarray,osg::Vec4 color);
private:
    /**
     * ȫ��Hud����ʹ��ȫ�ֶ�����Ϊ�˷����ڳ����л�ȡHUD��������ͨ������������
     * ���ҡ�
     */
    static osg::ref_ptr<osg::Camera> g_Hud;

private:
    Utility() {}
    ~Utility() {}
};
#endif