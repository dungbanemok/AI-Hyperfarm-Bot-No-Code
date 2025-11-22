#include <iostream>
#include "core/config.h"
#include "core/types.h"
#include "core/coordinates.h"

int main(int argc, char* argv[]) {
    std::cout << "AI Hyperfarm Bot - Starting..." << std::endl;
    
    try {
        // Load configuration
        auto config = hyperfarm::Config::loadFromFile("config.json");
        
        std::cout << "Configuration loaded successfully" << std::endl;
        std::cout << "Max workers: " << config->manager.max_workers << std::endl;
        std::cout << "Target FPS: " << config->capture.target_fps << std::endl;
        
        // TODO: Initialize Manager and start workers
        
        std::cout << "AI Hyperfarm Bot initialized successfully" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
