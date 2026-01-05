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

    SceneGraph3D() : 
        root(std::make_shared<SceneNode3D>(nullptr)),
        nodes(std::unordered_map<UUID, std::shared_ptr<SceneNode3D>>()) {}

    inline std::shared_ptr<SceneNode3D> get_root() const { return root; }
    inline void set_root_transform(const Matrix4x4d& transform) { root->global_transform = transform; }

    bool transforms_dirty() const;
    Matrix4x4d get_global_transform(const std::shared_ptr<Primitive3D> primitive);
    void update_global_transforms();

    std::vector<std::pair<std::shared_ptr<Primitive3D>, Matrix4x4d>> get_global_transforms();

    void add_item(const std::shared_ptr<Primitive3D> item, const std::shared_ptr<Primitive3D> parent);

    inline void add_item(const std::shared_ptr<Primitive3D> item)
    {
        add_item(item, nullptr);
    }

    void remove_item(const std::shared_ptr<Primitive3D> item);

    inline void clear()
    {
        root->children.clear();
        nodes.clear();
    }

    std::vector<std::pair<std::shared_ptr<Primitive3D>, Matrix4x4d>> get_draw_queue();

    inline int num_items() const { return nodes.size(); }
    inline bool contains_item(const std::shared_ptr<Primitive3D> item) const { return nodes.contains(item->get_id()); }

private:

    std::shared_ptr<SceneNode3D> root;
    std::unordered_map<UUID, std::shared_ptr<SceneNode3D>> nodes;

};

}
