#include <Geode/Geode.hpp>

using namespace geode::prelude;

#ifndef TIMELYLAYER_HPP
#define TIMELYLAYER_HPP

class TimelyLayer : public Popup<CreatorLayer*> {
    protected:
        bool setup(CreatorLayer*) override;
        
    public:

        CreatorLayer* m_creatorLayer;

        static TimelyLayer* create(CreatorLayer*);
        
        void onNewWeekly(CCObject* sender);

        void onNewDaily(CCObject* sender);

        void onNewEvent(CCObject* sender);
};

#endif
