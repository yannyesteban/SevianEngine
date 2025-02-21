#include "Widget.h"

glm::mat4 SEVIAN::WIDGET::Widget::getModelMatrix () {

    
    auto translation = glm::translate ( glm::mat4 ( 1.0f ), glm::vec3(position, 1.0f) );
    

    glm::mat4 rotationMatrix = glm::mat4 ( 1.0f );
    

    auto scaleMatrix = glm::scale ( glm::mat4 ( 1.0f ), glm::vec3(size.x, size.y, 1.0f) );
    


    auto modelMatrix = translation * rotationMatrix * scaleMatrix;
	return modelMatrix;
}
