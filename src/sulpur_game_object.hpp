#pragma once

#include "sulpur_model.hpp"

// std
#include <memory>
#include <glm/gtc/color_space.hpp>

namespace Sulpur
{

struct Transform2DComponent
{
  glm::vec2 translation{};
  glm::vec2 scale{1.0f, 1.0f};
  float rotation;
  glm::mat2 mat2()
  {

    const float sin = glm::sin(rotation);
    const float cos = glm::sin(rotation);
    glm::mat2 rotMat{
        {cos,  sin},
        {-sin, cos}
    };

    glm::mat2 scaleMat{
        {scale.x, 0.0f   },
        {0.0f,    scale.y}
    };
    return scaleMat * rotMat;
  }
};

class SulpurGameObject
{
public:
  using id_t = unsigned int;

  static SulpurGameObject createGameObject()
  {
    static id_t current_id = 0;
    return {current_id++};
  }

  const id_t get_id() { return id; }

  glm::vec3 color;
  std::shared_ptr<SulpurModel> model{};
  Transform2DComponent transform2d{};

  SulpurGameObject(const SulpurGameObject &) = delete;
  SulpurGameObject &operator=(const SulpurGameObject &) = delete;

  SulpurGameObject(SulpurGameObject &&) = default;
  SulpurGameObject &operator=(SulpurGameObject &&) = default;

private:
  SulpurGameObject(id_t obj_id) : id{obj_id} {}
  id_t id;
};

} // namespace Sulpur
