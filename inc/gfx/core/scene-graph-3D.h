#pragma once

#include <utility>

#include "gfx/core/primitive-3D.h"
#include "gfx/core/types/uuid.h"

namespace gfx
{

struct SceneNode3D
{
    SceneNode3D(std::shared_ptr<Primitive3D> item) : primitive(item) {}

    UUID get_id() const
    {
        if (primitive)
        {
            return primitive->get_id();
        }
        return UUID(0, 0);
    }

    std::shared_ptr<Primitive3D> primitive;
    Matrix4x4d global_transform = Matrix4x4d::identity();
    uint64_t cached_transform_version = 0;
    std::shared_ptr<SceneNode3D> parent = nullptr;
    std::vector<std::shared_ptr<SceneNode3D>> children;
};


class SceneGraph3D
{

public:

    SceneGraph3D();

    std::shared_ptr<SceneNode3D> get_root() const;
    void set_root_transform(const Matrix4x4d& transform);

    bool transforms_dirty() const;
    Matrix4x4d get_global_transform(const std::shared_ptr<Primitive3D> primitive);
    void update_global_transforms();

    std::vector<std::pair<std::shared_ptr<Primitive3D>, Matrix4x4d>> get_global_transforms();

    void add_item(const std::shared_ptr<Primitive3D> item, const std::shared_ptr<Primitive3D> parent);
    void add_item(const std::shared_ptr<Primitive3D> item);

    void remove_item(const std::shared_ptr<Primitive3D> item);

    void clear();

    std::vector<std::pair<std::shared_ptr<Primitive3D>, Matrix4x4d>> get_draw_queue();

    int num_items() const;
    bool contains_item(const std::shared_ptr<Primitive3D> item) const;

private:

    std::shared_ptr<SceneNode3D> root;
    std::unordered_map<UUID, std::shared_ptr<SceneNode3D>> nodes;

};

}
