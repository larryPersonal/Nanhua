//
//  TabbedView.h
//  PopulationTest
//
//  Created by Ong Heng Le on 26/4/13.
//
//

#ifndef PopulationTest_TabbedView_h
#define PopulationTest_TabbedView_h

#include "cocos2d.h"
using namespace cocos2d;

class TabbedView:public cocos2d::CCNode
{
    
private:
  
    /* NOTE:
     * I have to override CCLayerMultiplex because
     * cocos2d-x forgot to initialize its protected m_pLayers...
     */
    class LayerMultiplex:public cocos2d::CCLayerMultiplex
    {
    public:
        
        LayerMultiplex()
        {
            m_pLayers = CCArray::create();
            m_pLayers->retain();
        }
        ~LayerMultiplex()
        {
            m_pLayers->release();
        }
        
        static LayerMultiplex* create()
        {
            LayerMultiplex* pRet = new LayerMultiplex();
            if (pRet && pRet->init())
            {
                pRet->autorelease();
            }
            else
            {
                CC_SAFE_DELETE(pRet);
            }
            return pRet;
        }
    };
    
    LayerMultiplex* layers;
    
    int currentIndex;
    
public:
    CCMenu* menu;
    
    void createTabbedView(CCPoint tabOrigin, CCPoint contentOrigin);
    void addTabWithContent(CCMenuItem* tab, CCPoint tabOffset, CCLayer* contentLayer, bool maualCall = false);
    void setOrigins(CCPoint tabOrigin, CCPoint contentOrigin);
    
    void onTabSelected(CCObject* pSender);
    
    /*override on manual*/
    void onTabSelected(CCMenuItem* tab);

};

#endif
