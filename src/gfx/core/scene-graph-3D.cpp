#include <algorithm>
#include <utility>
#include <stack>

#include "gfx/core/scene-graph-3D.h"
#include "gfx/math/matrix.h"

namespace gfx
{

SceneGraph3D::SceneGraph3D() :
    root(std::make_shared<SceneNode3D>(nullptr)),
    nodes(std::unordered_map<UUID, std::shared_ptr<SceneNode3D>>())
{
}

std::shared_ptr<SceneNode3D> SceneGraph3D::get_root() const
{
    return root;
}

void SceneGraph3D::set_root_transform(const Matrix4x4d& transform)
{
    root->global_transform = transform;
}

bool SceneGraph3D::transforms_dirty() const
{
    for (const auto& [id, node] : nodes)
    {
        if (node->primitive == nullptr)
        {
            continue;
        }
        int64_t current_version { node->primitive->get_transform_version() };
        if (current_version != node->cached_transform_version)
        {
            return true;
        }
    }
    return false;
}

void SceneGraph3D::update_global_transforms()
{
    std::stack<std::pair<std::shared_ptr<SceneNode3D>, Matrix4x4d>> stack;

    stack.push({ get_root(), get_root()->global_transform });

    while (!stack.empty())
    {
        auto [node, parent_transform] { stack.top() };
        stack.pop();

        if (node->primitive)
        {
            node->global_transform = parent_transform * node->primitive->get_transform();
        }
        else
        {
            node->global_transform = parent_transform;
        }

        node->cached_transform_version = node->primitive ? 
            node->primitive->get_transform_version() : 
            0;

        for (const auto &child : node->children)
        {
            stack.push({ child, node->global_transform });
        }
    }
}

Matrix4x4d SceneGraph3D::get_global_transform(const std::shared_ptr<Primitive3D> primitive)
{
    auto node { nodes.contains(primitive->get_id()) ? nodes.at(primitive->get_id()) : nullptr };
    if (node == nullptr)
    {
        return Matrix4x4d::identity();
    }
    if (node->primitive == nullptr)
    {
        return node->global_transform;
    }

    return node->global_transform;
}

std::vector<std::pair<std::shared_ptr<Primitive3D>, Matrix4x4d>> SceneGraph3D::get_draw_queue()
{
    if (transforms_dirty())
    {
        update_global_transforms();
    }

    std::vector<std::pair<std::shared_ptr<Primitive3D>, Matrix4x4d>> draw_queue;
    for (const auto& [id, node] : nodes)
    {
        if (node->primitive != nullptr)
        {
            draw_queue.push_back({ node->primitive, get_global_transform(node->primitive) });
        }
    }

    return draw_queue;
}

std::vector<std::pair<std::shared_ptr<Primitive3D>, Matrix4x4d>> SceneGraph3D::get_global_transforms()
{
    if (transforms_dirty())
    {
        update_global_transforms();
    }

    std::vector<std::pair<std::shared_ptr<Primitive3D>, Matrix4x4d>> transforms;
    for (const auto& [id, node] : nodes)
    {
        if (node->primitive != nullptr)
        {
            transforms.push_back({ node->primitive, get_global_transform(node->primitive) });
        }
    }

    return transforms;
}

void SceneGraph3D::add_item(const std::shared_ptr<Primitive3D> item, const std::shared_ptr<Primitive3D> parent)
{
    auto new_node { std::make_shared<SceneNode3D>(item) };
    if (nodes.contains(new_node->get_id()))
    {
        return;
    }
    nodes[new_node->get_id()] = new_node;

    if (parent != nullptr && nodes.contains(parent->get_id()))
    {
        auto parent_node { nodes[parent->get_id()] };
        new_node->parent = parent_node;
        parent_node->children.push_back(new_node);
        return;
    }
    new_node->parent = root;
    root->children.push_back(new_node);
} 

void SceneGraph3D::add_item(const std::shared_ptr<Primitive3D> item)
{
    add_item(item, nullptr);
}

void SceneGraph3D::remove_item(const std::shared_ptr<Primitive3D> item)
{
    if (!nodes.contains(item->get_id()))
    {
        return;
    }

    nodes[item->get_id()]->parent->children.erase(std::remove_if(
        nodes[item->get_id()]->parent->children.begin(), 
        nodes[item->get_id()]->parent->children.end(), 
        [item](const std::shared_ptr<SceneNode3D> node) { return node->get_id() == item->get_id(); }
    ), nodes[item->get_id()]->parent->children.end());

    std::stack<std::shared_ptr<SceneNode3D>> stack;
    stack.push(nodes[item->get_id()]);
    while (!stack.empty())
    {
        auto node { stack.top() };
        stack.pop();

        for (const auto& child : node->children)
        {
            stack.push(child);
        }

        nodes.erase(node->get_id());
    }
}

void SceneGraph3D::clear()
{
    root->children.clear();
    nodes.clear();
}

int SceneGraph3D::num_items() const
{
    return nodes.size();
}

bool SceneGraph3D::contains_item(const std::shared_ptr<Primitive3D> item) const
{
    return nodes.contains(item->get_id());
}

}
