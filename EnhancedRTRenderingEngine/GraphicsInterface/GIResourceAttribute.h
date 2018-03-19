#pragma once

namespace ResourceAttribute {
    template<class T>
    class Creatable {
    public:
        template<class... Args>
        static std::shared_ptr<T> Create(Args&&... args) {
            return std::make_shared<T>(std::forward<Args>(args)...);
        }
    };
}