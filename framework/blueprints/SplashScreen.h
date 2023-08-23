//
// Created by drgio on 23/08/2023.
//

#ifndef DEMO_SPLASHSCREEN_H
#define DEMO_SPLASHSCREEN_H

#include "Blueprint.h"

namespace fmwk {

    class SplashScreen : public Blueprint{
    public:
        explicit SplashScreen(std::string textureName);

    protected:
        void buildEntity() override;

    private:
        std::string _textureName;
    };

} // fmwk

#endif //DEMO_SPLASHSCREEN_H
