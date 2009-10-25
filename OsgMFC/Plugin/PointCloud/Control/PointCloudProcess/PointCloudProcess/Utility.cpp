#include "StdAfx.h"
#include "Utility.h"
#include <osg/Point>
#include <osg/LineWidth>
#include <osg/PolygonMode>
#include "ObjectClassCode.h"

using namespace PointCloudProcess;
// Init Global Hud Object
osg::ref_ptr<osg::Camera> Utility::g_Hud = NULL;

// Create HUD camera
osg::Camera* Utility::createGlobalHud(int width,int height)
{
    if(width <= 0 ) width = 1280;
    if(height <=0 ) height = 1024;

    osg::Camera* camera = new osg::Camera;

    camera->setProjectionMatrix(osg::Matrix::ortho2D(0,width,0,height));

    camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);

    camera->setViewMatrix(osg::Matrix::identity());

    camera->setClearMask(GL_DEPTH_BUFFER_BIT);

    camera->setRenderOrder(osg::Camera::POST_RENDER);

    camera->setAllowEventFocus(false);

    camera->setName("Hud");

    g_Hud = camera;

    return camera;
}

osg::Geode* Utility::createPointGeode( osg::ref_ptr<osg::Vec3Array> vertexarray,osg::Vec4 color)
{
    osg::Geode *geode = new osg::Geode;
    osg::Geometry *galaxy = new osg::Geometry;
    osg::Vec4Array *colorsArr = new osg::Vec4Array;

    int nvertices = vertexarray->size();

    osg::Vec3Array *pointsArr = new osg::Vec3Array(nvertices);
    *pointsArr = *(vertexarray.get());

    galaxy->setVertexArray(pointsArr);
    galaxy->setColorArray(colorsArr);

    colorsArr->push_back(color);
    galaxy->setColorBinding(osg::Geometry::BIND_OVERALL);

    galaxy->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS, 
        0, nvertices));
    
    osg::Point* pointattr = new osg::Point(2.0);
    geode->getOrCreateStateSet()->setAttributeAndModes(pointattr);
    geode->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED );

    geode->addDrawable(galaxy);
    geode->computeBound();
    
    geode->setName("Hightlight Point Geometry");

    // Todo will classify the usage of nodemask;
    geode->setNodeMask(CLASSCODE::PseudSelectedPoint);

    return geode;
}

osg::Geode* Utility::createLineGeode( osg::ref_ptr<osg::Vec3Array> vertexarray,osg::Vec4 color )
{
    osg::Geode *geode = new osg::Geode;
    osg::Geometry *galaxy = new osg::Geometry;
    osg::Vec4Array *colorsArr = new osg::Vec4Array;

    int nvertices = vertexarray->size();

    osg::Vec3Array *pointsArr = new osg::Vec3Array(nvertices);
    *pointsArr = *(vertexarray.get());

    galaxy->setVertexArray(pointsArr);
    galaxy->setColorArray(colorsArr);

    colorsArr->push_back(color);
    galaxy->setColorBinding(osg::Geometry::BIND_OVERALL);

    galaxy->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 
        0, nvertices));

    osg::LineWidth* linewidthattr = new osg::LineWidth(2.0);
    geode->getOrCreateStateSet()->setAttributeAndModes(linewidthattr);
    geode->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED );

    geode->addDrawable(galaxy);
    geode->computeBound();

    geode->setName("Hightlight Line Geometry");

    // Todo will classify the usage of nodemask;
    geode->setNodeMask(CLASSCODE::PseudSelectedLine);

    return geode;
}

