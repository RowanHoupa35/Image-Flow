#ifndef GRAYSCALE_FILTER_HPP
#define GRAYSCALE_FILTER_HPP

#include "../Filter.hpp"
#include <chrono>

class GrayscaleFilter : public Filter {
public:
    void apply(const Image& input, Image& output) override;
    std::string getName() const override { return "Grayscale"; }
    std::unique_ptr<Filter> clone() const override {
        return std::make_unique<GrayscaleFilter>(*this);
    }
    double getLastExecutionTime() const override { return lastExecutionTime; }
    
private:
    double lastExecutionTime = 0.0;
};

#endif