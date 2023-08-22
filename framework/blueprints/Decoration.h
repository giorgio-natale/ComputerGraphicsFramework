//
// Created by drgio on 22/08/2023.
//

#ifndef DEMO_DECORATION_H
#define DEMO_DECORATION_H

#include "Blueprint.h"

namespace fmwk {

    class Decoration : public Blueprint{
    public:
        Decoration(std::string name, const glm::vec3 &position, const glm::quat &rotation, const glm::vec3 &scale,
                   std::string modelName);

        Decoration(const glm::vec3 &position, const glm::quat &rotation, const glm::vec3 &scale,
                   std::string modelName);

    protected:
        void buildEntity() override;

    private:
        glm::vec3 _position;
        glm::quat _rotation;
        glm::vec3 _scale;
        std::string _modelName;
        std::string _name;
    };

} // fmwk

#endif //DEMO_DECORATION_H
