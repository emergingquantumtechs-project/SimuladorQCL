#ifndef MATERIAL_MANAGER_HPP
#define MATERIAL_MANAGER_HPP

#include <string>
#include <vector>

#include "Material.hpp"

class MaterialManager{

    private:
        
        std::vector<Material> materials;

        public:

            void load(const std::string& filename);
            void print() const;

        Material get(const std::string& name) const;

    };

#endif