osg::Geode* Utility::createTriGeode( osg::ref_ptr<osg::Vec3Array> vertexarray,osg::Vec4 color )
{
    osg::Geode *geode = new osg::Geode;
    osg::Geometry *galaxy = new osg::Geometry;
    osg::Vec4Array *colorsArr = new osg::Vec4Array;

    int nvertices = vertexarray->size();

    osg::Vec3Array *pointsArr = new osg::Vec3Array(nvertices);
    *pointsArr = *(vertexarray.get());

    galaxy->setVertexArray(pointsArr);
    galaxy->setColorArray(colorsArr);

    colorsArr->push_back(color);
    galaxy->setColorBinding(osg::Geometry::BIND_OVERALL);

    galaxy->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 
        0, nvertices));

    osg::LineWidth* linewidthattr = new osg::LineWidth(4.0);
    geode->getOrCreateStateSet()->setAttributeAndModes(linewidthattr);
    geode->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED );

    geode->addDrawable(galaxy);
    geode->computeBound();

    geode->setName("Hightlight Face Geometry");

    // Todo will classify the usage of nodemask;
    geode->setNodeMask(CLASSCODE::PseudSelectedFace);

    return geode;
}
//////////////////////////////////////////////////////////////////////////
Utility::SelectBox::SelectBox():
m_selectbox(NULL),
m_orthocamera(NULL),
m_initialized(false)
{

}

bool Utility::SelectBox::Init( osg::Camera* hud )
{
    if(m_initialized) return true;

    if(hud == NULL) return false;

    osg::Geode* geode = new osg::Geode();
    geode->setName("SelectBox");

    osg::Geometry* geom = new osg::Geometry;

    osg::Vec3Array* vertices = new osg::Vec3Array;

    vertices->push_back(osg::Vec3(0,0,0));
    vertices->push_back(osg::Vec3(1,0,0));
    vertices->push_back(osg::Vec3(1,1,0));
    vertices->push_back(osg::Vec3(0,1,0));
    geom->setVertexArray(vertices);

    osg::Vec3Array* normals = new osg::Vec3Array;
    normals->push_back(osg::Vec3(0.0f,0.0f,1.0f));
    geom->setNormalArray(normals);
    geom->setNormalBinding(osg::Geometry::BIND_OVERALL);

    osg::Vec4Array* colors = new osg::Vec4Array;
    colors->push_back(osg::Vec4(0.0f,1.0,0.0f,1.0f));
    geom->setColorArray(colors);
    geom->setColorBinding(osg::Geometry::BIND_OVERALL);

    GLushort idxLoops0[4] = {0, 1, 2, 3 };
    geom->addPrimitiveSet( new osg::DrawElementsUShort( osg::PrimitiveSet::LINE_LOOP, 4, idxLoops0 ) );
    geode->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED );
    geode->getOrCreateStateSet()->setMode( GL_DEPTH_TEST,osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED );
    geode->addDrawable(geom);

    m_selectbox = geode;
    m_selectbox->setNodeMask(CLASSCODE::NONE);

    hud->addChild(m_selectbox.get());

    m_orthocamera = hud;

    m_initialized = true;

    return true;
}

void Utility::SelectBox::Show(osg::Vec2 point1,osg::Vec2 point2 )
{
    if(!m_initialized) return;

    osg::Geometry* geom = m_selectbox->getDrawable(0)->asGeometry();
    if(geom)
    {
        osg::Vec3Array* vertices = dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
        vertices->at(0) = osg::Vec3(point1.x(),point1.y(),0.0);
        vertices->at(1) = osg::Vec3(point2.x(),point1.y(),0.0);
        vertices->at(2) = osg::Vec3(point2.x(),point2.y(),0.0);
        vertices->at(3) = osg::Vec3(point1.x(),point2.y(),0.0);
        m_selectbox->setNodeMask(CLASSCODE::PseudMouseDrawBox);

        geom->dirtyBound();
        geom->dirtyDisplayList();
    }
}

void Utility::SelectBox::SetInvisible( void )
{
    if(!m_initialized) return;

    m_selectbox->setNodeMask(CLASSCODE::NONE);
}

bool Utility::SelectBox::IsInitialized()
{
    return m_initialized;
}