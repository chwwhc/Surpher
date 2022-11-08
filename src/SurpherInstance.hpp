#ifndef SURPHER_SURPHERINSTANCE_HPP
#define SURPHER_SURPHERINSTANCE_HPP

#include <unordered_map>
#include <vector>
#include <memory>
#include <any>

struct SurpherClass;
struct Token;

struct SurpherInstance : public std::enable_shared_from_this<SurpherInstance> {
    const std::shared_ptr<SurpherClass> surpher_class;
    std::unordered_map<std::string, std::any> fields;

    explicit SurpherInstance(std::shared_ptr<SurpherClass> surpher_class);

    virtual std::string SurpherInstanceToString();

    std::any get(const Token &name);

    void set(const Token &name, const std::any &value);
};

#endif //SURPHER_SURPHERINSTANCE_HPP
